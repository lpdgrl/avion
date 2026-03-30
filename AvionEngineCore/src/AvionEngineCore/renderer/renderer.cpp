#include "AvionEngineCore/renderer/renderer.hpp"
#include "AvionEngineCore/core/resource_manager.hpp"

namespace avion::gfx {

    Renderer::Renderer(core::resman::ResourceManager& resman, ShaderStorage& storage)
    : m_storage_shaders(storage)
    , m_resman(resman)
    {}

    Renderer::~Renderer() {
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
        InitShaders();

        // text_ = new TextRender(PATH_TO_FONT);
        // text_->Initialaztion();

        InitRenderer();
        InitRendererText();
        InitCamera();
    }

    void Renderer::InitCamera() {
        glm::vec3 camera_pos    = glm::vec3(0.0f, 0.0f, 10.0f);
        glm::vec3 camera_up     = glm::vec3(0.0f, 1.0f, 0.0f);

        camera_ = new Camera(camera_pos, camera_up);
    }

    void Renderer::InitRenderer() {
        LoadVerticesCube();
        LoadVerticesPyramid();
        LoadVerticesSourceLigth();
    }

    void Renderer::InitShaders() 
    {
      std::string simple_cube("lighting");
      std::string shader_model_light_source("shader_model_light_source");
    
      m_storage_shaders.RegisterShader(
          simple_cube, 
          m_resman.GetResource<std::filesystem::path>("simple_cube_transform.vert")->c_str(),
          m_resman.GetResource<std::filesystem::path>("lighting.frag")->c_str());

      m_storage_shaders.RegisterShader(
          shader_model_light_source,
          m_resman.GetResource<std::filesystem::path>("simple_light_transform.vert")->c_str(),
          m_resman.GetResource<std::filesystem::path>("simple_light_color.frag")->c_str());

    }

    void Renderer::InitTexture() 
    {
      // m_storage_shaders.PutData("simple_cube_SL_prefab_material", "material.diffuse", 0);
      // m_storage_shaders.PutData("simple_cube_SL_prefab_material", "material.specular", 1);
      // m_storage_shaders.PutData("simple_cube_SL_prefab_material", "material.emission", 2);

      // m_storage_shaders.UseShader("simple_cube_SL_prefab_material");
    }
    

    void Renderer::LoadVerticesCube() {
        // actual!!
        float cube[] = {
            // positions          // normals           // texture coords
           -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
           -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
           -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

           -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
           -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
           -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

           -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
           -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
           -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
           -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
           -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
           -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

           -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
           -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
           -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

           -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
           -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
           -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
        };

        //Buffers for objects
        GenerateBuffer(1, OpenglObjectType::kVAO, VertexObjectType::kCube);
        GenerateBuffer(1, OpenglObjectType::kVBO, VertexObjectType::kCube);

        BindVertexArray(GetVAO(VertexObjectType::kCube));

        BindBuffer(GL_ARRAY_BUFFER, GetVBO(VertexObjectType::kCube));
        BufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

        // BindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetEBO(MapKey::OBJECTS));
        // BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_objects), indices_objects, GL_DYNAMIC_DRAW);

        // Position attribute 
        SetVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
        EnableVertexAttribArray(0);

        // Normal attribute
        SetVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        EnableVertexAttribArray(1);

        SetVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        EnableVertexAttribArray(2);

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
        GenerateBuffer(1, OpenglObjectType::kVAO, VertexObjectType::kPyramid);
        GenerateBuffer(1, OpenglObjectType::kVBO, VertexObjectType::kPyramid);

        BindVertexArray(GetVAO(VertexObjectType::kPyramid));

        BindBuffer(GL_ARRAY_BUFFER, GetVBO(VertexObjectType::kPyramid));
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
        GenerateBuffer(1, OpenglObjectType::kVAO, VertexObjectType::kLight);
        GenerateBuffer(1, OpenglObjectType::kVBO, VertexObjectType::kLight);
        
        BindVertexArray(GetVAO(VertexObjectType::kLight));

        BindBuffer(GL_ARRAY_BUFFER, GetVBO(VertexObjectType::kLight));
        BufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

        SetVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        EnableVertexAttribArray(0);

        BindBuffer(GL_ARRAY_BUFFER, 0);
        BindVertexArray(0);
    }

    void Renderer::InitRendererText() {
        GenerateBuffer(1, OpenglObjectType::kVAO, VertexObjectType::kText);
        GenerateBuffer(1, OpenglObjectType::kVBO, VertexObjectType::kText);

        BindVertexArray(GetVAO(VertexObjectType::kText));
        BindBuffer(GL_ARRAY_BUFFER, GetVBO(VertexObjectType::kText));
        
        BufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

        EnableVertexAttribArray(0);
        SetVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

        BindBuffer(GL_ARRAY_BUFFER, 0);
        BindVertexArray(0); 
    }

