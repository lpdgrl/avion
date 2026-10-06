#include "AvionEngineCore/api/backend/backend.hpp"

namespace avion::api::backend
{
  Backend::Backend(RenderAPI api, ResManager& resman, RenderStatistics& rnd_stat)
  : m_resman(resman)
  , m_api(api)
  , m_render_stat(rnd_stat)
  {

  }

  void Backend::Init(RenderState render_state)
  {
    AV_LOG_DEBUG("Backend::Init()");
    if (m_api == RenderAPI::kOpengl)
    {
      auto font_path = m_resman.GetResource<ResManager::FsPath>("dejavusans.ttf");
      if (!font_path.has_value())
      {
        assert(false);
      }
      m_renderer = std::make_unique<opengl::OpenglRenderer>(m_shader_storage);
      m_renderer->Init(render_state, font_path.value()->c_str());
      m_current_state = render_state;
      m_default_state = render_state;
    }
    else if (m_api == RenderAPI::kVulkan)
    {
      AV_LOG_TODO("Backend::Init(): Vulkan API plug!");
      return;
    }
    CompileAndLoadShaders();
  }

  void Backend::ApplyDefaultRenderState()
  {
    m_current_state = m_default_state;
    m_renderer->ApplyCurrentState(m_current_state);
  }

  Backend::ModelHandler Backend::CreateModel(CpuModelData& cpu_model_data)
  {
    ModelHandler handler;
    detail::MeshData data 
    {
      .vertices = std::as_bytes(std::span(cpu_model_data.vertices)),
      .vertices_size = cpu_model_data.vertices.size(),
      .indices = std::span(cpu_model_data.indices),
      .indices_size = cpu_model_data.indices.size(),
      .stride_size = sizeof(VertexModel),
      .offset_position = offsetof(VertexModel, position),
      .offset_normals = offsetof(VertexModel, normal),
      .offset_tex_coords = offsetof(VertexModel, tex_coords),
      .offset_bone_ids = offsetof(VertexModel, bone_ids),
      .offset_weights = offsetof(VertexModel, weights)
    };
    handler.id = m_renderer->CreateBuffer(data);
    return handler;
  }

  Backend::TextureHandler Backend::CreateTexture2D(const TextureData& data)
  {
    TextureHandler handle;
    handle.id = m_renderer->CreateTexture2D(
      {
        .color_channels = data.color_channels,
        .width = data.width,
        .height = data.height,
        .buffer{std::bit_cast<std::byte*>(data.buffer), data.buffer_size}
      }
    );

    return handle;
  }

  void Backend::CreateFrameBuffer(const std::string& name, float width, float height)
  {
    m_renderer->CreateFrameBuffer(name, width, height);
  }

  void Backend::BeginFrame()
  {
    if (m_is_dirty_state)
    {
      m_renderer->ApplyCurrentState(m_current_state);
      m_is_dirty_state = false;
    }
    m_renderer->PrepareDraw();
  }

  void Backend::EndFrame()
  {
    // AV_LOG_TODO("Backend::EndFrame(): TO DO NOTHING")
    Draw();
  }

  void Backend::UpdateFrame()
  {

  }

  void Backend::Draw()
  {
    while (!m_renderable_queue.empty())
    {
      auto&& item = m_renderable_queue.front();
      auto render_stat = m_renderer->Draw(item);
      m_renderable_queue.pop_front();
      // Getting rendering stat
      m_render_stat.Update(std::get<0>(render_stat), std::get<1>(render_stat), std::get<2>(render_stat));
    }
  }

  auto Backend::DrawTiles(const std::vector<RenderItem2D>& items) -> void
  {
    for (const auto& item : items)
    {
      m_renderer->DrawItem2D(item);
    }
  }

  auto Backend::DrawText(const std::string& text, float x, float y, float scale, glm::vec3 color) -> void
  {
    RenderState state = m_current_state;
    state.depth_state.enabled = false;
    ApplyRenderState(state);

    m_renderer->DrawText(text, x, y, scale, color);

    state.depth_state.enabled = true;
    ApplyRenderState(state);
  }

  auto Backend::ApplyRenderState(const RenderState& state) const noexcept -> void
  {
    m_renderer->ApplyCurrentState(state);
  }

  void Backend::SubmitRenderableItem(RenderItem item) noexcept
  {
    m_renderable_queue.emplace_back(std::move(item));
  }

  auto Backend::ReloadChangedShaders(const std::vector<EventChangedFiles>& events) noexcept -> void
  {
    if (events.empty())
    {
      AV_LOG_DEBUG(std::format("Backend::ReloadChangedShaders: vector of event changed files is empty"));
      return;
    }
    for (const auto& event : events)
    {
      std::string name_shader(event.path.filename());
      std::size_t pos = name_shader.find('.');
      if (pos == std::string::npos)
      {
        continue;
      }
      name_shader = name_shader.substr(0, pos);
      // AV_LOG_DEBUG("Backend::ReloadChangedShaders: " + name_shader);
      m_shader_storage.ReloadShader(name_shader, event.path);
    }
  }

