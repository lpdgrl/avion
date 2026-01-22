#include "AvionEngineCore/renderer/renderer.hpp"
#include "AvionEngineCore/core/window.hpp"

namespace avion::gfx {

    Renderer::Renderer(core::Window* window): window_(window) {}

    Renderer::~Renderer() {
        delete shader_;
        delete shader_text_;
        delete shader_ligth_;
        // delete text_;
        delete camera_;
        
        for (auto& [key, value] : vao_) {
            glDeleteVertexArrays(1, &value);
        }

        for (auto& [key, value] : vbo_) {
            glDeleteBuffers(1, &value);
        }

        std::cout << "Renderer is destroyed" << '\n';
    }

    void Renderer::Init() {
        shader_ = new Shader(PATH_TO_FILE_VERTEX_SHADER, PATH_TO_FILE_FRAGMENT_SHADER);
        shader_text_ = new Shader(PATH_TO_VERTEX_SHADER_TEXT, PATH_TO_FRAGMENT_SHADER_TEXT);
        shader_ligth_ = new Shader(PATH_TO_VERTEX_SHADER_LIGTH, PATH_TO_FRAGMENT_SHADER_LIGTH);

        // text_ = new TextRender(PATH_TO_FONT);
        // text_->Initialaztion();

        InitRenderer();
        InitRendererText();
        InitCamera();
    }

    void Renderer::InitCamera() {
        glm::vec3 camera_pos    = glm::vec3(0.0f, 0.0f, 10.0f);
        // glm::vec3 camera_front  = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 camera_up     = glm::vec3(0.0f, 1.0f, 0.0f);

        camera_ = new Camera(camera_pos, camera_up);
    }

