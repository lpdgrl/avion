#include "../../../includes/AvionEngineCore/core/material.hpp"

namespace avion::core {

  PrefabMaterial GetEnumValuePrefabMatByIndex(int index_prefab_material) 
  {
    if (index_prefab_material < 0 || index_prefab_material >= kNumberPrefabMaterials) {
      AV_LOG_ERROR("GetEnumValuePrefabMatByIndex. index prefab material is invalid");
      assert(false);
    }
    return static_cast<PrefabMaterial>(index_prefab_material);
  }

  const Material& GetPrefabMaterialByIndex(int index_prefab_material) 
  {
    if (index_prefab_material < 0 || index_prefab_material >= kNumberPrefabMaterials) {
      AV_LOG_ERROR("GetPrefabMaterialByIndex. index prefab material is invalid");
      assert(false);
    }
    return *materials[index_prefab_material];
  }

  void InitPrefabMaterials() 
  {
    materials.reserve(kNumberPrefabMaterials);

    materials.push_back(&prefab_None);
    materials.push_back(&prefab_Emerald);
    materials.push_back(&prefab_Gold);
    materials.push_back(&prefab_Jade);
    materials.push_back(&prefab_Obsidian);
    materials.push_back(&prefab_Pearl);
    materials.push_back(&prefab_Ruby);
    materials.push_back(&prefab_Turquoise);
    materials.push_back(&prefab_Brass);
    materials.push_back(&prefab_Bronze);
    materials.push_back(&prefab_Chrome);
    materials.push_back(&prefab_Copper);
    materials.push_back(&prefab_Silver);
    materials.push_back(&prefab_BlackPlastic);
    materials.push_back(&prefab_CyanPlastic);
    materials.push_back(&prefab_GreenPlastic);
    materials.push_back(&prefab_RedPlastic);
    materials.push_back(&prefab_WhitePlastic);
    materials.push_back(&prefab_YellowPlastic);
    materials.push_back(&prefab_BlackRubber);
    materials.push_back(&prefab_CyanRubber);
    materials.push_back(&prefab_GreenRubber);
    materials.push_back(&prefab_RedRubber);
    materials.push_back(&prefab_WhiteRubber);
    materials.push_back(&prefab_YellowRubber);

  }

} // namespace avion::core