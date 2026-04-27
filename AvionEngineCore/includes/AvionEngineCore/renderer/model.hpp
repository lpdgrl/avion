#ifndef AVION_GFX_MODEL_H 
#define AVION_GFX_MODEL_H
  
  #include <memory>

  #include <assimp/Importer.hpp>
  #include <assimp/scene.h>
  #include <assimp/postprocess.h>

  #include "../macro.h"
  #include "mesh.hpp"

  namespace avion::core::resman
  {
    class ResourceManager;
  }

  namespace avion::gfx
  {
    class Model 
    {
      public:
        using ResManager = core::resman::ResourceManager;

        Model() = delete;
        Model(const std::string& path, const std::string& filename, ResManager& resman);

        bool LoadModel();

        std::vector<Mesh>& GetMeshs() noexcept;

        ~Model() = default;
      private:
        void                    ProcessNode(aiNode *node, const aiScene *scene);
        Mesh                    ProcessMesh(aiMesh *mesh, const aiScene *scene);
    
        std::vector<Texture_t>  LoadMaterialTextures(aiMaterial *mat, aiTextureType type);

        TextureType             ConvertAiTextureType(aiTextureType type) const noexcept;
     
      private:
        std::string                 m_filename;
        std::string                 m_path;
        std::vector<Mesh>           m_meshes;
        ResManager&                 m_resman;
    };
    
  } // namespace avion::gfx

#endif
