#pragma once
//⌨️ Input.hpp / Input.cpp
//
//This is where keyboard input is read in a clean and simple way :
//

//
//
//This InputState is then given to the Player.
//Super beginner - friendly.
#include "MainLibraries/main.hpp"
#include "Physics.hpp"
#include "Player.hpp"
#include "Audio.hpp"

//#include "Bullet.hpp"
struct InputPlayer {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
	bool shooting = false;
	bool jump = false;
};

enum class Direction { None, Left, Right, Up, Down };

class Input {
public:
	Input() {}
	bool shooting = false;
	Direction dirPlayer;
    InputPlayer getInputState() {
        InputPlayer state;
        state.up = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
        state.down = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
        state.left = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
        state.right = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
		state.shooting = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
		state.jump = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
        return state;
	}
	template<typename T,typename T2>

	void inputUpdate(InputPlayer& inputState,T& shape, float& dt, T2& v ,Physics& physics,bool& isMoving ,SceneType& crScene, std::string& lastDirMove,Audio& audio) {
	
		// update velocity (NO dt here)
		isMoving = false;  // reset first

	//	if (inputState.jump) {
	//		//v.vy = -200.f;
	///*		isMoving = true;
	//		lastDirMove = "Top";*/
	//	}

		v.vx = 0.f;
		v.vy = 0.f;

		if (inputState.up) {
			v.vy = -200.f;
			isMoving = true;
			lastDirMove = "Top";
			dirPlayer = Direction::Up;
		}

		 if (inputState.down) {
			v.vy = 200.f;
			isMoving = true;
			lastDirMove = "Down";
			dirPlayer = Direction::Down;
		}
		 if (inputState.left) {
			v.vx = -200.f;
			isMoving = true;
			dirPlayer = Direction::Left;
			lastDirMove = "Left";
		 }
		 if (inputState.right) {
			v.vx = 200.f;
			isMoving = true;
			dirPlayer = Direction::Right;
			lastDirMove = "Right";
		}

		if (inputState.shooting && crScene == SceneType::GAME) {
			if (!wasPressedShoot) {
				shooting = true;          // Fire once
				wasPressedShoot = true;   // Mark that we've already fired
				
			}
			else {
				shooting = false;         // Holding → no repeat shots
			}
		}
		else if(!inputState.shooting && crScene == SceneType::GAME){
			// Reset when player releases the button
			wasPressedShoot = false;
			shooting = false;
		}

		physics.moveEntityWithVelocity(shape, v, dt, crScene);

	}

	


private:
 bool wasPressedShoot = false;
};