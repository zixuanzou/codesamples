#include "Entity.hpp"

Entity::Entity(Vector3f position, PhysicsWorld& world) : EntityPosition(position), m_PhysicsWorld(world) {
    m_RigidBody = m_PhysicsWorld.AddRigidBody(EntityPosition);
    EntityID = m_RigidBody->ID();
}

void Entity::Update() {
    EntityPosition = m_RigidBody->Position();
    ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(EntityPosition.x, EntityPosition.y, EntityPosition.z));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(EntitySize.x, EntitySize.y, EntitySize.z));
}

void Entity::MoveEntity(Vector3f translation) {
    EntityPosition = m_RigidBody->Position() + translation;
    m_RigidBody->SetPosition(EntityPosition);
}
