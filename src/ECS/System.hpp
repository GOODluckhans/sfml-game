#pragma once 
#include "../MainLibraries/main.hpp"


class MovementSystem {
public:
    template<typename T>
    void update(
        Velocity& velocities,
        float& dt,
        T& shape,const std::string nameObj = "normalStatic"
    ) {

        if (nameObj == "normalStatic")
            shape.move({ velocities.vx * dt,velocities.vy * dt });
        else if (nameObj == "bullet")
            shape.move({ velocities.vx * 400.f *  dt,velocities.vy * 400.f * dt });


    }
};
