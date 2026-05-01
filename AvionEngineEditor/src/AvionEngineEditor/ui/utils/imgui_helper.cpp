#include "AvionEngineEditor/ui/utils/imgui_helper.hpp"

namespace avion::ui::utils
{
  
  void SetupWindow(ImVec2 pos, ImVec2 size, ImGuiCond cond, const ImVec2 pivot)
  {
    ImGui::SetNextWindowPos(pos, cond, pivot);
    ImGui::SetNextWindowSize(size, cond);
  }

} // namespace avion::ui::utils