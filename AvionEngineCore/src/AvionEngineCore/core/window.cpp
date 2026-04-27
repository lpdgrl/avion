#include "AvionEngineCore/core/window.hpp"

namespace avion::core {
  Window::Window(const std::string& window_name, int width, int height)
    : window_name_(window_name)
    , width_window_(width)
    , height_window_(height)
    , controller_(controller::Controller((1.0 * width / 2), (1.0 * height / 2))) 
    , m_resman(std::make_unique<resman::ResourceManager>(std::filesystem::canonical("/proc/self/exe").c_str()))
    , m_pl_queue(std::make_unique<gfx::PipelineQueue>())
    , scene_(Scene(kObjectsCreate, *m_resman.get(), *m_pl_queue.get()))
    {}

  Window::Window(const char* window_name, int width, int height)
    : window_name_(window_name)
    , width_window_(width)
    , height_window_(height)
    , controller_(controller::Controller((1.0 * width / 2), (1.0 * height / 2)))
    , m_resman(std::make_unique<resman::ResourceManager>(std::filesystem::canonical("/proc/self/exe").c_str()))
    , m_pl_queue(std::make_unique<gfx::PipelineQueue>())
    , scene_(Scene(kObjectsCreate, *m_resman.get(), *m_pl_queue.get()))
    {}

  Window::~Window() {
      delete widget_;
      delete pipeline_;
      
      if (window_) {
          glfwDestroyWindow(window_);
          glfwTerminate();
          AV_LOG_INFO("Window is destroyed");
      }
  }

  void Window::Init() {
      glfwInit();

      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

      CreateWindow();

      glfwSetFramebufferSizeCallback(window_, FrameBufferSizeCallback);
      // glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

      glfwSetWindowUserPointer(window_, &controller_);
      glfwSetKeyCallback(window_, controller::Controller::KeyCallback);
      glfwSetCursorPosCallback(window_, controller::Controller::MouseCallback);
      glfwSetMouseButtonCallback(window_, controller::Controller::MouseButtonCallback);
      glfwSetCursorPos(window_, width_window_, 1.0 * height_window_ / 2.0);
      
      pipeline_ = new gfx::Pipeline(scene_, *m_resman.get(), *m_pl_queue.get());
      pipeline_->Init(width_window_, height_window_);

      ImGui::CreateContext();   
      widget_ = new gui::Widget(window_, *m_resman.get());
      widget_->Init();
  }

  void Window::Update() {
      Render();
  }

