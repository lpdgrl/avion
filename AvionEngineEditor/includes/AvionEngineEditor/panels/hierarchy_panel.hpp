#ifndef AVION_EDITOR_HIERARCHY_PANEL_H
#define AVION_EDITOR_HIERARCHY_PANEL_H

  #include "AvionEngineEditor/ui/av_gui_scoped_window.hpp"
  #include "AvionEngineEditor/ui/av_gui_tree_node.hpp"
  #include "AvionEngineEditor/editor/editor_context.hpp"

  #include <type_traits>

  namespace avion::editor::panel
  {
    
    class HierarchyPanel
    {
      using ScopeWindow       = ui::avguicontext::AvGuiScopedWindow;
      using TreeNode          = ui::avguicontext::AvGuiTreeNode;
      using EditorContext     = detail::EditorContext;
      using SelectionContext  = detail::SelectionContext;
      
      public:
        HierarchyPanel(EditorContext editor_ctx, SelectionContext& select_ctx);

        void Render();   
        ~HierarchyPanel() = default;
      
      private:
        void RenderHierarchyScene() const noexcept;

        void TestSelectObject() const noexcept;
      private:
        EditorContext     m_editor_ctx;
        SelectionContext& m_selection_ctx;
        std::uint16_t     m_number_objects{};
    };
  } // namespace avion::editor::panel
#endif
