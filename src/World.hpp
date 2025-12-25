#pragma once
//🌍 World.hpp / World.cpp
//
//This manages :
//
//the map
//
//simple collisions
//
//enemies(if you add them)
//
class Player;
//The Player will be updated based on what happens in the World.
#include "MainLibraries/main.hpp"
//#include "Entity.hpp"
#include "Renderer.hpp"
#include "EntityFuncition.hpp"
#include "Player.hpp"
//#include "Input.hpp"
//#include "Physics.hpp"
#include "ECS/ComponentStorage.hpp"
#include "ECS/EntityManager.hpp"


class World : public EntityFunction {
public:
	World() {}
	Entity obj;
	EntityManager entityManager;
	ComponentStorage<sf::RectangleShape> componentStorage;
	float& getDeltaTime() {
		dt = clock.restart().asSeconds();
		return dt;
	}

	Velocity& getVelocityPlayer(Velocity& vel) {
		return vel;
	}


	Velocity& getVelocityEnemy(Velocity& vel) {
		return vel;
	}


	void Shape() {

		obj = entityManager.createEntity();

		componentStorage.positions[obj].x = 300.f;
		componentStorage.positions[obj].y = 400.f;
		componentStorage.sizes[obj].x = 200.f;
		componentStorage.sizes[obj].y = 50.f;

		sf::RectangleShape wall;
		wall.setSize({ componentStorage.sizes[obj].x, componentStorage.sizes[obj].y });
		wall.setFillColor(sf::Color::Red);
		wall.setPosition({ componentStorage.positions[obj].x, componentStorage.positions[obj].y });
		wall.rotate(sf::degrees(100));
		walls.push_back(wall);
	}

	void render(sf::RenderWindow& window, Renderer& renderer, Camera* camera = nullptr) override {
		std::string wall = "wall";
		for(auto& a : getWalls())
			renderer.drawPlayer( a,wall);
	
	}

	void update(float &dt, World& world, Audio& audio, Input* input = nullptr , Physics* physics = nullptr, SceneType* type = nullptr) override {
		// World doesn’t move, so this can be empty
	}
	// Access walls for rendering or collision
	std::vector<sf::RectangleShape>& getWalls() { return walls; }

private:
	std::vector<sf::RectangleShape> walls;
	sf::Clock clock;
	float dt = 0;
	//sf::Vector2f velocityPlayer = { 0.f, 0.f };

	//sf::Vector2f velocityEnemy = { 0.f, 0.f };
};