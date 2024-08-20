#ifndef Shader_hpp
#define Shader_hpp

#include <iostream>
#include <GL/glew.h>

#include <glm/glm.hpp>

#include <unordered_map>

class Shader {
private:
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
    
    unsigned int CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    unsigned int CompileShader(const std::string& source, unsigned int type);
    
    int GetUniformLocation(const std::string& name);
    
public:
    Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    ~Shader();
    
    void Bind() const;
    void Unbind() const;
    
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
};

#endif /* Shader_hpp */
