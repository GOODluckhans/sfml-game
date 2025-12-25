#pragma once 
#include "MainLibraries/main.hpp"
class Hitboxes {
public:
	Hitboxes() {}
	virtual ~Hitboxes() {}
	virtual void createHitbox(sf::Sprite& entity) = 0;
	virtual sf::RectangleShape& getHitbox() = 0;
	//virtual void updateHitbox() = 0;
	virtual void renderHitbox(sf::RenderWindow& window, Renderer& renderer) = 0;
protected:

	 std::unordered_map<std::string,sf::RectangleShape> hitboxShape;
};

//tomorrow letme fix the player hitbox update function