  void Window::Render() {
      // float scr_aspect = 1.0 * height_window_ / width_window_;

      int selected_object_id = 0;
      int selected_lights_id = 0;

      bool pickup_obj = false;
      int id_pickup = 0;

      lt_ = glfwGetTime();
      
  while (!glfwWindowShouldClose(window_)) {
          DeltaTimeUpdate();
          ProcessEvents();

          glClearColor(0.2f, 0.2f, 0.2f, 1.f);
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

          GLfloat color_buffer[3];
          GLfloat depth;

          double x_px = controller_.GetLastXposCursor();
          double y_px = controller_.GetLastYposCursor();

          // Why height - y_px - 1?
          // glReadPixels(x_px, height_window_ - y_px - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
          // glReadPixels(x_px, height_window_ - y_px - 1, 1, 1, GL_RGB, GL_FLOAT, color_buffer);
          // std::cout << "Clicked on pixel: " << std::setprecision(2) << x_px << " " << y_px << " " << color_buffer[0] << " " << color_buffer[1] << " " << color_buffer[2] << " Depth: " << depth << '\n';

          // auto obj_coord_pickup = render_->PickUpObject(x_px, y_px, width_window_, height_window_, depth);
          
          double x_ndc = 2 * x_px / width_window_ - 1;
          double y_ndc = 1 - 2 * y_px / height_window_;

          widget_->Frame();
          widget_->WindowLogs({
              .fps = fps_, 
              .delay = delay_,
              .x_px = x_px,
              .y_px = y_px, 
              .x_ndc = x_ndc,
              .y_ndc = y_ndc
          });

          const auto& objects = scene_.GetAllObjects();
          const auto& lights = scene_.GetAllSourceLights();
          
          // This code for ObjectPickup
          // if (controller_.IsDownMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
          //     auto pickup_it = std::find_if(objects.begin(), objects.end(), [&](core::SceneObject& scene_object) {
          //     auto [pz, sz, _, mixing_color] = scene_object.object.GetParams();

          //         return pz.x + (sz.x / 2) >= obj_coord_pickup.x 
          //             && pz.x - (sz.x / 2) <= obj_coord_pickup.x 
          //             && depth < 1.0
          //             && pz.y + (sz.y / 2) >= obj_coord_pickup.y
          //             && pz.y - (sz.y / 2) <= obj_coord_pickup.y; 
          //     });

          //     if (auto& obj = pickup_it->object; pickup_it != objects.end()) {
          //         pickup_it->object.SetMixingColor(glm::vec3(0.96f, 0.25f, 0.94f));

          //         id_pickup = scene_.GetNumberObjects() - 1;
          //         pickup_obj = true;

          //         std::cout << "Do find pickup!!!" << '\n';
          //     } else { std::cout << "Doesn't find pickup!!!" << '\n'; pickup_obj = false;}
          // }

          selected_object_id = widget_->WindowListObjects(objects);
          selected_lights_id = widget_->w_ListLights(lights);
          
          // gfx::ShaderObject& shader_object = pipeline_->GetShaderObjectStruct();
          
          // shader_object.screen_aspect.value = scr_aspect;
          // shader_object.delta.value = std::sin(GetDeltaTime());

          auto opt_result = widget_->w_AddModel();
          if (opt_result.has_value())
          {
            scene_.AddModel(opt_result.value());
          }

          auto added_opt_obj = widget_->WindowAddObject();
          if (added_opt_obj.has_value()) {
              auto obj = added_opt_obj.value();
              scene_.AddObjectToScene(obj.type_obj, obj.params); 
          }
          
          auto added_opt_light = widget_->w_LightAdd(); 
          if (added_opt_light.has_value()) {
              auto type_light = added_opt_light.value();
              scene_.AddSourceLight(type_light);
          }

          if (selected_object_id > 0) {
              Object* object_ptr = scene_.GetObject(selected_object_id);
              if (object_ptr) {
                  auto object_params = object_ptr->GetParams();
                  bool changed_material = widget_->WindowMaterial(object_params);
                  if (changed_material) {
                      object_ptr->SetParams(object_params);
                  }
              } 
          } 

          // TODO: This is bad code. 
          if (selected_lights_id > 0 && lights.size() > 0) {
              // TODO: This is bad access to element of light 
              auto* scene_light = scene_.GetLight(selected_lights_id);

              // TODO: This bad practice transfer raw pointer from unique_ptr. Maybe observer??
              LightParams params{
                .light = scene_light->light.get(),
                .color = scene_light->color,
                .size = scene_light->size
              };


              bool changed_light = widget_->w_LightProperties(params);
          }

          pipeline_->TransferDataToFrameBuffer();

          widget_->Render();

          glfwSwapBuffers(window_);
          
          // TODO: Undestand when call ClearStateKeys and How it works - glfwPollEvents
          controller_.ClearStateKeys();
          glfwPollEvents();
          FramePerSecond();
      }
  }

  void Window::ProcessEvents() {
      if (IsDown(GLFW_KEY_W)) {
          pipeline_->ChangeCameraPosition(gfx::CameraMovement::FORWARD, delta_time_);
      }

      if (IsDown(GLFW_KEY_A)) {
          pipeline_->ChangeCameraPosition(gfx::CameraMovement::LEFT, delta_time_);
      }

      if (IsDown(GLFW_KEY_D)) {
          pipeline_->ChangeCameraPosition(gfx::CameraMovement::RIGHT, delta_time_);
      }

      if (IsDown(GLFW_KEY_S)) {
          pipeline_->ChangeCameraPosition(gfx::CameraMovement::BACKWARD, delta_time_);
      }

      if (WasPressedKey(GLFW_KEY_H)) {
          cursor_state_ = cursor_state_ ? false : true;
      }    

      if (!cursor_state_) {
          auto [xoffset, yoffset] = GetOffsetController();
          pipeline_->ProcessMouseMovement(xoffset, yoffset);
      }

      if (cursor_state_) {
          glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      } else if (!cursor_state_) {
          glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      }
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

      if (current_time - lt_ >= 1.0) {
          fps_ = n_frames;
          delay_ = 1000.0 / static_cast<double>(n_frames);
          n_frames = 0;    
          lt_++;
      }
  }

} // namespace avion::core
