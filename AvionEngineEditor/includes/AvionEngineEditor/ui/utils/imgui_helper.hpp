#ifndef AVION_UI_UTILS_IMGUI_HELPER_H
#define AVION_UI_UTILS_IMGUI_HELPER_H
  
  #include <string>

  #include "imgui.h"

  namespace avion::ui::utils
  {
    void SetupWindow(ImVec2 pos, ImVec2 size, ImGuiCond cond = 0, const ImVec2 pivot = ImVec2(0, 0));
  } // namespace avion::ui::utils


#endif