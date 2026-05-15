#ifndef AVION_EDITOR_ASSET_PANEL_H
#define AVION_EDITOR_ASSET_PANEL_H

  #include "AvionEngineEditor/ui/av_gui_scoped_window.hpp"
  #include "AvionEngineEditor/editor/editor_context.hpp"
  #include "AvionEngineEditor/ui/av_gui_tab_bar.hpp"
  #include "AvionEngineEditor/ui/av_gui_tab_item.hpp"
  
  #include <array>

  namespace avion::editor::panel
  {
    
    class AssetPanel
    {
      using TabBar          = ui::avguicontext::AvGuiTabBar;
      using TabItem         = ui::avguicontext::AvGuiTabItem;
      using ScopeWindow     = ui::avguicontext::AvGuiScopedWindow;
      using EditorContext   = detail::EditorContext;

      public:
        AssetPanel(EditorContext context);

        void Render();
        
        ~AssetPanel() = default;
      private:
        void RenderAsset()        const noexcept;
        void RenderModelTab()     const noexcept;
        void RenderPrimitiveTab() const noexcept;
      private:
        EditorContext m_context;
    };

  } // namespace avion::editor::panel


#endif