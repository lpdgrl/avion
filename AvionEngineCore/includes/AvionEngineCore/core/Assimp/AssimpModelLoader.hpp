#ifndef AVION_CORE_ASSIMP_MODEL_LOADER_H
#define AVION_CORE_ASSIMP_MODEL_LOADER_H 1

  #include <string>
  #include <optional>

  #include "AvionEngineCore/macro.h"
  #include "AvionEngineCore/core/ModelManager/ModelData.hpp"

  namespace avion::core::assimp
  {
    class AssimpModelLoader
    {
      public:
      using CpuModelData    = core::modelmanager::detail::CpuModelData;
      using LoadModelResult = std::optional<CpuModelData>; 

      AssimpModelLoader() = delete;
      
      AssimpModelLoader(const AssimpModelLoader&) = delete;
      AssimpModelLoader(AssimpModelLoader&&) = delete;

      AssimpModelLoader& operator=(const AssimpModelLoader&) = delete;
      AssimpModelLoader& operator=(AssimpModelLoader&&) = delete;

      ~AssimpModelLoader() = delete;

      static LoadModelResult Load(const std::string& path);

      protected:

      private:
    };
  } // namespace avion::core::assimp

#endif