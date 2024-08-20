#include "ManifoldAlgo.hpp"

ManifoldPoints ComputeSphereSphereManifoldPoints(const SphereCollider* a, const Transform* ta,
                                                 const SphereCollider* b, const Transform* tb) {
    Vector3f posA = a->Center + ta->Position;
    Vector3f posB = b->Center + tb->Position;
    float radiusA = a->Radius;
    float radiusB = b->Radius;
    
    Vector3f ab = posA - posB;
    float distance = ab.Magnitude();
    if (distance > radiusA + radiusB) {
        return ManifoldPoints();
    }
    
    Vector3f normal = ab.Normalize();
    Vector3f deepA = posA + normal * radiusA;
    Vector3f deepB = posB + normal * radiusB;
    float depth = (deepA - deepB).Magnitude();
    
    return ManifoldPoints(deepA, deepB, normal, depth);
}

ManifoldPoints ComputeSphereCuboidManifoldPoints(const SphereCollider* a, const Transform* ta,
                                                 const CuboidCollider* b, const Transform* tb) {
    return GJK(a, ta, b, tb);
}

ManifoldPoints ComputeCuboidCuboidManifoldPoints(const CuboidCollider* a, const Transform* ta,
                                                 const CuboidCollider* b, const Transform* tb) {
    return GJK(a, ta, b, tb);
}

Vector3f Support(const Collider* a, const Transform* ta,
                 const Collider* b, const Transform* tb,
                 Vector3f direction) {
    return a->FindFurthestPoint(ta, direction) - b->FindFurthestPoint(tb, direction * -1.0f);
}

ManifoldPoints GJK(const Collider* a, const Transform* ta,
                   const Collider* b, const Transform* tb) {
    
    Vector3f posA = a->Center + ta->Position;
    Vector3f posB = b->Center + tb->Position;
    Vector3f direction = posB - posA;
    Vector3f support = Support(a, ta, b, tb, direction);
    
    Simplex simplex;
    simplex.PushFront(support);
    direction = support * -1.0f;
    
    int iteration = 0;
    while (true) {
        if (iteration++ > MAX_ITERATION) {
            return ManifoldPoints();
        }
        support = Support(a, ta, b, tb, direction);
        if (support.Dot(direction) <= 0) {
            return ManifoldPoints();
        }
        simplex.PushFront(support);
        
        if (HandleSimplex(simplex, direction)) {
            return EPA(a, ta, b, tb, simplex);
        }
    }
}

bool HandleSimplex(Simplex& simplex, Vector3f& direction) {
    if (simplex.Size() == 2) {
        return SimplexLineCase(simplex, direction);
    }
    else if (simplex.Size() == 3) {
        return SimplexTriangleCase(simplex, direction);
    }
    else if (simplex.Size() == 4) {
        return SimplexTetrahedronCase(simplex, direction);
    }
    else {
        return false;
    }
}

bool SimplexLineCase(Simplex& simplex, Vector3f& direction) {
    Vector3f a = simplex[0];
    Vector3f b = simplex[1];
    
    Vector3f ab = b - a;
    Vector3f ao = a * -1.0f;
    
    if (SameDirection(ab, ao)) {
        direction = ab.Cross(ao).Cross(ab);
    }
    else {
        simplex = { a };
        direction = ao;
    }
    
    return false;
}

bool SimplexTriangleCase(Simplex& simplex, Vector3f& direction) {
    Vector3f a = simplex[0];
    Vector3f b = simplex[1];
    Vector3f c = simplex[2];
    
    Vector3f ab = b - a;
    Vector3f ac = c - a;
    Vector3f ao = a * -1.0f;
    
    Vector3f abc = ab.Cross(ac);
    
    if (SameDirection(abc.Cross(ac), ao)) {
        if (SameDirection(ac, ao)) {
            simplex = { a, c };
            direction = ac.Cross(ao).Cross(ac);
        }
        else {
            return SimplexLineCase(simplex = { a, b }, direction);
        }
    }
    else {
        if (SameDirection(ab.Cross(abc), ao)) {
            return SimplexLineCase(simplex = { a, b }, direction);
        }
        else {
            if (SameDirection(abc, ao)) {
                direction = abc;
            }
            else {
                simplex = { a, c, b };
                direction = abc * -1.0f;
            }
        }
    }
    
    return false;
}

bool SimplexTetrahedronCase(Simplex& simplex, Vector3f& direction) {
    Vector3f a = simplex[0];
    Vector3f b = simplex[1];
    Vector3f c = simplex[2];
    Vector3f d = simplex[3];
    
    Vector3f ab = b - a;
    Vector3f ac = c - a;
    Vector3f ad = d - a;
    Vector3f ao = a * -1.0f;
    
    Vector3f abc = ab.Cross(ac);
    Vector3f acd = ac.Cross(ad);
    Vector3f adb = ad.Cross(ab);
    
    if (SameDirection(abc, ao)) {
        return SimplexTriangleCase(simplex = { a, b, c }, direction);
    }
    
    if (SameDirection(acd, ao)) {
        return SimplexTriangleCase(simplex = { a, c, d }, direction);
    }
    
    if (SameDirection(adb, ao)) {
        return SimplexTriangleCase(simplex = { a, d, b }, direction);
    }
    
    return true;
}

