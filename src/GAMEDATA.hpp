#pragma once 
#include "MainLibraries/main.hpp"

struct gameData {
	//enemy health
	int healthEnem = 100;
	sf::Vector2f healthEnemPos{ 10.f,100.f };
	//player data
	int score = 0;
	int health = 100;
};