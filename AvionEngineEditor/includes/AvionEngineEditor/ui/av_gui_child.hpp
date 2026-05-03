#ifndef AVION_EDITOR_GUI_CHILD_H
#define AVION_EDITOR_GUI_CHILD_H

  #include <string>

  #include "imgui.h"

  namespace avion::ui::avguicontext
  {

    class AvGuiChild
    {
      public:
        AvGuiChild() = delete;

        AvGuiChild(const char *window_name, const ImVec2 size = ImVec2(0, 0), ImGuiChildFlags child_flags = 0, ImGuiWindowFlags window_flags = 0);
        
        ~AvGuiChild();
        
        bool IsOpen() const noexcept;
      private:
        bool m_is_opened = false;
    };

  } // nammespace avion::ui::avguicontext


#endif