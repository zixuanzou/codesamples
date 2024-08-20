#ifndef Collider_hpp
#define Collider_hpp

#include <array>

#include "Transform.hpp"
#include "ManifoldPoints.hpp"

class SphereCollider;
class CuboidCollider;

class Collider {
public:
    Vector3f Center;
    
    virtual ManifoldPoints TestCollision(const Transform* transform,
                                         const Collider* collider,
                                         const Transform* colliderTransform) const = 0;
    
    virtual ManifoldPoints TestCollision(const Transform* transform,
                                         const SphereCollider* sphereCollider,
                                         const Transform* sphereTransform) const = 0;
    
    virtual ManifoldPoints TestCollision(const Transform* transform,
                                         const CuboidCollider* cuboidCollider,
                                         const Transform* cuboidTransform) const = 0;
    
    virtual Vector3f FindFurthestPoint(const Transform* transform, Vector3f direction) const = 0;
};

class SphereCollider : public Collider {
public:
    float Radius;
    
    ManifoldPoints TestCollision(const Transform* transform,
                                 const Collider* collider,
                                 const Transform* colliderTransform) const override;
    
    ManifoldPoints TestCollision(const Transform* transform,
                                 const SphereCollider* sphereCollider,
                                 const Transform* sphereTransform) const override;
    
    ManifoldPoints TestCollision(const Transform* transform,
                                 const CuboidCollider* cuboidCollider,
                                 const Transform* cuboidTransform) const override;
    
    Vector3f FindFurthestPoint(const Transform* transform, Vector3f direction) const override;
};

class CuboidCollider : public Collider {
public:
    float HalfX;
    float HalfY;
    float HalfZ;
    
    ManifoldPoints TestCollision(const Transform* transform,
                                 const Collider* collider,
                                 const Transform* colliderTransform) const override;
    
    ManifoldPoints TestCollision(const Transform* transform,
                                 const SphereCollider* sphereCollider,
                                 const Transform* sphereTransform) const override;
    
    ManifoldPoints TestCollision(const Transform* transform,
                                 const CuboidCollider* cuboidCollider,
                                 const Transform* cuboidTransform) const override;
    
    Vector3f FindFurthestPoint(const Transform* transform, Vector3f direction) const override;
    
    std::array<Vector3f, 8> GetCuboidVertices(const Transform& transform) const;
};

#endif /* Collider_hpp */
