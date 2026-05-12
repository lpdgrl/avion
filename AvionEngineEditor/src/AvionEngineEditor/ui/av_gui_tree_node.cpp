#include "AvionEngineEditor/ui/av_gui_tree_node.hpp"

namespace  avion::ui::avguicontext
{
    AvGuiTreeNode::AvGuiTreeNode(const char *node_name, ImGuiTreeNodeFlags flags)
    : m_is_opened(ImGui::TreeNodeEx(node_name, flags))
    {
    }

    AvGuiTreeNode::~AvGuiTreeNode()
    { 
      if (m_is_opened)
      {
        ImGui::TreePop();
      }
    }

    bool AvGuiTreeNode::IsOpen() const noexcept
    {
      return m_is_opened;
    }

    bool AvGuiTreeNode::IsSelectable() noexcept
    {
      return ImGui::IsItemClicked();
    }

} // namespace avion::gui::avguicontext

