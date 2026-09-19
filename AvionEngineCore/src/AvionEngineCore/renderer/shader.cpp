#include "AvionEngineCore/renderer/shader.hpp"
#include <fstream>
#include <sstream>

namespace avion::gfx {
  Shader::Shader (std::string_view vertex_code, std::string_view fragment_code)
  {   
      api::backend::opengl::GLShader vertex(api::backend::opengl::detail::ShaderType::Vertex, vertex_code);
      api::backend::opengl::GLShader fragment(api::backend::opengl::detail::ShaderType::Fragment, fragment_code);

      if (vertex.IsSuccess() && fragment.IsSuccess())
      {
        m_program.AttachShader(vertex);
        m_program.AttachShader(fragment);
        m_program.LinkProgram();
        m_is_completed = m_program.IsSuccess();
      }
  }

  Shader::~Shader() {
    // TODO: Delete shader from GPU memory
    AV_LOG_INFO(std::format("Shader::~Shader(): shader is destroyed."));
  }

  void Shader::Use() 
  {
    // AV_LOG_DEBUG(std::format("Shader::Use: id_prog {}", m_program.GetId()));
    glUseProgram(m_program.GetId());
  }

  void Shader::SetBool(const std::string &name, bool value) const
  {
      glUniform1i(glGetUniformLocation(m_program.GetId(), name.c_str()), (int)value);
  }

  void Shader::SetInt(const std::string &name, int value) const
  {
      glUniform1i(glGetUniformLocation(m_program.GetId(), name.c_str()), value);
  }

