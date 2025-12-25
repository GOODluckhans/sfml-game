#pragma once 

#include "MainLibraries/main.hpp"
#include "Physics.hpp"
#include "Renderer.hpp"
#include "EntityFuncition.hpp"
#include "World.hpp"
#include "Input.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/ComponentStorage.hpp"
#include "EntityStates.hpp"
#include "Animation.hpp"



//#include "Bullet.hpp"
struct InputEnemy {
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool shooting = false;
};
class Enemy : public EntityFunction , public State {
public:
	Enemy() {}

	EntityManager entityManager;
	ComponentStorage<sf::Sprite> componentStorage;
	Entity enemEntity;
	void Shape(const char* file) {
		//player
		enemEntity = entityManager.createEntity();
		


		componentStorage.sprites[enemEntity].texture = new sf::Texture(file);



		sf::Sprite enemShape(*componentStorage.sprites[enemEntity].texture);
		
		componentStorage.positions[enemEntity].x = 375.f;
		componentStorage.positions[enemEntity].y = 0.0f;
		componentStorage.sizes[enemEntity].x = 0.2f;
		componentStorage.sizes[enemEntity].y = 0.2f;
		enemShape.setScale({ componentStorage.sizes[enemEntity].x, componentStorage.sizes[enemEntity].y });
		enemShape.setPosition({ componentStorage.positions[enemEntity].x, componentStorage.positions[enemEntity].y });
		enemSprite.push_back(enemShape);
	}
	Velocity& vel = componentStorage.velocities[enemEntity];
	void update(float& dt, World& world, Audio& audio, Input* input , Physics* physics, SceneType* type ) override {
		// Move enemy using its velocity
		vel = componentStorage.velocities[enemEntity];
		getShape().move({ vel.vx  * dt, vel.vy* dt });
		physics->moveEntityWithVelocity(getShape(),world.getVelocityEnemy(vel), dt, *type);
		// Randomly change direction
		changeTimer += dt;
		if (changeTimer >= changeInterval) {
			chooseNewDirection(world);
			changeTimer = 0.f;
		}
	}

	sf::Sprite& getShape() {
		return enemSprite[0];
	}

	void render(sf::RenderWindow& window, Renderer& renderer, Camera* camera = nullptr) override {
		renderer.drawPlayer( getShape(),"entity_enemy");

	}

	sf::Vector2f direction;
	InputEnemy inputEnemy;

	void changeState(EntityStates& newState) override {
		state = newState;


	}


	void enter()  override {

	}
	void updateState(float dt, World& world, Animation& anim) override {

		EntityStates newState = state;
		if (state != EntityStates::Attacking) {  // attacking locks movement
			if (isMoving) {
				newState = EntityStates::Moving;
			}
			else {
				newState = EntityStates::Idle;
				anim.handleIdle(getShape(), lastDirMove);
			}

			changeState(newState);


		}

	}
	void exit()override {

	}
private:

	bool isMoving = false;
	float changeTimer = 0.f;
	float changeInterval = 1.5f; // change direction every 1.5 seconds
	void chooseNewDirection(World& world) {
		int dir = rand() % 5;
		inputEnemy.up = false;
		inputEnemy.down = false;
		inputEnemy.left = false;  // it shall be reset and on the top
		inputEnemy.right = false;
		isMoving = false;

			switch (dir) {
			case 0:
				vel = { 100.f, 0.f };
				inputEnemy.right = true;
				isMoving = true;
				lastDirMove = "Right";
				break;

			case 1:
				vel = { -100.f, 0.f };
				inputEnemy.left = true;
				isMoving = true;
				lastDirMove = "Left";

				break;

			case 2:
				vel = { 0.f, 100.f };
				inputEnemy.down = true;
				isMoving = true;
				lastDirMove = "Down";

				break;

			case 3:
				vel = { 0.f, -100.f };
				inputEnemy.up = true;
				isMoving = true;
				lastDirMove = "Top";

				break;
			case 4:
				vel = { 0.f, 0.f };

				inputEnemy.up = false;
				inputEnemy.down = false;
				inputEnemy.left = false;  // it shall be reset and on the top
				inputEnemy.right = false;
				isMoving = false;
				break;
			}
	}


	std::string lastDirMove;
	sf::Texture texture;
	std::vector<sf::Sprite> enemSprite;
};