#include "Cuboid.hpp"

Cuboid::Cuboid(Vector3f center, PhysicsWorld& world, float dimX, float dimY, float dimZ, bool isDynamic, bool takesGravity) : Entity(center, world), m_Center(center), m_DimX(dimX), m_DimY(dimY), m_DimZ(dimZ) {
    
    EntitySize = Vector3f(dimX, dimY, dimZ);
    
    ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(center.x, center.y, center.z));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(dimX, dimY, dimZ));
    
    CuboidCollider* collider = new CuboidCollider();
    collider->Center = Vector3f(dimX / 2.0f, dimY / 2.0f, dimZ / 2.0f);
    collider->HalfX = dimX / 2.0f;
    collider->HalfY = dimY / 2.0f;
    collider->HalfZ = dimZ / 2.0f;
    m_RigidBody->SetCollider(collider);
    
    m_RigidBody->SetIsDynamic(isDynamic);
    m_RigidBody->SetTakesGravity(takesGravity);
}

void Cuboid::DrawEntity(const Renderer& renderer, const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    renderer.Draw(va, ib, shader);
}