bool SameDirection(const Vector3f& a, const Vector3f& b) {
    return a.Dot(b) > 0;
}

ManifoldPoints EPA(const Collider* a, const Transform* ta,
                   const Collider* b, const Transform* tb,
                   const Simplex& simplex) {
    
    std::vector<Vector3f> polytope;
    for (int i = 0; i < simplex.Size(); i++) {
        polytope.push_back(simplex[i]);
    }
    
    std::vector<size_t> faces = {
        0, 1, 2,
        0, 3, 1,
        0, 2, 3,
        1, 3, 2
    };
    
    auto [normals, distances, minFaceIndex] = GetFaceNormals(polytope, faces);
    
    Vector3f minNormal;
    float minDistance = std::numeric_limits<float>::max();
    
    int iteration = 0;
    while (minDistance == std::numeric_limits<float>::max()) {
        if (iteration++ > MAX_ITERATION) {
            break;
        }
        
        minNormal = normals[minFaceIndex];
        minDistance = distances[minFaceIndex];
        
        Vector3f support = Support(a, ta, b, tb, minNormal);
        float supportDistance = support.Dot(minNormal);
        
        if (std::abs(supportDistance - minDistance) > 0.001f) {
            minDistance = std::numeric_limits<float>::max();
            std::vector<std::pair<size_t, size_t>> uniqueEdges;
            
            for (size_t i = 0; i < normals.size(); i++) {
                if (SameDirection(normals[i], support - polytope[faces[i * 3]])) {
                    size_t f = i * 3;
                    
                    AddIfUnique(uniqueEdges, faces, f, f + 1);
                    AddIfUnique(uniqueEdges, faces, f + 1, f + 2);
                    AddIfUnique(uniqueEdges, faces, f + 2, f);
                    
                    faces[f + 2] = faces.back();
                    faces.pop_back();
                    faces[f + 1] = faces.back();
                    faces.pop_back();
                    faces[f] = faces.back();
                    faces.pop_back();
                    
                    normals[i] = normals.back();
                    normals.pop_back();
                    
                    i--;
                }
            }
            
            if (uniqueEdges.size() == 0) {
                break;
            }
            
            std::vector<size_t> newFaces;
            for (auto [edge1, edge2] : uniqueEdges) {
                newFaces.push_back(edge1);
                newFaces.push_back(edge2);
                newFaces.push_back(polytope.size());
            }
            
            polytope.push_back(support);
            
            auto [newNormals, newDistances, newMinFaceIndex] = GetFaceNormals(polytope, newFaces);
            
            float newMinDistance = std::numeric_limits<float>::max();
            for (size_t i = 0; i < normals.size(); i++) {
                if (distances[i] < newMinDistance) {
                    newMinDistance = distances[i];
                    minFaceIndex = i;
                }
            }
            
            if (newDistances[newMinFaceIndex] < newMinDistance) {
                minFaceIndex = newMinFaceIndex + normals.size();
            }
            
            faces.insert(faces.end(), newFaces.begin(), newFaces.end());
            normals.insert(normals.end(), newNormals.begin(), newNormals.end());
        }
    }
    
    if (minDistance == std::numeric_limits<float>::max()) {
        return ManifoldPoints();
    }
    
    ManifoldPoints points(minNormal, minDistance + 0.001f);
    
    return points;
}

std::tuple<std::vector<Vector3f>, std::vector<float>, size_t> GetFaceNormals(std::vector<Vector3f>& polytope, std::vector<size_t>& faces) {
    
    std::vector<Vector3f> normals;
    std::vector<float> distances;
    float minDistance = std::numeric_limits<float>::max();
    size_t minFaceIndex = 0;
    
    for (size_t i = 0; i < faces.size(); i += 3) {
        Vector3f a = polytope[faces[i]];
        Vector3f b = polytope[faces[i + 1]];
        Vector3f c = polytope[faces[i + 2]];
        
        Vector3f normal = ((b - a).Cross(c - a)).Normalize();
        float distance = normal.Dot(a);
        if (distance < 0) {
            normal = normal * -1.0f;
            distance *= -1;
        }
        
        normals.emplace_back(normal);
        distances.push_back(distance);
        
        if (distance < minDistance) {
            minFaceIndex = i / 3;
            minDistance = distance;
        }
    }
    
    return std::make_tuple(normals, distances, minFaceIndex);
}

void AddIfUnique(std::vector<std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b) {
    auto reverse = std::find(edges.begin(), edges.end(), std::make_pair(faces[b], faces[a]));
    if (reverse != edges.end()) {
        edges.erase(reverse);
    }
    else {
        edges.emplace_back(faces[a], faces[b]);
    }
}
