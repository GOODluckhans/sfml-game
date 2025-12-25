#pragma once 
#include "MainLibraries/main.hpp"

struct animationStruct {
	sf::Clock clock;
};

class Animation {
public:
	Animation() {}
	animationStruct animStruct;
	void animInit(sf::IntRect& spriteAnims) {
		spriteAnim = spriteAnims;
	}

	void setTextureRect(sf::Sprite& character) {
		character.setTextureRect(spriteAnim);
	}

	template<typename T>
	void animationRun(T& structPlayer, sf::Sprite& character
		, int maximumX = 306, int addingX = 102
	) {
		if (structPlayer.right) {
			spriteAnim.position.y = 456;
			if (animStruct.clock.getElapsedTime().asSeconds() > 0.25f) {
				if (spriteAnim.position.x == maximumX) {  // use `.left`, not `.position.x`
					spriteAnim.position.x = 0;
				}
				else {
					spriteAnim.position.x += addingX;
				}
				character.setTextureRect(spriteAnim);
				animStruct.clock.restart();
			}
		}
		else if (structPlayer.left) {
			spriteAnim.position.y = 304;

			if (animStruct.clock.getElapsedTime().asSeconds() > 0.25f) {
				if (spriteAnim.position.x == maximumX) {  // use `.left`, not `.position.x`
					spriteAnim.position.x = 0;
				}
				else {
					spriteAnim.position.x += addingX;
				}
				character.setTextureRect(spriteAnim);
				animStruct.clock.restart();
			}
		}
		else if (structPlayer.up) {
			spriteAnim.position.y = 152;

			if (animStruct.clock.getElapsedTime().asSeconds() > 0.25f) {
				if (spriteAnim.position.x == maximumX) {  // use `.left`, not `.position.x`
					spriteAnim.position.x = 0;
				}
				else {
					spriteAnim.position.x += addingX;
				}
				character.setTextureRect(spriteAnim);
				animStruct.clock.restart();
			}
		}
		else if (structPlayer.down) {
			spriteAnim.position.y = 0;

			if (animStruct.clock.getElapsedTime().asSeconds() > 0.25f) {
				if (spriteAnim.position.x == maximumX) {  // use `.left`, not `.position.x`
					spriteAnim.position.x = 0;
				}
				else {
					spriteAnim.position.x += addingX;
				}
				character.setTextureRect(spriteAnim);
				animStruct.clock.restart();
			}
		}
	}

	template<typename T>
	void handleIdle(T& character, const std::string& lastDirMove, int width = 102, int height = 152,
		int xDown = 0, int xTop = 0, int xLeft = 0, int xRight = 306,
		int yDown = 0, int yTop = 152, int yLeft = 304, int yRight = 456
	) {
		if (lastDirMove == "Down")
			spriteAnim = sf::IntRect({ xDown, yDown }, { width, height });
		else if (lastDirMove == "Top")
			spriteAnim = sf::IntRect({ xTop, yTop }, { width, height });
		else if (lastDirMove == "Left")
			spriteAnim = sf::IntRect({ xLeft, yLeft }, { width, height });
		else if (lastDirMove == "Right")
			spriteAnim = sf::IntRect({ xRight, yRight }, { width, height });

		character.setTextureRect(spriteAnim);
	}


private:
	sf::IntRect spriteAnim;
};
