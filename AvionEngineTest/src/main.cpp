#include <iostream>
#include <cstdlib> 
#include <array>

#include "AvionEngineCore/render/render.hpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* name_window = "Test Engine";

int main() {
    Render* render = new Render(name_window, SCR_WIDTH, SCR_HEIGHT);

    render->InitWindow();
    render->InitRender();
    render->InitRenderText();

    GLFWwindow* window = render->GetWindow();
    render->SetPerspectiveProjection(45.f, SCR_WIDTH, SCR_HEIGHT, 0.1f, 100.f);

    std::cout << "Test Engine" << std::endl;

    std::array<glm::vec3, 10> cube_positions {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 size{1.f, 1.f, 1.f};

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.3f, 0.45f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (const auto& cube_pos : cube_positions) {
            render->Draw(cube_pos, size, AxisRotate::AXIS_Y, 34.f);
        }
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}