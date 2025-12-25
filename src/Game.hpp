#pragma once
#include "MainLibraries/main.hpp"
#include "Renderer.hpp"
#include "Player.hpp"
#include "World.hpp"
#include "Input.hpp"

#include "Enemy.hpp"
#include "NPC.hpp"
#include "Collision.hpp"
#include "Physics.hpp"
#include "GUI.hpp"

#include "Animation.hpp"
#include "Scene/SceneManager.hpp"
#include "Scene/GameScene.hpp"

#include "Scene/MenuScene.hpp"
#include "Tiled.hpp"

#include "SAVERDATA/savedGame.hpp"
#include "Audio.hpp"
#include "Video.hpp"
#include "NEWFILE.hpp"
#include "INVENTORY.hpp"
#include "Collectable_item.hpp"
#include "SpawningObj.hpp"
#include "animals.hpp"
//🎮 Game.hpp / Game.cpp
//
//This is the main brain of your game.
//It controls :
//
//creating the window
//
//the game loop(event → update → render)
//
//delta time
//
//the world + player + renderer
//
//Basically the “director” of your game.x1x
struct windowInfo {
	unsigned int WIDTH = 768;
	unsigned int HEIGHT = 448;

};
class Game {
public:
	
	Game() :
		window(sf::VideoMode({ windowInfo().WIDTH,windowInfo().HEIGHT}), "My SFML Game", sf::Style::Close)
	{
		window.setFramerateLimit(60);
	} // creating window + initializing everything

	void run() {
		// Update Shapes
		UpdateShapes();
		

		//sound
		sf::SoundBuffer buffer;
		if (!buffer.loadFromFile("assets/Sounds/gunshot-37055.ogg")) {
			// error handling
			return;
		}
		sf::Sound sound(buffer);

		//video
	/*	sfe::Movie movie;
		if (!movie.openFromFile("assets/Video/Crowsworn Pixel Art Animations.mp4")) {
			return;
		}*/
		video.updload(window, videoPlaying);  // <-- load ONLY once
		
		//float dtVid = 0;
		sf::Clock videoClock;

			animationInit();
		//create new file
		createNewFiles();

		//hitboxes
		player.createHitbox(player.getShape());
		//animal class 
		animal.gettingAnimalGoods(player.getShape(),spawningObj,renderer);

		//spawning objects
		spawningObj.spawnObject(tiled);
			spawningObj.firstSpawnWorld(window);

		while (window.isOpen()) {
			//hitboxes update
			//player.updateHitbox();
	
			//slots
			collectableItem.removeItem(gui,renderer);

			// ---------------- Input System ----------------
			proccessEvents();
			InputPlayer inputState = input.getInputState();

			// ---------------- Time System ----------------
			deltaTime = world.getDeltaTime();
			// deltaTimeEnemy = world.getDeltaTimeEnemy();

			// Reset player velocity x-component
			world.getVelocityPlayer(player.componentStorage.velocities[player.plr]).vx = 0.f;
			//-------------------Video /////////////////////////////////////
			//video.update(movie);
			   // Read next frame
			//dtVid = world.getDeltaTime();
			if (!videoPlaying && sceneManager.currentScene == SceneType::GAME) {
				videoPlaying = true;
				videoEnded = false;
				if (typeStory.empty()) continue;
				if (typeStory == "testNpc Story") {
					video.updload(window, videoPlaying, "Crowsworn Pixel Art Animations.mp4");  // <-- load ONLY once
					typeStory = "";
				}
			}

			video.runVid(videoEnded, videoClock,renderer,videoPlaying);

			

			// ---------------- Player State System ----------------
			player.updateState(deltaTime, world, animationPlayer); 
			enemy.updateState(deltaTime, world, animationEnemy);
			// ---------------- Physics System ----------------
			player.update(deltaTime, world, audio, &input, &physics, &sceneManager.currentScene);
			enemy.update(deltaTime, world, audio, &input, &physics, &sceneManager.currentScene);

			// Bullet updates
			player.updateBullets(deltaTime, world, audio, window, input, bullet);
			//COLLISION ONLY ______________
			collisionWorks();
		
			bullet.deleted(collision.deletingBullet);
			
			animal.update(deltaTime,world,audio);
			// Apply gravity
					//physics.gravityEffect(player.getShape(), deltaTime, world.getVelocityPlayer(player.componentStorage.velocities[player.plr]), sceneManager.currentScene);
			//physics.gravityEffect(enemy.getShape(), deltaTime, world.getVelocityEnemy(enemy.componentStorage.velocities[enemy.enemEntity]), sceneManager.currentScene);

			// Shooting
			if (input.shooting) {
				player.shoot(deltaTime, world, window, bullet, camera);

				audio.playSound(sound);
			}
			// ---------------- Animation System ----------------
			animationPlayer.animationRun(inputState, player.getShape());
			animationEnemy.animationRun(enemy.inputEnemy, enemy.getShape());

			//animationPlayer.handleIdle(player.getShape(),lastDirMove);
			// ---------------- Camera System ----------------
	

			camera.follow(player.getShape(), deltaTime, window,
				collision.isOnMainWorld,collision.newWorldDir,spawningObj.sprites,spawningObj.sprites);
			camera.init(window);
			// ---------------- Rendering System ----------------
			DrawIt(window);

			// GUI updates
			gui.GAMEData.healthEnemPos = enemy.getShape().getPosition() + sf::Vector2f(0.f, -10.f);
			gui.update();
				renderer.render(window, camera, sceneManager, drawType, collision.isTop,spawningObj.isRenderingOBJ,collision.depthAnimalBool);
			//}
			//spawning objects 
			spawningObj.update(deltaTime, world, audio, &input, &physics, &sceneManager.currentScene);
			//spawningObj.spawningObjectByTheWorldRandomly(player.get.getPosition());
		}

	} // Starts the game loop


