#ifndef AVION_CORE_MODEL_MANAGER_H
#define AVION_CORE_MODEL_MANAGER_H 1

  #include <memory>
  #include <functional>
  #include <unordered_map>
  #include <string>
  #include <optional>

  #include "AvionEngineCore/core/Assimp/AssimpModelLoader.hpp"
  #include "AvionEngineCore/core/resource_manager.hpp"
  #include "AvionEngineCore/renderer/model.hpp"
  #include "AvionEngineCore/core/TextureManager/TextureHandler.hpp"
  #include "AvionEngineCore/core/ModelManager/PrimitiveModel.hpp"

  namespace avion::core::modelmanager
  {
    class ModelManager
    {
      public:
        struct ModelItem;

        using FileName                = std::string;
        using Model                   = gfx::Model;
        using ModelPtr                = std::shared_ptr<Model>;
        using FsPath                  = resman::ResourceManager::FsPath;
        using AssimpModelLoader       = assimp::AssimpModelLoader;
        using ModelData               = detail::ModelData;
        using ModelHandler            = detail::ModelHandler;
        using Material                = core::material::Material;
        using MaterialType            = core::material::MaterialType;
        using TextureHandler          = texturemanager::detail::TextureHandler;
        using TextureType             = texturemanager::detail::TextureType;
        using PrimitiveType           = detail::PrimitiveType;
        using ModelHandleStorage      = std::unordered_map<FileName, ModelHandler>;
        using ModelStorage            = std::unordered_map<FileName, ModelPtr>;
        using ResmanCallback          = std::function<FsPath*(std::string_view filename_model)>;  
        using TextureManagerCallback  = std::function<std::optional<TextureHandler>(const FsPath& path)>;
        using BackendCallback         = std::function<ModelHandler(ModelData& model_data)>;
        using LoadModelResult         = std::optional<ModelItem>;

        struct ModelItem
        {
          ModelHandler model_handler;
          ModelPtr model;
        };

        ModelManager() = default;

        ModelManager(const ModelManager& other) = delete;
        ModelManager(ModelManager&& other) = delete;
        
        ModelManager& operator=(const ModelManager& rhs) = delete;
        ModelManager& operator=(ModelManager&& rhs) = delete;

        ~ModelManager() = default;

        LoadModelResult Load(const std::string& filename);
        LoadModelResult Load(PrimitiveType type) noexcept;
        // TODO: It's TMP! Create sprite over model manager!!
        LoadModelResult Load(const std::string& filename_sprite, PrimitiveType type) noexcept;

        bool Remove(const std::string& filename) noexcept;

        template <typename Self>
        decltype(auto) Get(this Self& self, const std::string& filename) noexcept;

        std::optional<ModelHandler> GetModelHandle(const std::string& filename) noexcept;
        
        bool Contains(const std::string& filename) const noexcept;

        void SetResmanCallback(ResmanCallback callback);
        void SetBackendCallback(BackendCallback callback);
        void SetTextureManagerCallback(TextureManagerCallback callback) { m_cb_texture = callback; }
        
      protected:

      private:
        template <typename Self>
        decltype(auto) Find(this Self& self, const std::string& filename) noexcept;

        LoadModelResult CreateModelCopy(const std::string& filename) noexcept;

      private:
        ResmanCallback          m_cb_resman;
        BackendCallback         m_cb_backend;
        TextureManagerCallback  m_cb_texture;
        ModelStorage            m_storage;
        ModelHandleStorage      m_handle_storage;
        std::uint32_t           m_number_copy_models{};
    };

    template <typename Self>
    decltype(auto) ModelManager::Get(this Self& self, const std::string& filename) noexcept
    {
       return self.Find(filename);
    }

    template <typename Self>
    decltype(auto) ModelManager::Find(this Self& self, const std::string& filename) noexcept
    {
      using ReturnType = std::conditional_t<
        std::is_const_v<std::remove_reference_t<Self>>,
        const Model*,
        Model*
      >;
      
      if (auto it_model = self.m_storage.find(filename); it_model != self.m_storage.end())
      {
        return static_cast<ReturnType>(it_model->second.get());
      }
      return ReturnType{nullptr};
    }
  } // namespace avion::core::modelmanager

#endif 