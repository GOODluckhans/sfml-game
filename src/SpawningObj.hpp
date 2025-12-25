#pragma once 
#include "MainLibraries/main.hpp"
#include "Tiled.hpp"
#include "EntityFuncition.hpp"
#include "Scene/SceneManager.hpp"
#include "Renderer.hpp"
#include "ECS/ComponentStorage.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/Entity.hpp"
#include <json.hpp>
#include <fstream>
using json = nlohmann::json;
struct WorldTile
{
	sf::Vector2u size;   // <--- Store the texture size HERE
	sf::Vector2f offset; // <--- Where this tile exists in the world

};

struct worldObjData {
	sf::FloatRect rect;
	std::string name;
};

struct animalDataPos {
	sf::FloatRect rect;
	std::string name;
};

class TextureManager {
public:
    std::unordered_map<std::string, sf::Texture> textures;

    sf::Texture& get(const std::string& path) {
        if (!textures.contains(path)) {
            textures[path].loadFromFile(path);
        }
        return textures[path];
    }
};


class SpawningObj : public EntityFunction {
public:
	SpawningObj() {}
	TextureManager textureManager;
	std::unordered_map<std::string, WorldTile> worlds;
	void spawnObject(Tiled& tiled) {
		int index = 0;

		for (auto& solid : tiled.solids) {
			if (solid.name.rfind("TREE_BOTTOM", 0) == 0) { // match all TREE_BOTTOM*

				
				sf::Sprite sprite(textureManager.get("assets/Textures/TREE_TOP.png"));
				sprite.setPosition({ solid.rect.position.x - 7.5f, solid.rect.position.y - 55.f });
				sprite.setScale({ 1.f, 1.f });

				// IMPORTANT: unique name
				sprites.emplace("TREE_TOP" + std::to_string(index), sprite);

				//textures.push_back(texture);
				index++;
			}
			else  if (solid.name.rfind("PUT_PLANT_", 0) == 0) { // match all TREE_BOTTOM*
				std::string plantType = solid.name.substr(10); // get the plant type after "PUT_PLANT_"std::String
				plantType.erase(
					std::remove_if(plantType.begin(), plantType.end(),
						[](char c) { return std::isdigit(c); }),
					plantType.end()
				);

				// 2. Remove "BOTTOM"
				const std::string toRemove = "BOTTOM";
				size_t pos = plantType.find(toRemove);
				if (pos != std::string::npos) {
					plantType.erase(pos, toRemove.length());
				}

				// 3. Add Top (consistent!)
				plantType += "Top";  // Make sure your file is LavenderTop.png

		
				sf::Sprite sprite(textureManager.get("assets/Textures/" + plantType + ".png"));
				sprite.setPosition({ solid.rect.position.x - 8.f, solid.rect.position.y - 32.f });
				sprite.setScale({ .5f, .5f });

				// IMPORTANT: unique name
				sprites.emplace("PUT_" + plantType + std::to_string(index), sprite);

				index++;
			}

		}
	}


