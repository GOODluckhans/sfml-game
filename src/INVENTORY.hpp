#pragma once 
#include "MainLibraries/main.hpp"
#include "Scene/SceneManager.hpp"
struct slots{
	bool slot1 = false;
	bool slot2 = false;
};
class Inventory{
public:
	Inventory() {}
	template<typename T>
	void inventoryWorks(std::unordered_map<std::string,sf::Sprite>& slots,sf::RenderWindow &window,T&event,sceneStruct& currentScene) {
		for (auto& [key, val] : slots) {
			sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
			sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

			if (auto Mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (Mouse->button == sf::Mouse::Button::Left && currentScene.currentScene == SceneType::GAME) {

					if (val.getGlobalBounds().contains(mouseWorld)) {
						if (key == "ui_inventory0") {
							std::cout << "Inventory slot 1 clicked!" << std::endl;
						}
						else if (key == "ui_inventory1") {
							std::cout << "Inventory slot 2 clicked!" << std::endl;
						}
					}
				}
			}

		}
		
	}
};	