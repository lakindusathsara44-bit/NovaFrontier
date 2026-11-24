#include "GameRules.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

using namespace novafrontier;

int main() {
    {
        Health health(100);
        const auto hit = health.apply_damage(35);
        assert(hit.applied == 35 && health.current() == 65 && health.is_alive());
        assert(health.heal(50) == 35 && health.current() == 100);
        assert(health.apply_damage(-10).applied == 0);
        assert(health.heal(-10) == 0);
        assert(health.apply_damage(500).applied == 100 && !health.is_alive());
        assert(health.heal(20) == 0);
        health.reset();
        assert(health.current() == 100);
    }
    {
        WeaponAmmo rifle({3, 5});
        assert(rifle.fire().fired);
        assert(rifle.fire().fired);
        assert(rifle.begin_reload());
        assert(!rifle.fire().fired);
        assert(rifle.complete_reload() == 1);
        assert(rifle.magazine() == 3 && rifle.reserve() == 4);
        assert(!rifle.begin_reload());
        assert(rifle.fire().fired);
        assert(rifle.begin_reload());
        rifle.cancel_reload();
        assert(!rifle.is_reloading() && rifle.complete_reload() == 0);
    }
    {
        ObjectiveSequence mission({{"reach", "Reach the relay", ObjectiveStatus::Active},
                                   {"restore", "Restore power", ObjectiveStatus::Locked}});
        assert(mission.active() && mission.active()->id == "reach");
        assert(!mission.complete_active("restore"));
        assert(mission.complete_active("reach"));
        assert(mission.active() && mission.active()->id == "restore");
        assert(mission.complete_active("restore") && mission.is_complete());
    }
    {
        bool threw = false;
        try { Health invalid(0); } catch (const std::invalid_argument&) { threw = true; }
        assert(threw);
        threw = false;
        try {
            ObjectiveSequence invalid({{"same", "A", ObjectiveStatus::Locked},
                                       {"same", "B", ObjectiveStatus::Locked}});
        } catch (const std::invalid_argument&) { threw = true; }
        assert(threw);
    }
    std::cout << "NOVA: FRONTIER core rules tests passed.\n";
    return 0;
}
