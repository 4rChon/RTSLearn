#include <Game/Actions/Action.h>
#include <Game/Game.h>
#include <Game/Player.h>
#include <TypeDefs.h>
#include <memory>

Action::Action(const Vec2i& target, std::weak_ptr<Unit> actor, std::weak_ptr<Player> player, std::weak_ptr<Game> game, char sprite)
    : target(target)
    , actor(actor)
    , player(player)
    , game(game)
    , sprite(sprite) {
}

char Action::get_sprite() const {
    return sprite;
}
