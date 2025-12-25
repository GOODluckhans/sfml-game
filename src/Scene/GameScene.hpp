#pragma once 
#include "../MainLibraries/main.hpp"
#include "SceneManager.hpp"
#include "ECS/ComponentStorage.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/Entity.hpp"


class MainGame  {
public:

	MainGame() {}
	template<typename T>
	bool handleEvent(const T& event, sceneStruct& currentScene) {
		if (auto keyboard = event->getIf<sf::Event::KeyPressed>()) {
			if (keyboard->scancode == sf::Keyboard::Scancode::Escape && currentScene.currentScene == SceneType::GAME) {
				return true;

			}
		}
		return false;
	}

private:


};