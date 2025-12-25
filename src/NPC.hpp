#pragma once 

#include "MainLibraries/main.hpp"
#include "Physics.hpp"
#include "Renderer.hpp"
#include "EntityFuncition.hpp"
#include "World.hpp"
#include "Input.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/ComponentStorage.hpp"
#include "EntityStates.hpp"
#include "Animation.hpp"

class NPC : public EntityFunction {
public:
	NPC() {}

	EntityManager entityManager;
	ComponentStorage<sf::Sprite> componentStorage;
	Entity npcEntity;
	void Shape(const char* file) {
		//player
		npcEntity = entityManager.createEntity();



		componentStorage.sprites[npcEntity].texture = new sf::Texture(file);



		sf::Sprite testnpc(*componentStorage.sprites[npcEntity].texture);

		componentStorage.positions[npcEntity].x = 375.f;
		componentStorage.positions[npcEntity].y = 280.0f;
		componentStorage.sizes[npcEntity].x = 0.2f;
		componentStorage.sizes[npcEntity].y = 0.2f;
		testnpc.setScale({ componentStorage.sizes[npcEntity].x, componentStorage.sizes[npcEntity].y });
		testnpc.setPosition({ componentStorage.positions[npcEntity].x, componentStorage.positions[npcEntity].y });
		npcSprites.emplace("testNpc", testnpc);
	}
	Velocity& vel = componentStorage.velocities[npcEntity];
	void update(float& dt, World& world, Audio& audio, Input* input, Physics* physics, SceneType* type) override {
		
	}


	std::unordered_map<std::string,sf::Sprite>& getShapes() {
		return npcSprites;
	}

	void render(sf::RenderWindow& window, Renderer& renderer, Camera* camera = nullptr) override {
		for (auto& [key , a] : npcSprites)
			renderer.drawPlayer( a, key);

	}

	void npcWorks(const std::string& theNpcNAME) {
		if (theNpcNAME == "testNpc") {
			std::cout << "Hello, I am a test NPC!" << std::endl;
		}
		else {
			std::cout << "This NPC has nothing to say." << std::endl;
		}
	}

private:

	sf::Texture texture;
	std::unordered_map<std::string, sf::Sprite> npcSprites;
};