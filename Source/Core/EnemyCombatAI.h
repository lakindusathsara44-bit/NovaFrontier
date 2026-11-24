#pragma once

#include <cstdint>

namespace novafrontier {

enum class EnemyAIState : std::uint8_t { Patrol, Investigate, Chase, Attack, Search, Disabled };

struct EnemyAIConfig {
    float detection_range{1800.0F};
    float attack_range{250.0F};
    float search_duration_seconds{4.0F};
    float investigate_duration_seconds{5.0F};
};

struct EnemyPerception {
    bool player_visible{false};
    bool noise_heard{false}; // One-frame event, not a persistent level.
    float player_distance{0.0F}; // Centimetres.
    int health{100};
};

struct EnemyAIDecision {
    EnemyAIState state{EnemyAIState::Patrol};
    bool should_move{true};
    bool should_attack{false};
    bool has_last_known_target{false};
};

class EnemyCombatAI {
public:
    explicit EnemyCombatAI(EnemyAIConfig config = {});
    EnemyAIDecision update(const EnemyPerception& perception, float delta_seconds);
    void reset() noexcept;
    EnemyAIState state() const noexcept { return state_; }
    float state_elapsed_seconds() const noexcept { return state_elapsed_seconds_; }

private:
    void transition_to(EnemyAIState next) noexcept;
    EnemyAIDecision make_decision() const noexcept;
    EnemyAIConfig config_;
    EnemyAIState state_{EnemyAIState::Patrol};
    float state_elapsed_seconds_{0.0F};
};

} // namespace novafrontier
