#include "AvionEngineEditor/panels/asset_panel.hpp"

namespace avion::editor::panel
{

  AssetPanel::AssetPanel(AssetPanel::EditorContext context)
  : m_context(context)
  {

  }

  void AssetPanel::Render() 
  {

    ImVec2 pos(0, 700);
    ImVec2 size(1320, 380);
    ui::utils::SetupWindow(pos, size, ImGuiCond_Always);

    ScopeWindow window("Assets", nullptr, ImGuiWindowFlags_NoMove);
    
    if (!window.IsOpen())
    {
      return;
    }
    
    RenderAsset();
  }

  void AssetPanel::RenderAsset() const noexcept
  {
    TabBar tab("asset_tab");
    RenderModelTab();
    RenderPrimitiveTab();
    RenderLightTab();
  }

  void AssetPanel::RenderModelTab() const noexcept
  {
    TabItem tab_item("Models");
    if (tab_item.IsOpen())
    {
      const auto& model = m_context.engine.GetResourceManager().GetModelLoadedList();
      static int index_select = -1;

      for (size_t i = 0; i < model.size(); ++i)
      {
        if (ImGui::Selectable(model[i].c_str(), index_select == i))
        {
          index_select = i;
        }
      }

      if (index_select != -1)
      {
        m_context.engine.GetScene().AddModel(model[index_select]);
        index_select = -1;
      }
    }
  }

  void AssetPanel::RenderPrimitiveTab() const noexcept
  {
    TabItem tab_item("Primitives");
    if (tab_item.IsOpen())
    {
      auto& scene = m_context.engine.GetScene();
      static std::array<bool, 2> selection;

      ImGui::Selectable("Cube", &selection[0]);
      ImGui::Selectable("Pyramid", &selection[1]);

      core::ObjectParams params {
        .transform{
          .position{0.f, 0.f, 0.f},
          .size{1.f, 1.f, 1.f},
        },
        .color{0.f, 1.f, 0.f},
        .mixing_color{},
        .material{.ambient{0.f, 0.5f, 0.f}, .diffuse{0.f, 1.f, 0.f}, .specular{}, .shininess{}}
      };

      // Add cube to scene
      if (selection[0])
      {
        scene.AddObjectToScene(core::ObjectType::kCube, params);
      }
      // Add pyramid to scene
      else if (selection[1])
      {
        scene.AddObjectToScene(core::ObjectType::kPyramid, params);
      }

      std::ranges::fill(selection, false);
    }
  }

  void AssetPanel::RenderLightTab() const noexcept
  {
    TabItem tab_item("Light");
    if (tab_item.IsOpen())
    {
      auto& scene = m_context.engine.GetScene();
      static std::array<bool, 3> selection;

      ImGui::Selectable("Point light", &selection[0]);
      ImGui::Selectable("Direction light", &selection[1]);
      ImGui::Selectable("Spot light", &selection[2]);

      // Add point light to scene
      if (selection[0])
      {
        scene.AddSourceLight(avion::core::LightType::kPointLight);
      }
      // Add direction light to scene
      else if (selection[1])
      {
        scene.AddSourceLight(avion::core::LightType::kDirLight);
      }
      // Add spot light to scene
      else if (selection[2])
      {
        scene.AddSourceLight(avion::core::LightType::kSpotLight);
      }

      std::ranges::fill(selection, false);
    }
  }


} // namespace avion::editor::panel