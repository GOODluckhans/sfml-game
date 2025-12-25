#pragma once 
#include "MainLibraries/main.hpp"
#include <json.hpp>
#include <fstream>
using json = nlohmann::json;
struct TiledStruct {
	sf::FloatRect rect;
	std::string name;
};

class Tiled {
public:

	std::vector<TiledStruct> loadTiled(const std::string& layername,const char* filename) {
		std::ifstream file(filename);
		if (!file.is_open()) {
			std::cerr << "Failed to open " << filename << "\n";
			return {};
		}
		json j;
		try {
			file >> j;
		}
		catch (const nlohmann::json::parse_error& e) {
			std::cerr << "JSON parse error in " << filename << ": " << e.what() << '\n';
			return {};
		}

        int index = 0;
        std::string  name;
        for (auto& layer : j["layers"]) {
            if (layer["type"] == "objectgroup" && layer["name"] == layername) {

                for (auto& obj : layer["objects"]) {

                    if (obj["name"] == "" || isCOLLECT(obj["name"]) || isCOLLECTPLANT(obj["name"]) || isBOTTOMTREE(obj["name"])|| isSpawn(obj["name"])) {

                        float x = obj["x"].get<float>();
                        float y = obj["y"].get<float>();
                        float w = obj["width"].get<float>();
                        float h = obj["height"].get<float>();

                        sf::FloatRect rect;
        
                        if (isBOTTOMTREE(obj["name"])){
                            // ?? ONLY FOR TREE BOTTOM OBJECTS
                            float shrink = 5.f;   // how much shorter
                            float raise = 2.f;   // how much to move up

                            rect = sf::FloatRect(
                                sf::Vector2f(x + 10.f, y - (h - raise)),
                                sf::Vector2f(w - 25.f, h - shrink)
                            );

                            name = "TREE_BOTTOM" + std::to_string(index++);
                            obj["name"] = name;
                        }
                        else if (isCOLLECTPLANT(obj["name"])) {
                            // ?? ONLY FOR TREE BOTTOM OBJECTS
                            float shrink = 5.f;   // how much shorter
                            float raise = 2.f;   // how much to move up

                            rect = sf::FloatRect(
                                sf::Vector2f(x + 10.f, y - (h - raise)),
                                sf::Vector2f(w - 25.f, h - shrink)
                            );

                            name = "PUT_PLANT_LavenderBOTTOM" + std::to_string(index++);
                            obj["name"] = name;
                        }
                   
                        else {
                            rect = sf::FloatRect(
                                sf::Vector2f(x, y ),
                                sf::Vector2f(w, h )
                            );
                            name = obj["name"];
                        }

                        solids.push_back({ rect, name });

                    }
                }
            }

		}
     
            std::ofstream out(filename);
            out << j.dump(4);
        

		return solids;
	}
	std::vector<TiledStruct> solids;
	bool isCOLLECT(const std::string& key) {
		return key.rfind("PUT_", 0) == 0;
	}
    bool isCOLLECTPLANT(const std::string& key) {
        return key.rfind("PUT_PLANT_", 0) == 0;
    }
	bool isBOTTOMTREE(const std::string& key) {
		return key.rfind("TREE_BOTTOM", 0) == 0;
	}
    bool isSpawn(const std::string& key) {
        return key.rfind("SPAWN_", 0) == 0;
	}

};