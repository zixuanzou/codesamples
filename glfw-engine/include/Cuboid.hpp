#ifndef Cuboid_hpp
#define Cuboid_hpp

#include "Entity.hpp"

class Cuboid : public Entity {
private:
    Vector3f m_Center;
    float m_DimX;
    float m_DimY;
    float m_DimZ;
    
public:
    Cuboid(Vector3f center, PhysicsWorld& world, float dimX, float dimY, float dimZ, bool isDynamic, bool takesGravity);
    
    void DrawEntity(const Renderer& renderer, const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const override;
    
};

#endif /* Cuboid_hpp */
