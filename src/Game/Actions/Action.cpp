#include <Game/Actions/Action.h>
#include <Game/Game.h>
#include <Game/Player.h>
#include <TypeDefs.h>
#include <memory>

Action::Action(const vec2& target, Unit& actor, Player& player, Game& game, char sprite)
    : target(target)
    , actor(&actor)
    , player(&player)
    , game(&game)
    , sprite(sprite) {
}

char Action::get_sprite() const {
    return sprite;
}