	void update(float& dt, World& world, Audio& audio, Input* input = nullptr, Physics* physics = nullptr, SceneType* type = nullptr) override {
		//std::cout << spritesWorldObj.size() << '\n';
		if (spritesWorldObj.size() > 11) {
			spritesWorldObj.clear();
			hitboxSpawnedObj.clear();
		}
		//if (spriteAnimals.size() > 11) /*spritesWorldObj*/.clear();

		//ground laggy on

	}
	void render(sf::RenderWindow& window, Renderer& renderer, Camera* camera = nullptr) override {
		bool TRUE = true;
		for (auto& [name, sprite] : sprites) {
			renderer.drawPlayer(sprite, name);
		}
		for (auto& [name, sprite] : spawnPlace) {
			renderer.drawPlayer(sprite, name);
		}
		for (auto& [name, sprite] : spritesWorldObj) {
			renderer.drawPlayerTWEAKABLE(name, &sprite, nullptr);
		};
		for (auto& [name, hitboxes] : hitboxSpawnedObj) {
			renderer.drawPlayerTWEAKABLE(name, nullptr, &hitboxes);
		}
		for (auto& [name, sprite] : spriteAnimals) {
			if (renderer.drawableSpriteANIMAL.size() >= 20) {
				auto it = renderer.drawableSpriteANIMAL.begin(); // oldest
				renderer.drawableSpriteANIMAL.erase(it);
			}

			renderer.drawPlayerTWEAKABLE(name, &sprite, nullptr);

		}


	}
	std::unordered_map<std::string, sf::Sprite> spawnPlace;
	void firstSpawnWorld( sf::RenderWindow& window) {
		e = entityManager.createEntity();

		// load texture

		sf::Sprite bg1(textureManager.get("assets/Textures/WorldSpawns/spawnPlace.png"));

		sf::Vector2u windowSize = window.getSize();                           // e.g. 800×600
		sf::Vector2u textureSize = textureManager.get("assets/Textures/WorldSpawns/spawnPlace.png").getSize();

		// Set origin center → biar scaling mulus
		//bg1.setOrigin({ textureSize.x / 2.f, textureSize.y / 2.f });

		// Position exactly at the center
		bg1.setPosition({ 0,0 });


		bg1.setScale({ (float)window.getSize().x / window.getSize().x, (float)window.getSize().y / textureSize.y });

		spawnPlace.emplace("world_center", bg1);
	}


	int indexWorld_NegativeY = 0;
	int indexWorld_PositiveY = 0;
	int indexWorld_NegativeX = 0;
	int indexWorld_PositiveX = 0;

	WorldTile tile;
	sf::Vector2f spawnPos;
	std::string currentWorldKey;
	float downOffset, upOffset, leftOffset, rightOffset;
	void worldGenerator(const std::string& dirSpawn, sf::Vector2f& currentWorldOffset, sf::RenderWindow& window, const std::string& textureName,
		int lastWorld_NEGX, int lastWorld_NEGY, int lastWorld_POSX, int lastWorld_POSY, const sf::Vector2f& playerPos) {
		// Create 

		if (dirSpawn == "") return;
		std::string worldKey;
		if (sprites.contains(worldKey)) return; // don’t spawn again
		if (dirSpawn == "Up") {
			worldKey = "world_" + dirSpawn + std::to_string(indexWorld_NegativeY) + "_" + textureName;
			indexWorld_NegativeY++; // only increment if we actually spawn
		}
		else if (dirSpawn == "Down") {
			worldKey = "world_" + dirSpawn + std::to_string(indexWorld_PositiveY) + "_" + textureName;
			indexWorld_PositiveY++; // only increment if we actually spawn
		}
		else if (dirSpawn == "Left") {
			worldKey = "world_" + dirSpawn + std::to_string(indexWorld_NegativeX) + "_" + textureName;
			indexWorld_NegativeX++;
		}
		else if (dirSpawn == "Right") {
			worldKey = "world_" + dirSpawn + std::to_string(indexWorld_PositiveX) + "_" + textureName;
			indexWorld_PositiveX++;
		}
		//world_Down0_Boreal Forest(Taiga)

		//e = entityManager.createEntity();

		// Load texture safely using shared_ptr
		//auto texture = std::make_shared<sf::Texture>();
		//if (!texture->loadFromFile(textureFile)) {
		//    std::cerr << "Failed to load texture: " << textureFile << "\n";
		//    return;
		//}




		//textures.push_back(texture);
		e = entityManager.createEntity();
		std::string nameWorld = textureName;
		std::string pathFile = "assets/Textures/WorldSpawns/" + nameWorld + ".png";
		// load texture
		
		isRenderingOBJ[worldKey] = false;
		// Create 

		sf::Sprite sprite(textureManager.get(pathFile));

		// Center origin for proper scaling
		sf::Vector2u texSize = textureManager.get(pathFile).getSize();
		sprite.setOrigin({ texSize.x / 2.f, texSize.y / 2.f });

		// Scale sprite to fit window
		sf::Vector2u winSize = window.getSize();
		sprite.setScale({
			static_cast<float>(winSize.x) / texSize.x,
			static_cast<float>(winSize.y) / texSize.y }
			);

			sf::Vector2f spawnPos;
			if (dirSpawn == "Down") { spawnPos = { static_cast<float>(texSize.x) * 0.5f, playerPos.y + (texSize.y * 0.5f - 1.f) }; }
			else if (dirSpawn == "Up") { spawnPos = { static_cast<float>(texSize.x) * 0.5f, playerPos.y - (texSize.y * 0.5f - 8.f) }; }
			else if (dirSpawn == "Right") { spawnPos = { playerPos.x + (texSize.x * 0.5f - 5.f),  static_cast<float>(texSize.y) * 0.5f }; }  // tomorrow let me fix this spawn for right and up so it and shrinking 
			else if (dirSpawn == "Left") { spawnPos = { playerPos.x - (texSize.x * 0.5f - 18.f),  static_cast<float>(texSize.y) * 0.5f }; }

		// finally apply
		sprite.setPosition(spawnPos);
		spawningObjectByTheWorldRandomly(sprite, worldKey, dirSpawn);
		//spawningAnimals(worldKey,playerPos);
		// Store sprite with unique name (or overwrite if same)
		sprites.emplace(worldKey, sprite);
		//std::cout << sprites.size() << " worlds spawned\n";
	}

