#pragma once 

#include "MainLibraries/main.hpp"
enum class EntityStates {
    Idle,
    Moving,
    Attacking,
    Jumping
};
//class Player;
class World;
class Animation;
class State {
public:
    State() {}
    virtual ~State() {}

    virtual void changeState(EntityStates& state) = 0; // animation
   virtual void enter(/*Player& player*/) = 0; // animation
   virtual void updateState(/*Player& player, */float dt,World &world, Animation& anim) = 0;// If pressing movement, switch to walk state
   virtual void exit(/*Player& player*/) = 0;
protected:
    EntityStates state = EntityStates::Idle;
};