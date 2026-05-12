#ifndef AVION_EDITOR_GUI_TREE_NODE_H
#define AVION_EDITOR_GUI_TREE_NODE_H

  #include "imgui.h"

  #include "AvionEngineEditor/ui/utils/imgui_helper.hpp"

  namespace avion::ui::avguicontext
  {

    class AvGuiTreeNode
    {
      public:
        AvGuiTreeNode() = delete;

        AvGuiTreeNode(const char *node_name, ImGuiTreeNodeFlags flags = 0);
        
        ~AvGuiTreeNode();
        
        bool IsOpen() const noexcept;
        bool IsSelectable() noexcept;
      private:
        bool m_is_opened = false;
    };

  } // nammespace avion::ui::avguicontext


#endif