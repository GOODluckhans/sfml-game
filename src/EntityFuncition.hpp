#pragma once
//You should start with Entity first.Here’s why :
//
//Entity is the base class for anything that exists in your game world : player, enemies, pickups, obstacles, etc.
//
//Once you have Entity, you can make Player and Enemy inherit from it, which keeps your code scalable and clean.
//
//If you skip Entity and jump straight to Enemy, you might end up duplicating logic later when you add more objects.

#include "MainLibraries/main.hpp"
//#include "Renderer.hpp"
//#include "Input.hpp"

class World;
class Input;
class Renderer;
class Physics;
class Camera;
class sceneStruct;
class Audio;
class EntityFunction {
public:
    EntityFunction() {}
    virtual ~EntityFunction() {}

    virtual void render(sf::RenderWindow& window, Renderer& renderer, Camera* camera = nullptr) {}


    // Every entity must be able to update itself
    // Every entity must be able to udate itself
    virtual void update(float& dt, World& world,Audio& audio, Input* input = nullptr, Physics* physics = nullptr, SceneType* type = nullptr) = 0;

    // Optionally, position getter/setter
    sf::Vector2f getPosition() const { return position; }
    void setPosition(const sf::Vector2f& pos) { position = pos; }
private: 
	sf::Vector2f position;

};