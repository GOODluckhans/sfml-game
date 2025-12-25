#pragma once 
#include "MainLibraries/main.hpp"
#include "Renderer.hpp"
#include "SAVERDATA/savedGame.hpp"
#include "GAMEDATA.hpp"
#include "INVENTORY.hpp"
//struct guiText {
//	int score = 0;
//	int health = 100;
//	int healthEnem = 100
//	sf::Vector2f healthEnemPos{ 10.f,100.f };
//};

class GUI  {
public:
	GUI() {
	
	}
	gameData GAMEData;
	sf::Font font; // it shall not be local variable

	void init(const char* file,sf::RenderWindow&window) {
		// Score text
		
		if (!font.openFromFile(file))
			return;
		sf::Text scoreText(font);
		scoreText.setCharacterSize(20);
		scoreText.setFillColor(sf::Color::Yellow);
		scoreText.setPosition({ 10.f, 70.f });
		scoreText.setString("TAI");
		Texts.emplace("ui_score",scoreText);


		sf::Text Health(font);
		Health.setCharacterSize(20);
		Health.setFillColor(sf::Color::Red);
		Health.setPosition({ 10.f, 100.f });
		Health.setString("TAI");
		Texts.emplace("ui_health", Health);

		sf::Text HealthEnem(font);
		HealthEnem.setCharacterSize(20);
		HealthEnem.setFillColor(sf::Color::Red);
		HealthEnem.setPosition({ 10.f, 100.f });
		HealthEnem.setString("EEK");
		Texts.emplace("uiMove_healthEnem", HealthEnem);

		sf::Text informations(font);
		informations.setCharacterSize(15);
		informations.setFillColor(sf::Color::Black);
		informations.setPosition({ 10.f, 10.f });
		informations.setString("PRESS L KEY FOR BASIC SCENE !");
		Texts.emplace("ui_information", informations);
		makingInventoryGUI(window);
	}

	void update() {
		for (auto& [key, value] : Texts) {
			if (key == "ui_score") {
				value.setString("Score: " + std::to_string(GAMEData.score)); // Example score update
			}
			else if (key == "ui_health") {
				value.setString("Health: " + std::to_string(GAMEData.health)); // Example health update
			}
			else if (key == "uiMove_healthEnem") {
				value.setPosition(GAMEData.healthEnemPos);
				value.setString("Health: " + std::to_string(GAMEData.healthEnem)); // Example health update
			}
		}


	}

	void render(Renderer& render,sf::RenderWindow& window) {
		if (!Texts.empty() || !sprites.empty())
		{

			for (auto& [key,value] : Texts) {
				render.drawPlayer(value,key);

			}
			for (auto& [key, value] : sprites) {
				render.drawPlayer( value, key);

			}
		}
	}
	template<typename T>
	void guiActive(sf::RenderWindow& window,T&event, sceneStruct& currentScene) {
		INVENTORYsystem.inventoryWorks(sprites,window,event,currentScene);
	}
	std::unordered_map<std::string,sf::Text> Texts;
	std::unordered_map<std::string, sf::Sprite> sprites;
protected:


	void makingInventoryGUI(sf::RenderWindow &window,const std::string& file = "assets/Textures/inventoryGui.png") {
		for (size_t i = 0; i < 1; i++)
		{
			float offset = 100.f * static_cast<float>(i); // bigger offset
			auto texture = std::make_shared<sf::Texture>(file);
			sf::Sprite inventorySprite(*texture);
			inventorySprite.setPosition({ 0 + offset,350});
			inventorySprite.setScale({ 0.2f,0.2f });
			sprites.emplace("ui_inventory"+std::to_string(i), inventorySprite);
			textures.push_back(texture);
		}
	
	}

	std::vector<std::shared_ptr<sf::Texture>> textures;
	Inventory INVENTORYsystem;


};