#pragma once
#include <vector>
#include <iostream>
#include "MainLibraries/main.hpp"
#include "Camera.hpp"
#include "Scene/SceneManager.hpp"
//🖼️ Renderer.hpp / Renderer.cpp
//
//Very simple : it only draws things.
//
//draw player
//
//draw world
//
//draw UI(later)
//
//No logic is allowed in here.
//This helps beginners learn the correct game architecture pattern.
enum class DrawType
{
	FIRST,
	LAST,
};
class Renderer {
public:

	Renderer() {}
	std::unordered_map<std::string, bool> isRenderingSupporter;
	int indexRenderingDown = 0;
	int indexRenderingRight = 0;
	int indexRenderingUp = 0;
	int indexRenderingLeft = 0;
	void render(sf::RenderWindow& window, Camera& camera, sceneStruct& currentScene, DrawType& drawType, bool& isTop,
		std::unordered_map<std::string, bool>& isRendering, std::unordered_map<std::string, bool>& depthAnimalBool) {
		sf::FloatRect viewRect(
			camera.getView(window).getCenter() - camera.getView(window).getSize() / 2.f,
			camera.getView(window).getSize()
		);

		window.setView(camera.getView(window));
		window.clear(sf::Color::Black);

		if (currentScene.currentScene == SceneType::GAME) {


			for (auto& [key, value] : drawable) {
				if (isWorld(key)) {

					window.draw(*value);
				}



			}
			for (auto& a : dynamicObjects) {
				if (a.get()->getGlobalBounds().findIntersection(viewRect)) {
					window.draw(*a);
				}
			}
			dynamicObjects.clear();

			depthRender(isTop, window, depthAnimalBool,viewRect);


			for (auto& [key, val] : drawable) {


				if (isHitbox(key))
					window.draw(*val);
				if (isTreeDepth(key)) {
					window.draw(*val);
				}
				if (isPut(key)) {
					window.draw(*val);
				}
			}

		}

		window.setView(window.getDefaultView());
		if (currentScene.currentScene == SceneType::GAME) {
			for (auto& [key, value] : drawable) {
				if (isUI(key) && !isMenu(key))
					window.draw(*value);
				else if (isVID(key))
					window.draw(*value);
				else if (isCollected(key))
					window.draw(*value);
			}
		}
		else {
			for (auto& [key, value] : drawable) {
				if (isMenu(key) && !isUI(key))
					window.draw(*value);
			}
		}
		window.display();



	}
	template<typename T>

	void drawPlayer(T& a, const std::string& name ) {

	
	
			drawable[name] = std::make_shared<T>(a);

		

	}
	void drawPlayerTWEAKABLE(const std::string& name, sf::Sprite* a = nullptr, sf::RectangleShape* b = nullptr) {
		if (b) {
			drawableRectangle[name] = std::make_shared<sf::RectangleShape>(*b); // copy constructor works
		}

		if (a) {
			drawableSprite[name] = std::make_shared<sf::Sprite>(*a); // copy constructor of sf::Sprite
		}
		if (a && name.find("Animal_") != std::string::npos) {
			drawableSpriteANIMAL[name] = std::make_shared<sf::Sprite>(*a);
		}
		//if(a && name.find("world_") != std::string::npos)
	}


	bool isUI(const std::string& key) {
		return key.rfind("ui_", 0) == 0;
	}
	bool isVID(const std::string& key) {
		return key.rfind("vid_", 0) == 0;
	}

	bool isMenu(const std::string& key) {
		return key.rfind("menu_", 0) == 0;
	}

	bool isCollected(const std::string& key) {
		return key.rfind("Collected_", 0) == 0;
	}
	bool isHitbox(const std::string& key) {
		return key.rfind("hitbox_", 0) == 0;
	}
	bool isTreeDepth(const std::string& key) {
		return key.rfind("TREE_TOP", 0) == 0;
	}
	bool isPut(const std::string& key) {
		return key.rfind("PUT_", 0) == 0;
	}
	bool isSpawnedObjWorld(const std::string& key) {
		return key.rfind("spawnedObjWorld_", 0) == 0;
	}
	bool isWorld(const std::string& key) {
		return key.rfind("world_", 0) == 0;

	}
	bool isHitboxSpawnedObjWorld(const std::string& key) {
		return key.rfind("hitboxSpawnedObjWorld_", 0) == 0;
	}

