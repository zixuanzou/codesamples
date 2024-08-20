#ifndef RigidBody_hpp
#define RigidBody_hpp

#include "Transform.hpp"
#include "Collider.hpp"

class RigidBody {
private:
    Vector3f m_Velocity;
    Vector3f m_Force;
    float m_Mass;
    int m_ID;
    
    bool m_IsDynamic;
    bool m_TakesGravity;
    
    Transform* m_Transform;
    Collider* m_Collider;
    
public:
    RigidBody();
    
    void SetPosition(const Vector3f& position);
    void SetVelocity(const Vector3f& velocity);
    void SetForce(const Vector3f& force);
    void SetMass(const float mass);
    void SetIsDynamic(bool isDynamic);
    void SetTakesGravity(bool takesGravity);
    void SetTransform(Transform* transform);
    void SetCollider(Collider* collider);
    
    Vector3f Position();
    Vector3f Velocity();
    Vector3f Force();
    float Mass();
    float InvMass();
    bool IsDynamic();
    bool TakesGravity();
    Transform* Transform();
    Collider* Collider();
    int ID();
    
    void ApplyForce(const Vector3f& force);
    
};

#endif /* RigidBody_hpp */
