#pragma once 

#include "../MainLibraries/main.hpp"

#pragma once
#include <unordered_map>
#include "Entity.hpp"
#include "Component.hpp"

template<typename T>
class ComponentStorage {
public:

    ComponentStorage() {}

    std::unordered_map<Entity, Position> positions;
    std::unordered_map<Entity, Velocity> velocities;
    std::unordered_map<Entity, Size> sizes;
    std::unordered_map<Entity, SpriteRender> sprites;

    //std::unordered_map<Entity, Health> healths;
    //std::unordered_map<Entity, Damage> damages;

    //void add(Entity e, const T& component) {
    //    data[e] = component;
    //}

    //T& get(Entity e) {
    //    return data[e];  // assume exists
    //}

    //bool has(Entity e) const {
    //    return data.find(e) != data.end();
    //}
};
