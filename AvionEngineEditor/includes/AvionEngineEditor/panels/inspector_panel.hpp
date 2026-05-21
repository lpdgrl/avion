#ifndef AVION_EDITOR_OBJECT_PANEL_H
#define AVION_EDITOR_OBJECT_PANEL_H

  #include "AvionEngineEditor/editor/editor_context.hpp"
  #include "AvionEngineEditor/ui/av_gui_scoped_window.hpp"
  #include "AvionEngineEditor/ui/av_gui_combo_item.hpp"
  #include "AvionEngineEditor/ui/av_gui_tab_bar.hpp"
  #include "AvionEngineEditor/ui/av_gui_tab_item.hpp"

  namespace avion::editor::panel
  {
    
    class InspectorPanel
    {
      using TabBar            = ui::avguicontext::AvGuiTabBar;
      using TabItem           = ui::avguicontext::AvGuiTabItem;
      using ComboItem         = ui::avguicontext::AvGuiComboItem;
      using ScopeWindow       = ui::avguicontext::AvGuiScopedWindow;
      using EditorContext     = detail::EditorContext;
      using SelectionContext  = detail::SelectionContext;

      public:
        InspectorPanel(EditorContext context, SelectionContext& select_ctx);

        void Render();
        
        ~InspectorPanel() = default;
      private:
        void RenderTabSelectObject() const noexcept;
        void RenderTabModel() const noexcept;
        void RenderTabLight() const noexcept;
        void RenderTabPrimitive() const noexcept;

        size_t DrawComboTexture(
          const char* label, 
          size_t index_selected_texture, 
          const core::resman::ResourceManager::ListTexture& textures,
          core::Material& material) const noexcept;

      private:
        EditorContext     m_editor_ctx;
        SelectionContext&  m_select_ctx;
    };

  } // namespace avion::editor::panel


#endif