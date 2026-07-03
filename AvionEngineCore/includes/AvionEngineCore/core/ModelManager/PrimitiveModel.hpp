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
        using ModelData     = detail::ModelData;
        using Vertex   = detail::Vertex;


        static ModelData Make(PrimitiveType type) noexcept;
      protected:

      private:
        static ModelData Make(const std::vector<float> raw_vertices, std::size_t num_vertex, std::size_t num_indices) noexcept;
        static ModelData MakeCube() noexcept;
        static ModelData MakePyramid() noexcept;      
        static ModelData MakePlane() noexcept;
    };

    std::string PrimitiveTypeToString(PrimitiveType) noexcept;

  } // namespace avion::core::modelmanager::detail

#endif

