#include "AvionEngineCore/render/window.hpp"

Window::Window(const std::string& window_name, int width, int height)
    : window_name_(window_name)
    , width_window_(width)
    , height_window_(height)
    , controller_(Controller((1.0 * width / 2), (1.0 * height / 2))) 
    , scene_(Scene(kObjectsCreate))
    {}

Window::Window(const char* window_name, int width, int height)
    : window_name_(window_name)
    , width_window_(width)
    , height_window_(height)
    , controller_(Controller((1.0 * width / 2), (1.0 * height / 2)))
    , scene_(Scene(kObjectsCreate))
    {}

Window::~Window() {
    delete render_;
    delete gui_;
    if (window_) {
        glfwDestroyWindow(window_);
        glfwTerminate();
        std::cout << "Window is destroyed" << '\n';
    }
}

void Window::Init() {
    render_ = new Renderer(this);

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
    glfwSetKeyCallback(window_, Controller::KeyCallback);
    glfwSetCursorPosCallback(window_, Controller::MouseCallback);
    glfwSetCursorPos(window_, width_window_, height_window_ / 2);

    render_->Init();
    render_->SetPerspectiveProjection(45.f, width_window_, height_window_, 0.1f, 50.f);

    ImGui::CreateContext();

    gui_ = new Gui(window_);
    gui_->Init();
}

void Window::Update() {
    Render();
}

void Window::Render() {
    glm::vec3 sz{1.f, 1.f, 1.f};
    glm::vec3 clr{0.f, 0.f, 0.f};
    
    glm::vec3 size_other{0.5f, 0.5f, 0.5f};
    glm::vec3 objectColor{1.0f, 0.5f, 0.31f};
    glm::vec3 colorLigth{1.0f, 1.0f, 1.0f};
    glm::vec3 ligth_position{-1.3f,  -0.5f, -1.f};
    glm::vec3 view_pos{0.f, 0.f, 0.f};

    Shader* shader = render_->GetShaderPtr("object");
    Shader* shader_ligth = render_->GetShaderPtr("ligth");

    glm::vec3 color;
    glm::vec3 position;
    glm::vec3 size;

    bool state_button_addobject = false;

    while (!glfwWindowShouldClose(window_)) {
        controller_.ClearStateKeys();
        glfwPollEvents();
        ProcessEvents();
        DeltaTimeUpdate();

        gui_->Frame();
        
        render_->UpdateCoordinatesCamera(delta_time_);
        render_->Update();

        ligth_position.x += sin(glfwGetTime()) * delta_time_ * 0.5f;
        ligth_position.y += sin(glfwGetTime()) * delta_time_ * 0.5f;
        ligth_position.z += sin(glfwGetTime()) * delta_time_ * 2.5f;

        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto& objects = scene_.GetAllObjects();
        if (objects.size() != 0) {
            auto [ps, sz, _] = objects.back().GetParams();
            gui_->WindowAddObject(ps, sz, color, state_button_addobject);
            Object& obj = objects[objects.size() - 1];
            obj.SetPosition(ps);
            obj.SetSize(sz);
        } else {
            gui_->WindowAddObject(position, size, color, state_button_addobject);
        }
        
        gui_->WindowLigthColor(colorLigth);
        
        if (state_button_addobject) {
            scene_.AddObjectToScene(position, size, color);
        }

        gui_->WindowListObjects(scene_.GetAllObjects());

        for (const auto& object : scene_.GetAllObjects()) {
            auto [position, size, cl] = object.GetParams();

            render_->SetLigth(shader, colorLigth, cl);
            shader->setVec3("ligthPos", ligth_position);
            shader->setVec3("view_pos", view_pos);

            render_->Draw(shader, position, size, AxisRotate::NONE, 0.f, MapKey::OBJECTS);
        }
        
        shader_ligth->setVec3("ligthColor", colorLigth);
        render_->Draw(shader_ligth, ligth_position, size_other, AxisRotate::AXIS_X, 10.f, MapKey::LIGHT);

        gui_->Render();

        glfwSwapBuffers(window_);
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
    delta_time_ = current_frame - last_frame_;
    last_frame_ = current_frame;
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