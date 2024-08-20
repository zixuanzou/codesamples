#include "Shader.hpp"

Shader::Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) : m_RendererID(0) {
    m_RendererID = CreateShader(vertexShaderSource, fragmentShaderSource);
}

Shader::~Shader() {
    glDeleteProgram(m_RendererID);
}

unsigned int Shader::CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
    unsigned int program = glCreateProgram();
    unsigned int vertexShader = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
    unsigned int fragmentShader = CompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glValidateProgram(program);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return program;
}

unsigned int Shader::CompileShader(const std::string& source, unsigned int type) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    
    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* infoLog = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, infoLog);
        std::cout << "Failed to compile shader" << std::endl;
        std::cout << infoLog << std::endl;
    }
    
    return id;
}

int Shader::GetUniformLocation(const std::string& name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1) {
        std::cout << "Uniform location not found" << std::endl;
    }
    m_UniformLocationCache[name] = location;
    return location;
}

void Shader::Bind() const {
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}
