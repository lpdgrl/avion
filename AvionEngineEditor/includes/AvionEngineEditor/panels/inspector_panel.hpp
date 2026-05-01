#ifndef AVION_EDITOR_OBJECT_PANEL_H
#define AVION_EDITOR_OBJECT_PANEL_H

  #include "AvionEngineEditor/ui/av_gui_scoped_window.hpp"
  #include "AvionEngineEditor/editor/editor_context.hpp"

  namespace avion::editor::panel
  {
    
    class InspectorPanel
    {
      using ScopeWindow   = ui::avguicontext::AvGuiScopedWindow;
      using EditorContext = detail::EditorContext;

      public:
        InspectorPanel(EditorContext context);

        void Render();
        
        ~InspectorPanel() = default;
      private:
        EditorContext m_context;
    };

  } // namespace avion::editor::panel


#endif