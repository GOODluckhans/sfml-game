#pragma once 

#include "MainLibraries/main.hpp"
class Camera {
public: 
	Camera() {}
	int maxDir[4] = {-1,-1,-1,-1};
	template<typename T>
    void follow(T& charMain, float& dt, sf::RenderWindow& window,
        bool& isOnMainWorld, const std::string newDir, std::unordered_map<std::string, sf::Sprite>& worldCountSprites,
        std::unordered_map<std::string, sf::Sprite>&spriteGround) {
		// Assume you already have these somewhere:
		//for (auto& tex : texSize) /*{*/
		if (isOnMainWorld) {
			float mapWidth = window.getSize().x;   // total width of your world
			float mapHeight = window.getSize().y;  // total height of your world

			// Get view size
			float halfW = view.getSize().x / 2.f;
			float halfH = view.getSize().y / 2.f;

			// Start by centering camera on player
			sf::Vector2f camPos = charMain.getPosition();

			// Clamp so camera never shows outside the map
			camPos.x = std::max(camPos.x, halfW);
			camPos.x = std::min(camPos.x, mapWidth - halfW);
			camPos.y = std::max(camPos.y, halfH);
			camPos.y = std::min(camPos.y, mapHeight - halfH);

			view.setCenter(camPos);
			//}
		}
		else {
            auto getNnum = [&](const std::string& key) ->std::string {
                std::string currentMap = key;
                size_t start = currentMap.find_first_of("0123456789");
                size_t end = currentMap.find_first_not_of("0123456789", start);

                int value = std::stoi(currentMap.substr(start, end - start));

                value = std::max(0, value);

                std::string digits = std::to_string(value);
                return digits;
                };
            for (auto& [key, val] : worldCountSprites) {
             /*   for (auto& [keyGround, spriteGround] : spriteGround) {
*/

                    float mapWidth, mapHeight;
                    sf::Vector2f camPos = charMain.getPosition();
                    float halfW = view.getSize().x / 2.f;
                    float halfH = view.getSize().y / 2.f;

                    if (newDir == "Down") {
                        int num = -1;
                        if (key.find(newDir) != std::string::npos) { //after eat le me fix this by the world digits 
                            
                            

                            std::string currentMap = key;

                       
                            num = std::stoi(getNnum(currentMap));
                            maxDir[0] = std::max(maxDir[0], num);
                        }

                        int times = maxDir[0] + 2;
                        mapWidth = window.getSize().x;
                        mapHeight = window.getSize().y * times;

                        camPos.x = std::clamp(camPos.x, halfW, mapWidth - halfW);
                        camPos.y = std::clamp(camPos.y, halfH, mapHeight + halfH);
                    }
                    else if (newDir == "Right") {
                        int num = -1;
                        if (key.find(newDir) != std::string::npos) {

                            std::string currentMap = key;


                            num = std::stoi(getNnum(currentMap));
                            maxDir[1] = std::max(maxDir[1], num);

                        }
                        int times = maxDir[1] + 2;
                        mapWidth = window.getSize().x * times;
                        mapHeight = window.getSize().y;
                        camPos.x = std::clamp(camPos.x, halfW, mapWidth + halfW);
                        camPos.y = std::clamp(camPos.y, halfH, mapHeight - halfH);
                    }
                    else if (newDir == "Left") {
                        int num = -1;
                        if (key.find(newDir) != std::string::npos) {
                            std::string currentMap = key;


                            num = std::stoi(getNnum(currentMap));
                            maxDir[2] = std::max(maxDir[2], num);
                        }
                        int times = maxDir[2] + 2;
                        mapWidth = window.getSize().x * times;
                        mapHeight = window.getSize().y;

                        camPos.x = std::clamp(camPos.x, -mapWidth + halfW, -halfW);
                        camPos.y = std::clamp(camPos.y, halfH, mapHeight - halfH);
                    }
                    else if (newDir == "Up") {
                        int num = -1;
                        if (key.find(newDir) != std::string::npos) {
                            std::string currentMap = key;


                            num = std::stoi(getNnum(currentMap));
                            maxDir[3] = std::max(maxDir[3], num);
                        }
                        int times = maxDir[3] + 2;
                        mapWidth = window.getSize().x;
                        mapHeight = window.getSize().y * times;

                        camPos.x = std::clamp(camPos.x, halfW, mapWidth - halfW);
                        camPos.y = std::clamp(camPos.y, -mapHeight + halfH, -halfH);
                    }

                    view.setCenter(camPos);
                }
            //}

		}
	}

	void init(sf::RenderWindow& window) {
		view.setSize(
			{ (float)window.getSize().x,
			(float)window.getSize().y }
		);
		view.zoom(0.5f + 0.5f);
	}


	sf::View& getView(sf::RenderWindow & window) {
		return view; 
	}
private:
	sf::View view;
	float followSpeed = 5.f;	
};