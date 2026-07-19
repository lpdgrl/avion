#include "AvionEngineCore/core/TextureManager/TextureManager.hpp"

namespace avion::core::texturemanager
{
  TextureManager::LoadTextureResult TextureManager::Load(const FsPath& filename)
  {
    LoadTextureResult result;

    AV_LOG_DEBUG("TextureManager::Load: " + std::string(filename.filename().c_str()));

    if (auto texture = Get(filename.filename()); texture.has_value())
    {
      result = TextureHandler({}, texture.value().id);
      return result;
    }

    auto texture = m_cb_resman(filename);
    if (!texture.has_value())
    {
      AV_LOG_ERROR("TextureManager::Load: texture isn't loading");
      return std::nullopt;
    }
    auto* p_texture = texture.value();

    TextureData data {
      .color_channels = p_texture->GetColorChannels(),
      .width = p_texture->GetWidth(),
      .height = p_texture->GetHeight(),
      .buffer_size = p_texture->GetSize(),
      .buffer = p_texture->GetBuffer()
    };

    result = m_cb_backend(data);
    assert(result.has_value() && "TextureManager::Load: backend isn't created opengl texture");
    p_texture->SetUploadOpenGL();

    m_texture_storage.try_emplace(filename.filename().c_str(), result.value().id, p_texture);

    return result;
  }

  void TextureManager::SetResmanCallback(ResmanCallback callback)
  {
    m_cb_resman = callback;
  }
  
  void TextureManager::SetBackendCallback(BackendCallback callback)
  {
    m_cb_backend = callback;
  }

  bool TextureManager::Contains(const std::string& filename) const noexcept
  {
    auto it = m_texture_storage.find(filename);
    return (it != m_texture_storage.cend() ? true : false);
  }

  std::optional<TextureManager::TextureItem> TextureManager::Get(const std::string& filename) noexcept
  {
    std::optional<TextureManager::TextureItem> result;
    auto it = m_texture_storage.find(filename);

    if (it == m_texture_storage.end())
    {
      return std::nullopt;
    }

    result = it->second;
    return result;
  }

} // namespace avion::core::texturemanager