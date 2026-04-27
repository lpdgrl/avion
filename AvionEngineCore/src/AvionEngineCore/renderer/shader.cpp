#include "AvionEngineCore/renderer/shader.hpp"
#include <fstream>
#include <sstream>

namespace avion::gfx {

  // constructor reads and builds the shader
  Shader::Shader (const char* vertexPath, const char* fragmentPath)
  {
      
      // 1. retrieve the vertex/fragment source code from filePath
      std::string vertexCode;
      std::string fragmentCode;
      std::ifstream vShaderFile;
      std::ifstream fShaderFile;

      // ensure ifstream objects can throw exceptions:
      vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
      fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
      try
      {
          // open files
          vShaderFile.open(vertexPath);
          fShaderFile.open(fragmentPath);
          std::stringstream vShaderStream, fShaderStream;

          //read file's buffer contents into stream
          vShaderStream << vShaderFile.rdbuf();
          fShaderStream << fShaderFile.rdbuf();

          // close file handlers
          vShaderFile.close();
          fShaderFile.close();

          // convert stream into string
          vertexCode = vShaderStream.str();
          fragmentCode = fShaderStream.str();
      }
      catch(std::ifstream::failure& e) 
      {
          std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ " << vertexPath << " or " << fragmentPath << " " << e.what() << std::endl;
      }
      const char* vShaderCode = vertexCode.c_str();
      const char* fShaderCode = fragmentCode.c_str();
      
      // 2. compile shaders
      unsigned int vertex, fragment;
      int success;
      char infoLog[512];
      
      // vertex Shader
      
      vertex = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertex, 1, &vShaderCode, NULL);
      glCompileShader(vertex);
      
      // print compile error if any
      glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
      if (!success)
      {
          glGetShaderInfoLog(vertex, 512, NULL, infoLog);
          std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED " << vertexPath << infoLog << std::endl;
      }
      
      // fragment Shader
      fragment = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragment, 1, &fShaderCode, NULL);
      glCompileShader(fragment);

      // print compile error if any
      glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
      if (!success)
      {
          glGetShaderInfoLog(fragment, 512, NULL, infoLog);
          std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragmentPath << infoLog << std::endl;
      }

      // shader Program
      m_id_prog = glCreateProgram();
      glAttachShader(m_id_prog, vertex);
      glAttachShader(m_id_prog, fragment);
      glLinkProgram(m_id_prog);

      //print linking errors if any
      glGetProgramiv(m_id_prog, GL_LINK_STATUS, &success);
      if (!success) {
          glGetProgramInfoLog(m_id_prog, 512, NULL, infoLog);
          std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
      }
      
      // delete the shaders as they're linked into our now and no longer necessary
      glDeleteShader(vertex);
      glDeleteShader(fragment);
      
  }

  Shader::~Shader() {
    // TODO: Delete shader from GPU memory
    AV_LOG_DEBUG("Shader is destroyed");
  }

  void Shader::Use() 
  {
      glUseProgram(m_id_prog);
  }

  void Shader::SetBool(const std::string &name, bool value) const
  {
      glUniform1i(glGetUniformLocation(m_id_prog, name.c_str()), (int)value);
  }

  void Shader::SetInt(const std::string &name, int value) const
  {
      glUniform1i(glGetUniformLocation(m_id_prog, name.c_str()), value);
  }

  void Shader::SetFloat(const std::string &name, float value) const
  {
      glUniform1f(glGetUniformLocation(m_id_prog, name.c_str()), value);
  }
  void Shader::SetMat4(const std::string &name, glm::mat4& value) const {
      glUniformMatrix4fv(glGetUniformLocation(m_id_prog, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
  }

  void Shader::SetVec3(const std::string &name, glm::vec3& value) const {
      glUniform3fv(glGetUniformLocation(m_id_prog, name.c_str()), 1, &value[0]);
  }

  void Shader::SetVec4(const std::string& name, glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(m_id_prog, name.c_str()), 1, &value[0]);
  }

  Shader::ShaderProgram Shader::GetID() const noexcept {
      return m_id_prog;
  }

  Shader::ShaderProgram Shader::GetID() noexcept {
      return m_id_prog;
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


  /* 
  ShaderExecutor::ShaderExecutor() 
  {
    m_data.reserve(kSizeShaderData);
  }
  */

  ShaderExecutor::ShaderExecutor(const std::string& vertex, const std::string& fragment)
    : m_shader(std::make_unique<Shader>(vertex.c_str(), fragment.c_str()))
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

  void ShaderStorage::RegisterShader(const std::string& name_shader, std::string vertex, std::string fragment)
  {
    auto it_sh = m_storage_shaders.find(name_shader);
    if (it_sh == m_storage_shaders.end()) {
      m_storage_shaders.emplace(name_shader, std::make_unique<ShaderExecutor>(vertex, fragment));
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

} // namespace avion::gfx
