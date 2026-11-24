#include "EnemyPerceptionMemory.h"
#include <cmath>
#include <stdexcept>
namespace novafrontier {
namespace {
bool finite_non_negative(float value) noexcept { return std::isfinite(value) && value >= 0.0F; }
bool finite_vector(const Vector3& value) noexcept {
    return std::isfinite(value.x) && std::isfinite(value.y) && std::isfinite(value.z);
}
}
EnemyPerceptionMemory::EnemyPerceptionMemory(EnemyPerceptionMemoryConfig config) : config_(config) {
    if (!finite_non_negative(config_.hearing_range_cm) ||
        !finite_non_negative(config_.player_memory_duration_seconds) ||
        !finite_non_negative(config_.noise_memory_duration_seconds))
        throw std::invalid_argument("Invalid enemy perception memory configuration");
}
EnemyPerceptionFrame EnemyPerceptionMemory::update(const EnemyObservation& observation, float delta_seconds) {
    if (!finite_non_negative(delta_seconds) ||
        !finite_non_negative(observation.player_distance_cm) ||
        !finite_non_negative(observation.noise_distance_cm) ||
        !finite_vector(observation.player_position) ||
        !finite_vector(observation.noise_position))
        throw std::invalid_argument("Enemy perception contains invalid time, distance, or position");

    if (observation.player_visible) {
        has_last_known_player_ = true;
        last_known_player_position_ = observation.player_position;
        last_seen_age_seconds_ = 0.0F;
    } else if (has_last_known_player_) {
        last_seen_age_seconds_ += delta_seconds;
        if (last_seen_age_seconds_ >= config_.player_memory_duration_seconds) {
            has_last_known_player_ = false;
            last_seen_age_seconds_ = 0.0F;
        }
    }

    const bool audible_noise = observation.noise_heard &&
        observation.noise_distance_cm <= config_.hearing_range_cm;
    if (audible_noise) {
        has_last_heard_position_ = true;
        last_heard_position_ = observation.noise_position;
        last_heard_age_seconds_ = 0.0F;
    } else if (has_last_heard_position_) {
        last_heard_age_seconds_ += delta_seconds;
        if (last_heard_age_seconds_ >= config_.noise_memory_duration_seconds) {
            has_last_heard_position_ = false;
            last_heard_age_seconds_ = 0.0F;
        }
    }

    EnemyPerceptionFrame frame;
    frame.ai_input.player_visible = observation.player_visible;
    frame.ai_input.noise_heard = audible_noise;
    frame.ai_input.player_distance = observation.player_distance_cm;
    frame.ai_input.health = observation.health;
    frame.has_last_known_player = has_last_known_player_;
    frame.last_known_player_position = last_known_player_position_;
    frame.last_seen_age_seconds = last_seen_age_seconds_;
    frame.has_last_heard_position = has_last_heard_position_;
    frame.last_heard_position = last_heard_position_;
    frame.last_heard_age_seconds = last_heard_age_seconds_;
    return frame;
}
void EnemyPerceptionMemory::reset() noexcept {
    has_last_known_player_ = false;
    last_known_player_position_ = {};
    last_seen_age_seconds_ = 0.0F;
    has_last_heard_position_ = false;
    last_heard_position_ = {};
    last_heard_age_seconds_ = 0.0F;
}
} // namespace novafrontier
