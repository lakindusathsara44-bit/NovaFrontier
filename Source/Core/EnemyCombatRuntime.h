#pragma once
#include "EnemyCombatAI.h"
#include "EnemyPerceptionMemory.h"
#include "WeaponCombat.h"
namespace novafrontier {
struct EnemyCombatRuntimeConfig {
    EnemyAIConfig ai{};
    EnemyPerceptionMemoryConfig perception{};
    WeaponCombatConfig weapon{};
};
struct EnemyCombatFrame {
    EnemyAIDecision decision{};
    EnemyPerceptionFrame perception{};
    bool has_navigation_target{false};
    Vector3 navigation_target{};
    WeaponShotFeedback shot{};
};
class EnemyCombatRuntime {
public:
    explicit EnemyCombatRuntime(EnemyCombatRuntimeConfig config = {});
    EnemyCombatFrame update(const EnemyObservation& observation, float delta_seconds);
    bool begin_reload() noexcept { return weapon_.begin_reload(); }
    int complete_reload() noexcept { return weapon_.complete_reload(); }
    void cancel_reload() noexcept { weapon_.cancel_reload(); }
    int magazine() const noexcept { return weapon_.magazine(); }
    int reserve() const noexcept { return weapon_.reserve(); }
    EnemyAIState state() const noexcept { return ai_.state(); }
private:
    EnemyCombatAI ai_;
    EnemyPerceptionMemory perception_;
    WeaponCombat weapon_;
};
} // namespace novafrontier
