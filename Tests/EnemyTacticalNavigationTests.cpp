#include "EnemyTacticalNavigation.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

using namespace novafrontier;

int main() {
    {
        EnemyTacticalNavigation navigation;
        EnemyTacticalInput input;
        input.state = EnemyAIState::Patrol;
        input.position = {-200.0F, 0.0F, 0.0F};
        input.patrol_waypoints = {{0.0F, 0.0F, 0.0F}, {500.0F, 0.0F, 0.0F},
                                  {500.0F, 500.0F, 0.0F}};
        auto decision = navigation.update(input, 0.016F);
        assert(decision.movement_mode == TacticalMovementMode::Patrol);
        assert(decision.should_move && decision.target.x == 0.0F);
        input.position = {0.0F, 0.0F, 0.0F};
        decision = navigation.update(input, 0.016F);
        assert(decision.waypoint_advanced && decision.waypoint_index == 1U);
        assert(decision.target.x == 500.0F);
    }
    {
        EnemyTacticalNavigationConfig config;
        config.patrol_route_mode = PatrolRouteMode::PingPong;
        EnemyTacticalNavigation navigation(config);
        EnemyTacticalInput input;
        input.patrol_waypoints = {{0.0F, 0.0F, 0.0F}, {1000.0F, 0.0F, 0.0F},
                                  {2000.0F, 0.0F, 0.0F}};
        input.position = input.patrol_waypoints[0];
        auto decision = navigation.update(input, 0.016F);
        assert(decision.waypoint_index == 1U);
        input.position = input.patrol_waypoints[1];
        decision = navigation.update(input, 0.016F);
        assert(decision.waypoint_index == 2U);
        input.position = input.patrol_waypoints[2];
        decision = navigation.update(input, 0.016F);
        assert(decision.waypoint_index == 1U);
    }
    {
        EnemyTacticalNavigation navigation;
        EnemyTacticalInput input;
        input.state = EnemyAIState::Chase;
        input.has_navigation_target = true;
        input.navigation_target = {800.0F, 50.0F, 0.0F};
        auto decision = navigation.update(input, 0.016F);
        assert(decision.movement_mode == TacticalMovementMode::MoveToTarget);
        assert(decision.should_move && decision.target.x == 800.0F);
        input.position = input.navigation_target;
        decision = navigation.update(input, 0.016F);
        assert(decision.arrived && !decision.should_move);
    }
    {
        EnemyTacticalNavigation navigation;
        EnemyTacticalInput input;
        input.state = EnemyAIState::Attack;
        input.has_threat_position = true;
        input.threat_position = {1000.0F, 0.0F, 0.0F};
        input.under_fire = true;
        input.cover_points = {
            {"far_good", {900.0F, 0.0F, 0.0F}, true, true, false, 1.0F},
            {"near_bad", {200.0F, 0.0F, 0.0F}, true, true, false, 0.1F},
            {"blocked", {100.0F, 0.0F, 0.0F}, true, false, false, 1.0F},
            {"occupied", {150.0F, 0.0F, 0.0F}, true, true, true, 1.0F},
            {"unreachable", {120.0F, 0.0F, 0.0F}, false, true, false, 1.0F}
        };
        auto decision = navigation.update(input, 0.016F);
        assert(decision.movement_mode == TacticalMovementMode::MoveToCover);
        assert(decision.has_cover && decision.cover_id == "near_bad");
        assert(decision.target.x == 200.0F && decision.should_move);
        input.position = decision.target;
        decision = navigation.update(input, 0.016F);
        assert(decision.has_cover && decision.arrived && !decision.should_move);
    }
    {
        EnemyTacticalNavigation navigation;
        EnemyTacticalInput input;
        input.state = EnemyAIState::Chase;
        input.has_threat_position = true;
        input.under_fire = true;
        input.cover_points = {
            {"stable", {700.0F, 0.0F, 0.0F}, true, true, false, 0.5F}
        };
        auto decision = navigation.update(input, 0.016F);
        assert(decision.cover_id == "stable");
        input.cover_points.push_back({"slightly_better", {650.0F, 0.0F, 0.0F},
                                      true, true, false, 0.5F});
        decision = navigation.update(input, 0.016F);
        assert(decision.cover_id == "stable"); // Hysteresis prevents frame-to-frame cover jitter.
    }
    {
        EnemyTacticalNavigation navigation;
        EnemyTacticalInput input;
        input.state = EnemyAIState::Chase;
        input.health = 20;
        input.max_health = 100;
        input.has_threat_position = true;
        input.cover_points = {{"low_health_cover", {250.0F, 0.0F, 0.0F},
                               true, true, false, 0.8F}};
        auto decision = navigation.update(input, 0.016F);
        assert(decision.movement_mode == TacticalMovementMode::MoveToCover);
        assert(decision.cover_id == "low_health_cover");

        input.health = 0;
        decision = navigation.update(input, 0.016F);
        assert(decision.movement_mode == TacticalMovementMode::Hold);
        assert(!decision.has_cover && !decision.should_move);
    }
    {
        EnemyTacticalNavigation navigation;
        EnemyTacticalInput input;
        input.state = EnemyAIState::Chase;
        input.has_navigation_target = true;
        input.navigation_target = {400.0F, 0.0F, 0.0F};
        input.has_threat_position = true;
        input.under_fire = true;
        input.cover_points = {{"blocked", {100.0F, 0.0F, 0.0F}, true, false, false, 1.0F}};
        auto decision = navigation.update(input, 0.016F);
        assert(decision.movement_mode == TacticalMovementMode::MoveToTarget);
        assert(!decision.has_cover && decision.target.x == 400.0F);
    }
    {
        EnemyTacticalNavigation navigation;
        EnemyTacticalInput input;
        input.state = EnemyAIState::Disabled;
        input.has_threat_position = true;
        input.under_fire = true;
        input.cover_points = {{"cover", {100.0F, 0.0F, 0.0F}, true, true, false, 1.0F}};
        auto decision = navigation.update(input, 0.016F);
        assert(decision.movement_mode == TacticalMovementMode::Hold);
        assert(!decision.should_move && !decision.has_cover);
    }
    {
        bool rejected = false;
        try {
            EnemyTacticalNavigationConfig config;
            config.low_health_fraction = 1.5F;
            EnemyTacticalNavigation invalid(config);
        } catch (const std::invalid_argument&) { rejected = true; }
        assert(rejected);

        EnemyTacticalNavigation navigation;
        EnemyTacticalInput input;
        input.cover_points = {
            {"same", {0.0F, 0.0F, 0.0F}, true, true, false, 1.0F},
            {"same", {1.0F, 0.0F, 0.0F}, true, true, false, 1.0F}
        };
        rejected = false;
        try { navigation.update(input, 0.016F); }
        catch (const std::invalid_argument&) { rejected = true; }
        assert(rejected);
    }

    std::cout << "Enemy tactical navigation tests passed\n";
    return 0;
}
