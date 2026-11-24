#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace novafrontier {

struct DamageResult {
    int health_before{};
    int health_after{};
    int applied{};
    bool was_alive{};
    bool is_alive{};
};

class Health {
public:
    explicit Health(int maximum = 100);
    int current() const noexcept { return current_; }
    int maximum() const noexcept { return maximum_; }
    bool is_alive() const noexcept { return current_ > 0; }
    DamageResult apply_damage(int amount) noexcept;
    int heal(int amount) noexcept;
    void reset() noexcept { current_ = maximum_; }

private:
    int maximum_;
    int current_;
};

struct WeaponConfig {
    int magazine_capacity{30};
    int reserve_ammo{90};
};

struct ShotResult {
    bool fired{};
    int magazine_after{};
    int reserve_after{};
};

class WeaponAmmo {
public:
    explicit WeaponAmmo(WeaponConfig config = {});
    int magazine() const noexcept { return magazine_; }
    int reserve() const noexcept { return reserve_; }
    int capacity() const noexcept { return capacity_; }
    bool is_reloading() const noexcept { return reloading_; }
    ShotResult fire() noexcept;
    bool begin_reload() noexcept;
    int complete_reload() noexcept;
    void cancel_reload() noexcept { reloading_ = false; }

private:
    int capacity_;
    int magazine_;
    int reserve_;
    bool reloading_{false};
};

enum class ObjectiveStatus : std::uint8_t { Locked, Active, Completed };

struct Objective {
    std::string id;
    std::string title;
    ObjectiveStatus status{ObjectiveStatus::Locked};
};

class ObjectiveSequence {
public:
    explicit ObjectiveSequence(std::vector<Objective> objectives);
    const std::vector<Objective>& all() const noexcept { return objectives_; }
    const Objective* active() const noexcept;
    bool complete_active(const std::string& id) noexcept;
    bool is_complete() const noexcept;

private:
    std::vector<Objective> objectives_;
};

} // namespace novafrontier
