#ifndef AVION_EDITOR_GUI_SCOPED_WINDOW_H
#define AVION_EDITOR_GUI_SCOPED_WINDOW_H

  #include "imgui.h"

  #include "AvionEngineEditor/ui/utils/imgui_helper.hpp"

  namespace avion::ui::avguicontext
  {

    class AvGuiScopedWindow
    {
      public:
        AvGuiScopedWindow() = delete;

        AvGuiScopedWindow(const char* window_name, bool* open = nullptr, ImGuiWindowFlags flags = 0);
        
        ~AvGuiScopedWindow();
        
        bool IsOpen() const noexcept;
      private:
        bool m_is_opened = false;
    };

  } // nammespace avion::ui::avguicontext


#endif