#include <iostream>
#include <cstdlib> 

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

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render->Draw({0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, AxisRotate::NONE, 0.f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}