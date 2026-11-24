#include "EnemyCombatRuntime.h"
#include <cassert>
#include <iostream>
#include <stdexcept>
using namespace novafrontier;
int main() {
    EnemyCombatRuntime runtime;
    EnemyObservation observation;
    observation.player_visible = true;
    observation.player_position = {100.0F, 200.0F, 0.0F};
    observation.player_distance_cm = 120.0F;
    auto frame = runtime.update(observation, 0.016F);
    assert(frame.decision.state == EnemyAIState::Attack);
    assert(frame.decision.should_attack && frame.shot.fired);
    assert(frame.shot.magazine_after == 29);
    assert(!frame.has_navigation_target);

    frame = runtime.update(observation, 0.01F);
    assert(frame.decision.state == EnemyAIState::Attack && !frame.shot.fired);
    frame = runtime.update(observation, 0.1F);
    assert(frame.shot.fired && frame.shot.magazine_after == 28);

    observation.player_visible = false;
    frame = runtime.update(observation, 0.016F);
    assert(frame.decision.state == EnemyAIState::Search);
    assert(frame.perception.has_last_known_player && frame.has_navigation_target);
    assert(frame.navigation_target.x == 100.0F && !frame.shot.fired);

    observation.noise_heard = true;
    observation.noise_position = {900.0F, 0.0F, 0.0F};
    observation.noise_distance_cm = 1500.0F;
    frame = runtime.update(observation, 0.016F);
    assert(!frame.perception.ai_input.noise_heard);

    observation.noise_distance_cm = 500.0F;
    observation.noise_position = {700.0F, 50.0F, 0.0F};
    frame = runtime.update(observation, 0.016F);
    assert(frame.perception.ai_input.noise_heard);
    assert(frame.perception.has_last_heard_position);

    EnemyPerceptionMemory memory({1200.0F, 0.5F, 0.5F});
    EnemyObservation seen;
    seen.player_visible = true;
    seen.player_position = {1.0F, 2.0F, 3.0F};
    auto memory_frame = memory.update(seen, 0.01F);
    assert(memory_frame.has_last_known_player);
    seen.player_visible = false;
    memory_frame = memory.update(seen, 0.5F);
    assert(!memory_frame.has_last_known_player);

    bool rejected = false;
    try { seen.player_distance_cm = -1.0F; memory.update(seen, 0.01F); }
    catch (const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    runtime.reset();
    assert(runtime.state() == EnemyAIState::Patrol);
    std::cout << "Enemy combat runtime tests passed\n";
    return 0;
}
