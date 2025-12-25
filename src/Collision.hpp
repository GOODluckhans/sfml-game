#pragma once 
#include "MainLibraries/main.hpp"
#include "World.hpp"
#include "Input.hpp"
//#include "Physics.hpp"  // Full Physics class definition
#include "Player.hpp"
#include "GUI.hpp"
#include "SATcollision.hpp"
#include "Tiled.hpp"
#include "Collectable_item.hpp"
#include "DepthSystem.hpp"
#include "SpawningObj.hpp"
#include "Scene/GameScene.hpp"
#include <json.hpp>
#include <fstream>
using json = nlohmann::json;
//class Player;  // forward declare


class Collision {
public:
	bool deletingBullet = false;
	Collision() {}
	template<typename T, typename  U>
	static bool isColliding(const T& a, const U& b) {
		sf::FloatRect bBound;
		if constexpr (requires { b.getGlobalBounds(); }) {
			bBound = b.getGlobalBounds();

		}
		else if constexpr (requires { b.rect; }) {
			bBound = b.rect;
		}
		else {
			static_assert(false, "Wall type does not have getGlobalBounds() or rect member.");
		}
		if (a.getGlobalBounds().findIntersection(bBound)) {
			return true;
		}
		return false;
	}
	DepthSystem depths;
	//depth render
	bool isTop= true;
	std::unordered_map<std::string,bool> depthAnimalBool;
	

	int indexOfWorldOBJ = 0;
	template<typename T, typename T2, typename T3, typename T4>

