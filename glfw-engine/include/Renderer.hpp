#ifndef Renderer_hpp
#define Renderer_hpp

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

class Renderer {
public:
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void Clear() const;
};

#endif /* Renderer_hpp */
