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
    render->SetOrthoProjection(0.f, 0.f, 0.f, 0.f, -50.f, 50.f);

    std::cout << "Test Engine" << std::endl;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render->DrawText("Test Avion Engine", 300.f, 400.f, 0.5f, WHITE);

        render->Draw({300.f, 300.f}, {50.f, 50.f}, AxisRotate::NONE, 0.f);
        render->Draw({200.f, 100.f}, {25.f, 25.f}, AxisRotate::NONE, 0.f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}