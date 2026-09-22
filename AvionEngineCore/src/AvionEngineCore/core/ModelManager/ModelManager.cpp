#include "AvionEngineCore/core/ModelManager/ModelManager.hpp"

namespace avion::core::modelmanager
{
  ModelManager::LoadModelResult ModelManager::Load(const std::string& filename)
  {
    LoadModelResult model_load_result;
    // TODO:
    if (Contains(filename))
    {
      AV_LOG_INFO("ModelManager::Load: model " + filename + " already loaded");
      model_load_result = CreateModelCopy(filename);
      return model_load_result;
    }

    auto* p_fs_path = m_cb_resman(filename);
    if (!p_fs_path)
    {
      AV_LOG_ERROR("ModelManager::Load: model isn't load to resman!");
      return std::nullopt;
    }

    std::string extension_file;
    std::string name_file;
    if (p_fs_path->has_extension())
    {
      extension_file = p_fs_path->extension().c_str();
      name_file = detail::GetFileName(p_fs_path->filename().c_str());
    }
    else
    {
      name_file = filename;
    }

    auto assimp_result = AssimpModelLoader::Load(p_fs_path->c_str());
    if (!assimp_result.has_value())
    {
      AV_LOG_ERROR("ModelManager::Load: model " + filename + " isn't loading!");
      return std::nullopt;
    }

    auto& model_data = assimp_result.value();
    // Create Buffer in GPU
    auto gpu_buffer_handle = m_cb_backend(model_data);

    // loading and create opengl texture for model
    Material material;
    
    material.type = (model_data.texture_source.empty()) ? MaterialType::kRegular : MaterialType::kTexture;
    for (const auto& texture : model_data.texture_source)
    {
      auto path = p_fs_path->parent_path() / texture.path;
      auto texture_handler = m_cb_texture(path);
      switch (texture.type)
      {
        case TextureType::kDiffuse:
        {
          material.diffuse_texture.emplace_back(texture.type, texture_handler.value().id);
          break;
        }
        case TextureType::kSpecular:
        {
          material.specular_texture.emplace_back(texture.type, texture_handler.value().id);
          break;
        }
      }
    }

    // TODO: Model data moving to model
    std::uint32_t id{};
    if (m_storage.empty())
    {
      id = 0U;
    }
    else 
    {
      id = m_storage.back().id;
      ++id;
    }

    auto& result = m_storage.emplace_back(std::make_shared<Model>(filename, model_data, material, 
      model_data.has_animation, extension_file), filename, id, gpu_buffer_handle.id);
    // Added to cache
    m_cache.try_emplace(filename, result);
    // auto [it_handle, _] = m_handle_storage.emplace(filename, model_handler);
    model_load_result = ModelItem
    {
      .model = result.model,
      .name = result.name,
      .id = result.id,
      .gpu_buffer_handle = result.gpu_buffer_handle
    };

    AV_LOG_INFO("ModelManager::Load: model " + filename + " is loading success");
    return model_load_result;
  }

  ModelManager::LoadModelResult ModelManager::Load(PrimitiveType type) noexcept
  {
    LoadModelResult model_load_result;
    std::string filename(detail::PrimitiveTypeToString(type));

    // TODO:
    if (Contains(filename))
    {
      AV_LOG_INFO("ModelManager::Load(PrimitiveType type): model " + filename + " already loaded");
      model_load_result = std::move(CreateModelCopy(filename));
      return model_load_result;
    }

    auto model_data = detail::PrimitiveModel::Make(type);
    auto gpu_buffer_handle = m_cb_backend(model_data);

    Material material;
    material.type  = MaterialType::kRegular;
    material.color = glm::vec3(0.5f, 0.5f, 0.5f);

    // auto [it_handle, _] = m_handle_storage.emplace(filename, model_handler);
    std::uint32_t id{};
    if (m_storage.empty())
    {
      id = 0U;
    }
    else 
    {
      id = m_storage.back().id;
      ++id;
    }


    auto& result = m_storage.emplace_back(
      std::make_shared<Model>(filename, model_data, material, false),
      filename,
      id,
      gpu_buffer_handle.id
    );

    // model_load_result = ModelItem{.model_handler = model_handler, .model = it->second};
    model_load_result = result;
    return model_load_result;
  }

