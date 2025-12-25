#pragma once
#include "../MainLibraries/main.hpp"
#include "fstream"
#include "../GAMEDATA.hpp"
#include <json.hpp>
using json = nlohmann::json;
class SAVINGDATA {
public:
	SAVINGDATA() {}
	void loadSavedGame(gameData& data, const std::string& filename) {
		std::ifstream file(filename);
		if (!file.is_open()) return; // no save yet

		json j;
		file >> j;


		data.health = j.value("player", json::object()).value("health", 100);
		data.score = j.value("player", json::object()).value("scores", 0);

		// Enemy
		data.healthEnem = j.value("Enemy", json::object()).value("healthEnem", 100);

	
	}

	void saveGame(const gameData& data, const std::string& filename) {

		json j; 
		j["player"]["health"] = data.health;
		j["player"]["scores"] = data.score;

		j["Enemy"]["healthEnem"] = data.healthEnem;

		std::ofstream outFile(filename);
		outFile << j.dump(4);

	}

	

};