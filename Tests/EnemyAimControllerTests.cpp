#include "EnemyAimController.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace novafrontier;

namespace {
bool near(float a, float b, float tolerance = 0.02F) {
    return std::fabs(a - b) <= tolerance;
}
}

int main() {
    {
        EnemyAimController controller;
        EnemyAimInput input;
        auto decision = controller.update(input, 0.016F);
        assert(!decision.has_target && !decision.can_fire);
        assert(near(decision.aim_direction.x, 1.0F));
        assert(controller.locked_target_id().empty());
    }
    {
        EnemyAimControllerConfig config;
        config.reaction_time_seconds = 0.2F;
        config.aim_turn_rate_degrees_per_second = 90.0F;
        config.fire_tolerance_degrees = 2.0F;
        EnemyAimController controller(config);
        EnemyAimInput input;
        input.target_visible = true;
        input.target_id = "player-1";
        input.target_position = {0.0F, 1000.0F, 0.0F};
        input.target_radius_cm = 5.0F;

        auto decision = controller.update(input, 0.1F);
        assert(decision.has_target && !decision.reaction_complete);
        assert(near(decision.angular_error_degrees, 81.0F));
        assert(!decision.can_fire);
        input.current_aim_direction = decision.aim_direction;
        decision = controller.update(input, 0.1F);
        assert(near(decision.reaction_elapsed_seconds, 0.1F));
        assert(!decision.reaction_complete);
        input.current_aim_direction = decision.aim_direction;
        decision = controller.update(input, 0.1F);
        assert(decision.reaction_complete);
        assert(near(decision.angular_error_degrees, 63.0F));
        assert(!decision.can_fire);
    }
    {
        EnemyAimControllerConfig config;
        config.reaction_time_seconds = 0.0F;
        config.aim_turn_rate_degrees_per_second = 720.0F;
        config.fire_tolerance_degrees = 2.0F;
        EnemyAimController controller(config);
        EnemyAimInput input;
        input.target_visible = true;
        input.target_id = "target-a";
        input.target_position = {1000.0F, 0.0F, 0.0F};
        auto decision = controller.update(input, 0.016F);
        assert(decision.can_fire && decision.reaction_complete);
        assert(decision.accuracy_factor > 0.0F && decision.accuracy_factor <= 1.0F);

        input.target_id = "target-b";
        decision = controller.update(input, 0.016F);
        assert(!decision.reaction_complete);
        assert(near(decision.reaction_elapsed_seconds, 0.0F));

        input.target_visible = false;
        decision = controller.update(input, 0.016F);
        assert(!decision.has_target && controller.locked_target_id().empty());
    }
    {
        EnemyAimControllerConfig config;
        config.projectile_speed_cm_per_second = 1000.0F;
        config.maximum_lead_time_seconds = 0.5F;
        EnemyAimController controller(config);
        EnemyAimInput input;
        input.target_visible = true;
        input.target_id = "moving-target";
        input.target_position = {1000.0F, 0.0F, 0.0F};
        input.target_velocity_cm_per_second = {0.0F, 100.0F, 0.0F};
        auto decision = controller.update(input, 0.016F);
        assert(near(decision.lead_time_seconds, 0.5F));
        assert(near(decision.aim_point.x, 1000.0F));
        assert(near(decision.aim_point.y, 50.0F));
    }
    {
        EnemyAimControllerConfig config;
        config.reaction_time_seconds = 0.0F;
        config.aim_turn_rate_degrees_per_second = 1000.0F;
        EnemyAimController controller(config);
        EnemyAimInput input;
        input.target_visible = true;
        input.target_id = "accuracy-target";
        input.target_position = {1000.0F, 0.0F, 0.0F};
        input.skill = 0.8F;
        const auto still = controller.update(input, 0.1F);
        input.moving = true;
        const auto moving = controller.update(input, 0.1F);
        input.suppressed = true;
        const auto suppressed = controller.update(input, 0.1F);
        assert(moving.accuracy_factor < still.accuracy_factor);
        assert(suppressed.accuracy_factor < moving.accuracy_factor);
    }
    {
        EnemyAimController controller;
        EnemyAimInput input;
        input.target_visible = true;
        input.target_id = "invalid";
        input.current_aim_direction = {0.0F, 0.0F, 0.0F};
        bool rejected = false;
        try { controller.update(input, 0.016F); }
        catch (const std::invalid_argument&) { rejected = true; }
        assert(rejected);

        input.current_aim_direction = {1.0F, 0.0F, 0.0F};
        input.skill = 1.2F;
        rejected = false;
        try { controller.update(input, 0.016F); }
        catch (const std::invalid_argument&) { rejected = true; }
        assert(rejected);
    }

    std::cout << "Enemy aim controller tests passed\n";
    return 0;
}
