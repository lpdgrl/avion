#include "AvionEngineEditor/ui/av_gui_scoped_window.hpp"

namespace  avion::ui::avguicontext
{
 
    AvGuiScopedWindow::AvGuiScopedWindow(const char* window_name, bool* open, ImGuiWindowFlags flags)
    : m_is_opened(ImGui::Begin(window_name, open, flags))
    {
    }

    AvGuiScopedWindow::~AvGuiScopedWindow()
    {
      ImGui::End();
    }


    bool AvGuiScopedWindow::IsOpen() const noexcept
    {
      return m_is_opened;
    }

} // namespace avion::gui::avguicontext

