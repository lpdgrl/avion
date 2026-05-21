#include "AvionEngineEditor/ui/av_gui_child.hpp"

namespace  avion::ui::avguicontext
{
    AvGuiChild::AvGuiChild(const char *window_name, const ImVec2 size, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags)
    : m_is_opened(ImGui::BeginChild(window_name, size, child_flags, window_flags))
    {
    }

    AvGuiChild::~AvGuiChild()
    { 
      ImGui::EndChild();
    }


    bool AvGuiChild::IsOpen() const noexcept
    {
      return m_is_opened;
    }

} // namespace avion::gui::avguicontext

