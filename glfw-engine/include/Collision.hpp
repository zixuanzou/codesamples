#ifndef Collision_hpp
#define Collision_hpp

#include "RigidBody.hpp"

struct Collision {
    RigidBody* rbA;
    RigidBody* rbB;
    ManifoldPoints points;
    
    Collision(RigidBody* a, RigidBody* b, ManifoldPoints p);
};

#endif /* Collision_hpp */
