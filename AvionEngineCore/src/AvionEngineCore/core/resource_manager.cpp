#include "AvionEngineCore/core/resource_manager.hpp"
#include "AvionEngineCore/core/texture.hpp"

namespace avion::core::resman
{
  // Path::Path(const char* relative_path)
  // : m_path(relative_path)
  // {

  // }

  // Path::Path(const std::string& relative_path)
  // : m_path(relative_path)
  // {
    
  // }

  // Path::Path(std::string_view relative_path)
  // : m_path(relative_path)
  // {

  // }

  // std::string Path::GetAbsolutePath() const 
  // {
  //   return std::filesystem::weakly_canonical(m_path);
  // }


  ResourceManager::ResourceManager(std::string_view path)
  : m_path_exe(NormalizePath(path))
  {
    
  }

  const ResourceManager::ListTexture& ResourceManager::GetListTexture() const noexcept
  {
    return m_list_texture_loaded;
  }

  void ResourceManager::CreateAndLoadTexture(const std::string& filename, FsPath& path)
  {
    std::string path_str(path.c_str());
    
    auto it = m_resources.emplace(filename, std::make_unique<ResourceHolder<Texture>>(path_str)).first;
    m_list_texture_loaded.push_back(filename);

    ResourceHolder<Texture>* holder_observer = static_cast<ResourceHolder<Texture>*>(it->second.get());
    bool result = holder_observer->data.LoadTexture();

    std::string msg; 
    msg = (result) ? ("Texture is loading success " + path_str) : "Texture isn't loading success";
    AV_LOG_INFO("ResourceManager: " + msg);
  }

  void ResourceManager::LoadShader(const std::string& filename, FsPath& path) 
  {
    std::string path_str(path.c_str());

    auto [_, result] = m_resources.emplace(filename, std::make_unique<ResourceHolder<FsPath>>(path_str));
    
    std::string msg;
    msg = (result) ? ("Shader is finding  " + path_str) : "Shader isn't finding success";
    AV_LOG_INFO("ResourceManager: " + msg);
  }

  std::string ResourceManager::NormalizePath(std::string_view path) const
  {
    std::string msg("ResourceManager::NormalizePath: ");

    FsPath curr_path(path);
    std::string res(curr_path.parent_path().parent_path().parent_path());

    AV_LOG_DEBUG(msg + " " + res);
    return res;
  }

  bool ResourceManager::RegisterResource(ResourceType resource, std::string_view path_to_resource) 
  {
    FsPath path = m_path_exe / path_to_resource;

    // auto create_load_texture = [&](const std::string& filename, FsPath& path) {
      
    // };

    // TODO: Simple way to control resources    
    for (const auto& it_entry : std::filesystem::directory_iterator(path)) {
      std::string filename(it_entry.path().filename());
      FsPath path_canonical_resource(std::filesystem::canonical(it_entry));
      
      if (auto it_res = m_resources.find(filename); it_res != m_resources.end())
      {
        AV_LOG_INFO("ResourceManager::RegisterResource: resource " + filename + " is duplicate");
        continue;
      }
      // TODO: It is stupid every once checking type of resource
      switch(resource)
      {
        case ResourceType::kTexture: 
        {
          CreateAndLoadTexture(filename, path_canonical_resource);
          break;
        }
        case ResourceType::kShader:
        {
          LoadShader(filename, path_canonical_resource);
          break;
        }
        case ResourceType::kUnknown:
        {
          AV_LOG_DEBUG("ResourceManager::RegisterResource: ResourceType::kUnknown");
        }
      }
      
    }

    // TODO: Always return true that it is wrong
    return true;
  }
  
} // namespace avion::core::resman::fs