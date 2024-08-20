#ifndef Sphere_hpp
#define Sphere_hpp

#include "Entity.hpp"

class Sphere : public Entity {
private:
    Vector3f m_Center;
    float m_Radius;
    
public:
    Sphere(Vector3f center, PhysicsWorld& world, float radius, bool isDynamic, bool takesGravity);
    
    void DrawEntity(const Renderer& renderer, const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const override;
    
};

std::vector<glm::vec3> GenerateIcosphereVertices(float radius, int level, std::vector<glm::vec3>& indices);

#endif /* Sphere_hpp */
