#ifndef AVION_EDITOR_GUI_COMBO_ITEM_H
#define AVION_EDITOR_GUI_COMBO_ITEM_H

  #include <string>

  #include "imgui.h"

  namespace avion::ui::avguicontext
  {

    class AvGuiComboItem
    {
      public:
        AvGuiComboItem() = delete;

        AvGuiComboItem(const char *label, const char *preview_value, ImGuiComboFlags flags = 0);
        
        ~AvGuiComboItem();
        
        bool IsOpen() const noexcept;
      private:
        bool m_is_opened = false;
    };
    
  } // namespace avion::ui::avguicontext

#endif