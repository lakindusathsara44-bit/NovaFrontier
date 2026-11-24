#include "EnemyAimController.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace novafrontier {
namespace {

constexpr float kPi = 3.14159265358979323846F;
constexpr float kRadiansToDegrees = 180.0F / kPi;
constexpr float kDegreesToRadians = kPi / 180.0F;

bool finite_non_negative(float value) noexcept {
    return std::isfinite(value) && value >= 0.0F;
}

bool finite_vector(const Vector3& value) noexcept {
    return std::isfinite(value.x) && std::isfinite(value.y) && std::isfinite(value.z);
}

float dot(const Vector3& a, const Vector3& b) noexcept {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 add(const Vector3& a, const Vector3& b) noexcept {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

Vector3 subtract(const Vector3& a, const Vector3& b) noexcept {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

Vector3 multiply(const Vector3& value, float scalar) noexcept {
    return {value.x * scalar, value.y * scalar, value.z * scalar};
}

Vector3 cross(const Vector3& a, const Vector3& b) noexcept {
    return {a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
}

float length(const Vector3& value) noexcept {
    return std::sqrt(dot(value, value));
}

Vector3 normalized(const Vector3& value) {
    const float magnitude = length(value);
    if (!std::isfinite(magnitude) || magnitude <= 1.0e-6F) {
        throw std::invalid_argument("Aim direction must be finite and non-zero");
    }
    return multiply(value, 1.0F / magnitude);
}

float clamp_unit(float value) noexcept {
    return std::max(-1.0F, std::min(1.0F, value));
}

Vector3 rotate_towards(const Vector3& current, const Vector3& desired,
                       float maximum_step_radians) {
    const float cosine = clamp_unit(dot(current, desired));
    const float angle = std::acos(cosine);
    if (angle <= maximum_step_radians || angle <= 1.0e-6F) {
        return desired;
    }

    Vector3 perpendicular = subtract(desired, multiply(current, cosine));
    float perpendicular_length = length(perpendicular);
    if (perpendicular_length <= 1.0e-6F) {
        const Vector3 fallback_axis = std::fabs(current.z) < 0.9F
            ? Vector3{0.0F, 0.0F, 1.0F}
            : Vector3{0.0F, 1.0F, 0.0F};
        perpendicular = cross(current, fallback_axis);
        perpendicular_length = length(perpendicular);
    }
    perpendicular = multiply(perpendicular, 1.0F / perpendicular_length);
    return normalized(add(multiply(current, std::cos(maximum_step_radians)),
                          multiply(perpendicular, std::sin(maximum_step_radians))));
}

float angular_error_degrees(const Vector3& a, const Vector3& b) noexcept {
    return std::acos(clamp_unit(dot(a, b))) * kRadiansToDegrees;
}

} // namespace

EnemyAimController::EnemyAimController(EnemyAimControllerConfig config)
    : config_(config) {
    if (!finite_non_negative(config_.reaction_time_seconds) ||
        !finite_non_negative(config_.aim_turn_rate_degrees_per_second) ||
        !finite_non_negative(config_.fire_tolerance_degrees) ||
        !finite_non_negative(config_.maximum_fire_tolerance_degrees) ||
        config_.maximum_fire_tolerance_degrees < config_.fire_tolerance_degrees ||
        !finite_non_negative(config_.projectile_speed_cm_per_second) ||
        !finite_non_negative(config_.maximum_lead_time_seconds) ||
        !std::isfinite(config_.target_size_reference_degrees) ||
        config_.target_size_reference_degrees <= 0.0F ||
        !std::isfinite(config_.minimum_target_size_factor) ||
        config_.minimum_target_size_factor < 0.0F || config_.minimum_target_size_factor > 1.0F ||
        !std::isfinite(config_.moving_accuracy_multiplier) ||
        config_.moving_accuracy_multiplier < 0.0F || config_.moving_accuracy_multiplier > 1.0F ||
        !std::isfinite(config_.suppressed_accuracy_multiplier) ||
        config_.suppressed_accuracy_multiplier < 0.0F ||
        config_.suppressed_accuracy_multiplier > 1.0F) {
        throw std::invalid_argument("Invalid enemy aim controller configuration");
    }
}

EnemyAimDecision EnemyAimController::update(const EnemyAimInput& input, float delta_seconds) {
    if (!finite_non_negative(delta_seconds) ||
        !finite_vector(input.muzzle_position) ||
        !finite_vector(input.current_aim_direction) ||
        !finite_vector(input.target_position) ||
        !finite_vector(input.target_velocity_cm_per_second) ||
        !finite_non_negative(input.target_radius_cm) ||
        !std::isfinite(input.skill) || input.skill < 0.0F || input.skill > 1.0F) {
        throw std::invalid_argument("Invalid enemy aim input");
    }

    const Vector3 current_direction = normalized(input.current_aim_direction);
    EnemyAimDecision decision;
    decision.aim_direction = current_direction;

    if (!input.target_visible) {
        reset();
        return decision;
    }
    if (input.target_id.empty()) {
        throw std::invalid_argument("Visible aim target must have a stable non-empty ID");
    }

    if (input.target_id != locked_target_id_) {
        locked_target_id_ = input.target_id;
        reaction_elapsed_seconds_ = 0.0F;
    } else {
        reaction_elapsed_seconds_ += delta_seconds;
    }

    const Vector3 initial_offset = subtract(input.target_position, input.muzzle_position);
    const float initial_distance = length(initial_offset);
    if (!std::isfinite(initial_distance)) {
        throw std::invalid_argument("Target distance overflowed");
    }

    float lead_time = 0.0F;
    if (config_.projectile_speed_cm_per_second > 0.0F) {
        lead_time = std::min(initial_distance / config_.projectile_speed_cm_per_second,
                             config_.maximum_lead_time_seconds);
        Vector3 predicted = add(input.target_position,
                                multiply(input.target_velocity_cm_per_second, lead_time));
        const float predicted_distance = length(subtract(predicted, input.muzzle_position));
        if (!std::isfinite(predicted_distance)) {
            throw std::invalid_argument("Ballistic lead solution overflowed");
        }
        lead_time = std::min(predicted_distance / config_.projectile_speed_cm_per_second,
                             config_.maximum_lead_time_seconds);
    }

    const Vector3 aim_point = add(input.target_position,
                                  multiply(input.target_velocity_cm_per_second, lead_time));
    if (!finite_vector(aim_point)) {
        throw std::invalid_argument("Predicted aim point is not finite");
    }

    const Vector3 desired_direction = normalized(subtract(aim_point, input.muzzle_position));
    const float maximum_step = config_.aim_turn_rate_degrees_per_second *
                               delta_seconds * kDegreesToRadians;
    const Vector3 next_direction = rotate_towards(current_direction, desired_direction, maximum_step);
    const float error = angular_error_degrees(next_direction, desired_direction);

    const float target_distance = std::max(length(subtract(aim_point, input.muzzle_position)), 1.0e-3F);
    const float target_angular_radius = std::atan2(input.target_radius_cm, target_distance) *
                                        kRadiansToDegrees;
    const float effective_tolerance = std::min(
        config_.maximum_fire_tolerance_degrees,
        std::max(config_.fire_tolerance_degrees, target_angular_radius * 0.5F));
    const float alignment_factor = effective_tolerance > 0.0F
        ? std::max(0.0F, 1.0F - error / effective_tolerance)
        : (error <= 1.0e-4F ? 1.0F : 0.0F);
    const float target_size_factor = std::max(
        config_.minimum_target_size_factor,
        std::min(1.0F, target_angular_radius / config_.target_size_reference_degrees));

    decision.has_target = true;
    decision.aim_point = aim_point;
    decision.aim_direction = next_direction;
    decision.angular_error_degrees = error;
    decision.reaction_elapsed_seconds = reaction_elapsed_seconds_;
    decision.lead_time_seconds = lead_time;
    decision.reaction_complete = reaction_elapsed_seconds_ >= config_.reaction_time_seconds;
    decision.can_fire = decision.reaction_complete && error <= effective_tolerance;
    decision.accuracy_factor = input.skill * target_size_factor * alignment_factor;
    if (input.moving) {
        decision.accuracy_factor *= config_.moving_accuracy_multiplier;
    }
    if (input.suppressed) {
        decision.accuracy_factor *= config_.suppressed_accuracy_multiplier;
    }
    decision.accuracy_factor = std::max(0.0F, std::min(1.0F, decision.accuracy_factor));
    return decision;
}

void EnemyAimController::reset() noexcept {
    locked_target_id_.clear();
    reaction_elapsed_seconds_ = 0.0F;
}

} // namespace novafrontier
