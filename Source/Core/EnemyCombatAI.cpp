#include "EnemyCombatAI.h"

#include <cmath>
#include <stdexcept>

namespace novafrontier {
namespace {
bool valid_non_negative(float value) noexcept {
    return std::isfinite(value) && value >= 0.0F;
}
} // namespace

EnemyCombatAI::EnemyCombatAI(EnemyAIConfig config) : config_(config) {
    if (!valid_non_negative(config_.detection_range) ||
        !valid_non_negative(config_.attack_range) ||
        !valid_non_negative(config_.search_duration_seconds) ||
        !valid_non_negative(config_.investigate_duration_seconds) ||
        config_.attack_range > config_.detection_range) {
        throw std::invalid_argument("Invalid enemy AI configuration");
    }
}

EnemyAIDecision EnemyCombatAI::update(const EnemyPerception& perception, float delta_seconds) {
    if (!valid_non_negative(delta_seconds) || !valid_non_negative(perception.player_distance)) {
        throw std::invalid_argument("Enemy AI time and distance must be finite and non-negative");
    }
    if (state_ == EnemyAIState::Disabled) return make_decision();
    if (perception.health <= 0) {
        transition_to(EnemyAIState::Disabled);
        return make_decision();
    }

    state_elapsed_seconds_ += delta_seconds;
    const bool detected = perception.player_visible &&
                          perception.player_distance <= config_.detection_range;
    if (detected) {
        transition_to(perception.player_distance <= config_.attack_range
                          ? EnemyAIState::Attack : EnemyAIState::Chase);
        return make_decision();
    }

    switch (state_) {
    case EnemyAIState::Patrol:
        if (perception.noise_heard) transition_to(EnemyAIState::Investigate);
        break;
    case EnemyAIState::Investigate:
        if (perception.noise_heard) state_elapsed_seconds_ = 0.0F;
        else if (state_elapsed_seconds_ >= config_.investigate_duration_seconds)
            transition_to(EnemyAIState::Patrol);
        break;
    case EnemyAIState::Chase:
    case EnemyAIState::Attack:
        transition_to(EnemyAIState::Search);
        break;
    case EnemyAIState::Search:
        if (perception.noise_heard) transition_to(EnemyAIState::Investigate);
        else if (state_elapsed_seconds_ >= config_.search_duration_seconds)
            transition_to(EnemyAIState::Investigate);
        break;
    case EnemyAIState::Disabled:
        break;
    }
    return make_decision();
}

void EnemyCombatAI::reset() noexcept {
    state_ = EnemyAIState::Patrol;
    state_elapsed_seconds_ = 0.0F;
}

void EnemyCombatAI::transition_to(EnemyAIState next) noexcept {
    if (state_ != next) {
        state_ = next;
        state_elapsed_seconds_ = 0.0F;
    }
}

EnemyAIDecision EnemyCombatAI::make_decision() const noexcept {
    EnemyAIDecision result;
    result.state = state_;
    result.should_attack = state_ == EnemyAIState::Attack;
    result.should_move = state_ == EnemyAIState::Patrol ||
                         state_ == EnemyAIState::Investigate ||
                         state_ == EnemyAIState::Chase ||
                         state_ == EnemyAIState::Search;
    result.has_last_known_target = state_ == EnemyAIState::Investigate ||
                                   state_ == EnemyAIState::Chase ||
                                   state_ == EnemyAIState::Attack ||
                                   state_ == EnemyAIState::Search;
    return result;
}

} // namespace novafrontier
