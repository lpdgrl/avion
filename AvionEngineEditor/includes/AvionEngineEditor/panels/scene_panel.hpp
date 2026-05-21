#ifndef AVION_EDITOR_SCENE_PANEL_H
#define AVION_EDITOR_SCENE_PANEL_H

  #include "AvionEngineEditor/ui/av_gui_scoped_window.hpp"
  #include "AvionEngineEditor/ui/av_gui_child.hpp"
  #include "AvionEngineEditor/editor/editor_context.hpp"

  namespace avion::editor::panel
  {
    
    class ScenePanel
    {
      using ScopeWindow   = ui::avguicontext::AvGuiScopedWindow;
      using ChildWindow   = ui::avguicontext::AvGuiChild;
      using EditorContext = detail::EditorContext;

      public:
        ScenePanel() = delete;
        ScenePanel(EditorContext& context);

        void Render();
        
        ~ScenePanel() = default;
      private:
        EditorContext& m_context;
    };

  } // namespace avion::editor::panel


#endif