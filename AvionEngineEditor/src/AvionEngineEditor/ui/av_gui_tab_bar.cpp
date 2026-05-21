#include "AvionEngineEditor/ui/av_gui_tab_bar.hpp"

namespace  avion::ui::avguicontext
{
 
    AvGuiTabBar::AvGuiTabBar(const char* window_name, ImGuiTabBarFlags flags)
    : m_is_opened(ImGui::BeginTabBar(window_name, flags))
    {
    }

    AvGuiTabBar::~AvGuiTabBar()
    { 
      if (IsOpen())
      {
        ImGui::EndTabBar();
      }
    }


    bool AvGuiTabBar::IsOpen() const noexcept
    {
      return m_is_opened;
    }

} // namespace avion::gui::avguicontext