	bool isPlayer(const std::string& key) {
		return key.rfind("player", 0) == 0;
	}

	bool isEntity(const std::string& key) {
		return key.rfind("entity_", 0) == 0;
	}
	bool isAnimal(const std::string& key) {
		return key.rfind("Animal_", 0) == 0;
	}
	void renderDynamic(std::shared_ptr<sf::RectangleShape>bullet) {


		dynamicObjects.push_back(bullet);

	}


	std::vector<std::shared_ptr<sf::RectangleShape>> dynamicObjects;
	void removeDrawable(const std::string& name) {
		drawable.erase(name);
	}

		// Your maps remain unchanged
		std::unordered_map<std::string, std::shared_ptr<sf::Sprite>> drawableSprite;
		std::unordered_map<std::string, std::shared_ptr<sf::Sprite>>drawableSpriteANIMAL;
		std::unordered_map<std::string, std::shared_ptr<sf::RectangleShape>> drawableRectangle;
	std::unordered_map<std::string, std::shared_ptr<sf::Text>> drawableText;
	std::unordered_map<std::string, std::shared_ptr<sf::Drawable>> drawable;
	std::unordered_map<std::string, std::shared_ptr<sf::Sprite>> drawableGrounds;


private:
	void depthRender(bool& isTop, sf::RenderWindow& window, std::unordered_map<std::string, bool>& depthAnimalBool,sf::FloatRect& viewRect) {

		//render for player
		if (isTop) {
			for (auto& [key, val] : drawableSprite)
				if (isPlayer(key)) {
					if(val.get()->getGlobalBounds().findIntersection(viewRect))
						window.draw(*val);
				}
		}
		for (auto& [keyDraw, val] : drawableSpriteANIMAL) {
			if (isAnimal(keyDraw)) {
				if (depthAnimalBool[keyDraw]) {
					if (val.get()->getGlobalBounds().findIntersection(viewRect))
						window.draw(*val);

				}
			}
		}

		auto getDirection = [&](const auto& key) -> bool {
			return  key.find("world_Down" + std::to_string(indexRenderingDown)) != std::string::npos ||
				key.find("world_Right" + std::to_string(indexRenderingRight)) != std::string::npos ||
				key.find("world_Left" + std::to_string(indexRenderingLeft)) != std::string::npos ||
				key.find("world_Up" + std::to_string(indexRenderingUp)) != std::string::npos;

			};

		auto gettingDraw = [&](auto& drawAbleMap, const std::string& type) {
			for (auto& [key, things] : drawAbleMap) {
				//if (!getDirection(key) || key == "world_center") continue;
				if (type == "sprite") {
					if (isSpawnedObjWorld(key)) {
				
						if (things.get()->getGlobalBounds().findIntersection(viewRect)) {

							window.draw(*things);
						}
					}
				}
				else if (type == "rectangle") {
					if (isHitboxSpawnedObjWorld(key)) {
					
						if (things.get()->getGlobalBounds().findIntersection(viewRect)) {

							window.draw(*things);
						}
						
						}
					
				}
			}
			};

		gettingDraw(drawableSprite, "sprite");
		gettingDraw(drawableRectangle, "rectangle");



		// tomorrow let me fix the depth of each animals
	   //render for player
		if (!isTop) {
			for (auto& [key, val] : drawableSprite)
				if (isPlayer(key)) {
					if (val.get()->getGlobalBounds().findIntersection(viewRect))
						window.draw(*val);
				}
		}

		for (auto& [keyDraw, val] : drawableSpriteANIMAL) {
			if (isAnimal(keyDraw)) {
				if (!depthAnimalBool[keyDraw]) {
					if(val.get()->getGlobalBounds().findIntersection(viewRect))
						window.draw(*val);

				}
			}
		}
	}

	};