#pragma once
#include "EnemyCombatAI.h"
namespace novafrontier {
struct Vector3 { float x{0.0F}; float y{0.0F}; float z{0.0F}; };
struct EnemyObservation {
    bool player_visible{false};
    Vector3 player_position{};
    float player_distance_cm{0.0F};
    bool noise_heard{false}; // One-frame event from the engine perception layer.
    Vector3 noise_position{};
    float noise_distance_cm{0.0F};
    int health{100};
};
struct EnemyPerceptionMemoryConfig {
    float hearing_range_cm{1200.0F};
    float player_memory_duration_seconds{8.0F};
    float noise_memory_duration_seconds{6.0F};
};
struct EnemyPerceptionFrame {
    EnemyPerception ai_input{};
    bool has_last_known_player{false};
    Vector3 last_known_player_position{};
    float last_seen_age_seconds{0.0F};
    bool has_last_heard_position{false};
    Vector3 last_heard_position{};
    float last_heard_age_seconds{0.0F};
};
class EnemyPerceptionMemory {
public:
    explicit EnemyPerceptionMemory(EnemyPerceptionMemoryConfig config = {});
    EnemyPerceptionFrame update(const EnemyObservation& observation, float delta_seconds);
    void reset() noexcept;
private:
    EnemyPerceptionMemoryConfig config_;
    bool has_last_known_player_{false};
    Vector3 last_known_player_position_{};
    float last_seen_age_seconds_{0.0F};
    bool has_last_heard_position_{false};
    Vector3 last_heard_position_{};
    float last_heard_age_seconds_{0.0F};
};
} // namespace novafrontier
