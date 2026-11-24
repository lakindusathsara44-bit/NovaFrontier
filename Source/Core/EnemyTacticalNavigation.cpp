#include "EnemyTacticalNavigation.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <unordered_set>

namespace novafrontier {
namespace {

bool finite_non_negative(float value) noexcept {
    return std::isfinite(value) && value >= 0.0F;
}

bool finite_vector(const Vector3& value) noexcept {
    return std::isfinite(value.x) && std::isfinite(value.y) && std::isfinite(value.z);
}

float distance_squared(const Vector3& a, const Vector3& b) noexcept {
    const float x = a.x - b.x;
    const float y = a.y - b.y;
    const float z = a.z - b.z;
    return x * x + y * y + z * z;
}

float distance(const Vector3& a, const Vector3& b) noexcept {
    return std::sqrt(distance_squared(a, b));
}

bool valid_cover(const EnemyCoverPoint& cover, const EnemyTacticalInput& input,
                const EnemyTacticalNavigationConfig& config) noexcept {
    return !cover.id.empty() && finite_vector(cover.position) &&
           std::isfinite(cover.quality) && cover.quality >= 0.0F && cover.quality <= 1.0F &&
           cover.reachable && cover.blocks_threat_line_of_sight && !cover.occupied &&
           distance(input.position, cover.position) <= config.maximum_cover_distance_cm;
}

float cover_score(const EnemyCoverPoint& cover, const Vector3& position,
                  const EnemyTacticalNavigationConfig& config) noexcept {
    return distance(position, cover.position) * config.cover_distance_weight -
           cover.quality * config.cover_quality_weight;
}

} // namespace

EnemyTacticalNavigation::EnemyTacticalNavigation(EnemyTacticalNavigationConfig config)
    : config_(config) {
    if (!finite_non_negative(config_.arrival_radius_cm) ||
        !finite_non_negative(config_.maximum_cover_distance_cm) ||
        !std::isfinite(config_.low_health_fraction) ||
        config_.low_health_fraction < 0.0F || config_.low_health_fraction > 1.0F ||
        !finite_non_negative(config_.cover_distance_weight) ||
        !finite_non_negative(config_.cover_quality_weight) ||
        !finite_non_negative(config_.cover_switch_score_margin)) {
        throw std::invalid_argument("Invalid enemy tactical navigation configuration");
    }
}

EnemyTacticalDecision EnemyTacticalNavigation::update(const EnemyTacticalInput& input,
                                                       float delta_seconds) {
    if (!finite_non_negative(delta_seconds) ||
        !finite_vector(input.position) ||
        (input.has_threat_position && !finite_vector(input.threat_position)) ||
        (input.has_navigation_target && !finite_vector(input.navigation_target)) ||
        input.max_health <= 0 || input.health < 0 || input.health > input.max_health) {
        throw std::invalid_argument("Invalid enemy tactical navigation input");
    }

    for (const auto& waypoint : input.patrol_waypoints) {
        if (!finite_vector(waypoint)) {
            throw std::invalid_argument("Patrol route contains an invalid waypoint");
        }
    }

    std::unordered_set<std::string> cover_ids;
    for (const auto& cover : input.cover_points) {
        if (cover.id.empty() || !cover_ids.insert(cover.id).second ||
            !finite_vector(cover.position) || !std::isfinite(cover.quality) ||
            cover.quality < 0.0F || cover.quality > 1.0F) {
            throw std::invalid_argument("Cover points contain invalid or duplicate data");
        }
    }

    EnemyTacticalDecision decision;
    decision.should_face_threat = input.has_threat_position;

    if (input.state == EnemyAIState::Disabled || input.health == 0) {
        selected_cover_id_.clear();
        decision.waypoint_index = waypoint_index_;
        return decision;
    }

    const float health_fraction = static_cast<float>(input.health) /
                                  static_cast<float>(input.max_health);
    const bool should_seek_cover = input.has_threat_position &&
                                   (input.under_fire || health_fraction <= config_.low_health_fraction);

    if (should_seek_cover) {
        const EnemyCoverPoint* best = nullptr;
        const EnemyCoverPoint* current = nullptr;
        float best_score = 0.0F;
        float current_score = 0.0F;

        for (const auto& cover : input.cover_points) {
            if (!valid_cover(cover, input, config_)) {
                continue;
            }
            const float score = cover_score(cover, input.position, config_);
            if (cover.id == selected_cover_id_) {
                current = &cover;
                current_score = score;
            }
            if (best == nullptr || score < best_score ||
                (score == best_score && cover.id < best->id)) {
                best = &cover;
                best_score = score;
            }
        }

        if (current != nullptr &&
            (best == nullptr || best->id == current->id ||
             best_score + config_.cover_switch_score_margin >= current_score)) {
            best = current;
        }

        if (best != nullptr) {
            selected_cover_id_ = best->id;
            decision.movement_mode = TacticalMovementMode::MoveToCover;
            decision.has_target = true;
            decision.target = best->position;
            decision.has_cover = true;
            decision.cover_id = best->id;
            decision.arrived = distance(input.position, best->position) <=
                               config_.arrival_radius_cm;
            decision.should_move = !decision.arrived;
            decision.waypoint_index = waypoint_index_;
            return decision;
        }
    }

    selected_cover_id_.clear();

    if (input.state == EnemyAIState::Patrol) {
        const auto count = input.patrol_waypoints.size();
        if (count == 0U) {
            decision.waypoint_index = 0U;
            return decision;
        }

        if (waypoint_index_ >= count) {
            waypoint_index_ = 0U;
            patrol_direction_ = 1;
        }

        if (distance(input.position, input.patrol_waypoints[waypoint_index_]) <=
            config_.arrival_radius_cm) {
            decision.arrived = true;
            decision.waypoint_advanced = true;
            if (count > 1U) {
                if (config_.patrol_route_mode == PatrolRouteMode::Loop) {
                    waypoint_index_ = (waypoint_index_ + 1U) % count;
                } else {
                    const auto next = static_cast<long long>(waypoint_index_) + patrol_direction_;
                    if (next >= static_cast<long long>(count)) {
                        patrol_direction_ = -1;
                        waypoint_index_ = count - 2U;
                    } else if (next < 0) {
                        patrol_direction_ = 1;
                        waypoint_index_ = 1U;
                    } else {
                        waypoint_index_ = static_cast<std::size_t>(next);
                    }
                }
            }
        }

        decision.movement_mode = TacticalMovementMode::Patrol;
        decision.has_target = true;
        decision.target = input.patrol_waypoints[waypoint_index_];
        decision.should_move = distance(input.position, decision.target) >
                               config_.arrival_radius_cm;
        decision.waypoint_index = waypoint_index_;
        return decision;
    }

    if ((input.state == EnemyAIState::Chase || input.state == EnemyAIState::Investigate ||
         input.state == EnemyAIState::Search) && input.has_navigation_target) {
        decision.movement_mode = TacticalMovementMode::MoveToTarget;
        decision.has_target = true;
        decision.target = input.navigation_target;
        decision.arrived = distance(input.position, decision.target) <=
                           config_.arrival_radius_cm;
        decision.should_move = !decision.arrived;
    }

    decision.waypoint_index = waypoint_index_;
    return decision;
}

void EnemyTacticalNavigation::reset() noexcept {
    waypoint_index_ = 0U;
    patrol_direction_ = 1;
    selected_cover_id_.clear();
}

} // namespace novafrontier