	int idk100 = 0;
	void spawningObjectByTheWorldRandomly(sf::Sprite& sprite, const std::string& worldKey, const std::string& dirSpawn) {
		if (worldKey.find("world_") != std::string::npos) {
			spawnedObj.push_back(worldKey);

			std::string treeWorld = "Tree_" + worldKey;
			std::string buildingWorld = "Structure_" + worldKey;
			spawnSingleOBJ(sprite, worldKey, treeWorld, dirSpawn);
			spawnSingleOBJ(sprite, worldKey, buildingWorld, dirSpawn);

			//spawnin

			std::cout << hitboxSpawnedObj.size() << '\n';
		}
	}

	void spawnSingleOBJ(sf::Sprite& sprite, const std::string& worldKey, std::string& typeOfSpawn, const std::string& dirSpawn) {
		//auto& counter = spawningCounter["tree_" + worldKey];
		//if (counter >= 3) return;
		//counter++;
		auto bounds = sprite.getGlobalBounds();
		if (typeOfSpawn.rfind("Tree_", 0) == 0) {


			std::string path = typeOfSpawn;


			std::string toDeleteInt;
			for (auto& c : path) {
				if (!std::isdigit(c)) continue;
				toDeleteInt += c;  // convert char digit to int
			}
			std::string toDelete = dirSpawn + toDeleteInt + "_";

			size_t pos = path.find(toDelete);
			if (pos != std::string::npos) {
				path.erase(pos, toDelete.length());
			}
			int numTrees = 10; // how many trees to spawn per tile

			for (int i = 0; i < numTrees; i++) {
				sf::Sprite temperateTree(textureManager.get("assets/Textures/spriteOfSpawnObj/" + path + ".png"));
				sf::RectangleShape hitbox;

				temperateTree.setScale({ 0.2f, 0.2f }); // scale first!

				// recalc bounds after scaling
				auto treeBounds = temperateTree.getGlobalBounds();

				// random offsets within tile bounds
				float maxXOffset = bounds.size.x - treeBounds.size.x;
				float maxYOffset = bounds.size.y - treeBounds.size.y;

				float randomX = bounds.position.x + static_cast<float>(rand()) / RAND_MAX * maxXOffset;
				float randomY = bounds.position.y + static_cast<float>(rand()) / RAND_MAX * maxYOffset;

				temperateTree.setPosition({ randomX, randomY });

				// hitbox
				auto temperateTreeBounds = temperateTree.getGlobalBounds();
				float widthRatio = 0.25f;
				float heightRatio = 0.15f;
				float xOffset = 0.40f;
				float yOffset = 0.8f;

				hitbox.setSize({
					temperateTreeBounds.size.x * widthRatio,
					temperateTreeBounds.size.y * heightRatio
					});

				hitbox.setPosition({
					temperateTreeBounds.position.x + (temperateTreeBounds.size.x * xOffset),
					temperateTreeBounds.position.y + (temperateTreeBounds.size.y * yOffset)
					});

				hitbox.setFillColor(sf::Color(255, 0, 0, 120));
				/*    worldObjDatas.push_back({ hitbox.getGlobalBounds(), "hitboxSpawnedObjWorld_" + typeOfSpawn + worldKey + std::to_string(i) });
					worldObjDatas.push_back({ temperateTree.getGlobalBounds(),"spawnedObjWorld_" + typeOfSpawn + worldKey + std::to_string(i) });*/

				spritesWorldObj.emplace("spawnedObjWorld_" + typeOfSpawn + worldKey + std::to_string(i), temperateTree);
				hitboxSpawnedObj.emplace("hitboxSpawnedObjWorld_" + typeOfSpawn + worldKey + std::to_string(i), hitbox);
				idk100++;
			}

		}
		else if (typeOfSpawn.rfind("Structure_", 0) == 0) {



			std::string path = typeOfSpawn;


			int toDeleteInt = 0;
			for (auto& c : path) {
				if (!std::isdigit(c)) continue;
				toDeleteInt = c - '0';  // convert char digit to int
			}
			std::string toDelete = dirSpawn + std::to_string(toDeleteInt) + "_";

			size_t pos = path.find(toDelete);
			if (pos != std::string::npos) {
				path.erase(pos, toDelete.length());
			}
			std::string typeOfBuild = path;


			int randomStructure = (rand() % 100) + 1;

			if (randomStructure <= 50) typeOfBuild = "_TAMPLE";
			else if (randomStructure <= 100) typeOfBuild = "_OLDHOUSE";

			std::cout << path << typeOfBuild << '\n';


			sf::Sprite temperateTree(textureManager.get("assets/Textures/spriteOfSpawnObj/" + path + typeOfBuild + ".png"));
			sf::RectangleShape hitbox;

			temperateTree.setPosition({
			  bounds.position.x + rand() % static_cast<int>(bounds.size.x),
			bounds.position.y + rand() % static_cast<int>(bounds.size.y)
				});

			temperateTree.setScale({ 0.2f, 0.2f });

			auto temperateTreeBounds = temperateTree.getGlobalBounds();
			float widthRatio = 0.25f;
			float heightRatio = 0.15f;
			float xOffset = 0.40f;
			float yOffset = 0.8f;

			hitbox.setSize({
				temperateTreeBounds.size.x * widthRatio,
				temperateTreeBounds.size.y * heightRatio
				});

			hitbox.setPosition({
				temperateTreeBounds.position.x + (temperateTreeBounds.size.x * xOffset),
				temperateTreeBounds.position.y + (temperateTreeBounds.size.y * yOffset)
				});
			hitbox.setFillColor(sf::Color(255, 0, 0, 120));

			/*        worldObjDatas.push_back({ hitbox.getGlobalBounds(), "hitboxSpawnedObjWorld_" + typeOfSpawn + worldKey + std::to_string(1) });
					worldObjDatas.push_back({ temperateTree.getGlobalBounds(),"spawnedObjWorld_" + typeOfSpawn + worldKey + std::to_string(1) });*/

			spritesWorldObj.emplace("spawnedObjWorld_" + typeOfSpawn + worldKey + std::to_string(1), temperateTree);
			hitboxSpawnedObj.emplace("hitboxSpawnedObjWorld_" + typeOfSpawn + worldKey + std::to_string(1), hitbox);

		}
	}


