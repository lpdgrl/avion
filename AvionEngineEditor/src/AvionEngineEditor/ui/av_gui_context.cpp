#include "AvionEngineEditor/ui/av_gui_context.hpp"

namespace avion::ui::avguicontext
{
  AvGuiContext::AvGuiContext(ResManager& resman)
  : m_resman(resman)
  {
  }

  bool AvGuiContext::Init(GLFWwindow* window)
  {
    ImGui::CreateContext();
    
    if (m_io == nullptr)
    {
      m_io = &ImGui::GetIO();
    }

    IMGUI_CHECKVERSION();
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    m_io->IniFilename = m_resman.GetResource<std::filesystem::path>("imgui.ini")->c_str(); 

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    return true;
  }

  bool AvGuiContext::BeginFrame()
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    return true;
  }

  bool AvGuiContext::EndFrame()
  {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return true;
  }

  bool AvGuiContext::Shutdown()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return true;
  }

} // namespace avion::ui::avguicontext