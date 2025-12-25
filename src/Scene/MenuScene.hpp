#pragma once 
#include "../MainLibraries/main.hpp"
#include "SceneManager.hpp"
#include "../Renderer.hpp"
#include <fstream>
#include "../SAVERDATA/savedGame.hpp"
#include "../GUI.hpp"
#include <json.hpp>
using json = nlohmann::json;
struct rules{
	bool isSaving = false;

};

class GameMenu {
public :
	GameMenu() {}
	sf::Font font;
	rules rule;
	void mainMenuGUI(const char* file) {
		if (font.openFromFile(file)) {
			sf::Text New_Game(font);
			New_Game.setString("New_Game");
			New_Game.setCharacterSize(14);
			New_Game.setFillColor(sf::Color::Green);
			New_Game.setOutlineThickness(12.f);
			New_Game.setOutlineColor(sf::Color::Red);

			// --- CENTER ORIGIN ---
			sf::FloatRect bounds = New_Game.getLocalBounds();
			New_Game.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
				bounds.position.y + bounds.size.y / 2.f });

			// Set position (this becomes the center now)
			New_Game.setPosition({ 55.f, 15.f });

			// Store it
			Texts.emplace("menu_New_Game", New_Game);

			for (int i = 0; i < 5; i++) {

				sf::Text Load_Game(font);
				Load_Game.setString("Load Game " + std::to_string(i + 1));
				Load_Game.setCharacterSize(14);
				Load_Game.setFillColor(sf::Color::Red);
				Load_Game.setOutlineThickness(12.f);
				Load_Game.setOutlineColor(sf::Color::Green);

				// --- CENTER ORIGIN ---
				sf::FloatRect Load_GameBounds = Load_Game.getLocalBounds();
				Load_Game.setOrigin({ Load_GameBounds.position.x + Load_GameBounds.size.x / 2.f,
					Load_GameBounds.position.y + Load_GameBounds.size.y / 2.f });

				// Set position (this becomes the center now)
				Load_Game.setPosition({ 55.f, 55.f + i * 40.f });

				// Store it
				Texts.emplace("menu_Load_Game" + std::to_string(i), Load_Game);


			}

		}
	}

	void drawMENU(Renderer& render, sf::RenderWindow& window) {
		if (!Texts.empty())
		{

			for (auto& [key, value] : Texts) {
				render.drawPlayer( value, key);

			}
		}
	}

    template<typename T>
	bool handleEvent(const T& event, sceneStruct& currentScene, sf::RenderWindow& window, std::string& isNew, SAVINGDATA& saved, GUI& gui) {
		json j;
		std::ifstream inFile("assets/JSON/loadGameSave.json");
		if (inFile.is_open()) {
			inFile >> j;
			inFile.close();
		}
		else {
			// initialize if file doesn't exist
			for (int k = 0; k < 5; k++)
				j["load" + std::to_string(k)] = false;
		}

		// --- Highlight loaded slots ---
		for (int i = 0; i < 5; i++) {
			if (j["load" + std::to_string(i)] == true) {
				std::string menuKey = "menu_Load_Game" + std::to_string(i);
				auto it = Texts.find(menuKey);
				if (it != Texts.end()) {
					it->second.setOutlineColor(sf::Color::Red);
				}
			}
		}
		for (auto& [key, a] : Texts) {
		for (int i = 0; i < 5; i++) {
		
			sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
			sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);
				
			if (auto Mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (Mouse->button == sf::Mouse::Button::Left && currentScene.currentScene == SceneType::MENU) {
					
					if (a.getGlobalBounds().contains(mouseWorld)) {
						if (key == "menu_New_Game" && !rule.isSaving) {
							currentScene.switchTo(SceneType::GAME); //<<---this is neet to be done later bcs it wasted my time 
							return true;
						}


					
							if (key == "menu_Load_Game" + std::to_string(i) && rule.isSaving&&!j["load" + std::to_string(i)]) {

								// --- Load existing JSON ---
							
								// --- Set the current slot to true ---
								j["load" + std::to_string(i)] = true;
								saveData(saved,gui,"assets/JSON/save"+std::to_string(i)+".json");
								// --- Save back to file ---
								std::ofstream outFile("assets/JSON/loadGameSave.json");
								outFile << j.dump(4);
								outFile.close();

								std::cout << "load" << i << " set to true!\n";

						


								return true;
							}
							else if (key == "menu_Load_Game" + std::to_string(i) && !rule.isSaving && j["load" + std::to_string(i)]) {
								currentScene.switchTo(SceneType::GAME);
								loadData(saved, gui, "assets/JSON/save" + std::to_string(i) + ".json");
								return true;
							}
						}

						
					}
				}
			}
		}
		return false;
    }
	template<typename T>

	bool isSaving(const T& event, sceneStruct& currentScene, sf::RenderWindow& window) {
		if (auto keyboard = event->getIf<sf::Event::KeyPressed>()) {
			if (keyboard->scancode == sf::Keyboard::Scancode::V) {
				return true;
			}
		}
		return false;
	}


	private:
		void loadData(SAVINGDATA &saved,GUI& gui,const std::string &path) {
			saved.loadSavedGame(gui.GAMEData, path);
		}

		void saveData(SAVINGDATA& saved, GUI& gui, const std::string& path) {
			saved.saveGame(gui.GAMEData, path);
		}

	std::unordered_map<std::string, sf::Text >Texts;
};