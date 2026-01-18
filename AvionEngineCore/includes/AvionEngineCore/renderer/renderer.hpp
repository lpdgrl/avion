#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <map>

#include "shader.hpp"
// #include "text_rendering.hpp"
#include "camera.hpp"

namespace avion::core {
    class Window;
} // namespace avion::core

namespace avion::gfx {

    #define WHITE glm::vec3(1.f, 1.f, 1.f)

    enum class TypeBuffers {
        EBO = 0,
        VBO = 1,
        VAO = 2
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


    // Класс Render - занимается отрисовкой сцены, расчетом матриц трансформаций и векторов
    class Renderer {  
    // Инвариат == ?
    public:
        Renderer() = delete;
        
        explicit Renderer(core::Window* window);

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
        void Draw(Shader* shader, const glm::vec3& position, const glm::vec3& size, 
            AxisRotate axis, GLfloat rotate, MapKey key);
        // void DrawText(std::string text, float x, float y, float scale, glm::vec3 color);

        void SetLigth(Shader* shader, glm::vec3& colorLigth, glm::vec3& objectColor);
        void SetOrthoProjection(float left, float right, float bottom, float top, float zNear, float zFar);
        void SetPerspectiveProjection(float fov, unsigned int width, unsigned int height, float near, float far);

        Shader* GetShaderPtr(std::string name) const;

        void UpdateCoordinatesCamera(GLfloat delta_time);
        void Update();

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

    private:
        Shader* shader_ = nullptr;
        Shader* shader_text_ = nullptr;
        Shader* shader_ligth_ = nullptr;
        // TextRender* text_ = nullptr;
        Camera* camera_ = nullptr;
        core::Window* window_ = nullptr;

        std::map<MapKey, GLuint> vao_;
        std::map<MapKey, GLuint> vbo_;
        std::map<MapKey, GLuint> ebo_;

        bool cursor_state_ = false;

        // TODO: Хранить путь к шейдерам в классе рендера неправильно!
        const char* PATH_TO_FILE_VERTEX_SHADER = "/home/lpdgrl/Project/code/avion/AvionEngineCore/src/AvionEngineCore/shaders/shader.vs";
        const char* PATH_TO_FILE_FRAGMENT_SHADER = "/home/lpdgrl/Project/code/avion/AvionEngineCore/src/AvionEngineCore/shaders/shader.fs";

        // TODO: Хранить путь к шейдерам в классе рендера неправильно!
        const char* PATH_TO_VERTEX_SHADER_TEXT = "/home/lpdgrl/Project/code/avion/AvionEngineCore/src/AvionEngineCore/shaders/text.vs";
        const char* PATH_TO_FRAGMENT_SHADER_TEXT = "/home/lpdgrl/Project/code/avion/AvionEngineCore/src/AvionEngineCore/shaders/text.fs";

        // TODO: Хранить путь к шейдерам в классе рендера неправильно!
        const char* PATH_TO_VERTEX_SHADER_LIGTH = "/home/lpdgrl/Project/code/avion/AvionEngineCore/src/AvionEngineCore/shaders/ligth_shader.vs";
        const char* PATH_TO_FRAGMENT_SHADER_LIGTH = "/home/lpdgrl/Project/code/avion/AvionEngineCore/src/AvionEngineCore/shaders/ligth_shader.fs";

        // TODO: Хранение пути в внутри классе рендера неправильно!
        const char* PATH_TO_FONT = "/home/lpdgrl/Project/code/avion/AvionEngineCore/data/fonts/dejavusans.ttf";
    };

} // namespace avion::gfx