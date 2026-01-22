#include "AvionEngineCore/core/window.hpp"

namespace avion::core {

    Window::Window(const std::string& window_name, int width, int height)
        : window_name_(window_name)
        , width_window_(width)
        , height_window_(height)
        , controller_(controller::Controller((1.0 * width / 2), (1.0 * height / 2))) 
        , scene_(Scene(kObjectsCreate))
        {}

    Window::Window(const char* window_name, int width, int height)
        : window_name_(window_name)
        , width_window_(width)
        , height_window_(height)
        , controller_(controller::Controller((1.0 * width / 2), (1.0 * height / 2)))
        , scene_(Scene(kObjectsCreate))
        {}

    Window::~Window() {
        delete render_;
        delete widget_;
        if (window_) {
            glfwDestroyWindow(window_);
            glfwTerminate();
            std::cout << "Window is destroyed" << '\n';
        }
    }

    void Window::Init() {
        render_ = new gfx::Renderer(this);

        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        CreateWindow();

        glfwSetFramebufferSizeCallback(window_, FrameBufferSizeCallback);
        // glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glfwSetWindowUserPointer(window_, &controller_);
        glfwSetKeyCallback(window_, controller::Controller::KeyCallback);
        glfwSetCursorPosCallback(window_, controller::Controller::MouseCallback);
        glfwSetMouseButtonCallback(window_, controller::Controller::MouseButtonCallback);
        glfwSetCursorPos(window_, width_window_, height_window_ / 2);

        render_->Init();
        render_->SetPerspectiveProjection(45.f, width_window_, height_window_, 0.1f, 50.f);

        ImGui::CreateContext();

        widget_ = new gui::Widget(window_);
        widget_->Init();
    }

    void Window::Update() {
        Render();
    }

    void Window::Render() {
        gfx::Shader* shader = render_->GetShaderPtr("object");
        gfx::Shader* shader_ligth = render_->GetShaderPtr("ligth");

        glm::vec3 color{1.f, 1.f, 1.f};
        glm::vec3 position{};
        glm::vec3 size{1.f, 1.f, 1.f};

        float scr_aspect = height_window_ / width_window_;
        float kAmbient = 0.1f;
        float kSpecular = 0.5f;
        float kShininess = 32.0f;

        bool state_button_addobject = false;

        int selected_object_id = 0;
        bool pickup_obj = false;
        int id_pickup = 0;
        TypeObject type_obj = TypeObject::kCube;

        double lt_ = glfwGetTime();

        while (!glfwWindowShouldClose(window_)) {
            DeltaTimeUpdate();

            GLfloat color_buffer[3];
            GLfloat depth;

            double x_px = controller_.GetLastXposCursor();
            double y_px = controller_.GetLastYposCursor();

            // Why height - y_px - 1?
            glReadPixels(x_px, height_window_ - y_px - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
            glReadPixels(x_px, height_window_ - y_px - 1, 1, 1, GL_RGB, GL_FLOAT, color_buffer);
            std::cout << "Clicked on pixel: " << std::setprecision(2) << x_px << " " << y_px << " " << color_buffer[0] << " " << color_buffer[1] << " " << color_buffer[2] << " Depth: " << depth << '\n';

            auto obj_coord_pickup = render_->PickUpObject(x_px, y_px, width_window_, height_window_, depth);
            
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

            widget_->WidgetShader(kSpecular, kAmbient, kShininess);

            render_->UpdateCoordinatesCamera(delta_time_);
            render_->Update();

            glClearColor(0.2f, 0.2f, 0.2f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            auto&& objects = scene_.GetAllObjects();

            if (controller_.IsDownMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
                auto pickup_it = std::find_if(objects.begin(), objects.end(), [&](core::SceneObject& scene_object) {
                auto [pz, sz, _, mixing_color] = scene_object.object.GetParams();

                    return pz.x + (sz.x / 2) >= obj_coord_pickup.x 
                        && pz.x - (sz.x / 2) <= obj_coord_pickup.x 
                        && depth < 1.0
                        && pz.y + (sz.y / 2) >= obj_coord_pickup.y
                        && pz.y - (sz.y / 2) <= obj_coord_pickup.y; 
                });

                if (auto&& obj = pickup_it->object; pickup_it != objects.end()) {
                    pickup_it->object.SetMixingColor(glm::vec3(0.96f, 0.25f, 0.94f));

                    id_pickup = scene_.GetNumberObjects() - 1;
                    pickup_obj = true;

                    std::cout << "Do find pickup!!!" << '\n';
                } else { std::cout << "Doesn't find pickup!!!" << '\n'; pickup_obj = false;}
            }

            selected_object_id = widget_->WindowListObjects(scene_.GetAllObjects());

            size_t objects_size = scene_.GetNumberObjects();
            if (objects_size != 0 && selected_object_id > 0) {
                Object* object = scene_.GetObject(pickup_obj ? id_pickup : selected_object_id);
                auto [ps, sz, _color, _] = object->GetParams();
                widget_->WindowAddObject(type_obj, ps, sz, _color, state_button_addobject);
                object->SetParams(ps, sz, _color);
            } else {
                widget_->WindowAddObject(type_obj, position, size, color, state_button_addobject);
            }
            
            if (state_button_addobject) {
                scene_.AddObjectToScene(type_obj, position, size, color);
            }

            for (const auto& object : objects) {
                auto type = object.type;
                auto& self_obj = object.object;
                auto [pz, sz, cl, _] = self_obj.GetParams();

                if (type == TypeObject::kLight) {
                    shader_ligth->use();
                    shader_ligth->setVec3("ligthColor", cl);
                    render_->Draw(shader_ligth, pz, sz, gfx::AxisRotate::NONE, 0.f, gfx::MapKey::kLight);
                    continue;
                }
                
                glm::vec3 l_pos;
                glm::vec3 c_light;

                Object* light = scene_.GetObject(TypeObject::kLight);
                if (light != nullptr) {
                    l_pos = static_cast<glm::vec3>(light->GetPosition());
                    c_light = static_cast<glm::vec3>(light->GetColor());
                }
                
                glm::vec3 mix_color = cl * self_obj.GetMixingColor().color;
                shader->use();
                shader->setFloat("kAmbient", kAmbient);
                shader->setFloat("kSpecular", kSpecular);
                shader->setFloat("kShininess", kShininess);
                shader->setVec3("objectColor", mix_color);
                shader->setVec3("ligthColor", c_light);
                shader->setVec3("ligthPos", l_pos);
                shader->setFloat("scr_aspect", scr_aspect);

                render_->Draw(shader, pz, sz, gfx::AxisRotate::NONE, 0.f, static_cast<gfx::MapKey>(type));
            }

            widget_->Render();

            glfwSwapBuffers(window_);
            // TODO: Undestand when call ClearStateKeys and How it works - glfwPollEvents
            controller_.ClearStateKeys();
            glfwPollEvents();
            FramePerSecond();
        }
    }

    void Window::ProcessEvents() {

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