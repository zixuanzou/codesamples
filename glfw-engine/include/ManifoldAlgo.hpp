#ifndef ManifoldAlgo_hpp
#define ManifoldAlgo_hpp

#include <iostream>
#include <vector>

#include "Collider.hpp"
#include "Simplex.hpp"

const int MAX_ITERATION = 1000;

ManifoldPoints ComputeSphereSphereManifoldPoints(const SphereCollider* a, const Transform* ta,
                                                 const SphereCollider* b, const Transform* tb);

ManifoldPoints ComputeSphereCuboidManifoldPoints(const SphereCollider* a, const Transform* ta,
                                                 const CuboidCollider* b, const Transform* tb);

ManifoldPoints ComputeCuboidCuboidManifoldPoints(const CuboidCollider* a, const Transform* ta,
                                                 const CuboidCollider* b, const Transform* tb);

Vector3f Support(const Collider* a, const Transform* ta,
                 const Collider* b, const Transform* tb,
                 Vector3f direction);

ManifoldPoints GJK(const Collider* a, const Transform* ta,
                   const Collider* b, const Transform* tb);

bool HandleSimplex(Simplex& simplex, Vector3f& direction);

bool SimplexLineCase(Simplex& simplex, Vector3f& direction);

bool SimplexTriangleCase(Simplex& simplex, Vector3f& direction);

bool SimplexTetrahedronCase(Simplex& simplex, Vector3f& direction);

bool SameDirection(const Vector3f& a, const Vector3f& b);

ManifoldPoints EPA(const Collider* a, const Transform* ta,
                   const Collider* b, const Transform* tb,
                   const Simplex& simplex);

std::tuple<std::vector<Vector3f>, std::vector<float>, size_t> GetFaceNormals(std::vector<Vector3f>& polytope, std::vector<size_t>& faces);

void AddIfUnique(std::vector<std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b);

#endif /* ManifoldAlgo_hpp */
