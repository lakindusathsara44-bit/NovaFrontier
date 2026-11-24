#pragma once

#include "GameRules.h"

namespace novafrontier {

struct WeaponCombatConfig {
    WeaponConfig ammunition{};
    float rounds_per_minute{600.0F};
    float base_spread_degrees{0.35F};
    float maximum_spread_degrees{2.5F};
    float spread_per_shot_degrees{0.25F};
    float spread_recovery_degrees_per_second{1.5F};
    float recoil_pitch_degrees{0.65F};
};

struct WeaponShotFeedback {
    bool fired{false};
    int magazine_after{};
    int reserve_after{};
    float spread_degrees{};
    float recoil_pitch_degrees{};
};

class WeaponCombat {
public:
    explicit WeaponCombat(WeaponCombatConfig config = {});

    // Advance cooldown and recover accumulated spread. Delta time is seconds.
    void tick(float delta_seconds);

    WeaponShotFeedback try_fire() noexcept;
    bool begin_reload() noexcept { return ammunition_.begin_reload(); }
    int complete_reload() noexcept;
    void cancel_reload() noexcept { ammunition_.cancel_reload(); }

    int magazine() const noexcept { return ammunition_.magazine(); }
    int reserve() const noexcept { return ammunition_.reserve(); }
    bool is_reloading() const noexcept { return ammunition_.is_reloading(); }
    float current_spread() const noexcept { return current_spread_degrees_; }
    float cooldown_remaining() const noexcept { return cooldown_remaining_seconds_; }

private:
    WeaponCombatConfig config_;
    WeaponAmmo ammunition_;
    float fire_interval_seconds_{};
    float cooldown_remaining_seconds_{};
    float current_spread_degrees_{};
};

} // namespace novafrontier
