#ifndef Entity_hpp
#define Entity_hpp

#include <iostream>
#include <vector>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "PhysicsWorld.hpp"
#include "Renderer.hpp"

class Entity {
protected:
    PhysicsWorld& m_PhysicsWorld;
    RigidBody* m_RigidBody;
    
public:
    Vector3f EntityPosition;
    Vector3f EntitySize;
    int EntityID;
    
    glm::mat4 ModelMatrix;
    
    Entity(Vector3f center, PhysicsWorld& world);
    virtual ~Entity() = default;
    
    void Update();
    void MoveEntity(Vector3f translation);
    
    virtual void DrawEntity(const Renderer& renderer, const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const = 0;
};

#endif /* Entity_hpp */
