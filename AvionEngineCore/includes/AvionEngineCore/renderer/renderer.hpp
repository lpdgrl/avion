#pragma once

#include <map>
#include <unordered_map>
#include <variant>
#include <type_traits>

#include "shader.hpp"
// #include "text_rendering.hpp"
#include "camera.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace avion::gfx {
    enum class TypeBuffers {
        EBO = 0,
        VBO = 1,
        VAO = 2
    };

    enum class ShaderType {
        kObjects = 0,
        kLight = 1,
    };

    enum class MapKey {
        OBJECTS = 0,
        TEXT = 1,
        kLight = 2,
        kCube = 3,
        kPyramid = 4
    };

    enum class AxisRotate {
        NONE   = -1,
        AXIS_X =  0,
        AXIS_Y =  1,
        AXIS_Z =  2,
    };

    template <typename T>
    struct ShaderParam {
        std::string name_param;
        T value;
    };

    // struct ShaderContext {
    //     ShaderContext(ShaderType type);
    //     const ShaderType type;
    // };

    struct ShaderLight  {
        ShaderType type = ShaderType::kLight;
        ShaderParam<glm::vec3> light_color;
        ShaderParam<glm::vec3> view_pos; 
        ShaderParam<glm::mat4> view_matrix;
        ShaderParam<glm::mat4> model_matrix;
    };

    struct ShaderObject {
        ShaderType type = ShaderType::kObjects;
        ShaderParam<GLfloat> light_ambient;
        ShaderParam<GLfloat> light_specular;
        ShaderParam<GLfloat> light_shininess;
        ShaderParam<GLfloat> screen_aspect;
        ShaderParam<glm::vec3> object_color;
        ShaderParam<glm::vec3> light_color;
        ShaderParam<glm::vec3> light_pos;
        ShaderParam<glm::vec3> view_pos; 
        ShaderParam<glm::mat4> view_matrix;
        ShaderParam<glm::mat4> model_matrix;
    };

    template <typename T_struct>
    struct RenderContext {
        T_struct shader; 
        glm::vec3 position;
        glm::vec3 size;
        AxisRotate axis;
        GLfloat rotate;
        MapKey key;
    };

    // Класс Render - занимается отрисовкой сцены, расчетом матриц трансформаций и векторов
    class Renderer {  
    public:
        Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;

        ~Renderer();

        inline GLuint GetVBO(MapKey key) const { return vbo_.at(key); }
        inline GLuint GetVAO(MapKey key) const { return vao_.at(key); }
        inline GLuint GetEBO(MapKey key) const { return ebo_.at(key); }

    public:
        void Init();
        void InitRenderer();
        void InitRendererText();

        void Draw(const glm::vec2& position, const glm::vec2& size, AxisRotate axis, GLfloat rotate);

        template <typename T_struct>
        void Draw(RenderContext<T_struct> render_context);
        // void Draw(Shader* shader, const glm::vec3& position, const glm::vec3& size, 
        //     AxisRotate axis, GLfloat rotate, MapKey key);
        // void DrawText(std::string text, float x, float y, float scale, glm::vec3 color);

        void SetLigth(Shader* shader, glm::vec3& colorLigth, glm::vec3& objectColor);

        void SetOrthoProjection(float left, float width, float bottom, float height, float zNear, float zFar);
        void SetPerspectiveProjection(float fov, unsigned int width, unsigned int height, float near, float far);

        Shader* GetShaderPtr(std::string name) const;

        void ChangeCameraPosition(CameraMovement direction, GLfloat delta_time) const noexcept; 

        void ProcessMouseMovement(double xoffset, double yoffset) const noexcept;
        
        glm::vec3 PickUpObject(double x_px, double y_px, int width, int height, GLfloat depth) const;

    private:
        void InitCamera();

        void GenerateBuffer(const GLsizei n, TypeBuffers type, MapKey key);
        void BufferData(GLenum target, GLsizeiptr sizeptr, const void* data, GLenum usage);

        void BindVertexArray(GLuint array);
        void BindBuffer(GLenum target, GLuint buffer);

        void SetVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, void* offset);
        void EnableVertexAttribArray(GLuint index);

        void LoadVerticesCube();
        void LoadVerticesPyramid();
        void LoadVerticesSourceLigth();

        glm::mat4 RotateMatrix(glm::mat4& model, AxisRotate axis, GLfloat rotate);

        glm::mat4 TranslateMatrix(glm::mat4& model, const glm::vec2& position);
        glm::mat4 TranslateMatrix(glm::mat4& model, const glm::vec3& position);
        
        glm::mat4 ScaleMatrix(glm::mat4& model, const glm::vec2& size);
        glm::mat4 ScaleMatrix(glm::mat4& model, const glm::vec3& size);

        template <typename T_struct>
        void UseShader(T_struct shader);

    private:
        Shader* shader_ = nullptr;
        Shader* shader_text_ = nullptr;
        Shader* shader_ligth_ = nullptr;
        // TextRender* text_ = nullptr;
        Camera* camera_ = nullptr;

        std::map<MapKey, GLuint> vao_;
        std::map<MapKey, GLuint> vbo_;
        std::map<MapKey, GLuint> ebo_;

        bool cursor_state_ = false;

        // TODO: Хранить путь к шейдерам в классе рендера неправильно!
        const char* PATH_TO_FILE_VERTEX_SHADER = "./AvionEngineCore/src/AvionEngineCore/shaders/shader.vs";
        const char* PATH_TO_FILE_FRAGMENT_SHADER = "./AvionEngineCore/src/AvionEngineCore/shaders/shader.fs";

        // TODO: Хранить путь к шейдерам в классе рендера неправильно!
        const char* PATH_TO_VERTEX_SHADER_TEXT = "./AvionEngineCore/src/AvionEngineCore/shaders/text.vs";
        const char* PATH_TO_FRAGMENT_SHADER_TEXT = "./AvionEngineCore/src/AvionEngineCore/shaders/text.fs";

        // TODO: Хранить путь к шейдерам в классе рендера неправильно!
        const char* PATH_TO_VERTEX_SHADER_LIGTH = "./AvionEngineCore/src/AvionEngineCore/shaders/ligth_shader.vs";
        const char* PATH_TO_FRAGMENT_SHADER_LIGTH = "./AvionEngineCore/src/AvionEngineCore/shaders/ligth_shader.fs";

        // TODO: Хранение пути в внутри классе рендера неправильно!
        const char* PATH_TO_FONT = "./AvionEngineCore/data/fonts/dejavusans.ttf";

        glm::mat4 projection_;
    };

    template <typename T_struct>
    void Renderer::Draw(RenderContext<T_struct> render_context) {
        auto [shader, position, size, axis, rotate, key] = render_context;

        glm::mat4 model_matrix = glm::mat4(1.f);
        model_matrix = TranslateMatrix(model_matrix, position);
        model_matrix = RotateMatrix(model_matrix, axis, rotate);
        model_matrix = ScaleMatrix(model_matrix, size);

        // view_matrix = TranslateMatrix(view_matrix, glm::vec3(0.f, 0.f, -5.f));

        glm::mat4 view_matrix = glm::mat4(1.f);                  
        view_matrix = camera_->GetViewMatrix();
        glm::vec3 view_pos = camera_->GetPosition();

        shader.view_matrix.value = view_matrix;
        shader.view_pos.value = view_pos;
        shader.model_matrix.value = model_matrix;
        
        UseShader<T_struct>(shader);
        // shader->use();
        // shader->setMat4("model", model_matrix);
        // shader->setVec3("view_pos", view_pos);
        // shader->setMat4("view", view_matrix);

        BindVertexArray(GetVAO(key));
        
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        BindVertexArray(0);
    }
    
    template <typename T_struct>
    void Renderer::UseShader(T_struct shader) {
        if constexpr (std::is_same_v<T_struct, ShaderObject>) {
            shader_->use();
            shader_->setFloat(shader.light_ambient.name_param.c_str(), shader.light_ambient.value);
            shader_->setFloat(shader.light_specular.name_param.c_str(), shader.light_specular.value);
            shader_->setFloat(shader.light_shininess.name_param.c_str(), shader.light_shininess.value);
            shader_->setFloat(shader.screen_aspect.name_param.c_str(), shader.screen_aspect.value);
            shader_->setVec3(shader.object_color.name_param.c_str(), shader.object_color.value);
            shader_->setVec3(shader.light_color.name_param.c_str(), shader.light_color.value);
            shader_->setVec3(shader.light_pos.name_param.c_str(), shader.light_pos.value);
            shader_->setMat4(shader.model_matrix.name_param.c_str(), shader.model_matrix.value);
            shader_->setVec3(shader.view_pos.name_param.c_str(), shader.view_pos.value);
            shader_->setMat4(shader.view_matrix.name_param.c_str(), shader.view_matrix.value);
        } else {
            shader_ligth_->use();
            shader_ligth_->setVec3(shader.light_color.name_param.c_str(), shader.light_color.value);
            shader_ligth_->setMat4(shader.model_matrix.name_param.c_str(), shader.model_matrix.value);
            shader_ligth_->setVec3(shader.view_pos.name_param.c_str(), shader.view_pos.value);
            shader_ligth_->setMat4(shader.view_matrix.name_param.c_str(), shader.view_matrix.value);
        }

    }

} // namespace avion::gfx