	void handleCollision(T3* player, T4* enemyClass, std::unordered_map<std::string, sf::Sprite>& NPC, sf::RectangleShape& pShape, World& world, T& enemy, T2& bullets, sf::RenderWindow& window, GUI& gui, Tiled& tiled, Collectable_item& collectableClass, bool& isNPC
		, SpawningObj& spawnObjClass, Renderer& renderer
		, MainGame& gameScene, Input& input) {
		for (auto& solid : tiled.solids) {
			for (auto& [keyNPC, val] : NPC) {
			
				if (tiled.isCOLLECTPLANT(solid.name)) {
			
					// You can also handle effects here if needed     << on this when i back to home i wanna fix the depth so it looks better depth system ,
					continue;
				}

				if (sat.SAT_OBB(pShape, solid)) {


					resolveCollision(pShape, solid, world, player);

					world.getVelocityPlayer(player->componentStorage.velocities[player->plr]).vy = 0.0f;
					world.getVelocityPlayer(player->componentStorage.velocities[player->plr]).vx = 0.0f;
				}
				else if (sat.SAT_OBB(pShape, val)) {
					resolveCollision(pShape, val, world, player);

					world.getVelocityPlayer(player->componentStorage.velocities[player->plr]).vy = 0.0f;
					world.getVelocityPlayer(player->componentStorage.velocities[player->plr]).vx = 0.0f;
					if (!isNPC)
						isNPC = true;
				}
				if (sat.SAT_OBB(enemy, solid)) {


					resolveCollision(enemy, solid, world, player);
					world.getVelocityEnemy(enemyClass->componentStorage.velocities[enemyClass->enemEntity]).vy = 0.0f; // fix later
					world.getVelocityEnemy(enemyClass->componentStorage.velocities[enemyClass->enemEntity]).vx = 0.0f;
				}
				else if (sat.SAT_OBB(enemy, val)) {
					resolveCollision(enemy, val, world, player);
					world.getVelocityEnemy(enemyClass->componentStorage.velocities[enemyClass->enemEntity]).vy = 0.0f; // fix later
					world.getVelocityEnemy(enemyClass->componentStorage.velocities[enemyClass->enemEntity]).vx = 0.0f;
				}
				if (sat.SAT_OBB(pShape, enemy)) {

					resolveCollision(pShape, enemy, world, player);
					world.getVelocityPlayer(player->componentStorage.velocities[player->plr]).vy = 0.0f;
					world.getVelocityPlayer(player->componentStorage.velocities[player->plr]).vx = 0.0f;
					gui.GAMEData.health = std::max(0, gui.GAMEData.health - 10);
				}
				////}
				else if (sat.SAT_OBB(bullets, enemy)) {
					resolveCollision(bullets, enemy, world, player);
					gui.GAMEData.score++;
					gui.GAMEData.healthEnem = std::max(0, gui.GAMEData.healthEnem - 1);
					deletingBullet = true;
				}
				else if (sat.SAT_OBB(bullets, solid)) {


					resolveCollision(bullets, solid, world, player);
				}
				if (sat.SAT_OBB(pShape, solid) && isPUT(solid.name)) {
					std::string nameItem = solid.name;
					nameItem.erase(0, 4); // Remove "PUT_" prefix
					collectableClass.Collecting(nameItem, gui);
				}
			}
		}
		//template<typename drawAbl
	

		for (auto& [name,spawnObj] : renderer.drawableSprite) {
			if (name.rfind("spawnedObjWorld_", 0) != 0) continue;
			sf::Vector2f playerCenter = getCenter(player->getShape());
			sf::Vector2f worldCenter = getCenter(*spawnObj);
	

			if (isColliding(player->getShape(), *spawnObj)) {
				isTop = (playerCenter.y < worldCenter.y);
				//resolveCollision(player->getShape(), spawnObj, world, player);
			}
			for (auto& [key, animal] : renderer.drawableSpriteANIMAL) {

				sf::Vector2f animalCenter = getCenter(*animal);
				if (isColliding(*animal, *spawnObj)) {
					depthAnimalBool[key] = (animalCenter.y < worldCenter.y);
				}
			}
		
		}
		for (auto& [name,hitboxWorldObj] : renderer.drawableRectangle) {
			if (name.rfind("hitboxSpawnedObjWorld_", 0) == 0) {
				sf::Vector2f worldCenter = getCenter(*hitboxWorldObj);


				if (isColliding(pShape,*hitboxWorldObj)) {
					//isTop = (playerCenter.y < worldCenter.y); 
					resolveCollision(pShape, *hitboxWorldObj, world, player);
				}
				
				for (auto& [key, animal] : renderer.drawableSpriteANIMAL) {
					//sf::Vector2f animalCenter = getCenter(animal);
					if (isColliding(*animal, *hitboxWorldObj)) {
						//depthAnimalBool[key] = (animalCenter.y < worldCenter.y);
						resolveCollision(*animal, *hitboxWorldObj, world, player);

					}
				}
				
			}
		}



		// cek collision

		for (auto& [keyGround, valGround] : spawnObjClass.sprites) {
			if (!sat.SAT_OBB(pShape, valGround))
				continue;

			// Determine the direction and the index
			int index = -1;
			if (keyGround.rfind("world_Down", 0) == 0) {
				index = std::stoi(keyGround.substr(10));
				renderer.indexRenderingDown = index;
			}
			else if (keyGround.rfind("world_Right", 0) == 0) {
				index = std::stoi(keyGround.substr(11));
				renderer.indexRenderingRight = index;
			}
			else if (keyGround.rfind("world_Left", 0) == 0) {
				index = std::stoi(keyGround.substr(10));
				renderer.indexRenderingLeft = index;
			}
			else if (keyGround.rfind("world_Up", 0) == 0) {
				index = std::stoi(keyGround.substr(8));
				renderer.indexRenderingUp = index;
			}
			else if (keyGround.rfind("world_center", 0) == 0) {
				renderer.isRenderingSupporter.clear();
				spawnObjClass.hitboxSpawnedObj.clear();
				spawnObjClass.spritesWorldObj.clear();
				continue; // no need to mark this as supporter
			} //im stucking here i wanna make when it hit 5 the animals will hold the value 5 if under 5 it spawnef

			// Mark the supporter if a valid direction was found
			if (index != -1)
				renderer.isRenderingSupporter[keyGround] = true;
		}

		//ANIMAL COLLIDING
		for (auto& [key, animal] : renderer.drawableSpriteANIMAL) {

			if (sat.SAT_OBB(pShape, *animal)) {

				resolveCollision(pShape, *animal, world, player);
			}
		}

		spawningAnimal(spawnObjClass,pShape,renderer);

	

		screenCollision(renderer, pShape, window, world, spawnObjClass, "player", gameScene, input, player, enemyClass);
		//screenCollision(enemy, window, world, "enemy", player, enemyClass);
		//screenCollision(bullets, window, world, "bullet", player, enemyClass);

	}


	int renderDownCounter = 0;

	sf::Vector2f currentWorldOffset = { 0.f, 0.f };
	bool isOnMainWorld = true;
	std::string newWorldDir = "Right";
	std::unordered_map<std::string, sf::Sprite	> test1;
	//std::unordered_map<std::string, bool> isRenderingSupporter;
private:
	template<typename T, typename U, typename T2>


