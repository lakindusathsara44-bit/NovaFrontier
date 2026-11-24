#pragma once
#include <algorithm>
#include <cmath>
namespace novafrontier {
struct Vector2 { float x{}; float y{}; };
struct PlayerConfig {
    float walk_speed{450.0F};
    float sprint_speed{700.0F};
    float crouch_speed{220.0F};
    float look_sensitivity{1.0F};
    float minimum_pitch{-80.0F};
    float maximum_pitch{80.0F};
    float jump_velocity{520.0F};
};
struct PlayerInputFrame {
    Vector2 move{};
    Vector2 look{};
    bool sprint_held{false};
    bool crouch_toggle_pressed{false};
    bool jump_pressed{false};
};
struct PlayerFrameState {
    Vector2 normalized_move{};
    float movement_speed{};
    float yaw_degrees{};
    float pitch_degrees{};
    float jump_velocity{};
    bool sprinting{false};
    bool crouched{false};
    bool jump_requested{false};
};
class FirstPersonController {
public:
    explicit FirstPersonController(PlayerConfig config = {});
    // Keyboard, gamepad and touch inputs use the same normalized frame.
    PlayerFrameState update(const PlayerInputFrame& input, bool grounded);
    void reset_view(float yaw_degrees = 0.0F, float pitch_degrees = 0.0F) noexcept;
    const PlayerFrameState& state() const noexcept { return state_; }
    const PlayerConfig& config() const noexcept { return config_; }
private:
    PlayerConfig config_;
    PlayerFrameState state_{};
};
} // namespace novafrontier
