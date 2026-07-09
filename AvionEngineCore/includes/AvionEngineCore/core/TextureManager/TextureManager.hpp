#ifndef AVION_CORE_TEXTURE_MANAGER_H
#define AVION_CORE_TEXTURE_MANAGER_H 1

  #include <memory>
  #include <functional>
  #include <unordered_map>
  #include <string>
  #include <optional>

  #include "AvionEngineCore/core/Assimp/AssimpModelLoader.hpp"
  #include "AvionEngineCore/core/resource_manager.hpp"
  #include "AvionEngineCore/core/texture.hpp"
  #include "AvionEngineCore/core/TextureManager/TextureHandler.hpp"

  namespace avion::core::texturemanager
  {
    
    class TextureManager
    {
      public:
        using FileName      = std::string;
        using Texture       = core::Texture;
        using FsPath        = resman::ResourceManager::FsPath;
        using AssimpModelLoader = assimp::AssimpModelLoader;
        using TextureHandler = detail::TextureHandler;
        using LoadTextureResult = std::optional<TextureHandler>;
        using TextureData = detail::TextureData;

        struct TextureItem
        {
          using TextureId = std::uint32_t;

          TextureId id;
          Texture* item = nullptr;
        };

        using TextureStorage = std::unordered_map<FileName, TextureItem>;
        
        using ResmanCallback  = std::function<std::optional<Texture*>(const FsPath& path)>;  
        using BackendCallback = std::function<TextureHandler(const TextureData& data)>;

        TextureManager() = default;

        TextureManager(const TextureManager& other) = delete;
        TextureManager(TextureManager&& other) = delete;
        
        TextureManager& operator=(const TextureManager& rhs) = delete;
        TextureManager& operator=(TextureManager&& rhs) = delete;

        ~TextureManager() = default;

        LoadTextureResult Load(const FsPath& filename);
        bool Remove(const std::string& filename) noexcept;
        
        bool Contains(const std::string& filename) const noexcept;
        std::optional<TextureItem> Get(const std::string& filename) noexcept;

        void SetResmanCallback(ResmanCallback callback);
        void SetBackendCallback(BackendCallback callback);
      protected:

      private:
        ResmanCallback m_cb_resman;
        BackendCallback m_cb_backend;
        TextureStorage m_texture_storage;
    };
  } // namespace avion::core::texturemanager

#endif 