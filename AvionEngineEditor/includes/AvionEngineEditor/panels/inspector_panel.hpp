#ifndef AVION_EDITOR_OBJECT_PANEL_H
#define AVION_EDITOR_OBJECT_PANEL_H 1

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
        InspectorPanel(EditorContext& context);

        void Render();
        
        ~InspectorPanel() = default;
      private:
        void RenderTabSelectObject() const noexcept;
        void RenderTabSelectItem() const noexcept;
        void RenderTabLight() const noexcept;

        auto DeleteFromScene(std::uint32_t id) const noexcept -> void;

        size_t DrawComboTexture
        (
          const char* label, 
          size_t index_selected_texture, 
          const core::resman::ResourceManager::ListTexture& textures,
          gfx::Model::Material& material
        ) const noexcept;

      private:
        EditorContext&    m_editor_ctx;
    };
  } // namespace avion::editor::panel

#endif