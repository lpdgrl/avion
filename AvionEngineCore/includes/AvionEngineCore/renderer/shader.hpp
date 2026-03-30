#pragma once

#include "glad/glad.h"
#include "../../glm/gtc/type_ptr.hpp"
#include "../macro.h"

#include <string>
#include <vector>
#include <variant>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <type_traits>

namespace avion::gfx {
  static constexpr std::size_t kSizeShaderData = 32;

  using ParamType = std::variant<glm::mat4, glm::vec4, glm::vec3, float, int, bool>;

  enum class ShaderType {
    kUnknown  = -1,
    kVertex   =  1,
    kFragment =  2,
  };

  struct ShaderParam {
    std::string name;
    ParamType value;
  };
  
  class Shader {
  public:
      using ShaderProgram = std::uint32_t;
      // constructor reads and builds the shader
      Shader(const char* vertexPath, const char* fragmentPath);

      ~Shader();
      
      // use/activate the shader
      void Use();
      
      void SetValue(const std::string& name, ParamType value) const;

      ShaderProgram GetID() const noexcept;
      ShaderProgram GetID() noexcept;
  
  private:
    void SetBool (const std::string &name, bool value) const;
    void SetInt (const std::string &name, int value) const;
    void SetFloat (const std::string &name, float value) const;
    void SetMat4 (const std::string &name, glm::mat4& value) const;
    void SetVec3 (const std::string &name, glm::vec3& value) const;
    void SetVec4 (const std::string &name, glm::vec4& value) const;  

  private:
    ShaderProgram m_id_prog;
  };

  class ShaderExecutor {
  public:
   //  ShaderExecutor();

    explicit ShaderExecutor(const std::string& vertex, const std::string& fragment);
   
    void Execute();
    
    template <typename T>
    void PutData(const std::string name_param, T data);

    ~ShaderExecutor() = default; 
  private:
    std::vector<ShaderParam> m_data;
    std::unique_ptr<Shader> m_shader;
    bool m_is_changed_data = false;
  };

  template <typename T>
  void ShaderExecutor::PutData(const std::string name_param, T data) 
  {
    m_data.emplace_back(std::move(name_param), data); 
  }

  class ShaderStorage {
  public:
    using Storage = std::unordered_map<std::string, std::unique_ptr<ShaderExecutor>>;

    ShaderStorage() = default;
    
    void RegisterShader(const std::string& name_shader, std::string vertex, std::string fragment); 
    void UnRegisterShader();
    
    void UseShader(const std::string& key);
    
    template <typename  T>
    void PutData(const std::string name_sahder, const std::string& name_param, T data);

    const Storage& GetStorage() const noexcept;

    ~ShaderStorage() = default;
  private:
    Storage m_storage_shaders;
  };

  template <typename T>
  void ShaderStorage::PutData(const std::string name_shader, const std::string& name_param, T data)
  {
    if (name_shader.empty()) {
      AV_LOG_ERROR("ShaderStorage::PutData name_shader is empty.");
      assert(!name_shader.empty());
    }
    
    AV_LOG_DEBUG("ShaderStorage::PutData " + name_shader + " " + name_param);
    auto it_sh = m_storage_shaders.find(name_shader);
    if (it_sh == m_storage_shaders.end()) {
      AV_LOG_ERROR("Shader " + name_shader + " isn't register");
      assert(false);
    }
    
    auto& executor = it_sh->second;
    executor->PutData(name_param, data);
  }

} // namespace avion::gfx