	sceneStruct sceneManager;
	std::string lastDirMove = "Down";

private:

	void proccessEvents() {

		std::optional<sf::Event> event; // <-- move it here

		while ((event = window.pollEvent())) {

			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
			if (auto keyboard = event->getIf<sf::Event::KeyPressed>()) {
				if (keyboard->scancode == sf::Keyboard::Scancode::E && sceneManager.currentScene == SceneType::GAME) {
					for (auto& [keyNpc, val] : npc.getShapes()) {
					if (isNPC) {
						isNPC = false;
						npc.npcWorks(keyNpc);
						typeStory = keyNpc + " Story";
						}
					}
				}

			}

			sceneFunc(event,window);
			gui.guiActive(window,event,sceneManager);
			//collecting smth
			if (event.has_value()) {
				collectableItem.catchingEvent(&(*event));
			}
			animal.catchAnimal(window, renderer,event,camera.getView(window));

		}

	}

	void DrawIt(sf::RenderWindow& window) {
		
		player.render(window, renderer, &camera);
		//world.render(window, renderer);
		enemy.render(window, renderer);
		npc.render(window,renderer);
		player.renderBullets(window, renderer, bullet);
		gui.render(renderer, window);
		menuScene.drawMENU(renderer, window);
		video.render(window, renderer);
		player.renderHitbox(window,renderer);
		spawningObj.render(window,renderer);
	}

	void UpdateShapes() {
		menuScene.mainMenuGUI("assets/fonts/BitcountPropDoubleInk-VariableFont_CRSV,ELSH,ELXP,SZP1,SZP2,XPN1,XPN2,YPN1,YPN2,slnt,wght.ttf");
		player.Shape("assets/Textures/avatar.png");
		world.Shape();
		enemy.Shape("assets/Textures/avatar.png");
		npc.Shape("assets/Textures/avatar.png");
		gui.init("assets/fonts/BitcountPropDoubleInk-VariableFont_CRSV,ELSH,ELXP,SZP1,SZP2,XPN1,XPN2,YPN1,YPN2,slnt,wght.ttf",window);
		//inventory.makingInventoryGUI("assets/Textures/inventoryGui.png");
		tiled.loadTiled("Collision", "assets/JSON/map.json");

	}

	void animationInit() {
		sf::IntRect spriteAnimChar({ 0, 0 }, { 102, 152 });
		sf::IntRect spriteAnimEnemy({ 0, 0 }, { 102, 152 });
		sf::IntRect spriteAnimNPC({ 0, 0 }, { 102, 152 });
		animationPlayer.animInit(spriteAnimChar);
		animationPlayer.setTextureRect(player.getShape());
		animationEnemy.animInit(spriteAnimEnemy);
		animationEnemy.setTextureRect(enemy.getShape());
		animationNPC.animInit(spriteAnimNPC);
		for(auto& [keyNpc , a] : npc.getShapes())
			animationNPC.setTextureRect(a);
	}

	
	template<typename T>
	void sceneFunc(T& event,sf::RenderWindow&window) {
		//about saving the game to the load menu
		if (menuScene.handleEvent(event, sceneManager,window,loadOrNew,saved,gui)) {
			std::cout << "Switching to GAME load\n";
			menuScene.rule.isSaving = false;
			sceneManager.switchTo(SceneType::GAME);

		}

		
		 if (mainGame.handleEvent(event, sceneManager))
		{
	
			sceneManager.switchTo(SceneType::MENU);

		
		}
		// Check if the saving key/button is pressed
		if (menuScene.isSaving(event, sceneManager, window)) {

			// Toggle the rule boolean
			menuScene.rule.isSaving = !menuScene.rule.isSaving;

			// Switch scene based on the new state
			if (menuScene.rule.isSaving) {
				sceneManager.switchTo(SceneType::MENU);  // go to menu
			}
			else {
				sceneManager.switchTo(SceneType::GAME);  // back to game
			}
		}

	}

	void createNewFiles() {
		newFile.createNewFile();
	}

	void collisionWorks() {
		if (bullet.bulletShapes.empty()) {
			collision.handleCollision(&player, &enemy, npc.getShapes(), player.getHitbox(), world, enemy.getShape(), bullet.getBullet(),
				window, gui, tiled, collectableItem, isNPC, spawningObj, renderer, mainGame, input);

		}
		else {
			for (auto& a : bullet.bulletShapes) {
				collision.handleCollision(&player, &enemy, npc.getShapes(), player.getHitbox(), world, enemy.getShape(), a,
					window, gui, tiled, collectableItem, isNPC, spawningObj, renderer, mainGame, input);


			}
		}

	}
	ANIMAL animal;
	SpawningObj spawningObj;
	Collectable_item collectableItem;
	Inventory inventory;
	NEWFILE newFile;
	NPC npc;
	Video video;
	Audio audio;
	SAVINGDATA saved;
	Tiled tiled;
	MainGame mainGame;
	GameMenu menuScene;
	Animation animationPlayer;
	Animation animationEnemy;
	Animation animationNPC;
	GUI gui;
	Bullet bullet;
	Camera camera;
	Physics physics;
	Collision collision;
	Enemy enemy;
	Input input;
	Player player;
	World world;
	Renderer renderer;
	sf::RenderWindow window;
	sf::Clock clockGame;
	float deltaTime;
	bool videoPlaying = false;
	bool isNPC = false;
	bool videoEnded = false;
	std::string loadOrNew = "";
	std::string typeStory = "example";
	DrawType drawType = DrawType::LAST;
};