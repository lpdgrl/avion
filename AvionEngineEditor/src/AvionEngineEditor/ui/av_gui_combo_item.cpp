#include "AvionEngineEditor/ui/av_gui_combo_item.hpp"

namespace avion::ui::avguicontext
{
    AvGuiComboItem::AvGuiComboItem(const char *label, const char *preview_value, ImGuiComboFlags flags)
    : m_is_opened(ImGui::BeginCombo(label, preview_value, flags))
    {
    }

    AvGuiComboItem::~AvGuiComboItem()
    { 
      if (IsOpen())
      {
        ImGui::EndCombo();
      }
    }


    bool AvGuiComboItem::IsOpen() const noexcept
    {
      return m_is_opened;
    }

} // namespace avion::ui::avguicontext