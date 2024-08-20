#include "Collision.hpp"

Collision::Collision(RigidBody* a, RigidBody* b, ManifoldPoints p) : rbA(a), rbB(b), points(p) {}
