#include "AvionEngineEditor/panels/hierarchy_panel.hpp"

namespace avion::editor::panel
{

  HierarchyPanel::HierarchyPanel(HierarchyPanel::EditorContext editor_ctx, HierarchyPanel::SelectionContext& select_ctx)
  : m_editor_ctx(editor_ctx)
  , m_selection_ctx(select_ctx)
  {
  }

  void HierarchyPanel::Render() 
  {
    ImVec2 pos(0, 200);
    ImVec2 size(300, 500);
    ui::utils::SetupWindow(pos, size, ImGuiCond_Always);

    ScopeWindow window("Hierarchy", nullptr, ImGuiWindowFlags_NoMove);
    
    if (!window.IsOpen())
    {
      return;
    }

    m_number_objects = m_editor_ctx.engine.GetScene().GetAllNumberObjects();

    TreeNode node_objects("Objects", ImGuiTreeNodeFlags_DefaultOpen);
    if (node_objects.IsOpen()) 
    {
      RenderHierarchyScene();
      TestSelectObject();
    }
  }
  
  void HierarchyPanel::RenderHierarchyScene() const noexcept
  {
    static int selection_mask  = -1;
    static int selection_index = 0;

    const auto& scene_ctx = m_editor_ctx.engine.GetScene();
    {   
      TreeNode tree_node("Primitives", ImGuiTreeNodeFlags_DefaultOpen);
      if (tree_node.IsOpen())
      {
        std::string text;
        int id = -1;
        
        for (auto& object : scene_ctx.GetAllObjects())
        {
          id = object.object.GetId();
          text = std::move(core::detail::TypeObjectToString<core::ObjectType>(object.type));
          text += ", id: " + std::to_string(id);

          TreeNode node(text.c_str(), ImGuiTreeNodeFlags_Leaf | ui::utils::CheckSelectableTreeNode(selection_mask, selection_index));
          if (node.IsOpen())
          {
            if (node.IsSelectable())
            {
              selection_mask = selection_index;
              m_selection_ctx.primitive.is_select = true;
              m_selection_ctx.primitive.id = id;

              // TODO: Clear m_selection_ctx in method
              m_selection_ctx.model = detail::SelectModel();
              m_selection_ctx.light = detail::SelectLight();
            }
          }
          selection_index++;
        }
      }
    }

    {
      TreeNode tree_node("Lights", ImGuiTreeNodeFlags_DefaultOpen);
      if (tree_node.IsOpen())
      {
        std::string text;
        int id = -1;
        for (const auto& light : scene_ctx.GetAllSourceLights())
        {
          id = light.id;
          text = std::move(core::detail::TypeObjectToString<core::LightType>(light.type_light));
          text += ", id: " + std::to_string(id);
          
          TreeNode node(text.c_str(), ImGuiTreeNodeFlags_Leaf | ui::utils::CheckSelectableTreeNode(selection_mask, selection_index));
          if (node.IsOpen())
          {
            if (node.IsSelectable())
            {
              selection_mask = selection_index;
              m_selection_ctx.light.is_select = true;
              m_selection_ctx.light.id = id;

              // TODO: Clear m_selection_ctx in method
              m_selection_ctx.model = detail::SelectModel();
              m_selection_ctx.primitive = detail::SelectPrimitive();
            }
          }
          selection_index++;
        }
      }
    }

    {
      TreeNode tree_node("Models", ImGuiTreeNodeFlags_DefaultOpen);
      if (tree_node.IsOpen())
      {
        std::string text;
        for (const auto& [id, model] : scene_ctx.GetModels())
        {
          text = model.GetFileName();
          text += ", id: " + std::to_string(id);
          TreeNode node(text.c_str(), ImGuiTreeNodeFlags_Leaf | ui::utils::CheckSelectableTreeNode(selection_mask, selection_index));
          if (node.IsOpen())
          {
            if (node.IsSelectable())
            {
              selection_mask = selection_index;
              m_selection_ctx.model.is_select = true;
              m_selection_ctx.model.filename = model.GetFileName();

              // TODO: Clear m_selection_ctx in method
              m_selection_ctx.light = detail::SelectLight();
              m_selection_ctx.primitive = detail::SelectPrimitive();
            }
          }
          selection_index++;
        }
        selection_index = 0;
      }
    }
  }


  void HierarchyPanel::TestSelectObject() const noexcept
  {
    if (m_selection_ctx.primitive)
    {
      const auto& primitive = m_selection_ctx.primitive;
      std::string msg;
      msg += "Select is primitive";
      
      // decltype(auto) p_primitive = scene.GetObject(primitive.id);

      msg += " " + std::to_string(primitive.id);
    }
    
    if (m_selection_ctx.light)
    {
      const auto& light = m_selection_ctx.light;
      std::string msg;
      msg += "Select is light";
      
      // decltype(auto) p_light = scene.GetLight(light.id);

      msg += " " + std::to_string(light.id);
    }

    if (m_selection_ctx.model)
    {
      const auto& model = m_selection_ctx.model;
      std::string msg;
      msg += "Select is model";
      
      // decltype(auto) p_light = scene.GetModel(model.filename);

      msg += " " + model.filename;
    }

  }
} // namespace avion::editor::panel
