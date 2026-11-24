#include "WeaponCombat.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace novafrontier;

int main() {
    WeaponCombatConfig config;
    config.ammunition = {3, 5};
    config.rounds_per_minute = 600.0F;
    config.base_spread_degrees = 0.5F;
    config.maximum_spread_degrees = 1.0F;
    config.spread_per_shot_degrees = 0.25F;
    config.spread_recovery_degrees_per_second = 0.5F;
    config.recoil_pitch_degrees = 0.7F;

    WeaponCombat rifle(config);
    auto shot = rifle.try_fire();
    assert(shot.fired && shot.magazine_after == 2 && shot.reserve_after == 5);
    assert(std::fabs(shot.spread_degrees - 0.5F) < 0.0001F);
    assert(std::fabs(shot.recoil_pitch_degrees - 0.7F) < 0.0001F);

    // Fire-rate gate blocks an immediate second shot.
    shot = rifle.try_fire();
    assert(!shot.fired && rifle.magazine() == 2);

    rifle.tick(0.1F);
    shot = rifle.try_fire();
    assert(shot.fired && rifle.magazine() == 1);
    assert(std::fabs(shot.spread_degrees - 0.70F) < 0.0001F);

    // Spread recovery never falls below base spread.
    rifle.tick(2.0F);
    assert(std::fabs(rifle.current_spread() - 0.5F) < 0.0001F);

    // Reload blocks firing and transfers only the needed rounds.
    assert(rifle.begin_reload());
    assert(!rifle.try_fire().fired);
    assert(rifle.complete_reload() == 2);
    assert(rifle.magazine() == 3 && rifle.reserve() == 3);
    assert(!rifle.begin_reload());

    // Empty magazine and exhausted reserve are handled safely.
    rifle.tick(0.1F);
    assert(rifle.try_fire().fired);
    rifle.tick(0.1F);
    assert(rifle.try_fire().fired);
    rifle.tick(0.1F);
    assert(rifle.try_fire().fired);
    rifle.tick(0.1F);
    assert(!rifle.try_fire().fired && rifle.magazine() == 0);
    assert(rifle.begin_reload());
    assert(rifle.complete_reload() == 3);
    assert(rifle.magazine() == 3 && rifle.reserve() == 0);

    bool threw = false;
    try { rifle.tick(-0.1F); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);
    threw = false;
    try {
        WeaponCombatConfig invalid;
        invalid.rounds_per_minute = 0.0F;
        WeaponCombat broken(invalid);
    } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    std::cout << "NOVA: FRONTIER KX-9 combat tests passed.\n";
    return 0;
}
