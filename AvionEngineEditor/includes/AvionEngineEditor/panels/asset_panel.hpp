#ifndef AVION_EDITOR_ASSET_PANEL_H
#define AVION_EDITOR_ASSET_PANEL_H

  #include "AvionEngineEditor/ui/av_gui_scoped_window.hpp"
  #include "AvionEngineEditor/editor/editor_context.hpp"

  namespace avion::editor::panel
  {
    
    class AssetPanel
    {
      using ScopeWindow   = ui::avguicontext::AvGuiScopedWindow;
      using EditorContext = detail::EditorContext;

      public:
        AssetPanel(EditorContext context);

        void Render();
        
        ~AssetPanel() = default;
      private:
        EditorContext m_context;
    };

  } // namespace avion::editor::panel


#endif