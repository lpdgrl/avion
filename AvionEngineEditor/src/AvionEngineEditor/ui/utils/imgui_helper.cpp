#include "AvionEngineEditor/ui/utils/imgui_helper.hpp"

namespace avion::ui::utils
{
  
  void SetupWindow(ImVec2 pos, ImVec2 size, ImGuiCond cond, const ImVec2 pivot)
  {
    ImGui::SetNextWindowPos(pos, cond, pivot);
    ImGui::SetNextWindowSize(size, cond);
  }

  ImGuiTreeNodeFlags CheckSelectableTreeNode(int selection_mask, size_t index)
  {
    ImGuiTreeNodeFlags flag = 0;

    if (selection_mask == index)
    {
      flag = ImGuiTreeNodeFlags_Selected;
    }

    return flag;
  }

} // namespace avion::ui::utils