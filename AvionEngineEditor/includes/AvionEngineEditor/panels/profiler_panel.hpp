#ifndef AVION_EDITOR_PROFILER_PANEL_H
#define AVION_EDITOR_PROFILER_PANEL_H

  #include "AvionEngineEditor/ui/av_gui_scoped_window.hpp"
  #include "AvionEngineEditor/ui/av_gui_tab_bar.hpp"
  #include "AvionEngineEditor/ui/av_gui_tab_item.hpp"

  #include "AvionEngineEditor/editor/editor_context.hpp"

  namespace avion::editor::panel
  {

    class ProfilerPanel
    {
      using TabBar          = ui::avguicontext::AvGuiTabBar;
      using TabItem         = ui::avguicontext::AvGuiTabItem;
      using ScopeWindow     = ui::avguicontext::AvGuiScopedWindow;
      using ProfilerContext = detail::ProfilerContext;
     
      public:
        ProfilerPanel(ProfilerContext context);

        void Render();
        
        ~ProfilerPanel() = default;
      private:
      void ProfilerTab() const noexcept;
      void CameraTab()  const noexcept;
      
      private:
        ProfilerContext m_context;
    };

  } // namespace avion::editor::panel

#endif 