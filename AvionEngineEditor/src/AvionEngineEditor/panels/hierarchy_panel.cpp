#include "AvionEngineEditor/panels/hierarchy_panel.hpp"

namespace avion::editor::panel
{

  HierarchyPanel::HierarchyPanel(HierarchyPanel::EditorContext& editor_ctx)
  : m_editor_ctx(editor_ctx)
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

  void HierarchyPanel::ClearOldSelectedObject() const noexcept
  {
    auto& scene_ctx = m_editor_ctx.engine.GetScene();

    if (m_editor_ctx.selection_ctx.primitive.is_select)
    {
      if (auto* p = scene_ctx.GetObject(m_editor_ctx.selection_ctx.primitive.id); p != nullptr)
      {
        m_editor_ctx.selection_ctx.primitive = detail::SelectPrimitive();
        p->is_selectable = false;
      }
    } 
    else if (m_editor_ctx.selection_ctx.light.is_select)
    {
      if (auto* p = scene_ctx.GetLight(m_editor_ctx.selection_ctx.light.id); p != nullptr)
      {
        m_editor_ctx.selection_ctx.light = detail::SelectLight();
        p->is_selectable = false;
      }
    }
    else if (m_editor_ctx.selection_ctx.model.is_select)
    {
      if (auto* p = scene_ctx.GetModel(m_editor_ctx.selection_ctx.model.id); p != nullptr)
      {
        m_editor_ctx.selection_ctx.model = detail::SelectModel();
        p->is_selectable = false;
      }
    }
  }

  void HierarchyPanel::RenderHierarchyScene() const noexcept
  {
    static int selection_mask  = -1;
    static int selection_index = 0;

    auto& scene_ctx = m_editor_ctx.engine.GetScene();
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
              ClearOldSelectedObject();
              selection_mask = selection_index;
              m_editor_ctx.selection_ctx.primitive.is_select = true;
              m_editor_ctx.selection_ctx.primitive.id = id;
              object.is_selectable = true;
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
        for (auto& light : scene_ctx.GetAllSourceLights())
        {
          id = light.id;
          text = std::move(core::detail::TypeObjectToString(light.type));
          text += ", id: " + std::to_string(id);
          
          TreeNode node(text.c_str(), ImGuiTreeNodeFlags_Leaf | ui::utils::CheckSelectableTreeNode(selection_mask, selection_index));
          if (node.IsOpen())
          {
            if (node.IsSelectable())
            {
              ClearOldSelectedObject();
              selection_mask = selection_index;
              m_editor_ctx.selection_ctx.light.is_select = true;
              m_editor_ctx.selection_ctx.light.id = id;
              light.is_selectable = true;
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
        for (auto& ptr_model : scene_ctx.GetModels())
        {
          text = ptr_model->model.GetFileName();
          text += ", id: " + std::to_string(ptr_model->id);
          TreeNode node(text.c_str(), ImGuiTreeNodeFlags_Leaf | ui::utils::CheckSelectableTreeNode(selection_mask, selection_index));
          if (node.IsOpen())
          {
            if (node.IsSelectable())
            {
              ClearOldSelectedObject();
              selection_mask = selection_index;
              m_editor_ctx.selection_ctx.model.is_select = true;
              m_editor_ctx.selection_ctx.model.filename = ptr_model->model.GetFileName();
              m_editor_ctx.selection_ctx.model.id = ptr_model->id;
              ptr_model->is_selectable = true;
            }
          }
          selection_index++;
        }
        selection_index = 0;
      }
    }
  }
} // namespace avion::editor::panel