    void Renderer::Update() {
        if (cursor_state_) {
            glfwSetInputMode(window_->GetPointer(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else if (!cursor_state_) {
            glfwSetInputMode(window_->GetPointer(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }

    void Renderer::UpdateCoordinatesCamera(GLfloat delta_time) {    
        if (window_->IsDown(GLFW_KEY_W)) {
            camera_->ProcessKeyboard(CameraMovement::FORWARD, delta_time);
        }
        if (window_->IsDown(GLFW_KEY_A)) {
            camera_->ProcessKeyboard(CameraMovement::LEFT, delta_time);
        }
        if (window_->IsDown(GLFW_KEY_D)) {
            camera_->ProcessKeyboard(CameraMovement::RIGHT, delta_time);
        }
        if (window_->IsDown(GLFW_KEY_S)) {
            camera_->ProcessKeyboard(CameraMovement::BACKWARD, delta_time);
        }

        if (!cursor_state_) {
            auto [xoffset, yoffset] = window_->GetOffsetController();
            camera_->ProcessMouseMovement(xoffset, yoffset);
        }

        if (window_->WasPressedKey(GLFW_KEY_H)) {
            cursor_state_ = cursor_state_ ? false : true;
        }    
    }

    void Renderer::InitRenderer() {
        LoadVerticesCube();
        LoadVerticesPyramid();
        LoadVerticesSourceLigth();
    }

    void Renderer::LoadVerticesCube() {
        // actual!!
        float cube[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };

        //Buffers for objects
        GenerateBuffer(1, TypeBuffers::VAO, MapKey::kCube);
        GenerateBuffer(1, TypeBuffers::VBO, MapKey::kCube);

        BindVertexArray(GetVAO(MapKey::kCube));

        BindBuffer(GL_ARRAY_BUFFER, GetVBO(MapKey::kCube));
        BufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

        // BindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetEBO(MapKey::OBJECTS));
        // BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_objects), indices_objects, GL_DYNAMIC_DRAW);

        // Attribute of position player
        SetVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
        EnableVertexAttribArray(0);

        // Normal attribute
        SetVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        EnableVertexAttribArray(1);

        BindVertexArray(0);
        BindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Renderer::LoadVerticesPyramid() {
        float pyramid[] = {
            // Base (down)
            -0.5f, -0.5f, -0.5f,   0.f, -1.f,  0.f,
            0.5f, -0.5f, -0.5f,   0.f, -1.f,  0.f,
            0.5f, -0.5f,  0.5f,   0.f, -1.f,  0.f,

            -0.5f, -0.5f, -0.5f,   0.f, -1.f,  0.f,
            0.5f, -0.5f,  0.5f,   0.f, -1.f,  0.f,
            -0.5f, -0.5f,  0.5f,   0.f, -1.f,  0.f,

            // Front face (towards -Z)
            0.0f,  0.5f,  0.0f,   0.0f,  0.7071f, -0.7071f,
            -0.5f, -0.5f, -0.5f,   0.0f,  0.7071f, -0.7071f,
            0.5f, -0.5f, -0.5f,   0.0f,  0.7071f, -0.7071f,

            // Right face (towards +X)
            0.0f,  0.5f,  0.0f,   0.7071f, 0.7071f, 0.0f,
            0.5f, -0.5f, -0.5f,   0.7071f, 0.7071f, 0.0f,
            0.5f, -0.5f,  0.5f,   0.7071f, 0.7071f, 0.0f,

            // Back face (towards +Z)
            0.0f,  0.5f,  0.0f,   0.0f,  0.7071f,  0.7071f,
            0.5f, -0.5f,  0.5f,   0.0f,  0.7071f,  0.7071f,
            -0.5f, -0.5f,  0.5f,   0.0f,  0.7071f,  0.7071f,

            // Left face (towards -X)
            0.0f,  0.5f,  0.0f,  -0.7071f, 0.7071f, 0.0f,
            -0.5f, -0.5f,  0.5f,  -0.7071f, 0.7071f, 0.0f,
            -0.5f, -0.5f, -0.5f,  -0.7071f, 0.7071f, 0.0f,
        };

        //Buffers for objects
        GenerateBuffer(1, TypeBuffers::VAO, MapKey::kPyramid);
        GenerateBuffer(1, TypeBuffers::VBO, MapKey::kPyramid);

        BindVertexArray(GetVAO(MapKey::kPyramid));

        BindBuffer(GL_ARRAY_BUFFER, GetVBO(MapKey::kPyramid));
        BufferData(GL_ARRAY_BUFFER, sizeof(pyramid), pyramid, GL_STATIC_DRAW);

        // BindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetEBO(MapKey::OBJECTS));
        // BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_objects), indices_objects, GL_DYNAMIC_DRAW);

        // Attribute of position player
        SetVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
        EnableVertexAttribArray(0);

        // Normal attribute
        SetVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        EnableVertexAttribArray(1);

        BindVertexArray(0);
        BindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Renderer::LoadVerticesSourceLigth() {
        // actual!!
        float cube[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };

        // Create VAO and set vertex attrib for Ligth
        GenerateBuffer(1, TypeBuffers::VAO, MapKey::kLight);
        GenerateBuffer(1, TypeBuffers::VBO, MapKey::kLight);
        
        BindVertexArray(GetVAO(MapKey::kLight));

        BindBuffer(GL_ARRAY_BUFFER, GetVBO(MapKey::kLight));
        BufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

        SetVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        EnableVertexAttribArray(0);

        BindBuffer(GL_ARRAY_BUFFER, 0);
        BindVertexArray(0);
    }

    void Renderer::InitRendererText() {
        GenerateBuffer(1, TypeBuffers::VAO, MapKey::TEXT);
        GenerateBuffer(1, TypeBuffers::VBO, MapKey::TEXT);

        BindVertexArray(GetVAO(MapKey::TEXT));
        BindBuffer(GL_ARRAY_BUFFER, GetVBO(MapKey::TEXT));
        
        BufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

        EnableVertexAttribArray(0);
        SetVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

        BindBuffer(GL_ARRAY_BUFFER, 0);
        BindVertexArray(0); 
    }

    void Renderer::GenerateBuffer(const GLsizei n, TypeBuffers type, MapKey key) {
        GLuint b;
        
        switch(type)
        {
            case TypeBuffers::VAO: 
                glGenVertexArrays(n, &b);
                vao_.insert(std::make_pair(key, b));
                break;
            case TypeBuffers::VBO:
                glGenBuffers(n, &b);
                vbo_.insert(std::make_pair(key, b));
                break;
            case TypeBuffers::EBO:
                glGenBuffers(n, &b);
                ebo_.insert(std::make_pair(key, b));
                break;
        }
    }

    void Renderer::BindVertexArray(GLuint array) {
        glBindVertexArray(array);
    }

    void Renderer::BindBuffer(GLenum target, GLuint buffer) {
        glBindBuffer(target, buffer);
    }

    void Renderer::BufferData(GLenum target, GLsizeiptr sizeptr, const void* data, GLenum usage) {
        glBufferData(target, sizeptr, data, usage);
    }

    void Renderer::SetVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, void* offset) {
        glVertexAttribPointer(index, size, type, normalized, stride, (void*)offset);
    }

    void Renderer::EnableVertexAttribArray(GLuint index) {
        glEnableVertexAttribArray(index);
    }

    void Renderer::SetOrthoProjection(float left, float right, float bottom, float top, float zNear, float zFar) {
        glm::mat4 projectionMatrix = glm::ortho(left, static_cast<float>(window_->GetWidth()), bottom, static_cast<float>(window_->GetHeight()), zNear, zFar);

        shader_->use();
        shader_->setMat4("projection", projectionMatrix);
        BindVertexArray(GetVAO(MapKey::OBJECTS));

        shader_text_->use();
        glUniformMatrix4fv(glGetUniformLocation(shader_text_->GetID(), "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        BindVertexArray(GetVAO(MapKey::TEXT));
    }

    void Renderer::SetPerspectiveProjection(float fov, unsigned int width, unsigned int height, float near, float far) {
        // We set the projection once?
        projection_ = glm::perspective(glm::radians(fov), static_cast<float>(width) / static_cast<float>(height), near, far);
        
        shader_->use();
        shader_->setMat4("projection", projection_);
        BindVertexArray(GetVAO(MapKey::kCube));

        shader_ligth_->use();
        shader_ligth_->setMat4("projection", projection_);
        BindVertexArray(GetVAO(MapKey::kLight));

        shader_text_->use();
        glUniformMatrix4fv(glGetUniformLocation(shader_text_->GetID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection_));
        BindVertexArray(GetVAO(MapKey::TEXT));
    }

    void Renderer::SetLigth(Shader* shader, glm::vec3& colorLigth, glm::vec3& objectColor) {
        shader->use();
        shader->setVec3("objectColor", objectColor);
        shader->setVec3("ligthColor", colorLigth);
    }

    void Renderer::Draw(const glm::vec2& position, const glm::vec2& size, AxisRotate axis, GLfloat rotate) {
        
        shader_->use();
        BindVertexArray(GetVAO(MapKey::OBJECTS));
        // TODO: Переписать в отдельные методы - тогда могу скалировать, вращать, перемещать независимо любой объект
        glm::mat4 model_matrix = glm::mat4(1.f);

        // Выполняем трансформации матрицы
        model_matrix = TranslateMatrix(model_matrix, position);
        model_matrix = RotateMatrix(model_matrix, axis, rotate);
        model_matrix = ScaleMatrix(model_matrix, size);

        shader_->setMat4("model", model_matrix);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        BindVertexArray(GetVAO(MapKey::OBJECTS));
    }

    void Renderer::Draw(Shader* shader, const glm::vec3& position, const glm::vec3& size, AxisRotate axis, 
                        GLfloat rotate, MapKey key) {
       

        glm::mat4 model_matrix = glm::mat4(1.f);
        model_matrix = TranslateMatrix(model_matrix, position);
        model_matrix = RotateMatrix(model_matrix, axis, rotate);
        model_matrix = ScaleMatrix(model_matrix, size);

        // view_matrix = TranslateMatrix(view_matrix, glm::vec3(0.f, 0.f, -5.f));

        glm::mat4 view_matrix = glm::mat4(1.f);                  
        view_matrix = camera_->GetViewMatrix();
        glm::vec3 view_pos = camera_->GetPosition();

        shader->use();
        shader->setMat4("model", model_matrix);
        shader->setVec3("view_pos", view_pos);
        shader->setMat4("view", view_matrix);

        BindVertexArray(GetVAO(key));
        
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        BindVertexArray(0);
    }

    glm::vec3 Renderer::PickUpObject(double x_px, double y_px, int width, int height, GLfloat depth) const {
        glm::vec4 view_port(0, 0, width, height);
        glm::vec3 win_coord(x_px, height - y_px - 1, depth);
        auto view = camera_->GetViewMatrix();
        
        glm::vec3 obj_coord(glm::unProject(win_coord, view, projection_, view_port));

        std::cout << "Coordinates in object space: " << obj_coord.x << " " << obj_coord.y << " " << obj_coord.z << '\n';

        return obj_coord;
    }


    // void Renderer::DrawText(std::string text, float x, float y, float scale, glm::vec3 color) {
    //     shader_text_->use();

    //     glUniform3f(glGetUniformLocation(shader_text_->ID, "textColor"), color.x, color.y, color.z);
    //     glActiveTexture(GL_TEXTURE0);
    //     BindVertexArray(GetVAO(MapKey::TEXT));

    //     // iterate through all characters
    //     std::string::const_iterator c;
    //     for (c = text.begin(); c != text.end(); c++) 
    //     {
    //         Character ch = text_->GetCharacter(*c);

    //         float xpos = x + ch.Bearing.x * scale;
    //         float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

    //         float w = ch.Size.x * scale;
    //         float h = ch.Size.y * scale;
    //         // update VBO for each character
    //         float vertices[6][4] = {
    //             { xpos,     ypos + h,   0.0f, 0.0f },            
    //             { xpos,     ypos,       0.0f, 1.0f },
    //             { xpos + w, ypos,       1.0f, 1.0f },

    //             { xpos,     ypos + h,   0.0f, 0.0f },
    //             { xpos + w, ypos,       1.0f, 1.0f },
    //             { xpos + w, ypos + h,   1.0f, 0.0f }           
    //         };
    //         // Renderer glyph texture over quad
    //         glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    //         // update content of VBO memory
    //         BindBuffer(GL_ARRAY_BUFFER, GetVBO(MapKey::TEXT));
    //         glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

    //         BindBuffer(GL_ARRAY_BUFFER, 0);
    //         // Renderer quad
    //         glDrawArrays(GL_TRIANGLES, 0, 6);
    //         // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    //         x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    //     }
    //     glBindTexture(GL_TEXTURE_2D, 0);
    //     BindVertexArray(GetVAO(MapKey::TEXT));
    // }

    glm::mat4 Renderer::RotateMatrix(glm::mat4& model, AxisRotate axis, GLfloat rotate) {
        glm::vec3 r_vec;

        switch(axis) {
            case AxisRotate::AXIS_X: {
                r_vec = {1.f, 0.f, 0.f};
                break;
            }
            case AxisRotate::AXIS_Y: {
                r_vec = {0.f, 1.f, 0.f};
                break;
            }
            case AxisRotate::AXIS_Z: {
                r_vec = {0.f, 0.f, 1.f};
                break;
            }
            case AxisRotate::NONE: {
                r_vec = {0.f, 0.f, 0.5f};
                break;
            }
        }
        return glm::rotate(model, glm::radians(rotate), r_vec);
    }

    glm::mat4 Renderer::TranslateMatrix(glm::mat4& model, const glm::vec2& position) {
        return glm::translate(model, glm::vec3(position, 0.f));
    }

    glm::mat4 Renderer::TranslateMatrix(glm::mat4& model, const glm::vec3& position) {
        return glm::translate(model, glm::vec3(position));
    }

    glm::mat4 Renderer::ScaleMatrix(glm::mat4& model, const glm::vec2& size) {
        return glm::scale(model, glm::vec3(size.x, size.y, 0.f));
    }

    glm::mat4 Renderer::ScaleMatrix(glm::mat4& model, const glm::vec3& size) {
        return glm::scale(model, glm::vec3(size));
    }

    Shader* Renderer::GetShaderPtr(std::string name) const {
        if (name == "object") {
            return shader_;
        }

        else if (name == "ligth") {
            return shader_ligth_;
        }
        
        return nullptr;
    }
} // namespace avion::gfx