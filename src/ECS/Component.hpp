#pragma once 
#include "../MainLibraries/main.hpp"
//Entity component
struct Velocity {
    float vx = 0.f;
    float vy = 0.f;
};

struct Position {
    float x = 0, y = 0;
};

struct Size {

    float x = 0.0f;
    float y = 0.0f;
};

struct SpriteRender {
 
    sf::Texture* texture = nullptr;


};

//gamplay component 

//struct Health {
//    int hp;
//    int maxHp;
//};
//
//struct Damage {
//    int amount;
//};

