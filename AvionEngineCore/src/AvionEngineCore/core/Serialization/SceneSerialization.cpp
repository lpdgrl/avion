#include "AvionEngineCore/core/Serialization/SceneSerialization.hpp"

namespace avion::core::serialization
{
  auto SceneSerialization::Save(const detail::SceneSerialize& scene) -> void
  {
    std::string json;
    // TODO: Check error by when write json
    auto ec = glz::write_json(scene, json);
    AV_LOG_SCDEBUG(std::format("SceneSerialization::Save: {}", json));
    filesystem::FileReader::WriteFile("scene.json", json);
  }

  auto SceneSerialization::Load(const std::string& filename) -> detail::SceneSerialize
  {
    // TODO: Is temp way for testing 
    std::filesystem::path path("/home/lpdgrl/Project/code/avion/assets/scenes");
    path /= filename;

    detail::SceneSerialize scene;
    auto json = filesystem::FileReader::ReadFile(path);
    AV_LOG_SCDEBUG(std::format("Load json file: {}", json));
    auto ec = glz::read_json(scene, json);

    return scene;
  }
} // namespace avion::core::serialization