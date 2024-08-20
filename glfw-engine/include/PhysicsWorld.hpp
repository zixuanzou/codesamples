#ifndef PhysicsWorld_hpp
#define PhysicsWorld_hpp

#include <iostream>
#include <unordered_map>

#include "Collision.hpp"
#include "Solver.hpp"

class PhysicsWorld {
private:
    std::unordered_map<int, RigidBody*> m_Objects;
    std::vector<Solver*> m_Solvers;
    Vector3f m_Gravity;
    
public:
    PhysicsWorld();
    
    RigidBody* AddRigidBody(const Vector3f& position);
    void RemoveRigidBody(int rigidBodyID);
    void AddSolver(Solver* solver);
    void RemoveSolver(Solver* solver);
    
    void ApplyGravity();
    void ResolveCollisions(float deltaTime);
    void UpdateRigidBodies(float deltaTime);
    void Step(float deltaTime);
};

#endif /* PhysicsWorld_hpp */
