#include "FirstPersonController.h"
#include <stdexcept>
namespace novafrontier {
namespace {
constexpr float kFullTurn = 360.0F;
float wrap_yaw(float yaw) noexcept {
    yaw = std::fmod(yaw, kFullTurn);
    return yaw < 0.0F ? yaw + kFullTurn : yaw;
}
Vector2 normalize_input(Vector2 value) noexcept {
    const float magnitude = std::sqrt(value.x * value.x + value.y * value.y);
    if (magnitude <= 1.0F) return value;
    return {value.x / magnitude, value.y / magnitude};
}
}
FirstPersonController::FirstPersonController(PlayerConfig config) : config_(config) {
    if (config.walk_speed <= 0.0F || config.sprint_speed < config.walk_speed ||
        config.crouch_speed <= 0.0F || config.look_sensitivity <= 0.0F ||
        config.minimum_pitch >= config.maximum_pitch ||
        config.minimum_pitch < -90.0F || config.maximum_pitch > 90.0F ||
        config.jump_velocity <= 0.0F) {
        throw std::invalid_argument("Invalid first-person controller configuration");
    }
}
PlayerFrameState FirstPersonController::update(const PlayerInputFrame& input, bool grounded) {
    if (input.crouch_toggle_pressed) state_.crouched = !state_.crouched;
    state_.normalized_move = normalize_input(input.move);
    state_.yaw_degrees = wrap_yaw(state_.yaw_degrees + input.look.x * config_.look_sensitivity);
    state_.pitch_degrees = std::clamp(
        state_.pitch_degrees - input.look.y * config_.look_sensitivity,
        config_.minimum_pitch, config_.maximum_pitch);
    const bool has_movement = state_.normalized_move.x != 0.0F || state_.normalized_move.y != 0.0F;
    state_.sprinting = input.sprint_held && has_movement && !state_.crouched;
    if (state_.crouched) state_.movement_speed = config_.crouch_speed;
    else if (state_.sprinting) state_.movement_speed = config_.sprint_speed;
    else state_.movement_speed = config_.walk_speed;
    state_.jump_requested = input.jump_pressed && grounded && !state_.crouched;
    state_.jump_velocity = state_.jump_requested ? config_.jump_velocity : 0.0F;
    return state_;
}
void FirstPersonController::reset_view(float yaw_degrees, float pitch_degrees) noexcept {
    state_.yaw_degrees = wrap_yaw(yaw_degrees);
    state_.pitch_degrees = std::clamp(pitch_degrees, config_.minimum_pitch, config_.maximum_pitch);
}
} // namespace novafrontier
