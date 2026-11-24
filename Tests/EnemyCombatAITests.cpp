#include "EnemyCombatAI.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

using namespace novafrontier;

int main() {
    EnemyCombatAI ai;
    auto decision = ai.update({false, true, 900.0F, 100}, 0.016F);
    assert(decision.state == EnemyAIState::Investigate);
    assert(decision.should_move && !decision.should_attack && decision.has_last_known_target);

    decision = ai.update({true, false, 900.0F, 100}, 0.016F);
    assert(decision.state == EnemyAIState::Chase);
    assert(decision.should_move && !decision.should_attack);

    decision = ai.update({true, false, 120.0F, 100}, 0.016F);
    assert(decision.state == EnemyAIState::Attack);
    assert(decision.should_attack && !decision.should_move);

    decision = ai.update({false, false, 120.0F, 100}, 0.016F);
    assert(decision.state == EnemyAIState::Search);
    assert(decision.should_move && !decision.should_attack);

    decision = ai.update({false, false, 120.0F, 100}, 4.1F);
    assert(decision.state == EnemyAIState::Investigate);
    decision = ai.update({false, false, 120.0F, 100}, 5.1F);
    assert(decision.state == EnemyAIState::Patrol && !decision.has_last_known_target);

    decision = ai.update({true, false, 100.0F, 0}, 0.016F);
    assert(decision.state == EnemyAIState::Disabled);
    assert(!decision.should_move && !decision.should_attack);
    decision = ai.update({true, false, 100.0F, 100}, 1.0F);
    assert(decision.state == EnemyAIState::Disabled);
    ai.reset();
    assert(ai.state() == EnemyAIState::Patrol);

    bool rejected_bad_delta = false;
    try { ai.update({false, false, 10.0F, 100}, -0.1F); }
    catch (const std::invalid_argument&) { rejected_bad_delta = true; }
    assert(rejected_bad_delta);

    bool rejected_bad_config = false;
    try { EnemyCombatAI invalid({100.0F, 200.0F, 4.0F, 5.0F}); }
    catch (const std::invalid_argument&) { rejected_bad_config = true; }
    assert(rejected_bad_config);

    std::cout << "Enemy combat AI tests passed\n";
    return 0;
}
