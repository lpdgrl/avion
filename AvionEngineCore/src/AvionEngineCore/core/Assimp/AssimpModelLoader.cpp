#include "AvionEngineCore/core/Assimp/AssimpModelLoader.hpp"
#include "AvionEngineCore/core/Assimp/AssimpModelLoaderImpl.hpp"

namespace avion::core::assimp
{
  AssimpModelLoader::LoadModelResult AssimpModelLoader::Load(const std::string& path)
  {
    using namespace detail;

    LoadModelResult load_model_result;
    auto assimp_loader = MakeAssimpModelLoader();
    load_model_result = assimp_loader->Load(path);

    return load_model_result;
  }
} // namespace avion::core::assimp