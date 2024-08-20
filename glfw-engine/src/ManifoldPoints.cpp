#include "ManifoldPoints.hpp"

ManifoldPoints::ManifoldPoints() : HasCollision(false) {}

ManifoldPoints::ManifoldPoints(Vector3f normal, float depth) : Normal(normal), Depth(depth), HasCollision(true) {}

ManifoldPoints::ManifoldPoints(Vector3f a, Vector3f b, Vector3f normal, float depth) : A(a), B(b), Normal(normal), Depth(depth), HasCollision(true) {}
