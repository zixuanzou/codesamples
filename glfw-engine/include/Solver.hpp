#ifndef Solver_hpp
#define Solver_hpp

#include <iostream>
#include <vector>

#include "Collision.hpp"

class Solver {
public:
    virtual void Solve(std::vector<Collision>& collisions, float deltaTime) = 0;
};

class PositionSolver : public Solver {
public:
    void Solve(std::vector<Collision>& collisions, float deltaTime) override;
};

class ImpulseSolver : public Solver {
public:
    void Solve(std::vector<Collision>& collisions, float deltaTime) override;
};

#endif /* Solver_hpp */