  void Shader::SetFloat(const std::string &name, float value) const
  {
      glUniform1f(glGetUniformLocation(m_program.GetId(), name.c_str()), value);
  }
  void Shader::SetMat4(const std::string &name, glm::mat4& value) const {
      glUniformMatrix4fv(glGetUniformLocation(m_program.GetId(), name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
  }

  void Shader::SetVec3(const std::string &name, glm::vec3& value) const {
      glUniform3fv(glGetUniformLocation(m_program.GetId(), name.c_str()), 1, &value[0]);
  }

  void Shader::SetVec4(const std::string& name, glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(m_program.GetId(), name.c_str()), 1, &value[0]);
  }

  Shader::ShaderProgram Shader::GetID() const noexcept {
      return m_program.GetId();
  }

  Shader::ShaderProgram Shader::GetID() noexcept {
      return m_program.GetId();
  }

  auto Shader::IsCompleted() const noexcept -> bool
  {
    return m_is_completed;
  }

  void Shader::SetValue(const std::string& name, ParamType value) const
  {
      if (std::holds_alternative<int>(value)) {
        SetInt(name, std::get<int>(value));
      } else if (std::holds_alternative<bool>(value)) {
        SetBool(name, std::get<bool>(value)); 
      } else if (std::holds_alternative<float>(value)) {
        SetFloat(name, std::get<float>(value));
      } else if (std::holds_alternative<glm::mat4>(value)) {
        SetMat4(name, std::get<glm::mat4>(value));
      } else if (std::holds_alternative<glm::vec3>(value)) {
        SetVec3(name, std::get<glm::vec3>(value));
      } else if (std::holds_alternative<glm::vec4>(value)) {
        SetVec4(name, std::get<glm::vec4>(value));
      }
  }

  auto Shader::Reload(std::string_view vertex_code, std::string_view fragment_code) -> void
  {
      api::backend::opengl::GLShader vertex(api::backend::opengl::detail::ShaderType::Vertex, vertex_code);
      api::backend::opengl::GLShader fragment(api::backend::opengl::detail::ShaderType::Fragment, fragment_code);
      api::backend::opengl::GLShaderProgram new_program;

      if (vertex.IsSuccess() && fragment.IsSuccess())
      {
        new_program.AttachShader(vertex);
        new_program.AttachShader(fragment);
        new_program.LinkProgram();
        m_is_completed = new_program.IsSuccess();
        if (m_is_completed)
        { 
          using std::swap;
          swap(m_program, new_program);
          m_reloaded = true;
          return;
        }
      }
      m_reloaded = false;
  }

  auto Shader::HasReloaded() const noexcept -> bool
  {
    return m_reloaded;
  }

  auto Shader::UpdateStateAfterReloaded() noexcept -> void
  {
    m_reloaded = false;
  }

  /* 
  ShaderExecutor::ShaderExecutor() 
  {
    m_data.reserve(kSizeShaderData);
  }
  */

  ShaderExecutor::ShaderExecutor(std::string_view vertex_code, std::string_view fragment_code)
    : m_shader(std::make_unique<Shader>(vertex_code, fragment_code))
  {
    m_data.reserve(kSizeShaderData);
  }

  void ShaderExecutor::Execute()
  {
    if (m_data.empty()) {
      AV_LOG_ERROR("Data for shader is empty");
      assert(false);
    } 

    m_shader->Use();

    // TODO: When data isn't changed so no necessary write to shader 
    for (const auto& shader_param : m_data) {
      m_shader->SetValue(shader_param.name, shader_param.value);
    }

    m_data.clear();
  }

  void ShaderExecutor::ExecuteAfterUse()
  {
    if (m_data.empty())
    {
      AV_LOG_ERROR("Data for shader is empty");
      assert(false);
    }

    // TODO: When data isn't changed so no necessary write to shader 
    for (const auto& shader_param : m_data) {
      m_shader->SetValue(shader_param.name, shader_param.value);
    }

    m_data.clear();
  }

  auto ShaderExecutor::Reload(std::string_view vertex_code, std::string_view fragment_code) -> void
  {
    m_shader->Reload(vertex_code, fragment_code);
  }

  void ShaderStorage::ExecuteAfterUse(const std::string& name_shader)
  {
    auto it_sh = m_storage_shaders.find(name_shader);
    if (it_sh == m_storage_shaders.end()) {
      AV_LOG_ERROR("ShaderStorage::ExecuteAfterUse: " + name_shader + " isn't find");
      assert(false);
    }
    
    auto& executor = it_sh->second;
    executor->ExecuteAfterUse();
  }

  void ShaderStorage::RegisterShader(const std::string& name_shader, const Path& vertex, const Path& fragment)
  {
    auto it_sh = m_storage_shaders.find(name_shader);
    if (it_sh == m_storage_shaders.end()) 
    {
      auto vertex_code = core::filesystem::FileReader::ReadFile(vertex);
      auto fragment_code = core::filesystem::FileReader::ReadFile(fragment);
      if (vertex_code.empty() || fragment_code.empty())
      {
        AV_LOG_ERROR("ShaderStorage::RegisterShader: name shader " + name_shader + " is not register! Vertex or fragment code empty!");
        // TODO: Add result operation 
        return;
      }

      m_storage_shaders.emplace(name_shader, std::make_unique<ShaderExecutor>(vertex_code, fragment_code));
      AV_LOG_INFO("Shader " + name_shader + " is successfully register");
    }
  }

  void ShaderStorage::UnRegisterShader()
  {
    AV_LOG_TODO("ShaderStorage::UnRegisterShader isn't implementation");
  }

  void ShaderStorage::UseShader(const std::string& name_shader) 
  {
    auto it_sh = m_storage_shaders.find(name_shader);
    if (it_sh == m_storage_shaders.end()) {
      AV_LOG_ERROR("Shader " + name_shader + " isn't find");
      assert(false);
    }
    
    auto& executor = it_sh->second;
    executor->Execute();
  }

  const ShaderStorage::Storage& ShaderStorage::GetStorage() const noexcept
  {
    return m_storage_shaders;
  }

  auto ShaderStorage::ReloadShader(const std::string& name, const std::filesystem::path& path) noexcept -> void
  {
    auto it = m_storage_shaders.find(name);
    if (it == m_storage_shaders.end())
    {
      AV_LOG_ERROR(std::format("Shader {} isn't register to storage", name));
      return;
    }

    // TODO: think about making a dependency on the file system from here
    auto& shader_ptr = it->second;
    if (path.extension() == ".vert")
    {
      std::string filename(name);
      filename.append(".frag");
      std::filesystem::path frag = path.parent_path() / filename;
      auto frag_code = core::filesystem::FileReader::ReadFile(frag);
      auto vertex_code = core::filesystem::FileReader::ReadFile(path);
      shader_ptr->Reload(vertex_code, frag_code);
    } else if (path.extension() == ".frag")
    {
      std::string filename(name);
      filename.append(".vert");
      std::filesystem::path vert = path.parent_path() / filename;
      auto vertex_code = core::filesystem::FileReader::ReadFile(vert);
      auto frag_code = core::filesystem::FileReader::ReadFile(path);
      shader_ptr->Reload(vertex_code, frag_code);
    }
    m_shaders_reloaded.push(name);
  }

  auto ShaderStorage::GetShaderExecutorByName(const std::string& name) noexcept -> std::unique_ptr<ShaderExecutor>&
  {
    auto it_sh = m_storage_shaders.find(name);
    if (it_sh == m_storage_shaders.end()) {
      AV_LOG_ERROR("ShaderStorage::GetShaderExecutorByName(): " + name + " isn't registered");
      assert(false);
    }
    
    auto& executor = it_sh->second;
    return executor;
  }

} // namespace avion::gfx
