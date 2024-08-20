#include "Collider.hpp"
#include "ManifoldAlgo.hpp"

ManifoldPoints SphereCollider::TestCollision(const Transform* transform,
                                             const Collider* collider,
                                             const Transform* colliderTransform) const {
    return collider->TestCollision(colliderTransform, this, transform);
}

ManifoldPoints SphereCollider::TestCollision(const Transform* transform,
                                             const SphereCollider* sphereCollider,
                                             const Transform* sphereTransform) const {
    return ComputeSphereSphereManifoldPoints(this, transform, sphereCollider, sphereTransform);
}

ManifoldPoints SphereCollider::TestCollision(const Transform* transform,
                                             const CuboidCollider* cuboidCollider,
                                             const Transform* cuboidTransform) const {
    return ComputeSphereCuboidManifoldPoints(this, transform, cuboidCollider, cuboidTransform);
}

Vector3f SphereCollider::FindFurthestPoint(const Transform* transform, Vector3f direction) const {
    return transform->Position + Center + direction.Normalize() * Radius;
}

ManifoldPoints CuboidCollider::TestCollision(const Transform* transform,
                                             const Collider* collider,
                                             const Transform* colliderTransform) const {
    return collider->TestCollision(colliderTransform, this, transform);
}

ManifoldPoints CuboidCollider::TestCollision(const Transform* transform,
                                             const SphereCollider* sphereCollider,
                                             const Transform* sphereTransform) const {
    return ComputeSphereCuboidManifoldPoints(sphereCollider, sphereTransform, this, transform);
}

ManifoldPoints CuboidCollider::TestCollision(const Transform* transform,
                                             const CuboidCollider* cuboidCollider,
                                             const Transform* cuboidTransform) const {
    return ComputeCuboidCuboidManifoldPoints(this, transform, cuboidCollider, cuboidTransform);
}

Vector3f CuboidCollider::FindFurthestPoint(const Transform* transform, Vector3f direction) const {
    Vector3f furthestPoint = Vector3f(0.0f, 0.0f, 0.0f);
    float maxDistance = -std::numeric_limits<float>::max();
    std::array<Vector3f, 8> vertices = GetCuboidVertices(*transform);
    for (Vector3f vertex : vertices) {
        float distance = direction.Dot(vertex);
        if (distance > maxDistance) {
            maxDistance = distance;
            furthestPoint = vertex;
        }
    }
    return furthestPoint;
}

std::array<Vector3f, 8> CuboidCollider::GetCuboidVertices(const Transform& transform) const {
    Vector3f c = transform.Position + Center;
    Vector3f v1 = Vector3f(c.x - HalfX, c.y - HalfY, c.z - HalfZ);
    Vector3f v2 = Vector3f(c.x - HalfX, c.y - HalfY, c.z + HalfZ);
    Vector3f v3 = Vector3f(c.x - HalfX, c.y + HalfY, c.z - HalfZ);
    Vector3f v4 = Vector3f(c.x - HalfX, c.y + HalfY, c.z + HalfZ);
    Vector3f v5 = Vector3f(c.x + HalfX, c.y - HalfY, c.z - HalfZ);
    Vector3f v6 = Vector3f(c.x + HalfX, c.y - HalfY, c.z + HalfZ);
    Vector3f v7 = Vector3f(c.x + HalfX, c.y + HalfY, c.z - HalfZ);
    Vector3f v8 = Vector3f(c.x + HalfX, c.y + HalfY, c.z + HalfZ);
    return {v1, v2, v3, v4, v5, v6, v7, v8};
}