	// Remove the duplicate declaration of wallBounds in resolveCollision
	void resolveCollision(T& player, U& wall, World& world, T2* pClass) {
		// Simple collision response: just print a message
		//std::cout << "Collision detected!" << std::endl;
		sf::FloatRect objBounds = player.getGlobalBounds();
		sf::FloatRect wallBounds; // Only declare once

		if constexpr (requires { wall.getGlobalBounds(); }) {
			wallBounds = wall.getGlobalBounds();

		}
		else if constexpr (requires { wall.rect; }) {
			wallBounds = wall.rect;

		}
		else {
			static_assert(false, "Wall type does not have getGlobalBounds() or rect member.");
		}

		float overlapLeft = (objBounds.position.x + objBounds.size.x) - objBounds.position.x;
		float overlapRight = (wallBounds.position.x + wallBounds.size.x) - objBounds.position.x;
		float overlapTop = (objBounds.position.y + objBounds.size.y) - wallBounds.position.y;
		float overlapBottom = (wallBounds.position.y + wallBounds.size.y) - objBounds.position.y;

		// Find smallest overlap
		float minX = std::min(overlapLeft, overlapRight);
		float minY = std::min(overlapTop, overlapBottom);


		if (minX < minY) {
			// Resolve X only
			if (overlapLeft < overlapRight)
				player.setPosition({ wallBounds.position.x - objBounds.size.x, objBounds.position.y });
			else
				player.setPosition({ wallBounds.position.x + wallBounds.size.x, objBounds.position.y });
		}
		else {
			// Resolve Y only
			if (overlapTop < overlapBottom)
				player.setPosition({ objBounds.position.x, wallBounds.position.y - objBounds.size.y });
			else
				player.setPosition({ objBounds.position.x, wallBounds.position.y + wallBounds.size.y });
		}

	}

	int index = 0;
	// Collision.hpp (inside class Collision)
	int lastWorld_POSX = 0;
	int lastWorld_NEGX = 0;
	int lastWorld_POSY = 0;
	int lastWorld_NEGY = 0;
	int randomWorld = 0;

