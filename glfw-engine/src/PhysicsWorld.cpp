#include "PhysicsWorld.hpp"

PhysicsWorld::PhysicsWorld() {
    m_Gravity = Vector3f(0.0f, -9.81f, 0.0f);
}

RigidBody* PhysicsWorld::AddRigidBody(const Vector3f& position) {
    RigidBody* rb = new RigidBody();
    rb->SetTransform(new Transform(position, Vector3f(1, 1, 1), 0));
    rb->SetVelocity(Vector3f(0.0f, 0.0f, 0.0f));
    rb->SetForce(Vector3f(0.0f, 0.0f, 0.0f));
    rb->SetMass(0.1f);
    m_Objects[rb->ID()] = rb;
    return rb;
}

void PhysicsWorld::RemoveRigidBody(int rigidBodyID) {
    auto it = m_Objects.find(rigidBodyID);
    if (it == m_Objects.end()) return;
    m_Objects.erase(rigidBodyID);
}

void PhysicsWorld::AddSolver(Solver* solver) {
    m_Solvers.push_back(solver);
}

void PhysicsWorld::RemoveSolver(Solver* solver) {
    if (!solver) return;
    auto it = std::find(m_Solvers.begin(), m_Solvers.end(), solver);
    if (it == m_Solvers.end()) return;
    m_Solvers.erase(it);
}

void PhysicsWorld::ApplyGravity() {
    for (auto& [id, rb] : m_Objects) {
        if (rb->TakesGravity()) {
            rb->ApplyForce(m_Gravity);
        }
    }
}

void PhysicsWorld::ResolveCollisions(float deltaTime) {
    std::vector<Collision> collisions;
    std::size_t size = m_Objects.size();
    
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) break;
            RigidBody* a = m_Objects[i];
            RigidBody* b = m_Objects[j];
            if (!a->Collider() || !b->Collider()) continue;
            
            ManifoldPoints points = a->Collider()->TestCollision(a->Transform(), b->Collider(), b->Transform());
            
            if (points.HasCollision) {
                collisions.emplace_back(a, b, points);
            }
        }
    }
    
    for (Solver* solver : m_Solvers) {
        solver->Solve(collisions, deltaTime);
    }
}

void PhysicsWorld::UpdateRigidBodies(float deltaTime) {
    for (auto& [id, rb] : m_Objects) {
        
        if (!rb->IsDynamic()) continue;
        
        Vector3f velocity = rb->Velocity() + rb->Force() * rb->InvMass() * deltaTime;
        rb->SetVelocity(velocity);
        
        Vector3f position = rb->Position() + rb->Velocity() * deltaTime;
        rb->SetPosition(position);
        
        rb->SetForce(Vector3f(0.0f, 0.0f, 0.0f));
    }
}

void PhysicsWorld::Step(float deltaTime) {
    ApplyGravity();
    ResolveCollisions(deltaTime);
    UpdateRigidBodies(deltaTime);
}
