
#ifndef AVION_CORE_RESOURCE_MANAGER_H
#define AVION_CORE_RESOURCE_MANAGER_H
  
  #include <filesystem>
  #include <string_view>
  #include <string>
  #include <unordered_map>
  #include <type_traits>
  #include <vector>

  #include "../macro.h"

  namespace avion::core
  {
    class Texture;
  }

  namespace avion::core::resman 
  {
    
    // class Path {
    // public:
    //   Path() = delete;

    //   Path(const char* relative_path);
    //   Path(const std::string& relative_path);
    //   Path(std::string_view relative_path);
      
    //   std::string GetAbsolutePath() const; 

    //   ~Path() = default;
    // private:
    //   std::filesystem::path m_path; 
    // };

    enum class ResourceType {
      kUnknown = -1, 
      kTexture =  1,
      kShader  =  2,
      kModel   =  3,
      kConfig =  4,
    };

    class ResourceManager {
    public:
      using FsPath      = std::filesystem::path;
      using ListTexture = std::vector<std::string>;
      using ModelList   = std::vector<std::string>;
 
      ResourceManager() = delete;
      ResourceManager(std::string_view path);
      ~ResourceManager() = default;

      bool      RegisterResource(ResourceType resource, std::string_view path_to_resource); 
      Texture*  RegisterTexture(std::string_view path_to_resource);
      
      template <typename T>
      T* GetResource(std::string_view resource) const;

      const ListTexture& GetListTexture() const noexcept;
      const ModelList&   GetModelLoadedList() const noexcept;

    private:
      std::optional<Texture*> CreateAndLoadTexture(const std::string& filename, FsPath& path);
      void LoadShader(const std::string& path, bool result);
      void LoadConfig(const std::string& path, bool result);
      void LoadModel(const std::string& path, bool result);
      bool LoadTextFile(const std::string& filename, FsPath& path, ResourceType type);

      std::string NormalizePath(std::string_view path) const; 

    private:
      struct IResourceHolder {
        virtual ~IResourceHolder() = default;
      };

      template <typename T>
      struct ResourceHolder : IResourceHolder {
        T data;

        template <typename... Args>
        ResourceHolder(Args&&... args): data(std::forward<Args>(args)...) {}
      };

      using ResourceStorage = std::unordered_map<std::string, std::unique_ptr<IResourceHolder>>;

      ResourceStorage m_resources;
      ListTexture     m_texture_loaded_list;
      ModelList       m_model_loaded_list;
      FsPath          m_path_exe;
    };

    template<typename T>
    T* ResourceManager::GetResource(std::string_view resource) const 
    {
      if (resource.empty()) {
        std::string err(resource);
        AV_LOG_ERROR("ResourceManager::GetResource: key of resource is empty");
        return nullptr;
      }

      auto it_res = m_resources.find(resource.data());
      if (it_res == m_resources.end()) {
        std::string err(resource);
        AV_LOG_ERROR("ResourceManager::GetResource: " + err + " is invalid resource.");
        return nullptr;
      }

      ResourceHolder<T>* holder_observer = static_cast<ResourceHolder<T>*>(it_res->second.get());
      return &holder_observer->data;
    }
  } // namespace avion::core::resman

#endif
