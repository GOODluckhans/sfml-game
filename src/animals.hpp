#pragma once 
#include "MainLibraries/main.hpp"
#include "SpawningObj.hpp"
#include "EntityFuncition.hpp"
#include "Renderer.hpp"
struct animalData {
	float aiTimer = 0.0f;
	float x = 0.0f;
	float y = 0.0f;

};

class ANIMAL : public EntityFunction {
	sf::Sprite* m_spritePlayer = nullptr;
	SpawningObj* m_spawningObjClass = nullptr;
	Renderer* m_rendererClass = nullptr;
public:
	ANIMAL()
		{
	}
	std::unordered_map<std::string, animalData> animalDatas;
	void gettingAnimalGoods(sf::Sprite& player, SpawningObj& spawnObjClass,Renderer& render) {
		m_spawningObjClass = &spawnObjClass;
		m_spritePlayer = &player;
		m_rendererClass = & render;
	}
	sf::Clock despawn;
	bool isInvalidKey = false;
	void update(float& dt, World& world, Audio& audio, Input* input = nullptr, Physics* physics = nullptr, SceneType* type  = nullptr) override {
		if (despawn.getElapsedTime().asSeconds() > 10.f) {

			for (auto it = m_rendererClass->drawableSpriteANIMAL.begin();
				it != m_rendererClass->drawableSpriteANIMAL.end(); ++it) {

				if (it->first.rfind("Animal_", 0) == 0) {
					m_rendererClass->drawableSpriteANIMAL.erase(it);
					break; // 
				}
			}

			despawn.restart();
		}


		for (auto& [keyWorld, ground] : m_spawningObjClass->sprites) {
			sf::FloatRect groundBounds = ground.getGlobalBounds();
			for (auto& [key, animals] : m_rendererClass->drawableSpriteANIMAL) {
		
				std::string fixedKey2 = key;
				
				m_spawningObjClass->spriteAnimals.erase(key);
				if (fixedKey2.find_last_not_of("_") != std::string::npos)
					fixedKey2.erase(fixedKey2.find_last_not_of("0123456789_") + 1);

				
				for (auto& a : m_spawningObjClass->normalAnimals) {
					if (fixedKey2.find(a) != std::string::npos) {
						std::string suffix = fixedKey2.substr(fixedKey2.find(a) + a.length() + 1);  // << so from left go to search those animal so that is the length befor animal ("like , how many steps to get those animals)
						//std::cout << suffix << '\n';
						if (keyWorld == suffix) {
							isInvalidKey = false;
						}
						else {
							isInvalidKey = true;
						}
					}
				}


				if (isInvalidKey) continue;



				auto& animData = animalDatas[key];
				animData.aiTimer -= dt;
		
				if (animData.aiTimer <= 0) {
					animData.aiTimer = 2.f + static_cast<float>(rand() % 200) / 100.f;
					animData.x = groundBounds.position.x +
						static_cast<float>(rand()) / RAND_MAX * groundBounds.size.x;
					animData.y = groundBounds.position.y +
						static_cast<float>(rand()) / RAND_MAX * groundBounds.size.y;
				}

				sf::Vector2 dir = sf::Vector2f({ animData.x,animData.y }) - animals->getPosition();
				float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

				if (length > 1.f) {
					dir /= length;
					float speed = 40.f; // kecepatan animal
					animals->move(dir * speed * dt);

				}

			}
		}
	}


	void render(sf::RenderWindow& window, Renderer& renderer, Camera* camera) override {
		/*renderer.drawPlayer(getShape(), "player");*/

	}
	template<typename T>
	void catchAnimal(sf::RenderWindow& window, Renderer& renderer, T& event, sf::View& gameView) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f getPixelCoords = window.mapPixelToCoords(mousePos,gameView);

		for (auto& [key, animal] : renderer.drawableSpriteANIMAL) {
			if (animal.get()->getGlobalBounds().contains(getPixelCoords)) {
				if (auto keyboard = event->getIf<sf::Event::KeyPressed>()) {
					if (keyboard->scancode == sf::Keyboard::Scancode::E) {
						std::cout << key << "\n";
					}
				}
			}
		}
	}

};