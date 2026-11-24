#include "EnemyCombatRuntime.h"

namespace novafrontier {

EnemyCombatRuntime::EnemyCombatRuntime(EnemyCombatRuntimeConfig config)
    : ai_(config.ai), perception_(config.perception), weapon_(config.weapon) {}

EnemyCombatFrame EnemyCombatRuntime::update(const EnemyObservation& observation, float delta_seconds) {
    EnemyCombatFrame frame;
    frame.perception = perception_.update(observation, delta_seconds);
    frame.decision = ai_.update(frame.perception.ai_input, delta_seconds);
    weapon_.tick(delta_seconds);

    if (frame.decision.should_attack) {
        frame.shot = weapon_.try_fire();
    } else {
        frame.shot = {false, weapon_.magazine(), weapon_.reserve(), weapon_.current_spread(), 0.0F};
    }

    const auto state = frame.decision.state;
    if ((state == EnemyAIState::Chase || state == EnemyAIState::Search) &&
        frame.perception.has_last_known_player) {
        frame.has_navigation_target = true;
        frame.navigation_target = frame.perception.last_known_player_position;
    } else if (state == EnemyAIState::Investigate) {
        const bool heard_position_is_newer = frame.perception.has_last_heard_position &&
            (!frame.perception.has_last_known_player ||
             frame.perception.last_heard_age_seconds < frame.perception.last_seen_age_seconds);
        if (heard_position_is_newer) {
            frame.has_navigation_target = true;
            frame.navigation_target = frame.perception.last_heard_position;
        } else if (frame.perception.has_last_known_player) {
            frame.has_navigation_target = true;
            frame.navigation_target = frame.perception.last_known_player_position;
        } else if (frame.perception.has_last_heard_position) {
            frame.has_navigation_target = true;
            frame.navigation_target = frame.perception.last_heard_position;
        }
    }
    return frame;
}

} // namespace novafrontier
