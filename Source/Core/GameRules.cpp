#include "GameRules.h"

#include <algorithm>
#include <stdexcept>
#include <unordered_set>
#include <utility>

namespace novafrontier {

Health::Health(int maximum) : maximum_(maximum), current_(maximum) {
    if (maximum <= 0) throw std::invalid_argument("Health maximum must be positive");
}

DamageResult Health::apply_damage(int amount) noexcept {
    const int before = current_;
    const int applied = std::clamp(amount, 0, current_);
    current_ -= applied;
    return {before, current_, applied, before > 0, current_ > 0};
}

int Health::heal(int amount) noexcept {
    if (amount <= 0 || !is_alive()) return 0;
    const int restored = std::min(amount, maximum_ - current_);
    current_ += restored;
    return restored;
}

WeaponAmmo::WeaponAmmo(WeaponConfig config)
    : capacity_(config.magazine_capacity),
      magazine_(config.magazine_capacity),
      reserve_(config.reserve_ammo) {
    if (config.magazine_capacity <= 0 || config.reserve_ammo < 0)
        throw std::invalid_argument("Invalid weapon ammunition configuration");
}

ShotResult WeaponAmmo::fire() noexcept {
    if (reloading_ || magazine_ <= 0) return {false, magazine_, reserve_};
    --magazine_;
    return {true, magazine_, reserve_};
}

bool WeaponAmmo::begin_reload() noexcept {
    if (reloading_ || magazine_ == capacity_ || reserve_ == 0) return false;
    reloading_ = true;
    return true;
}

int WeaponAmmo::complete_reload() noexcept {
    if (!reloading_) return 0;
    const int needed = capacity_ - magazine_;
    const int transferred = std::min(needed, reserve_);
    magazine_ += transferred;
    reserve_ -= transferred;
    reloading_ = false;
    return transferred;
}

ObjectiveSequence::ObjectiveSequence(std::vector<Objective> objectives)
    : objectives_(std::move(objectives)) {
    std::unordered_set<std::string> ids;
    bool found_incomplete = false;
    for (auto& objective : objectives_) {
        if (objective.id.empty() || objective.title.empty())
            throw std::invalid_argument("Objective id and title must not be empty");
        if (!ids.insert(objective.id).second)
            throw std::invalid_argument("Objective ids must be unique");
        if (objective.status == ObjectiveStatus::Active) {
            if (found_incomplete) throw std::invalid_argument("Only the first incomplete objective can be active");
            found_incomplete = true;
        } else if (objective.status == ObjectiveStatus::Locked && !found_incomplete) {
            objective.status = ObjectiveStatus::Active;
            found_incomplete = true;
        } else if (objective.status == ObjectiveStatus::Completed && found_incomplete) {
            throw std::invalid_argument("Completed objectives cannot follow an incomplete objective");
        }
    }
}

const Objective* ObjectiveSequence::active() const noexcept {
    const auto it = std::find_if(objectives_.begin(), objectives_.end(), [](const Objective& o) {
        return o.status == ObjectiveStatus::Active;
    });
    return it == objectives_.end() ? nullptr : &*it;
}

bool ObjectiveSequence::complete_active(const std::string& id) noexcept {
    Objective* current = nullptr;
    for (auto& objective : objectives_) {
        if (objective.status == ObjectiveStatus::Active) { current = &objective; break; }
    }
    if (current == nullptr || current->id != id) return false;
    current->status = ObjectiveStatus::Completed;
    const auto next = std::find_if(objectives_.begin(), objectives_.end(), [](const Objective& o) {
        return o.status == ObjectiveStatus::Locked;
    });
    if (next != objectives_.end()) next->status = ObjectiveStatus::Active;
    return true;
}

bool ObjectiveSequence::is_complete() const noexcept {
    return std::all_of(objectives_.begin(), objectives_.end(), [](const Objective& o) {
        return o.status == ObjectiveStatus::Completed;
    });
}

} // namespace novafrontier
