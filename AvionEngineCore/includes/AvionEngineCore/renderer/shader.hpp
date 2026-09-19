#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <variant>
#include <memory>
#include <unordered_map>
#include <type_traits>
#include <queue>

#include "glad/glad.h"
#include "../../glm/gtc/type_ptr.hpp"

#include "AvionEngineCore/api/opengl/types/GLShader.hpp"
#include "AvionEngineCore/api/opengl/types/GLShaderProgram.hpp"

#include "AvionEngineCore/core/FileSystem/FileReader.hpp"

#include "AvionEngineCore/macro.h"

namespace avion::gfx {
  static constexpr std::size_t kSizeShaderData = 32;

  using ParamType = std::variant<glm::mat4, glm::vec4, glm::vec3, float, int, bool>;

  struct ShaderParam {
    std::string name;
    ParamType value;
  };
  
  class Shader {
  public:
      using ShaderProgram = std::uint32_t;

      // constructor reads and builds the shader
      Shader() = delete;
      Shader(std::string_view vertex_code, std::string_view fragment_code);

      ~Shader();
      
      // use/activate the shader
      void Use();
      
      void SetValue(const std::string& name, ParamType value) const;

      auto Reload(std::string_view vertex_code, std::string_view fragment_code) -> void;

      ShaderProgram GetID() const noexcept;
      ShaderProgram GetID() noexcept;

      auto HasReloaded() const noexcept -> bool;
      auto IsCompleted() const noexcept -> bool;

      auto UpdateStateAfterReloaded() noexcept -> void;
  
  private:
    void SetBool (const std::string &name, bool value) const;
    void SetInt (const std::string &name, int value) const;
    void SetFloat (const std::string &name, float value) const;
    void SetMat4 (const std::string &name, glm::mat4& value) const;
    void SetVec3 (const std::string &name, glm::vec3& value) const;
    void SetVec4 (const std::string &name, glm::vec4& value) const;  

  private:
    api::backend::opengl::GLShaderProgram m_program;
    bool m_reloaded{};
    bool m_is_completed{};
  };

  class ShaderExecutor {
  public:
   //  ShaderExecutor();

    explicit ShaderExecutor(std::string_view vertex_code, std::string_view fragment_code);
   
    void Execute();
    void ExecuteAfterUse();
    
    template <typename T>
    void PutData(const std::string& name_param, T data);

    auto Reload(std::string_view vertex_code, std::string_view fragment_code) -> void;

    ~ShaderExecutor() = default; 
  private:
    std::vector<ShaderParam> m_data;
    std::unique_ptr<Shader> m_shader;
    bool m_is_changed_data = false;
  };

  template <typename T>
  void ShaderExecutor::PutData(const std::string& name_param, T data) 
  {
    m_data.emplace_back(std::move(name_param), data); 
  }

  class ShaderStorage {
  public:
    using Path    = std::filesystem::path;
    using Storage = std::unordered_map<std::string, std::unique_ptr<ShaderExecutor>>;

    ShaderStorage() = default;
    
    void RegisterShader(const std::string& name_shader, const Path& vertex, const Path& fragment); 
    void UnRegisterShader();
    
    void UseShader(const std::string& key);
    void ExecuteAfterUse(const std::string& name_shader);
    
    template <typename T>
    void PutData(const std::string& name_sahder, const std::string& name_param, T data);

    const Storage& GetStorage() const noexcept;
    auto ReloadShader(const std::string& name, const std::filesystem::path& path) noexcept -> void;
    
    template <typename T>
    auto UpdateStateShadersReloaded(const std::string& param, T data) noexcept -> void;

    ~ShaderStorage() = default;

  private:
    auto GetShaderExecutorByName(const std::string& name) noexcept -> std::unique_ptr<ShaderExecutor>&;
  private:
    Storage m_storage_shaders;
    std::queue<std::string> m_shaders_reloaded;
  };

  template <typename T>
  void ShaderStorage::PutData(const std::string& name_shader, const std::string& name_param, T data)
  {
    if (name_shader.empty()) {
      AV_LOG_ERROR("ShaderStorage::PutData name_shader is empty.");
      assert(!name_shader.empty());
    }
    
    // AV_LOG_DEBUG("ShaderStorage::PutData " + name_shader + " " + name_param);
    auto it_sh = m_storage_shaders.find(name_shader);
    if (it_sh == m_storage_shaders.end()) {
      AV_LOG_ERROR("Shader " + name_shader + " isn't register");
      assert(false);
    }
    
    auto& executor = it_sh->second;
    executor->PutData(name_param, data);
  }

  template <typename T>
  auto ShaderStorage::UpdateStateShadersReloaded(const std::string& param, T data) noexcept -> void
  {
    while (!m_shaders_reloaded.empty())
    {
      auto& shader_name = m_shaders_reloaded.back();
      auto& executor = GetShaderExecutorByName(shader_name);
      executor->PutData(param, data);
      m_shaders_reloaded.pop();
    }
  }


} // namespace avion::gfx
