#include "AvionEngineCore/core/Serialization/SceneSerialization.hpp"

namespace avion::core::serialization
{
  auto SceneSerialization::Save(std::vector<detail::EntitySerialize> entities) -> void
  {
    std::string json;
    // TODO: Check error by when write json
    auto ec = glz::write_json(entities, json);
    AV_LOG_SCDEBUG(std::format("SceneSerialization::Save: {}", json));
    filesystem::FileReader::WriteFile("scene.json", json);
  }

  auto SceneSerialization::Load(const std::string& filename) -> std::vector<detail::EntitySerialize>
  {
    std::filesystem::path path("/home/lpdgrl/Project/code/avion/assets/scenes");
    path /= filename;

    std::vector<detail::EntitySerialize> result;
    auto json = filesystem::FileReader::ReadFile(path);
    AV_LOG_SCDEBUG(std::format("Load json file: {}", json));
    auto ec = glz::read_json(result, json);

    // detail::EntitySerialize entity;
    // bool first_read = true;
    // glz::error_ctx ec;
    // while (!(ec = glz::read_json(entity, json_sv)))
    // {
    //   std::size_t consumed = ec.count;
    //   first_read = false;
    //   json_sv = std::string_view(json).substr(consumed);
    //   result.push_back(entity);
    //   entity = detail::EntitySerialize();
    // }

    // if (first_read)
    // {
    //   result.push_back(entity);
    // }

    return result;
  }
} // namespace avion::core::serialization 