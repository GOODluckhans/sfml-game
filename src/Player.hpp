#pragma once
//🧍‍♂️ Player.hpp / Player.cpp
//
//This ONLY handles :
//
//player position
//
//movement speed
//
//movement updates
//
//animations(later)
//
//❗ It does NOT read keyboard input directly.
//Keyboard input is read by Input.cpp and then passed to the Player.

#include "MainLibraries/main.hpp"
#include "Renderer.hpp"
#include "EntityStates.hpp"
#include "EntityFuncition.hpp"
//#include "World.hpp"
class World;

#include "Input.hpp"
#include "Physics.hpp"
#include "Bullet.hpp"
#include "Camera.hpp"
#include "ECS/Entity.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/ComponentStorage.hpp"
#include "Animation.hpp"
#include "hitboxes.hpp"

class Player : public EntityFunction , public State , public Hitboxes{
public:
	Player() {}
	EntityManager entityManager;

	
	ComponentStorage<sf::Sprite> componentStorage;
	void Shape(const char* file) {
		//player

		plr = entityManager.createEntity();


		componentStorage.sprites[plr].texture = new sf::Texture(file);
	/*	auto& shape1 = componentStorage.sprites[plr].sprite;
		shape1.setTexture(*componentStorage.sprites[plr].texture);*/
		sf::Sprite shape1(*componentStorage.sprites[plr].texture);
	
		componentStorage.positions[plr] = { 200, 200 };
		componentStorage.sizes[plr] = { 0.2f, 0.2f };
		componentStorage.velocities[plr] = Velocity{ 0,0 };

		shape1.setPosition({ componentStorage.positions[plr].x , componentStorage.positions[plr].y });
		shape1.setScale({componentStorage.sizes[plr].x,componentStorage.sizes[plr].y });
		playerShape.push_back(shape1);

	}

	void update(float &dt,World & world , Audio & audio, Input* input, Physics* physics, SceneType* type) override {
		InputPlayer inputState = input->getInputState();
		//getShape().setPosition(getHitbox().getPosition());
		input->inputUpdate(inputState, getHitbox(), dt, world.getVelocityPlayer(componentStorage.velocities[plr]), *physics, isMoving,*type,lastDirMove,audio);
		auto bounds = getHitbox().getGlobalBounds();

		float footHeight = bounds.size.y * 0.25f;
		float footWidth = bounds.size.x * 0.5f; // 50% dari width
		getShape().setPosition({
			bounds.position.x - footWidth,
			bounds.position.y - (bounds.size.y + footHeight + 10.f)
			});
	}

	sf::Sprite& getShape() {
		for (auto& a : playerShape) {
			return a;
		}

	}

	void render(sf::RenderWindow& window,Renderer& renderer, Camera* camera) override {
		renderer.drawPlayerTWEAKABLE("player",&getShape());

	}


	void changeState(EntityStates& newState) override{
		state = newState;

	
	}


	void enter()  override{
		
	}
	void updateState(float dt,World& world,Animation& anim) override {
		
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

	//shoother

	void shoot(float& dt, World& world,sf::RenderWindow& window,Bullet& bullet,Camera& camera) {
		bullet.Shape(getShape());
	
		// Set direction toward mouse for the new bullet
		//for (int i = 0; i < bullet.bulletShapes.size(); i++)
			bullet.getDirectionTowardMouse(window, getShape(), camera);

	}

	void updateBullets(float& dt, World& world, Audio& audio,sf::RenderWindow& window ,Input& input,Bullet& bullet) {
		//for (auto& bullet : bullets) {
		bullet.update(dt, world,audio);


		/*	bullet->getBullet().move(bullet->getDirectionTowardMouse(window, bullet->getBullet()) * 500.f * dt);*/
		//}

	}

	void renderBullets(sf::RenderWindow& window, Renderer& renderer,Bullet & bullet) {
		
			bullet.render(window, renderer);
		
	}
	Entity plr;

	void createHitbox(sf::Sprite& enityt)override 
	{
		sf::RectangleShape hitbox;
		auto bounds = getShape().getGlobalBounds();
	
		float footHeight = bounds.size.y * 0.25f;
		float footWidth = bounds.size.x * 0.5f; // 50% dari width
		hitbox.setSize({
			footWidth,
			footHeight
			});

		hitbox.setPosition(getShape().getPosition());
		hitbox.setFillColor(sf::Color::Red);
		hitboxShape.emplace("hitbox_player", hitbox);
	}

	sf::RectangleShape& getHitbox() override {
		for (auto& [key, a] : hitboxShape) {
			if (key == "hitbox_player") {
				return a;
			}
		}
	}

	//void updateHitbox() override {
	//	for (auto& [key, a] : hitboxShape) {
	//		if (key == "hitbox_player") {
	//			a.setPosition(getShape().getPosition());
	//		}
	//	}
	//}

	void renderHitbox(sf::RenderWindow& window, Renderer& renderer) {
		for (auto& [key, a] : hitboxShape) {
			if (key == "hitbox_player") {
				renderer.drawPlayer(a,key);
			}
		}
	}

private:
	
	//std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
	sf::Texture texture;
	std::vector<sf::Sprite> playerShape;
	bool isMoving = false;
	std::string lastDirMove = "Down";
};