	void spawningAnimals(const std::string& worldKey, const sf::Vector2f& playerPos, sf::Sprite& spriteWorldGround, Renderer& render, const std::string& typeAnimal
		, const std::string& typeofPlace) {
		spriteAnimals.clear();
		int randomSpawnCounter = (rand() % 5) + 1;
		int animalID = 0;
		std::ifstream file("assets/JSON/animalData/animal.json");


	
	if (spriteAnimals.size() > 5) {
		spriteAnimals.clear();
	}


		if (!file.is_open()) {
			return;
		}

		json data;
		file >> data;


		if (!data["animals"].contains(typeofPlace))
			return;

		for (int i = 0; i < randomSpawnCounter; i++) {

			animal = animalEntityManager.createEntity();
			animalComponentStorage.sprites[animal].texture = new sf::Texture();
			std::string path = "assets/Textures/animals/normal/" + typeAnimal + ".png";
			sf::Sprite normalAnimal(textureManager.get(path));
			normalAnimal.setScale({ 0.5f,0.5f });
			float range = 120.f;

			float offsetX = (static_cast<float>(rand()) / RAND_MAX * 2.f - 1.f) * range;
			float offsetY = (static_cast<float>(rand()) / RAND_MAX * 2.f - 1.f) * range;

			normalAnimal.setPosition(
				spriteWorldGround.getPosition() +
				sf::Vector2f(offsetX, offsetY)
			);

			spriteAnimals.emplace("Animal_" + typeAnimal + "_" + worldKey + "_" + std::to_string(i), normalAnimal); //15





		}

		for (auto i = 0; i <= 2; i++) {
			for (auto& animalData : data["animals"][typeofPlace]) {
				animalKnownByPlace = animalEntityManager.createEntity();

				int roll = rand() % 100; // 0 - 99
				int chance = animalData["chance"].get<int>();
				if (roll < chance) {

					std::string path =
						"assets/Textures/animals/" + typeofPlace + "/" +
						animalData["name"].get<std::string>() + ".png";
				

					sf::Sprite randomAnim(textureManager.get(path));

					randomAnim.setScale({ 0.5f,0.5f });
					float range = 120.f;

					float offsetX = (static_cast<float>(rand()) / RAND_MAX * 2.f - 1.f) * range;
					float offsetY = (static_cast<float>(rand()) / RAND_MAX * 2.f - 1.f) * range;

					randomAnim.setPosition(
						spriteWorldGround.getPosition() +
						sf::Vector2f(offsetX, offsetY)
					);

					spriteAnimals.emplace("Animal_" + animalData["name"].get<std::string>() + "_" + worldKey + "_" + std::to_string(i
					), randomAnim); //15

				}
			}
		}



		file.close();



	}



	// outside the function
	std::vector<std::string> normalAnimals = { "sheep","chicken" };
	std::unordered_map<std::string, sf::Sprite> spriteAnimals;
	std::unordered_map<std::string, bool> isRenderingOBJ;
	std::unordered_map<std::string, unsigned int> spawningCounter;
	std::vector<std::string> spawnedObj;
	std::unordered_map<std::string, sf::RectangleShape> hitboxSpawnedObj;
	std::unordered_map<std::string, sf::Sprite> sprites;
	std::unordered_map<std::string, sf::Sprite> spritesWorldObj;
	std::vector<std::shared_ptr<sf::Texture>> textures;
	std::vector<worldObjData> worldObjDatas;
	std::vector<animalDataPos> animalObjData;
	ComponentStorage<sf::Sprite> componentStorage;
	Entity e;
	EntityManager entityManager;

	ComponentStorage<sf::Sprite> animalComponentStorage;
	Entity animal;
	Entity animalKnownByPlace;
	EntityManager animalEntityManager;

};