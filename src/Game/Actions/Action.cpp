#include <Game/Actions/Action.h>
#include <Game/Game.h>
#include <Game/Player.h>
#include <TypeDefs.h>
#include <memory>

Action::Action(const Vec2i& target, std::weak_ptr<Unit> actor, std::weak_ptr<Player> player, std::weak_ptr<Game> game)
    : target(target)
    , actor(actor)
    , player(player)
    , game(game) {
}