#include "FirstPersonController.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace novafrontier;
int main() {
    FirstPersonController player;
    auto state = player.update({{1.0F, 1.0F}}, true);
    assert(std::fabs(state.normalized_move.x - 0.7071067F) < 0.0001F);
    assert(std::fabs(state.normalized_move.y - 0.7071067F) < 0.0001F);
    assert(state.movement_speed == 450.0F);
    state = player.update({{1.0F, 0.0F}, {}, true}, true);
    assert(state.sprinting && state.movement_speed == 700.0F);
    state = player.update({{}, {}, true}, true);
    assert(!state.sprinting && state.movement_speed == 450.0F);
    state = player.update({{1.0F, 0.0F}, {}, true, true, true}, true);
    assert(state.crouched && !state.sprinting && !state.jump_requested);
    assert(state.movement_speed == 220.0F);
    state = player.update({}, true);
    assert(state.crouched);
    state = player.update({{}, {}, false, true, false}, true);
    assert(!state.crouched);
    state = player.update({{}, {}, false, false, true}, true);
    assert(state.jump_requested && state.jump_velocity == 520.0F);
    state = player.update({{}, {}, false, false, true}, false);
    assert(!state.jump_requested && state.jump_velocity == 0.0F);
    state = player.update({{}, {725.0F, -200.0F}}, true);
    assert(std::fabs(state.yaw_degrees - 5.0F) < 0.001F);
    assert(state.pitch_degrees == 80.0F);
    player.reset_view(-10.0F, -100.0F);
    assert(player.state().yaw_degrees == 350.0F);
    assert(player.state().pitch_degrees == -80.0F);
    bool threw = false;
    try { FirstPersonController invalid({500.0F, 400.0F}); }
    catch (const std::invalid_argument&) { threw = true; }
    assert(threw);
    std::cout << "NOVA: FRONTIER first-person foundation tests passed.\n";
    return 0;
}
