#ifndef AVION_CORE_MATERIAL_H 
#define AVION_CORE_MATERIAL_H

    #include "glm/glm.hpp"
    #include "texture.hpp"

    #include <vector>

    namespace avion::core {
        static constexpr std::uint8_t kNumberPrefabMaterials = 25;

        enum class MaterialType {
            kPrefabMaterial = 1,
            kTexture        = 2,
        };

        enum class PrefabMaterial : std::uint8_t {
            kNone           = 0,
            kEmerald        = 1,
            kGold           = 2,
            kJade           = 3,
            kObsidian       = 4,
            kPearl          = 5,
            kRuby           = 6,
            kTurquoise      = 7,
            kBrass          = 8,
            kBronze         = 9,
            kChrome         = 10,
            kCopper         = 11,
            kSilver         = 12,
            kBlackPlastic   = 13,
            kCyanPlastic    = 14,
            kGreenPlastic   = 15,
            kRedPlastic     = 16,
            kWhitePlastic   = 17,
            kYellowPlastic  = 18,
            kBlackRubber    = 19,
            kCyanRubber     = 10,
            kGreenRubber    = 21,
            kRedRubber      = 22,
            kWhiteRubber    = 23,
            kYellowRubber   = 24,
        };

        struct Material {
          bool is_texture = false;
          int texture_diffuse{};
          int texture_specular{};
          int texture_emission{};
          glm::vec3 ambient;
          glm::vec3 diffuse;
          glm::vec3 specular;
          float shininess;
      };

        static constexpr Material prefab_None {
          .ambient = glm::vec3(1.f),
          .diffuse = glm::vec3(1.f),
          .specular = glm::vec3(1.f),
          .shininess = 64
        };
        
        static constexpr Material prefab_Emerald {
            .ambient = glm::vec3(0.0215f, 0.1745, 0.0215),
            .diffuse = glm::vec3(0.07568f, 0.61424f, 0.07568f),
            .specular = glm::vec3(0.633f, 0.727811f, 0.633f),
            .shininess = 128 * 0.6 
        };

        static constexpr Material prefab_Gold {
            .ambient = glm::vec3(0.24725f, 0.1995f, 0.0745f),
            .diffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f),
            .specular = glm::vec3(0.628281f, 0.555802f, 0.366065f),
            .shininess = 128 * 0.4
        };

        static constexpr Material prefab_BlackPlastic {
            .ambient = glm::vec3(0.0f, 0.0f, 0.0f),
            .diffuse = glm::vec3(0.01f, 0.01f, 0.01f),
            .specular = glm::vec3(0.50f, 0.50f, 0.50f),
            .shininess = 128 * 0.25
        };

        static constexpr Material prefab_Jade {
            .ambient = glm::vec3(0.135f, 0.2225f, 0.1575f),
            .diffuse = glm::vec3(0.054f, 0.89f, 0.63f),
            .specular = glm::vec3(0.316228f, 0.316228f, 0.316228),
            .shininess = 128 * 0.1
        };

        static constexpr Material prefab_Obsidian {
            .ambient = glm::vec3(0.05375f, 0.05f, 0.06625f),
            .diffuse = glm::vec3(0.18275f, 0.17f, 0.22525f),
            .specular = glm::vec3(0.332741f, 0.328634f, 0.346435f),
            .shininess = 128 * 0.3f
        };
       
        static constexpr Material prefab_Pearl {
            .ambient = glm::vec3(0.25f, 0.20725f, 0.20725f),
            .diffuse = glm::vec3(1.f, 0.829f, 0.829f),
            .specular = glm::vec3(0.296648f, 0.296648f, 0.296648f),
            .shininess = 128 * 0.088f
        };
        
        static constexpr Material prefab_Ruby {
            .ambient = glm::vec3(0.1745f, 0.01175f, 0.01175f),
            .diffuse = glm::vec3(0.61424f, 0.04136f, 0.04136f),
            .specular = glm::vec3(0.727811f, 0.626959f, 0.626959f),
            .shininess = 128 * 0.6f
        };

        static constexpr Material prefab_Turquoise {
            .ambient = glm::vec3(0.1f, 0.18725f, 0.1745f),
            .diffuse = glm::vec3(0.396f, 0.74151f, 0.69102),
            .specular = glm::vec3(0.297254f, 0.30829f, 0.306678f),
            .shininess = 128 * 0.1f
        };

        static constexpr Material prefab_Brass {
            .ambient = glm::vec3(0.329412f, 0.223529f, 0.27451f),
            .diffuse = glm::vec3(0.780392f, 0.568627f, 0.113725f),
            .specular = glm::vec3(0.992157f, 0.941176f, 0.807843f),
            .shininess = 128 * 0.21794872f
        };

        static constexpr Material prefab_Bronze {
            .ambient = glm::vec3(0.2125f, 0.1275f, 0.054f),
            .diffuse = glm::vec3(0.714f, 0.4284f, 0.18144f),
            .specular = glm::vec3(0.393548f, 0.271906f, 0.166721f),
            .shininess = 128 * 0.2f
        };
        
        static constexpr Material prefab_Chrome {
            .ambient = glm::vec3(0.25f, 0.25f, 0.25f),
            .diffuse = glm::vec3(0.4f, 0.4f, 0.4f),
            .specular = glm::vec3(0.774597f, 0.774597f, 0.774597f),
            .shininess = 128 * 0.6f 
        };

        static constexpr Material prefab_Copper {
            .ambient = glm::vec3(0.19125f, 0.0735f, 0.0225f),
            .diffuse = glm::vec3(0.7038f, 0.27048f, 0.0828f),
            .specular = glm::vec3(0.256777f, 0.137622f, 0.086014f),
            .shininess = 128 * 0.1f
        };

        static constexpr Material prefab_Silver {
            .ambient = glm::vec3(0.19225f, 0.19225f, 0.19225f),
            .diffuse = glm::vec3(0.50754f, 0.50754f, 0.50754f),
            .specular = glm::vec3(0.508273f, 0.508273f, 0.508273f),
            .shininess = 128 * 0.4f 
        };

        static constexpr Material prefab_CyanPlastic {
            .ambient = glm::vec3(0.0f, 0.1f, 0.06f),
            .diffuse = glm::vec3(0.0f, 0.50980392f, 0.50980392f),
            .specular = glm::vec3(0.50196078f, 0.50196078f, 0.50186078f),
            .shininess = 128 * 0.25f 
        };
        
        static constexpr Material prefab_GreenPlastic {
            .ambient = glm::vec3(0.f, 0.f, 0.f),
            .diffuse = glm::vec3(0.1f, 0.35f, 0.1f),
            .specular = glm::vec3(0.45f, 0.55f, 0.45f),
            .shininess = 128 * 0.25 
        };

        static constexpr Material prefab_RedPlastic {
            .ambient = glm::vec3(0.f, 0.f, 0.f),
            .diffuse = glm::vec3(0.5f, 0.f, 0.f),
            .specular = glm::vec3(0.7f, 0.6f, 0.6f),
            .shininess = 128 * 0.25f 
        };

        static constexpr Material prefab_WhitePlastic {
            .ambient = glm::vec3(0.f, 0.f, 0.f),
            .diffuse = glm::vec3(0.55f, 0.55f, 0.55f),
            .specular = glm::vec3(0.70f, 0.70f, 0.70f),
            .shininess = 128 * 0.25f 
        };

        static constexpr Material prefab_YellowPlastic {
            .ambient = glm::vec3(0.f, 0.f, 0.f),
            .diffuse = glm::vec3(0.5f, 0.5f, 0.f),
            .specular = glm::vec3(0.60f, 0.60f, 0.50f),
            .shininess = 128 * 0.25f 
        };

        static constexpr Material prefab_BlackRubber {
            .ambient = glm::vec3(0.02f, 0.02f, 0.02f),
            .diffuse = glm::vec3(0.01f, 0.01f, 0.01f),
            .specular = glm::vec3(0.4f, 0.4f, 0.4f),
            .shininess = 128 * 0.078125f
        };

        static constexpr Material prefab_CyanRubber {
            .ambient = glm::vec3(0.0f, 0.05f, 0.05f),
            .diffuse = glm::vec3(0.4f, 0.5f, 0.5f),
            .specular = glm::vec3(0.04f, 0.7f, 0.7f),
            .shininess = 128 * 0.078125 
        };

        static constexpr Material prefab_GreenRubber {
            .ambient = glm::vec3(0.0f, 0.05f, 0.0f),
            .diffuse = glm::vec3(0.4f, 0.5f, 0.4f),
            .specular = glm::vec3(0.04f, 0.7f, 0.04f),
            .shininess = 128 * 0.078125f 
        };

        static constexpr Material prefab_RedRubber {
            .ambient = glm::vec3(0.05f, 0.0f, 0.0f),
            .diffuse = glm::vec3(0.5f, 0.4f, 0.4f),
            .specular = glm::vec3(0.7f, 0.04f, 0.04f),
            .shininess = 128 * 0.078125f 
        };

        static constexpr Material prefab_WhiteRubber {
            .ambient = glm::vec3(0.05f, 0.05f, 0.05f),
            .diffuse = glm::vec3(0.5f, 0.5f, 0.5f),
            .specular = glm::vec3(0.7f, 0.7f, 0.7f),
            .shininess = 128 * 0.078125f 
        };

        static constexpr Material prefab_YellowRubber {
            .ambient = glm::vec3(0.05f, 0.05f, 0.0f),
            .diffuse = glm::vec3(0.5f, 0.5f, 0.4f),
            .specular = glm::vec3(0.7f, 0.7f, 0.04f),
            .shininess = 128 * 0.078125f 
        };
        
        // TODO: THIS IS CRAZY WAY !!!!!!
        using MaterialPtr = const Material*;

        static std::vector<MaterialPtr> materials;
        
        void InitPrefabMaterials();

        const Material& GetPrefabMaterialByIndex(int index_prefab_material); 
        PrefabMaterial GetEnumValuePrefabMatByIndex(int index_prefab_material);

    } // namespace avion::core

#endif // AVION_CORE_MATERIAL_H 

