#ifndef AVION_EDITOR_GUI_TAB_ITEM_H
#define AVION_EDITOR_GUI_TAB_ITEM_H

  #include <string>

  #include "imgui.h"

  #include "AvionEngineEditor/ui/utils/imgui_helper.hpp"

  namespace avion::ui::avguicontext
  {

    class AvGuiTabItem
    {
      public:
        AvGuiTabItem() = delete;

        AvGuiTabItem(const char *window_name, bool *open = nullptr, ImGuiTabItemFlags flags = 0);
        
        ~AvGuiTabItem();
        
        bool IsOpen() const noexcept;
      private:
        bool m_is_opened = false;
    };

  } // nammespace avion::ui::avguicontext


#endif