#include <Game/Actions/ActionResult.h>
#include <Game/Constants.h>
#include <Game/PlayerInput.h>
#include <Game/Unit.h>
#include <Game/UnitType.h>
#include <TypeDefs.h>
#include <memory>
#include <string>
#include <sstream>
#include <mutex>

int Unit::next_id = 0;

Unit::Unit(UnitType type, const vec2& position, int owner)
    : type(type)
    , position(position)
    , owner(owner)
    , id(next_id++)
    , health(Constants::unit_health.at(type)) {}

void Unit::set_position(const vec2& position) {
    this->position = position;
}

int Unit::get_id() const {
    return id;
}

int Unit::get_owner() const {
    return owner;
}

const vec2& Unit::get_position() const {
    return position;
}

UnitType Unit::get_type() const {
    return type;
}

const std::string Unit::get_sprite() {
    std::lock_guard<std::mutex> guard(action_queue_lock);
    auto action_sprite = action_queue.empty() ? '.' : action_queue.front()->get_sprite();
    std::stringstream ss;
    ss << "\x1b[1;3" << owner + 5 << "m" << action_sprite << Constants::unit_sprite.at(type) << "\x1b[m";
    return ss.str();
}

void Unit::enqueue_action(std::unique_ptr<Action> player_input, bool replace_current_action) {
    std::lock_guard<std::mutex> guard(action_queue_lock);
    if (replace_current_action && !action_queue.empty()) {
        action_queue.front()->cancel();
        action_queue.pop();
    }

    action_queue.push(std::move(player_input));
}

void Unit::act() {
    if (action_queue.empty()) {
        return;
    }

    auto& action = action_queue.front();
    auto result = action->act();

    switch (result) {
        case ActionResult::Success: {
            std::lock_guard<std::mutex> guard(action_queue_lock);
            action_queue.pop();
            break;
        }
        case ActionResult::Failure: {
            action->cancel();
            std::lock_guard<std::mutex> guard(action_queue_lock);
            action_queue.pop();
            break;
        }
        case ActionResult::Running: {
            break;
        }
    }
}

std::vector<ActionType> Unit::get_available_actions() const {
    switch (type) {
        case UnitType::Worker:
            return { ActionType::Noop, ActionType::Click, ActionType::BuildTownHall, ActionType::BuildFarm, ActionType::BuildBarracks };
        case UnitType::TownHall:
            return { ActionType::Noop, ActionType::Click, ActionType::TrainWorker };
        case UnitType::Barracks:
            return { ActionType::Noop, ActionType::Click, ActionType::TrainMeleeWarrior, ActionType::TrainRangedWarrior };
        case UnitType::Farm:
            return { ActionType::Noop, ActionType::Click };
        case UnitType::MeleeWarrior:
            return { ActionType::Noop, ActionType::Click };
        case UnitType::RangedWarrior:
            return { ActionType::Noop, ActionType::Click };
        default:
            return { ActionType::Noop, ActionType::Click };
    }
}

bool Unit::modify_health(int amount) {
    health += amount;

    return is_alive();
}

bool Unit::is_alive() const {
    return health > 0;
}
