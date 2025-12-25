#pragma once 
#include "MainLibraries/main.hpp"


class Audio {
public:
	Audio() {}
		void playSound(sf::Sound& sound){
		
			sound.play();

		}
		void playMusic(const std::string& musicFile){}
		void stopMusic() {}
};