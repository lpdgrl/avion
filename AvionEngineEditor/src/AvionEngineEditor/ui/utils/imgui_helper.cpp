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

  void SliderFloat3V(const std::string& title, glm::vec3& values, float val_min, float val_max, const char* format, ImGuiSliderFlags flag)
  {
    std::string text;
    text.reserve(title.size() + 2);
    text.append("##");
    text.append(title);

    ImGui::SliderFloat3(text.c_str(), &values.x, val_min, val_max, format, flag);
  }

  void InputFloat3V(const std::string& title, glm::vec3& values, float step, float step_fast, const char* format, ImGuiInputTextFlags flag)
  {
    std::string text;
    text.reserve(title.size() + 2);
    text.append("##");
    text.append(title);

    ImGui::InputFloat((text + "x").c_str(), &values.x, step, step_fast, format, flag);
    ImGui::InputFloat((text + "y").c_str(), &values.y, step, step_fast, format, flag);
    ImGui::InputFloat((text + "z").c_str(), &values.z, step, step_fast, format, flag);
  }

} // namespace avion::ui::utils