    void Renderer::GenerateBuffer(const GLsizei n, OpenglObjectType type, VertexObjectType key) {
        GLuint b;
        
        switch(type)
        {
            case OpenglObjectType::kVAO: 
                glGenVertexArrays(n, &b);
                vao_.insert(std::make_pair(key, b));
                break;
            case OpenglObjectType::kVBO:
                glGenBuffers(n, &b);
                vbo_.insert(std::make_pair(key, b));
                break;
            case OpenglObjectType::kEBO:
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
        glVertexAttribPointer(index, size, type, normalized, stride, offset);
    }

    void Renderer::EnableVertexAttribArray(GLuint index) {
        glEnableVertexAttribArray(index);
    }

    void Renderer::Draw(RenderContext& render_context) {
        auto [type_shader, name_shader, transform, mat_tex, key] = render_context;

        glm::mat4 model_matrix = glm::mat4(1.f);
        model_matrix = TranslateMatrix(model_matrix, transform.position);
        model_matrix = RotateMatrix(model_matrix, transform.axis, transform.rotate);
        model_matrix = ScaleMatrix(model_matrix, transform.size);

        // view_matrix = TranslateMatrix(view_matrix, glm::vec3(0.f, 0.f, -5.f));

        glm::mat4 view_matrix = glm::mat4(1.f);                  
        view_matrix = camera_->GetViewMatrix();
        glm::vec3 view_pos = camera_->GetPosition();

        m_storage_shaders.PutData(name_shader, "view", view_matrix);
        m_storage_shaders.PutData(name_shader, "view_pos", view_pos);
        m_storage_shaders.PutData(name_shader, "model", model_matrix);

        m_storage_shaders.UseShader(name_shader);
        
        if (mat_tex.is_texture) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mat_tex.idx_texture);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, mat_tex.idx_texture_specular);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, mat_tex.idx_texture_emission);
        }

        BindVertexArray(GetVAO(key)); 
        glDrawArrays(GL_TRIANGLES, 0, 36);

        BindVertexArray(0);
    }

    void Renderer::SetOrthoProjection(float left, float width, float bottom, float height, float zNear, float zFar) {
        glm::mat4 projectionMatrix = glm::ortho(left, static_cast<float>(width), bottom, static_cast<float>(height), zNear, zFar);

        // shader_->use();
        // shader_->setMat4("projection", projectionMatrix);
        // BindVertexArray(GetVAO(MapKey::OBJECTS));

        // shader_text_->use();
        // glUniformMatrix4fv(glGetUniformLocation(shader_text_->GetID(), "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        // BindVertexArray(GetVAO(MapKey::TEXT));
    }

    void Renderer::SetPerspectiveProjection(float fov, unsigned int width, unsigned int height, float near, float far) {
        // We set the projection once?
        projection_ = glm::perspective(glm::radians(fov), static_cast<float>(width) / static_cast<float>(height), near, far);


        for (const auto& [_, shader] : m_storage_shaders.GetStorage()) {
          shader->PutData("projection", projection_);
          shader->Execute();
          BindVertexArray(GetVAO(VertexObjectType::kCube));
        }

        // shader_text_->use();
        // glUniformMatrix4fv(glGetUniformLocation(shader_text_->GetID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection_));
        // BindVertexArray(GetVAO(MapKey::TEXT));
    }

    void Renderer::Draw(const glm::vec2& position, const glm::vec2& size, AxisRotate axis, GLfloat rotate) {
        // shader_->use();
        // BindVertexArray(GetVAO(MapKey::OBJECTS));
        // // TODO: Переписать в отдельные методы - тогда могу скалировать, вращать, перемещать независимо любой объект
        // glm::mat4 model_matrix = glm::mat4(1.f);

        // // Выполняем трансформации матрицы
        // model_matrix = TranslateMatrix(model_matrix, position);
        // model_matrix = RotateMatrix(model_matrix, axis, rotate);
        // model_matrix = ScaleMatrix(model_matrix, size);

        // shader_->setMat4("model", model_matrix);

        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // BindVertexArray(GetVAO(MapKey::OBJECTS));
    }

    void Renderer::LoadTexture2D(std::uint32_t& index_texture, std::uint16_t width, std::uint16_t height, unsigned char* buffer, GLenum format) const 
    {
        glGenTextures(1, &index_texture);
        glBindTexture(GL_TEXTURE_2D, index_texture);
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, buffer);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

    void Renderer::ChangeCameraPosition(CameraMovement direction, GLfloat delta_time) const noexcept {
        camera_->ProcessKeyboard(direction, delta_time);
    }

    void Renderer::ProcessMouseMovement(double xoffset, double yoffset) const noexcept {
        camera_->ProcessMouseMovement(xoffset, yoffset);
    }
    
    void Renderer::InsertIdTextureBuffer(std::uint32_t idx) noexcept
    {
        idx_textures_.emplace(idx, idx);
    }

    std::uint32_t Renderer::GetIdTextureBuffer(std::uint32_t idx) noexcept 
    {
        if (auto it = idx_textures_.find(idx); it != idx_textures_.end()) {
            return it->second;
        }
        return 0;
    }

  
} // namespace avion::gfx
