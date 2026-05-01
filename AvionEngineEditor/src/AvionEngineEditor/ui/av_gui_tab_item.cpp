#include "AvionEngineEditor/ui/av_gui_tab_item.hpp"

namespace  avion::ui::avguicontext
{
 
    AvGuiTabItem::AvGuiTabItem(const char *window_name, bool *open, ImGuiTabItemFlags flags)
    : m_is_opened(ImGui::BeginTabItem(window_name, open, flags))
    {
    }

    AvGuiTabItem::~AvGuiTabItem()
    { 
      if (IsOpen())
      {
        ImGui::EndTabItem();
      }
      
    }


    bool AvGuiTabItem::IsOpen() const noexcept
    {
      return m_is_opened;
    }

} // namespace avion::gui::avguicontext

