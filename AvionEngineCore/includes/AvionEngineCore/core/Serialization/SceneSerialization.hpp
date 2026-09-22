#ifndef AVION_CORE_SERIALIZATION_SCENE_SERIALIZATION_H
#define AVION_CORE_SERIALIZATION_SCENE_SERIALIZATION_H 1

#include <vector>
#include <string>
#include <array>
#include <cstdint>
#include <span>

#include "glaze/glaze.hpp"

#include "AvionEngineCore/core/Serialization/Detail/EntitySerialize.hpp"
#include "AvionEngineCore/core/FileSystem/FileReader.hpp"

#include "AvionEngineCore/macro.h"

namespace avion::core::serialization
{

  class SceneSerialization
  {
    public:
      static auto Save(std::vector<detail::EntitySerialize> entities) -> void; 
      static auto Load(const std::string& filename = "scene.json") -> std::vector<detail::EntitySerialize>;
    protected:

    private:
  };
} // namespace avion::core::serialization

namespace avion::core::serialization::detail
{

} // namespace avion::core::serialization::detail

#endif