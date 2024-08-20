#include "Sphere.hpp"

glm::vec3 normalize(glm::vec3 vec) {
    float length = glm::length(vec);
    if (length > 0.0f) {
        return vec / length;
    }
    return  vec;
}

std::vector<glm::vec3> GenerateIcosphereVertices(float radius, int level, std::vector<glm::vec3>& indices) {
    
    const float t = (1.0f + std::sqrt(5.0f)) / 2.0f;
    
    std::vector<glm::vec3> icosahedronVertices = {
        {-1,  t,  0},
        { 1,  t,  0},
        {-1, -t,  0},
        { 1, -t,  0},
        
        { 0, -1,  t},
        { 0,  1,  t},
        { 0, -1, -t},
        { 0,  1, -t},
        
        { t,  0, -1},
        { t,  0,  1},
        {-t,  0, -1},
        {-t,  0,  1}
    };
    
    std::vector<glm::vec3> icosahedronFaces = {
        {0, 11, 5},
        {0, 5, 1},
        {0, 1, 7},
        {0, 7, 10},
        {0, 10, 11},
        
        {1, 5, 9},
        {5, 11, 4},
        {11, 10, 2},
        {10, 7, 6},
        {7, 1, 8},
        
        {3, 9, 4},
        {3, 4, 2},
        {3, 2, 6},
        {3, 6, 8},
        {3, 8, 9},
        
        {4, 9, 5},
        {2, 4, 11},
        {6, 2, 10},
        {8, 6, 7},
        {9, 8, 1},
    };
    
    std::vector<glm::vec3> vertices = icosahedronVertices;
    indices = icosahedronFaces;
    
    for (int i = 0; i < level; i++) {
        std::vector<glm::vec3> tempVertices;
        std::vector<glm::vec3> tempIndices;
        int index = 0;
        for (auto& face : indices) {
            glm::vec3 v1 = normalize(vertices[face.x]);
            glm::vec3 v2 = normalize(vertices[face.y]);
            glm::vec3 v3 = normalize(vertices[face.z]);
            
            glm::vec3 mid1 = normalize((v1 + v2) / 2.0f);
            glm::vec3 mid2 = normalize((v2 + v3) / 2.0f);
            glm::vec3 mid3 = normalize((v1 + v3) / 2.0f);
            
            tempVertices.push_back(v1);
            tempVertices.push_back(mid1);
            tempVertices.push_back(mid3);
            
            tempVertices.push_back(v2);
            tempVertices.push_back(mid1);
            tempVertices.push_back(mid2);
            
            tempVertices.push_back(v3);
            tempVertices.push_back(mid2);
            tempVertices.push_back(mid3);
            
            tempVertices.push_back(mid1);
            tempVertices.push_back(mid2);
            tempVertices.push_back(mid3);
            
            tempIndices.push_back({index, index + 1, index + 2});
            tempIndices.push_back({index + 3, index + 4, index + 5});
            tempIndices.push_back({index + 6, index + 7, index + 8});
            tempIndices.push_back({index + 9, index + 10, index + 11});
            
            index += 12;
        }
        vertices = tempVertices;
        indices = tempIndices;
    }
    
    for (auto& vertex : vertices) {
        vertex *= radius;
    }
    
    return vertices;
}

Sphere::Sphere(Vector3f center, PhysicsWorld& world, float radius, bool isDynamic, bool takesGravity) : Entity(center, world), m_Center(center), m_Radius(radius) {
    
    EntitySize = Vector3f(radius, radius, radius);
    
    ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(center.x, center.y, center.z));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(radius, radius, radius));
    
    SphereCollider* collider = new SphereCollider();
    collider->Center = Vector3f(radius, radius, radius);
    collider->Radius = radius;
    m_RigidBody->SetCollider(collider);
    
    m_RigidBody->SetIsDynamic(isDynamic);
    m_RigidBody->SetTakesGravity(takesGravity);
}

void Sphere::DrawEntity(const Renderer& renderer, const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    renderer.Draw(va, ib, shader);
}
