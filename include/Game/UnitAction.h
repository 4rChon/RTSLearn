#pragma once
enum class UnitAction {
    Noop = 0,
    Move,
    Attack,
    Build,
    Train,
    Mine,
    ReturnResources
};