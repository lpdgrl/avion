#include "AvionEngineCore/core/window.hpp"

namespace avion::core {
  Window::Window(const std::string& window_name, int width, int height, Profiler& profiler, CameraProxy& camera_proxy)
    : window_name_(window_name)
    , width_window_(width)
    , height_window_(height)
    , controller_(controller::Controller((1.0 * width / 2), (1.0 * height / 2))) 
    , m_profiler(profiler)
    , m_camera_proxy(camera_proxy)
    {

    }

  Window::Window(const char* window_name, int width, int height, Profiler& profiler, CameraProxy& camera_proxy)
    : window_name_(window_name)
    , width_window_(width)
    , height_window_(height)
    , controller_(controller::Controller((1.0 * width / 2), (1.0 * height / 2)))
    , m_profiler(profiler)
    , m_camera_proxy(camera_proxy)
    {

    }

  Window::~Window() {
    if (window_) {
      glfwDestroyWindow(window_);
      glfwTerminate();
      AV_LOG_INFO("Window is destroyed");
    }
  }

  void Window::Init() 
  {
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    CreateWindow();

    // glfwSetFramebufferSizeCallback(window_, FrameBufferSizeCallback);
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    // glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetWindowUserPointer(window_, &controller_);
    glfwSetKeyCallback(window_, controller::Controller::KeyCallback);
    glfwSetCursorPosCallback(window_, controller::Controller::MouseCallback);
    glfwSetMouseButtonCallback(window_, controller::Controller::MouseButtonCallback);
    glfwSetCursorPos(window_, width_window_, 1.0 * height_window_ / 2.0);

    m_profiler.frame_state.m_lt = glfwGetTime();
  }

  bool Window::WindowShouldClose()
  {
    return glfwWindowShouldClose(window_);
  }

  void Window::SwapBuffers()
  {
    glfwSwapBuffers(window_);
    m_profiler.render_state.render_stat.Clear();
  }

  void Window::PollEvents()
  {
    controller_.ClearStateKeys();
    glfwPollEvents(); 
  }

  void Window::ProcessEvents() 
  {
    if (IsDown(GLFW_KEY_W)) {
      m_camera_proxy.Update(static_cast<int>(MovementKey::kForward), delta_time_);
    }
      
    if (IsDown(GLFW_KEY_A)) {
      m_camera_proxy.Update(static_cast<int>(MovementKey::kLeft), delta_time_);
    }

    if (IsDown(GLFW_KEY_D)) {
      m_camera_proxy.Update(static_cast<int>(MovementKey::kRight), delta_time_);
    }

    if (IsDown(GLFW_KEY_S)) {
         m_camera_proxy.Update(static_cast<int>(MovementKey::kBackward), delta_time_);
    }

    if (WasPressedKey(GLFW_KEY_H)) {
        cursor_state_ = !cursor_state_ ? true : false;
    }    
    
    if (controller_.IsDownMouseButton(GLFW_MOUSE_BUTTON_RIGHT)) {
        auto [xoffset, yoffset] = GetOffsetController();
         m_camera_proxy.Update(xoffset, yoffset);
    }
  
    if (cursor_state_) {
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else if (!cursor_state_) {
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    GetLastPosCursor();
  }

  void Window::FrameBufferSizeCallback(GLFWwindow* window, int width, int height) {
      glViewport(0, 0, width, height);
  }

  void Window::CreateWindow() {
    window_ = glfwCreateWindow(width_window_, height_window_, window_name_.data(), NULL, NULL);
    if (window_ == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        // return -1;
    }

    glfwMakeContextCurrent(window_);
    
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
    }  
  }

  GLFWwindow* Window::GetPointer() const {
      return window_;
  }

  int Window::GetWidth() const noexcept {
      return width_window_;
  }

  int Window::GetHeight() const noexcept {
      return height_window_;
  }

  void Window::DeltaTimeUpdate() noexcept {
      GLfloat current_frame = glfwGetTime();
      delta_time_ = current_frame - last_time_;
      last_time_ = current_frame;
  }

  GLfloat Window::GetDeltaTime() const noexcept {
      return delta_time_;
  }

  Window::CoordinateOffset Window::GetOffsetController() noexcept {
      return controller_.GetOffset();
  }

  bool Window::IsDown(int key) const noexcept {
      return controller_.IsDown(key);
  }

  bool Window::WasPressedKey(int key) const noexcept {
      return controller_.WasPressed(key);
  }

  bool Window::WasReleasedKey(int key) const noexcept {
      return controller_.WasReleased(key);
  }

  void Window::FramePerSecond() noexcept {
      static int n_frames = 0;
      double current_time = glfwGetTime();
      n_frames++;

      if (current_time - m_profiler.frame_state.m_lt >= 1.0) {
          m_profiler.frame_state.m_frames = n_frames;
          m_profiler.frame_state.m_delay = 1000.0 / static_cast<double>(n_frames);
          n_frames = 0;    
          m_profiler.frame_state.m_lt++;
      }
  }

  void Window::GetLastPosCursor() noexcept
  {
    m_profiler.cursor_pos.x_px_cursor = controller_.GetLastXposCursor();
    m_profiler.cursor_pos.y_px_cursor = controller_.GetLastYposCursor();
              
    m_profiler.cursor_pos.x_ndc_cursor = 2 * m_profiler.cursor_pos.x_px_cursor / width_window_ - 1;
    m_profiler.cursor_pos.y_ndc_cursor = 1 - 2 * m_profiler.cursor_pos.y_px_cursor / height_window_;
  }

  Window::WindowSize Window::GetSize() const noexcept 
  {
    return {width_window_, height_window_};
  }
} // namespace avion::core
