#include "RigidBody.hpp"

RigidBody::RigidBody() {
    static int staticID = 0;
    m_ID = staticID++;
}

void RigidBody::SetPosition(const Vector3f& position) {
    m_Transform->Position = position;
}

void RigidBody::SetVelocity(const Vector3f& velocity) {
    m_Velocity = velocity;
}

void RigidBody::SetForce(const Vector3f& force) {
    m_Force = force;
}

void RigidBody::SetMass(const float mass) {
    m_Mass = mass;
}

void RigidBody::SetIsDynamic(bool isDynamic) {
    m_IsDynamic = isDynamic;
}

void RigidBody::SetTakesGravity(bool takesGravity) {
    m_TakesGravity = takesGravity;
}

void RigidBody::SetTransform(struct Transform* transform) {
    m_Transform = transform;
}

void RigidBody::SetCollider(class Collider* collider) {
    m_Collider = collider;
}

Vector3f RigidBody::Position() {
    return m_Transform->Position;
}

Vector3f RigidBody::Velocity() {
    return m_Velocity;
}

Vector3f RigidBody::Force() {
    return m_Force;
}

float RigidBody::Mass() {
    return m_Mass;
}

float RigidBody::InvMass() {
    return 1.0f / m_Mass;
}

bool RigidBody::IsDynamic() {
    return  m_IsDynamic;
}

bool RigidBody::TakesGravity() {
    return m_TakesGravity;
}

Transform* RigidBody::Transform() {
    return m_Transform;
}

Collider* RigidBody::Collider() {
    return m_Collider;
}

int RigidBody::ID() {
    return m_ID;
}

void RigidBody::ApplyForce(const Vector3f& force) {
    m_Force += force;
}
