#pragma once 

#include <map>
#include <unordered_map>
#include <type_traits>
#include <memory>

#include "shader.hpp"
// #include "text_rendering.hpp"
#include "camera.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace avion::core::resman 
{
  class ResourceManager;
}

namespace avion::core
{
  class Texture;
  struct CameraState;
}

namespace avion::gfx {

  class Mesh;

  enum class OpenglObjectType {
      kUnknown = -1,
      kEBO     = 1,
      kVBO     = 2,
      kVAO     = 3,
  };

  enum class ShaderLight {
      kShUnknown         = -1,
      kShSimpleLight     =  0,
      kShPrefabMaterial  =  1,
      kShDirLight        =  2,
      kShPointLight      =  3,
  };

  enum class VertexObjectType {
      kUnknown  = -1,
      kText     =  1,
      kLight    =  2,
      kCube     =  3,
      kPyramid  =  4,
      kModel    =  5,
  };

  enum class AxisRotate {
      NONE   = -1,
      AXIS_X =  0,
      AXIS_Y =  1,
      AXIS_Z =  2,
  };

  struct Transform {
    glm::vec3 position; 
    glm::vec3 size;
    AxisRotate axis = AxisRotate::NONE;
    GLfloat rotate = 0.f;
  };

  struct TransferMaterial {
    bool is_texture = false;
    int idx_texture = 0;
    int idx_texture_specular = 0;
    int idx_texture_emission = 0;
  };

  struct RenderContext {
    ShaderLight type_shader;
    std::string name_shader;
    Transform transform;
    TransferMaterial mat_tex;
    VertexObjectType key;
  };

  class Renderer {  
  public:
    using ResManager  = core::resman::ResourceManager;
    using CameraState = core::CameraState;
     
    Renderer(ShaderStorage& storage, CameraState& camera_state);

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    Renderer(Renderer&&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    ~Renderer();

    inline GLuint GetVBO(VertexObjectType key) const { return vbo_.at(key); }
    inline GLuint GetVAO(VertexObjectType key) const { return vao_.at(key); }
    inline GLuint GetEBO(VertexObjectType key) const { return ebo_.at(key); }

  public:
    void Init();
    void InitTexture();

    void Draw(const glm::vec2& position, const glm::vec2& size, AxisRotate axis, GLfloat rotate);
    void Draw(RenderContext& render_context);
    void Draw(Mesh& mesh, const std::string& name_shader);

    void SetOrthoProjection(float left, float width, float bottom, float height, float zNear, float zFar);
    void SetPerspectiveProjection(float fov, unsigned int width, unsigned int height, float near, float far);

    void ChangeCameraPosition(CameraMovement direction, GLfloat delta_time) const noexcept; 

    void ProcessMouseMovement(double xoffset, double yoffset) const noexcept;
    
    glm::vec3 PickUpObject(double x_px, double y_px, int width, int height, GLfloat depth) const;

    // TODO: For future implementation
    // void LoadTexture(TypeTexture) const;
    
    void SetRenderContext(RenderContext& render_ctx) noexcept;
    void RegisterMesh(Mesh* mesh) noexcept;
    void LoadTexture2D(std::uint32_t& index_texture, std::uint16_t width, std::uint16_t height, unsigned char* buffer, GLenum format) const;
    void LoadTexture2D(core::Texture* ptr_texture) const;


    glm::vec3 GetCameraPosition() const noexcept;

  private:
    void InitCamera();
    void InitRenderer();
    void InitRendererText();

    void GenerateBuffer(const GLsizei n, OpenglObjectType type, VertexObjectType key);
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
    Camera* camera_ = nullptr;
    CameraState& m_camera_state;

    std::map<VertexObjectType, GLuint> vao_;
    std::map<VertexObjectType, GLuint> vbo_;
    std::map<VertexObjectType, GLuint> ebo_;
    
    ShaderStorage& m_storage_shaders;

    bool cursor_state_ = false;
    //
    // TODO: Хранение пути в внутри классе рендера неправильно!
    const char* PATH_TO_FONT = "./AvionEngineCore/data/fonts/dejavusans.ttf";

    glm::mat4 projection_;
  };

} // namespace avion::gfx
