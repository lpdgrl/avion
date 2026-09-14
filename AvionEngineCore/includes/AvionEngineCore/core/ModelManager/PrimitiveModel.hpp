#ifndef AVION_CORE_MODEL_MANAGER_PRIMITIVE_MODEL_H 
#define AVION_CORE_MODEL_MANAGER_PRIMITIVE_MODEL_H 1

  #include <vector>
  #include <cstdint>
  #include <string>

  #include "AvionEngineCore/core/ModelManager/ModelData.hpp"

  namespace avion::core::modelmanager::detail
  {

    class PrimitiveModel
    {
      public:
        using PrimitiveType = detail::PrimitiveType;
        using CpuModelData     = detail::CpuModelData;
        using Vertex   = detail::Vertex;


        static CpuModelData Make(PrimitiveType type) noexcept;
      protected:

      private:
        static CpuModelData Make(PrimitiveType type, const std::vector<float> raw_vertices, std::size_t num_vertex, std::size_t num_indices) noexcept;
        static CpuModelData MakeCube() noexcept;
        static CpuModelData MakePyramid() noexcept;      
        static CpuModelData MakePlane() noexcept;
    };

    std::string PrimitiveTypeToString(PrimitiveType) noexcept;

  } // namespace avion::core::modelmanager::detail

#endif