  void Backend::CompileAndLoadShaders()
  {
    auto& shaders = m_resman.GetShaderPaths();
    // TODO: Initialization shaders and register them
    std::string simple_light("simple_light");
    std::string model("model");
    std::string select_single_color("select_single_color");
    std::string select_single_model("select_single_model");
    std::string grass("grass");
    std::string normals("normals");
    std::string blocks("block");
    std::string text("text");

    m_shader_storage.RegisterShader(
      blocks,
      m_resman.GetResource<ResManager::FsPath>("block.vert"),
      m_resman.GetResource<ResManager::FsPath>("block.frag")
    );

    m_shader_storage.RegisterShader(
      grass,
      m_resman.GetResource<ResManager::FsPath>("grass.vert"),
      m_resman.GetResource<ResManager::FsPath>("grass.frag")
    );

    m_shader_storage.RegisterShader(
      select_single_model,
      m_resman.GetResource<ResManager::FsPath>("select_single_model1.vert"),
      m_resman.GetResource<ResManager::FsPath>("select_single_model.frag")
    );

    m_shader_storage.RegisterShader(
      select_single_color,
      m_resman.GetResource<ResManager::FsPath>("select_single_color.vert"),
      m_resman.GetResource<ResManager::FsPath>("select_single_color.frag")
    );

    m_shader_storage.RegisterShader(
      model,
      m_resman.GetResource<ResManager::FsPath>("model.vert"),
      m_resman.GetResource<ResManager::FsPath>("model.frag")
    );

    m_shader_storage.RegisterShader(
      simple_light,
      m_resman.GetResource<ResManager::FsPath>("simple_light.vert"),
      m_resman.GetResource<ResManager::FsPath>("simple_light.frag")
    );

    m_shader_storage.RegisterShader(
      normals,
      m_resman.GetResource<ResManager::FsPath>("normals.vert"),
      m_resman.GetResource<ResManager::FsPath>("normals.frag")
    );

    m_shader_storage.RegisterShader(
      text,
      m_resman.GetResource<ResManager::FsPath>("text.vert"),
      m_resman.GetResource<ResManager::FsPath>("text.frag")
    );
  }

  // Common state
  bool Backend::SetViewportState(ViewportState viewport) noexcept
  {
    if (viewport == m_current_state.viewport_state)
    {
      return false;
    }

    m_current_state.viewport_state = viewport;
    m_is_dirty_state = true;
    return m_is_dirty_state;
  }

  // Color buffer
  bool Backend::SetColorState(BackgroundColor bg_color) noexcept
  {
    if (bg_color == m_current_state.color_state)
    {
      return false;
    }

    m_current_state.color_state = bg_color;
    m_is_dirty_state = true;
    return true;
  }

  // Depth buffer
  bool Backend::SetDepthState(DepthState state) noexcept
  {
    if (state == m_current_state.depth_state)
    {
      return false;
    }

    m_current_state.depth_state = state;
    m_is_dirty_state = true;
    return m_is_dirty_state;
  }

  // Stencil buffer
  bool Backend::SetStencilState(StencilState state) noexcept
  {
    if (state == m_current_state.stencil_state)
    {
      return false;
    }

    m_current_state.stencil_state = state;
    m_is_dirty_state = true;
    return m_is_dirty_state;
  }

  // Blending state
  bool Backend::SetBlendingState(BlendState state) noexcept
  {
    if (state == m_current_state.blend_state)
    {
      return false;
    }

    m_current_state.blend_state = state;
    m_is_dirty_state = true;
    return m_is_dirty_state;
  }

  std::string Backend::GetNameApi() const noexcept
  {
    return detail::ApiToString(m_api);
  }

  auto Backend::SetOrthoProjection(const detail::OrthoProjection& ortho) const noexcept -> void
  {
    m_renderer->SetOrthoProjection(ortho);
  }

  auto Backend::SetPerspectiveProjection(detail::PerspectiveProjection perspective) noexcept -> void
  {
    m_renderer->SetPerspectiveProjection(perspective);
  }
} // namespace avion::api::backend

namespace avion::api::backend::detail
{

  std::string ApiToString(RenderAPI api) noexcept
  {
    std::string str_api;
    switch (api)
    {
      case RenderAPI::kOpengl:
      {
        str_api.append("Opengl");
        break;
      }
      case RenderAPI::kVulkan:
      {
        str_api.append("Vulkan");
        break;
      }
      case RenderAPI::kUnknown:
      {
        str_api.append("Unknown");
        break;
      }
    }
    return str_api;
  }

} // namespace avion::api::backend::detail
