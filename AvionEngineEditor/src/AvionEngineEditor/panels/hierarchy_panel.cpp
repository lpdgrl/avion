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
      // if (auto* p = scene_ctx.GetObject(m_editor_ctx.selection_ctx.primitive.id); p != nullptr)
      // {
      //   m_editor_ctx.selection_ctx.primitive = detail::SelectPrimitive();
      //   p->is_selectable = false;
      // }
    } 
    else if (m_editor_ctx.selection_ctx.light.is_select)
    {
      // if (auto* p = scene_ctx.GetLight(m_editor_ctx.selection_ctx.light.id); p != nullptr)
      // {
      //   m_editor_ctx.selection_ctx.light = detail::SelectLight();
      //   p->is_selectable = false;
      // }
    }
    else if (m_editor_ctx.selection_ctx.model.is_select)
    {
      // if (auto* p = scene_ctx.GetModel(m_editor_ctx.selection_ctx.model.id); p != nullptr)
      // {
      //   m_editor_ctx.selection_ctx.model = detail::SelectModel();
      //   p->is_selectable = false;
      // }
    }
  }

  void HierarchyPanel::RenderHierarchyScene() const noexcept
  {
    static int selection_mask  = -1;
    static int selection_index = 0;

    auto& scene_ctx = m_editor_ctx.engine.GetScene();
    auto& items = scene_ctx.GetSceneItems();
    std::string text;

    for (auto& item : items)
    {
      text.append(std::to_string(item->id));
      text.append(":");
      text.append(item->item_type == core::ItemType::kSourceLight 
        ? core::detail::TypeObjectToString<core::LightType>(static_cast<core::LightItem*>(item.get())->light_type) 
        : item->ptr_model->GetFileName());

      TreeNode node(text.c_str(), ImGuiTreeNodeFlags_Leaf | ui::utils::CheckSelectableTreeNode(selection_mask, selection_index));
      if (node.IsOpen())
      {
        if (node.IsSelectable())
        {
          ClearOldSelectedObject();
          selection_mask = selection_index;
          m_editor_ctx.selection_ctx.item.is_select = true;
          m_editor_ctx.selection_ctx.item.id = item->id;
          // m_editor_ctx.selection_ctx.primitive.is_select = true;
          // m_editor_ctx.selection_ctx.primitive.id = item.id;
          // object.is_selectable = true;
        }
      }
      selection_index++;
      text.clear();
    }
    selection_index = 0;
  }
} // namespace avion::editor::panel
