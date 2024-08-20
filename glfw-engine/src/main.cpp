#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"

#include "PhysicsWorld.hpp"
#include "Cuboid.hpp"
#include "Sphere.hpp"

void processInput(GLFWwindow* window, glm::mat4& view) {
    const float cameraSpeed = 1.0f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        view = glm::translate(view, glm::vec3(0.0f, -cameraSpeed, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        view = glm::translate(view, glm::vec3(0.0f, cameraSpeed, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        view = glm::translate(view, glm::vec3(cameraSpeed, 0.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        view = glm::translate(view, glm::vec3(-cameraSpeed, 0.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, cameraSpeed));
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -cameraSpeed));
    }
    glm::vec3 rotationAxis;
    const float rotationSpeed = 0.1f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f);
        view = glm::rotate(view, rotationSpeed, rotationAxis);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        rotationAxis = glm::vec3(-1.0f, 0.0f, 0.0f);
        view = glm::rotate(view, rotationSpeed, rotationAxis);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        rotationAxis = glm::vec3(0.0f, -1.0f, 0.0f);
        view = glm::rotate(view, rotationSpeed, rotationAxis);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
        view = glm::rotate(view, rotationSpeed, rotationAxis);
    }
}

int main(int argc, const char * argv[]) {
    
    GLFWwindow* window;
    
    if (!glfwInit()) {
        return -1;
    }
    
    #ifdef __APPLE__
      glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
      glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif
    
    window = glfwCreateWindow(640, 640, "test", nullptr, nullptr);
    
    if (!window) {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    if (glewInit() != GLEW_OK) {
        std::cout << "Error" << std::endl;
    }
    
    std::cout << glGetString(GL_VERSION) << std::endl;
    
    {
        std::string vertexShaderSource = R"(
            #version 330 core
            layout(location = 0) in vec4 position;
            uniform mat4 u_MVP;
            void main()
            {
               gl_Position = u_MVP * position;
            }
        )";
        
        std::string fragmentShaderSource = R"(
            #version 330 core
            layout(location = 0) out vec4 color;
            uniform vec4 u_Color;
            void main()
            {
               color = u_Color;
            }
        )";
        
        // ======================================================================
        
        float cuboidVertices[] = {
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            
            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f
        };
        
        unsigned int cuboidIndices[] = {
            0, 1, 2,
            0, 2, 3,
            1, 5, 6,
            1, 2, 6,
            4, 5, 6,
            4, 6, 7,
            0, 3, 4,
            3, 4, 7,
            2, 3, 6,
            3, 6, 7,
            0, 1, 5,
            0, 4, 5
        };
        
        VertexArray cuboidVAO;
        VertexBuffer cuboidVBO(cuboidVertices, 24 * sizeof(float));
        
        VertexBufferLayout cuboidLayout;
        cuboidLayout.Push<float>(3);
        cuboidVAO.AddBuffer(cuboidVBO, cuboidLayout);
        
        IndexBuffer cuboidIBO(cuboidIndices, 36);
        
        // ======================================================================
        
        float radius = 1.0f;
        int recursionLevel = 3;
        std::vector<glm::vec3> sphereIndices;
        
        std::vector<glm::vec3> sphereVertices = GenerateIcosphereVertices(radius, recursionLevel, sphereIndices);
        
        int sphereVerticesSize = (int) sphereVertices.size() * 3;
        float sphereVerticesArray[sphereVerticesSize];
        
        int indexV = 0;
        for (auto& vertex : sphereVertices) {
            sphereVerticesArray[indexV] = vertex.x;
            sphereVerticesArray[indexV + 1] = vertex.y;
            sphereVerticesArray[indexV + 2] = vertex.z;
            indexV += 3;
        }
        
        VertexArray sphereVAO;
        VertexBuffer sphereVBO(sphereVerticesArray, (int) sizeof(sphereVerticesArray));
        
        VertexBufferLayout sphereLayout;
        sphereLayout.Push<float>(3);
        sphereVAO.AddBuffer(sphereVBO, sphereLayout);
        
        int sphereIndicesSize = (int) sphereIndices.size() * 3;
        unsigned int sphereIndicesArray[sphereIndicesSize];
        
        int index = 0;
        for (auto& face : sphereIndices) {
            sphereIndicesArray[index] = face.x;
            sphereIndicesArray[index + 1] = face.y;
            sphereIndicesArray[index + 2] = face.z;
            index += 3;
        }
        
        IndexBuffer sphereIBO(sphereIndicesArray, sphereIndicesSize);
        
        // ======================================================================
        
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 640.0f / 640.0f, 0.1f, 640.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -320.0f));
        
        Shader shader(vertexShaderSource, fragmentShaderSource);
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 1.0f, 1.0f, 1.0f);
        
        Renderer renderer;
        
        const char* glsl_version = "#version 150";
        
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        
        float sliderA[3] = {0.0f, 0.0f, 0.0f};
        float sliderB[3] = {0.0f, 0.0f, 0.0f};
        float sliderC[3] = {0.0f, 0.0f, 0.0f};
        Vector3f translationA;
        Vector3f translationB;
        Vector3f translationC;
        Vector3f prevA = Vector3f(sliderA[0], sliderA[1], sliderA[2]);
        Vector3f prevB = Vector3f(sliderB[0], sliderB[1], sliderB[2]);
        Vector3f prevC = Vector3f(sliderC[0], sliderC[1], sliderC[2]);
        Vector3f diffA = Vector3f(0.0f, 0.0f, 0.0f);
        Vector3f diffB = Vector3f(0.0f, 0.0f, 0.0f);
        Vector3f diffC = Vector3f(0.0f, 0.0f, 0.0f);
        
        PhysicsWorld world;
        std::vector<std::unique_ptr<Entity>> entities;
        
        PositionSolver* posSolver = new PositionSolver();
        ImpulseSolver* impSolver = new ImpulseSolver();
        world.AddSolver(posSolver);
        world.AddSolver(impSolver);
        
        std::unique_ptr<Cuboid> cuboid1 = std::make_unique<Cuboid>(Vector3f(0.0f, 0.0f, 0.0f), world, 20.0f, 20.0f, 20.0f, true, false);
        std::unique_ptr<Cuboid> cuboid2 = std::make_unique<Cuboid>(Vector3f(50.0f, 50.0f, 0.0f), world, 10.0f, 10.0f, 10.0f, true, false);
        std::unique_ptr<Sphere> sphere1 = std::make_unique<Sphere>(Vector3f(-50.0f, 0.0f, 0.0f), world, 10.0f, true, false);
        std::unique_ptr<Sphere> sphere2 = std::make_unique<Sphere>(Vector3f(-50.0f, -50.0f, 0.0f), world, 10.0f, true, false);
        
        std::unique_ptr<Cuboid> plane = std::make_unique<Cuboid>(Vector3f(0.0f, -80.0f, 0.0f), world, 200.0f, 10.0f, 10.0f, true, false);
        
        entities.push_back(std::move(cuboid1));
        entities.push_back(std::move(cuboid2));
        entities.push_back(std::move(sphere1));
        entities.push_back(std::move(sphere2));
        entities.push_back(std::move(plane));
        
        float lastFrameTime = 0.0f;
        
        while (!glfwWindowShouldClose(window)) {
            
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            
            renderer.Clear();
            
            processInput(window, view);
            
            translationA = Vector3f(sliderA[0], sliderA[1], sliderA[2]);
            translationB = Vector3f(sliderB[0], sliderB[1], sliderB[2]);
            translationC = Vector3f(sliderC[0], sliderC[1], sliderC[2]);
            diffA = translationA - prevA;
            diffB = translationB - prevB;
            diffC = translationC - prevC;
            prevA = translationA;
            prevB = translationB;
            prevC = translationC;
            
            entities[0]->MoveEntity(diffA);
            entities[1]->MoveEntity(diffB);
            entities[2]->MoveEntity(diffC);
            
            for (auto& entity : entities) {
                entity->Update();
                glm::mat4 model = entity->ModelMatrix;
                glm::mat4 mvp = projection * view * model;
                shader.SetUniformMat4f("u_MVP", mvp);
                if (dynamic_cast<Cuboid*>(entity.get())) {
                    entity->DrawEntity(renderer, cuboidVAO, cuboidIBO, shader);
                }
                else if (dynamic_cast<Sphere*>(entity.get())) {
                    entity->DrawEntity(renderer, sphereVAO, sphereIBO, shader);
                }
            }
            
            float currentFrameTime = glfwGetTime();
            float deltaTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;
            
            world.Step(deltaTime);
            
            ImGui::Begin("test");
            ImGui::SliderFloat3("Translation A", sliderA, -320.0f, 320.0f);
            ImGui::SliderFloat3("Translation B", sliderB, -320.0f, 320.0f);
            ImGui::SliderFloat3("Translation C", sliderC, -320.0f, 320.0f);
            ImGui::Text("FPS: %.1f", io.Framerate);
            ImGui::End();
            
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        
    }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwTerminate();
    
    return 0;
}