  ModelManager::LoadModelResult ModelManager::Load(const std::string& filename_sprite, ModelManager::PrimitiveType type) noexcept
  {
    LoadModelResult model_load_result;
    // TODO:
    if (Contains(filename_sprite))
    {
      AV_LOG_INFO("ModelManager::Load: sprite " + filename_sprite + " already loaded");
      model_load_result = CreateModelCopy(filename_sprite);
      return model_load_result;
    }

    auto* p_fs_path = m_cb_resman(filename_sprite);
    if (!p_fs_path)
    {
      AV_LOG_ERROR("ModelManager::Load: sprite isn't load to resman!");
      return std::nullopt;
    }

    auto model_data = detail::PrimitiveModel::Make(type);
    auto gpu_buffer_handle = m_cb_backend(model_data);
    // auto [it_handle, _] = m_handle_storage.emplace(filename_sprite, model_handler);

    // loading and create opengl texture for model
    Material material;
    material.type = MaterialType::kTexture;
    auto texture_handler = m_cb_texture(p_fs_path->c_str());
    material.diffuse_texture.emplace_back(TextureType::kDiffuse, texture_handler.value().id);

    // TODO: Model data moving to model
    std::uint32_t id{};
    if (m_storage.empty())
    {
      id = 0U;
    }
    else 
    {
      id = m_storage.back().id;
      ++id;
    }

    auto& result = m_storage.emplace_back(
      std::make_shared<Model>(filename_sprite, model_data, material, false),
      filename_sprite,
      id, 
      gpu_buffer_handle.id
    );

    // model_load_result = ModelItem{.model_handler = model_handler, .model = it->second};
    model_load_result = result;
    AV_LOG_INFO("ModelManager::Load: sprite " + filename_sprite + " is loading success");
    return model_load_result;
  }

  ModelManager::LoadModelResult ModelManager::CreateModelCopy(const std::string& filename) noexcept
  {
    LoadModelResult load_model_result;

    auto *model = Get(filename);

    auto opt_model_handle = GetModelHandle(filename);
    if (!opt_model_handle.has_value())
    {
      AV_LOG_ERROR("ModelManager::Load: model " + filename + " isn't has model handle!");
      return std::nullopt;
    }
    auto gpu_buffer_handle = opt_model_handle.value();

    std::uint32_t id{};
    if (m_storage.empty())
    {
      id = 0U;
    }
    else 
    {
      id = m_storage.back().id;
      ++id;
    }

    auto& result = m_storage.emplace_back(
      std::make_shared<Model>(filename, *model),
      filename, 
      id, 
      gpu_buffer_handle.id
    );

    // auto model_handle = opt_model_handle.value();
    // result = ModelItem{.model_handler = model_handle, .m\odel = it->second};
    load_model_result = result;
    return load_model_result;
  }

  bool ModelManager::Remove(const std::string& filename) noexcept
  {
    AV_LOG_INFO("ModelManager::Remove: NOTHING!");
    return false;
  }

  bool ModelManager::Contains(const std::string& filename) const noexcept
  {
    if ([[maybe_unused]] auto p_model = Find(filename); p_model != nullptr)
    {
      return true;
    }
    AV_LOG_INFO("ModelManager::Contains: manager isn't storing model " + filename);
    return false;
  }

  void ModelManager::SetResmanCallback(ResmanCallback callback)
  {
    m_cb_resman = callback;
  }

  void ModelManager::SetBackendCallback(BackendCallback callback)
  {
    m_cb_backend = callback;
  }

  std::optional<ModelManager::ModelHandler> ModelManager::GetModelHandle(const std::string& filename) noexcept
  {
    std::optional<ModelHandler> result;
    auto it = m_cache.find(filename);

    if (it == m_cache.end())
    {
      return std::nullopt;
    }

    result = {it->second.gpu_buffer_handle};
    return result;
  }

} // namespace avion::core::modelmanager
