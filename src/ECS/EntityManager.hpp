#pragma once 
#include "../MainLibraries/main.hpp"
#include "Entity.hpp"
class EntityManager {
public:
	EntityManager() {}

	Entity createEntity() {
		return nextEntityID++;
	}

private:
	Entity nextEntityID = 0;
};