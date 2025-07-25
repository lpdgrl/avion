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
    render->SetPerspectiveProjection(45.f, SCR_WIDTH, SCR_HEIGHT, 0.1f, 50.f);

    std::cout << "Test Engine" << std::endl;

    std::array<glm::vec3, 1> cube_positions {
        glm::vec3(0.0f,  -0.8f,  0.0f),
        // glm::vec3( 2.0f,  5.0f, -15.0f),
        // glm::vec3(-1.5f, -2.2f, -2.5f),
        // glm::vec3(-3.8f, -2.0f, -12.3f),
        // glm::vec3( 2.4f, -0.4f, -3.5f),
        // glm::vec3(-1.7f,  3.0f, -7.5f),
        // glm::vec3( 1.3f, -2.0f, -2.5f),
        // glm::vec3( 1.5f,  2.0f, -2.5f),
        // glm::vec3( 1.5f,  0.2f, -1.5f),
        // glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    glm::vec3 cube_pos{0.f, -0.8f, 0.f};
    glm::vec3 size{1.f, 1.f, 1.f};
    glm::vec3 size_other{0.5f, 0.5f, 0.5f};
    glm::vec3 objectColor{1.0f, 0.5f, 0.31f};
    glm::vec3 colorLigth{1.0f, 1.0f, 1.0f};
    glm::vec3 ligth_position{-1.3f,  -0.5f, -1.f};
    glm::vec3 view_pos{0.f, 0.f, 0.f};

    Shader* shader = render->GetShaderPtr("object");
    Shader* shader_ligth = render->GetShaderPtr("ligth");
    
    GLfloat delta_time = 0.f;
    GLfloat last_frame = 0.f;
    
    while (!glfwWindowShouldClose(window)) {

        GLfloat current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        ligth_position.x += sin(glfwGetTime()) * delta_time * 0.5f;
        ligth_position.y += sin(glfwGetTime()) * delta_time * 0.5f;
        ligth_position.z += sin(glfwGetTime()) * delta_time * 2.5f;


        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render->SetLigth(shader, colorLigth, objectColor);
        
        shader->setVec3("ligthPos", ligth_position);
        shader->setVec3("view_pos", view_pos);

        render->Draw(shader, cube_pos, size, AxisRotate::AXIS_Y, sin(glfwGetTime()) * 50.f, MapKey::OBJECTS);
        render->Draw(shader_ligth, ligth_position, size_other, AxisRotate::AXIS_X, 10.f, MapKey::LIGHT);

    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}