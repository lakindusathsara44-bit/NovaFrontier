#include "WeaponCombat.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace novafrontier {

WeaponCombat::WeaponCombat(WeaponCombatConfig config)
    : config_(config),
      ammunition_(config.ammunition),
      fire_interval_seconds_(60.0F / config.rounds_per_minute),
      current_spread_degrees_(config.base_spread_degrees) {
    if (!std::isfinite(config.rounds_per_minute) || config.rounds_per_minute <= 0.0F ||
        !std::isfinite(config.base_spread_degrees) || config.base_spread_degrees < 0.0F ||
        !std::isfinite(config.maximum_spread_degrees) ||
        config.maximum_spread_degrees < config.base_spread_degrees ||
        !std::isfinite(config.spread_per_shot_degrees) || config.spread_per_shot_degrees < 0.0F ||
        !std::isfinite(config.spread_recovery_degrees_per_second) ||
        config.spread_recovery_degrees_per_second < 0.0F ||
        !std::isfinite(config.recoil_pitch_degrees) || config.recoil_pitch_degrees < 0.0F) {
        throw std::invalid_argument("Invalid weapon combat configuration");
    }
}

void WeaponCombat::tick(float delta_seconds) {
    if (!std::isfinite(delta_seconds) || delta_seconds < 0.0F)
        throw std::invalid_argument("Weapon delta time must be finite and non-negative");

    cooldown_remaining_seconds_ = std::max(0.0F, cooldown_remaining_seconds_ - delta_seconds);
    current_spread_degrees_ = std::max(
        config_.base_spread_degrees,
        current_spread_degrees_ - config_.spread_recovery_degrees_per_second * delta_seconds);
}

WeaponShotFeedback WeaponCombat::try_fire() noexcept {
    if (cooldown_remaining_seconds_ > 0.000001F || ammunition_.is_reloading())
        return {false, magazine(), reserve(), current_spread_degrees_, 0.0F};

    const ShotResult shot = ammunition_.fire();
    if (!shot.fired)
        return {false, shot.magazine_after, shot.reserve_after, current_spread_degrees_, 0.0F};

    const float shot_spread = current_spread_degrees_;
    current_spread_degrees_ = std::min(
        config_.maximum_spread_degrees,
        current_spread_degrees_ + config_.spread_per_shot_degrees);
    cooldown_remaining_seconds_ = fire_interval_seconds_;

    return {true, shot.magazine_after, shot.reserve_after, shot_spread,
            config_.recoil_pitch_degrees};
}

int WeaponCombat::complete_reload() noexcept {
    const int transferred = ammunition_.complete_reload();
    if (transferred > 0) cooldown_remaining_seconds_ = std::max(0.0F, cooldown_remaining_seconds_);
    return transferred;
}

} // namespace novafrontier