	std::string theRandomWorld = "";
	template<typename T, typename T2, typename T3>
	void screenCollision(Renderer& renderer, T& obj, sf::RenderWindow& window, World& world, SpawningObj& spawningObj, const std::string& name
		, MainGame& gameScene, Input& input
		, T2* player = nullptr, T3* enemy = nullptr) {
		srand(static_cast<unsigned>(time(NULL)));
		auto& tile = spawningObj.worlds[spawningObj.currentWorldKey];
		sf::Vector2f pos = obj.getPosition();
		float width = 0.f;
		float height = 0.f;



		// Determine width/height depending on shape type
		if constexpr (std::is_same_v<std::decay_t<T>, sf::RectangleShape>) {
			width = obj.getSize().x;
			height = obj.getSize().y;
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, sf::CircleShape>) {
			width = obj.getRadius() * 2.f;
			height = obj.getRadius() * 2.f;
		}
		else {
			auto bounds = obj.getGlobalBounds();
			width = bounds.size.x;
			height = bounds.size.y;
		}

		bool offScreen = false;
	

		sf::Vector2u texSize = tile.size;
		float px = obj.getPosition().x ;
		float py = obj.getPosition().y;

		bool needNewWorld = true;
		
		std::string hitingGroundVal;
	
		for (auto& [a, b] : spawningObj.sprites) {

			if (!obj.getGlobalBounds().findIntersection(b.getGlobalBounds()))
				continue;
			isOnMainWorld = false;
			needNewWorld = false;
			break;
		}

		for (auto& [o, d] : spawningObj.spawnPlace) {
			if (obj.getGlobalBounds().findIntersection(d.getGlobalBounds())) {
				needNewWorld = false;
				isOnMainWorld = true;
			
				break;
			}
		}



		if (obj.getPosition().x < 0.0f) {
			newWorldDir = "Left";
		}
		else if (obj.getPosition().x > window.getSize().x) {
			newWorldDir = "Right";


		}
		else if (obj.getPosition().y < 0.0f) {
			newWorldDir = "Up";
		}
		else if (obj.getPosition().y > window.getSize().y) {
			newWorldDir = "Down";
		}

		if (needNewWorld) { 
			//spawningObj.sprites.clear();
			std::cout << spawningObj.sprites.size() << '\n';
			//renderer.drawable.clear();
			std::cout << "Drawable : " << renderer.drawable.size() << '\n';
			randomWorld = (rand() % 100) + 1;
			//spawningObj.spawningObjectByTheWorldRandomly(obj.getPosition());
			//std::cout << randomWorld << "\n"/*;*/

			std::ifstream file("assets/JSON/worldData/world.json");

			if (!file.is_open()) {
				return;
			}

			json data;
			file >> data;

			for (const auto& world : data["Worlds"]) {
				int chance = world["chance"].get<int>();
				int roll = rand() % 100; // 0 - 99

				std::cout
					<< "World: " << world["name"].get<std::string>()
					<< " | Chance: " << chance
					<< " | Roll: " << roll
					<< "\n";

				if (roll < chance) {
					theRandomWorld = world["name"].get<std::string>();
					std::cout << ">> PICKED: " << theRandomWorld << "\n";
					break;
				}
			}



			std::cout
				<< "World Index Status\n"
				<< "-------------------\n"
				<< "NEG X (Left)  : " << lastWorld_NEGX << '\n'
				<< "POS X (Right) : " << lastWorld_POSX << '\n'
				<< "NEG Y (Up)    : " << lastWorld_NEGY << '\n'
				<< "POS Y (Down)  : " << lastWorld_POSY << '\n'
				<< "-------------------\n";


			spawningObj.worldGenerator(newWorldDir, currentWorldOffset, window,theRandomWorld,
				lastWorld_NEGX,lastWorld_NEGY,lastWorld_POSX,lastWorld_POSY,obj.getPosition()); // <---it shall like this 	if (a == "world_Down1"){lastWorldIndex = 1;}
			needNewWorld = false;
			file.close();

		}
	}
	bool isPUT(const std::string& key) {
		return key.rfind("PUT_", 0) == 0;
	}
	bool isWorld(const std::string& key) {
		return key.rfind("world_", 0) == 0;
	}
	int extractWorldIndex(const std::string& key) {
		size_t pos = key.find_first_of("0123456789");
		if (pos == std::string::npos) return 0;
		return std::stoi(key.substr(pos));
	}
	template<typename T>
	sf::Vector2f getCenter(const T& r) {
		sf::FloatRect b;


		if constexpr (requires { r.getGlobalBounds(); }) {
			b = r.getGlobalBounds();

		}
		else if constexpr (requires { r.rect; }) {
			b = r.rect;

		}
		return sf::Vector2f(
			b.position.x + b.size.x * 0.5f,
			b.position.y + b.size.y * 1.f
		);
		};

	void spawningAnimal(SpawningObj& spawningObjClass, sf::RectangleShape& pShape,Renderer& renderer) {

		for (auto& [key, sprite] : spawningObjClass.sprites) {

			


				


			if (key.rfind("world_", 0) == 0) {


				if (isColliding(pShape, sprite)) {
					std::string currentMap = key;

					size_t start = currentMap.find_first_of("0123456789");
					size_t end = currentMap.find_first_not_of("0123456789", start);

					int value = std::stoi(currentMap.substr(start, end - start));

					value = std::max(0, value );

					std::string digits = std::to_string(value);
					//it is still laggy letme fic
					for (auto it = renderer.drawableSpriteANIMAL.begin();
						it != renderer.drawableSpriteANIMAL.end();) {
						size_t start2 = it->first.find_first_of("0123456789");
						size_t end2 = it->first.find_first_not_of("0123456789", start2);

						std::string digits2 = it->first.substr(start2, end2 - start2);

						//std::cout << "D2 = " << digits2 << '\n' << "D1 = " << digits << "\n";
						if (digits != digits2 && it->first.find(digits2) != std::string::npos) {
							it = renderer.drawableSpriteANIMAL.erase(it); 
						
						}
						else {
							++it;
						}


					}

					currentMap.erase(
						std::remove_if(currentMap.begin(), currentMap.end(), ::isdigit),
						currentMap.end()
					);

					spawningObjClass.spawningAnimals(key, pShape.getPosition(), sprite, renderer,spawningObjClass.normalAnimals[(rand() % spawningObjClass.normalAnimals.size())], currentMap.substr(6 + newWorldDir.length() + 1));

				}
			}
		}
	}

	std::vector<sf::Vector2u> textureSizes;

	SAT sat;
	std::unordered_map<std::string, sf::Sprite> worldSprites;

};