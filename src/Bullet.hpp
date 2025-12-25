#pragma once 

#include "MainLibraries/main.hpp"
#include "EntityFuncition.hpp"
#include "ECS/ComponentStorage.hpp"

#include "ECS/EntityManager.hpp"
#include "ECS/System.hpp"
class Bullet : public EntityFunction {
public:
	Entity bulletEntity;
	EntityManager entityManager;
	MovementSystem moveBUlletSys;
	ComponentStorage<sf::RectangleShape> componentStorage;
	Bullet() {

	
	}
	template<typename T>

	void Shape(T& shape){

		bulletEntity = entityManager.createEntity();

		componentStorage.positions[bulletEntity].x = shape.getPosition().x + 20.f;

		componentStorage.positions[bulletEntity].y = shape.getPosition().y + 20.f;
		componentStorage.sizes[bulletEntity].x = 5.0f;

		componentStorage.sizes[bulletEntity].y = 5.0f;

		getBullet().setSize({ componentStorage.sizes[bulletEntity].x,componentStorage.sizes[bulletEntity].y });
		getBullet().setFillColor(sf::Color::Yellow);
		getBullet().setPosition({ componentStorage.positions[bulletEntity].x ,componentStorage.positions[bulletEntity].y});
		bulletShapes.push_back(getBullet());
		// Get mouse position once


		bulletDirs.resize(bulletShapes.size(),sf::Vector2f(0.0f,0.0f));
	}

	void update(float& dt, World& world, Audio& audio, Input* input = nullptr, Physics* physics = nullptr, SceneType* type = nullptr) override {
		for (size_t i = 0; i < bulletShapes.size(); i++)
		{
				//bulletShapes[i].move(bulletDirs[i] * 400.f * dt);
			componentStorage.velocities[bulletEntity].vx = bulletDirs[i].x;
			componentStorage.velocities[bulletEntity].vy = bulletDirs[i].y;
				moveBUlletSys.update(componentStorage.velocities[bulletEntity],dt,bulletShapes[i],"bullet");
		}
		 
	}

	void render(sf::RenderWindow& window, Renderer& renderer, Camera* camera = nullptr) override {
		for (int i = 0; i < bulletShapes.size(); i++)
			//window.draw(bulletShapes[i]);
			renderer.renderDynamic(std::make_shared<sf::RectangleShape>(bulletShapes[i]));
	}

	sf::RectangleShape& getBullet() {
		return bulletShape;
	}

	template<typename T>
	void getDirectionTowardMouse(sf::RenderWindow& window, const T& player,Camera& camera)
	{
	
			if (bulletShapes.empty()) return;
			sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
			sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel, camera.getView(window));

			// Direction from player to mouse
			sf::Vector2f dir = mouseWorld - player.getPosition();

			// Normalize
			float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
			if (len != 0.f) dir /= len;

			//std::cout << dir.x << " , " << dir.y << std::endl;

			// Store in bulletDirs
			bulletDirs.back() = dir;
		
	}

	std::vector<sf::RectangleShape> bulletShapes;


	void deleted(bool& deleting) {
		if (deleting && !bulletShapes.empty()) {
			// Remove the first bullet
			bulletShapes.erase(bulletShapes.begin());
			// Remove the corresponding direction
			if (!bulletDirs.empty())
				bulletDirs.erase(bulletDirs.begin());

			deleting = false;
		}
	}


private:


	std::vector<sf::Vector2f> bulletDirs; // one per bulletShape
	//sf::Vector2f bulletVel = dir * 400.f;
	sf::RectangleShape bulletShape;
};