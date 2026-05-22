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
          id = object.id;
          text = std::move(core::detail::TypeObjectToString(object.type));
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
          text = std::move(core::detail::TypeObjectToString(light.type));
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
        for (const auto& ptr_model : scene_ctx.GetModels())
        {
          text = ptr_model->model.GetFileName();
          text += ", id: " + std::to_string(ptr_model->id);
          TreeNode node(text.c_str(), ImGuiTreeNodeFlags_Leaf | ui::utils::CheckSelectableTreeNode(selection_mask, selection_index));
          if (node.IsOpen())
          {
            if (node.IsSelectable())
            {
              selection_mask = selection_index;
              m_selection_ctx.model.is_select = true;
              m_selection_ctx.model.filename = ptr_model->model.GetFileName();
              m_selection_ctx.model.id = ptr_model->id;

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
} // namespace avion::editor::panel
