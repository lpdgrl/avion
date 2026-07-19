#ifndef AVION_CORE_TEXTURE_MANAGER_TEXTURE_HANDLER_H 
#define AVION_CORE_TEXTURE_MANAGER_TEXTURE_HANDLER_H 1

  #include <vector>
  #include <filesystem>
  #include <cstdint>

  #include "glm/glm.hpp"

  #include "AvionEngineCore/core/texture.hpp"

  namespace avion::core::texturemanager::detail
  {
    enum class TextureType
    {
      kUnknown = 0,
      kDiffuse, 
      kSpecular,
      kEmission,
      kNormal,
      kSolidColor
    };

    struct TextureHandler
    {
      TextureType type;
      std::uint32_t id;
    }; 

    struct TextureData
    {
      std::uint32_t color_channels = 0;
      std::uint32_t width = 0;
      std::uint32_t height = 0;
      std::uint64_t buffer_size = 0;
      std::uint8_t* buffer = nullptr;
    };
  }

#endif 