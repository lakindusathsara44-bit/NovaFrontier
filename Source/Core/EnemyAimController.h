#pragma once

#include "EnemyPerceptionMemory.h"

#include <string>

namespace novafrontier {

struct EnemyAimControllerConfig {
    float reaction_time_seconds{0.25F};
    float aim_turn_rate_degrees_per_second{180.0F};
    float fire_tolerance_degrees{2.5F};
    float maximum_fire_tolerance_degrees{5.0F};
    float projectile_speed_cm_per_second{0.0F}; // Zero means hitscan/no ballistic lead.
    float maximum_lead_time_seconds{0.75F};
    float target_size_reference_degrees{2.0F};
    float minimum_target_size_factor{0.25F};
    float moving_accuracy_multiplier{0.75F};
    float suppressed_accuracy_multiplier{0.60F};
};

struct EnemyAimInput {
    bool target_visible{false};
    std::string target_id;
    Vector3 muzzle_position{};
    Vector3 current_aim_direction{1.0F, 0.0F, 0.0F};
    Vector3 target_position{};
    Vector3 target_velocity_cm_per_second{};
    float target_radius_cm{35.0F};
    float skill{0.65F}; // 0 = untrained, 1 = best trained.
    bool moving{false};
    bool suppressed{false};
};

struct EnemyAimDecision {
    bool has_target{false};
    Vector3 aim_point{};
    Vector3 aim_direction{1.0F, 0.0F, 0.0F};
    float angular_error_degrees{0.0F};
    float reaction_elapsed_seconds{0.0F};
    float lead_time_seconds{0.0F};
    float accuracy_factor{0.0F};
    bool reaction_complete{false};
    bool can_fire{false};
};

class EnemyAimController {
public:
    explicit EnemyAimController(EnemyAimControllerConfig config = {});
    EnemyAimDecision update(const EnemyAimInput& input, float delta_seconds);
    void reset() noexcept;

    float reaction_elapsed_seconds() const noexcept { return reaction_elapsed_seconds_; }
    const std::string& locked_target_id() const noexcept { return locked_target_id_; }

private:
    EnemyAimControllerConfig config_;
    std::string locked_target_id_;
    float reaction_elapsed_seconds_{0.0F};
};

} // namespace novafrontier
