#ifndef AVION_EDITOR_GUI_TAB_BAR_H
#define AVION_EDITOR_GUI_TAB_BAR_H

  #include <string>

  #include "imgui.h"

  #include "AvionEngineEditor/ui/utils/imgui_helper.hpp"

  namespace avion::ui::avguicontext
  {

    class AvGuiTabBar
    {
      public:
        AvGuiTabBar() = delete;

        AvGuiTabBar(const char* window_name, ImGuiTabBarFlags flags = 0);
        
        ~AvGuiTabBar();
        
        bool IsOpen() const noexcept;
      private:
        bool m_is_opened = false;
    };

  } // nammespace avion::ui::avguicontext


#endif