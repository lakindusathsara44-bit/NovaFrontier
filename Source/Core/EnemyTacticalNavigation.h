#pragma once

#include "EnemyPerceptionMemory.h"

#include <cstddef>
#include <string>
#include <vector>

namespace novafrontier {

enum class PatrolRouteMode { Loop, PingPong };
enum class TacticalMovementMode { Hold, Patrol, MoveToTarget, MoveToCover };

struct EnemyCoverPoint {
    std::string id;
    Vector3 position{};
    bool reachable{true};                 // Supplied by the engine navigation layer.
    bool blocks_threat_line_of_sight{true}; // Supplied by an engine line trace.
    bool occupied{false};
    float quality{1.0F};                  // 0 = poor, 1 = excellent.
};

struct EnemyTacticalInput {
    EnemyAIState state{EnemyAIState::Patrol};
    Vector3 position{};
    bool has_threat_position{false};
    Vector3 threat_position{};
    bool has_navigation_target{false};
    Vector3 navigation_target{};
    bool under_fire{false};
    int health{100};
    int max_health{100};
    std::vector<Vector3> patrol_waypoints;
    std::vector<EnemyCoverPoint> cover_points;
};

struct EnemyTacticalNavigationConfig {
    float arrival_radius_cm{100.0F};
    float maximum_cover_distance_cm{1600.0F};
    float low_health_fraction{0.35F};
    float cover_distance_weight{1.0F};
    float cover_quality_weight{300.0F};
    float cover_switch_score_margin{150.0F};
    PatrolRouteMode patrol_route_mode{PatrolRouteMode::Loop};
};

struct EnemyTacticalDecision {
    TacticalMovementMode movement_mode{TacticalMovementMode::Hold};
    bool should_move{false};
    bool has_target{false};
    Vector3 target{};
    bool arrived{false};
    bool waypoint_advanced{false};
    std::size_t waypoint_index{0};
    bool has_cover{false};
    std::string cover_id;
    bool should_face_threat{false};
};

class EnemyTacticalNavigation {
public:
    explicit EnemyTacticalNavigation(EnemyTacticalNavigationConfig config = {});
    EnemyTacticalDecision update(const EnemyTacticalInput& input, float delta_seconds);
    void reset() noexcept;

    std::size_t waypoint_index() const noexcept { return waypoint_index_; }
    const std::string& selected_cover_id() const noexcept { return selected_cover_id_; }

private:
    EnemyTacticalNavigationConfig config_;
    std::size_t waypoint_index_{0};
    int patrol_direction_{1};
    std::string selected_cover_id_;
};

} // namespace novafrontier
