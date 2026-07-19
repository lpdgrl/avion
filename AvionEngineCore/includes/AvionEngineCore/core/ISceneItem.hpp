#ifndef AVION_CORE_ISCENE_ITEM_H
#define AVION_CORE_ISCENE_ITEM_H 1

  #include <cstdint>
  #include <memory>

  #include "AvionEngineCore/core/ModelManager/ModelManager.hpp"
  #include "AvionEngineCore/core/light.hpp"

  namespace avion::core
  {
    enum class LightType : std::int32_t
    {
      kUnknownLight = -1,
      kSimpleLight = 0,
      kDirLight = 1,
      kPointLight = 2,
      kSpotLight = 3,
    };

    enum class ItemType : std::uint8_t
    {
      kUnknown      = 0,
      kObject       = 1,
      kSourceLight  = 2,
      kCamera       = 3,
    };

    struct ISceneItem 
    {
        using ModelHandler = modelmanager::detail::ModelHandler;
        using ModelPtr     = modelmanager::ModelManager::ModelPtr;
        using ItemId       = std::uint32_t;

        ISceneItem(ModelHandler&& handler, ModelPtr&& ptr, ItemId id, ItemType type)
        : model_handler(std::move(handler))
        , ptr_model(std::move(ptr))
        , id(id)
        , item_type(type)
        {}

        virtual ~ISceneItem() = default;

        ModelHandler model_handler;
        ModelPtr     ptr_model;
        ItemId       id{};
        ItemType     item_type = ItemType::kUnknown;
        bool         is_selectable{};
    };

    struct ObjectItem : ISceneItem
    { 
      ObjectItem(ModelHandler&& handler, ModelPtr&& ptr, ItemId id, ItemType type)
      : ISceneItem(std::move(handler), std::move(ptr), id, type)
      {}
    };

    struct LightItem : ISceneItem
    {
      LightItem
      (
        ModelHandler&& handler, 
        ModelPtr&& ptr, 
        ItemId id, 
        ItemType item_type, 
        std::unique_ptr<ILight>&& light, 
        LightType light_type
      )
      : ISceneItem(std::move(handler), std::move(ptr), id, item_type)
      , light(std::move(light))
      , light_type(light_type)
      {}

      std::unique_ptr<ILight> light;
      LightType               light_type;
    };
  } // namespace avion::core

#endif 