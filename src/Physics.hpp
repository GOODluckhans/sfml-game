#pragma once

#include "MainLibraries/main.hpp"
#include "Input.hpp"
//#include "Collision.hpp"

#include "World.hpp"
#include "Bullet.hpp"
#include "GUI.hpp"

#include "ECS/System.hpp"
#include "Scene/SceneManager.hpp"
#include "Tiled.hpp"
#include "Collectable_item.hpp"
#include "SpawningObj.hpp"
#include "Scene/GameScene.hpp"
//class Collision;  // forward declaration in Input.hpp

class Enemy;
class Physics {
public:
	MovementSystem moveSys;
	Physics() {}
	template<typename T,typename T2>
	void gravityEffect(T& shape, float& dt, T2& v,SceneType& type) {
		if (type == SceneType::GAME) {
			const float gravity = 980.f; // pixels per second squared
			if constexpr (requires { v.vy; }) {
				v.vy += gravity * dt; // apply gravity to vertical velocity
				shape.move({ v.vx * dt, v.vy * dt }); // apply movement with dt
			}
			else if constexpr (requires { v.y; }) {
				v.y += gravity * dt; // apply gravity to vertical velocity for sf::Vector2f
				shape.move(v * dt); // apply movement with dt
			}
		}
	}
	template<typename T,typename T2>
	void moveEntityWithVelocity(T& shape, T2& velocity, float dt, SceneType& type) {
		if (type == SceneType::GAME) {
			moveSys.update(velocity, dt, shape);
		}
		//shape.move({ velocity.vx  * dt,velocity.vy * dt });
	}
};