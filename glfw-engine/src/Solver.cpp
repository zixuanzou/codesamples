#include "Solver.hpp"

void PositionSolver::Solve(std::vector<Collision>& collisions, float deltaTime) {
    for (auto& [rbA, rbB, points] : collisions) {
        float percent = 0.8f;
        float invMassA = rbA->InvMass();
        float invMassB = rbB->InvMass();
        
        Vector3f resolution = points.Normal * percent * std::max(points.Depth, 0.0f) / (invMassA + invMassB);
        
        Vector3f posA = rbA->Position();
        Vector3f posB = rbB->Position();
        
        if (rbA->IsDynamic()) rbA->SetPosition(posA - resolution * invMassA);
        if (rbB->IsDynamic()) rbB->SetPosition(posB - resolution * invMassB);
    }
}

void ImpulseSolver::Solve(std::vector<Collision>& collisions, float deltaTime) {
    for (auto& [rbA, rbB, points] : collisions) {
        Vector3f velA = rbA->Velocity();
        Vector3f velB = rbB->Velocity();
        Vector3f relativeVelocity = velB - velA;
        float velNormal = relativeVelocity.Dot(points.Normal);
        if (velNormal >= 0) continue;
        
        float restitution = 0.5f;
        float invMassA = rbA->InvMass();
        float invMassB = rbB->InvMass();
        float e = restitution * restitution;
        float j = -(1.0f + e) * velNormal / (invMassA + invMassB);
        Vector3f impulse = points.Normal * j;
        
        if (rbA->IsDynamic()) rbA->SetVelocity(velA - impulse * invMassA);
        if (rbB->IsDynamic()) rbB->SetVelocity(velB - impulse * invMassB);
    }
}
