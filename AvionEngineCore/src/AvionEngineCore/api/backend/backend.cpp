#include "AvionEngineCore/api/backend/backend.hpp"

namespace avion::api::backend
{
  Backend::Backend(RenderAPI api, ResManager& resman)
  : m_resman(resman)
  , m_api(api)
  {

  }

  void Backend::Init(RenderState render_state)
  {
    AV_LOG_DEBUG("Backend::Init()");
    if (m_api == RenderAPI::kOpengl)
    {
      m_renderer = std::make_unique<opengl::OpenglRenderer>(m_shader_storage);
      m_renderer->Init(render_state);
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

  Backend::ModelHandler Backend::CreateModel(ModelData& model_data)
  {
    ModelHandler handler;
    handler.id = m_renderer->CreateBuffer(
      {
        .vertices{std::bit_cast<std::byte*>(model_data.vertices.data()), model_data.vertices.size()},
        .vertices_size{model_data.vertices.size()},
        .indices{model_data.indices},
        .indices_size{model_data.indices.size()},
        .stride{sizeof(VertexModel)},
        .offset_position{0},
        .offset_normals{offsetof(VertexModel, normal)},
        .offset_tex_coords{offsetof(VertexModel, tex_coords)}
      }
    );
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
      m_renderer->Draw(item);
      m_renderable_queue.pop_front();
    }
  }

  void Backend::SubmitRenderableItem(RenderItem item) noexcept
  {
    m_renderable_queue.emplace_back(std::move(item));
  }

  void Backend::CompileAndLoadShaders()
  {
    // TODO: Initialization shaders and register them
    std::string simple_cube("lighting");
    std::string shader_model_light_source("shader_model_light_source");
    std::string frag_model("model");
    std::string select_single_object("single_object");
    std::string select_single_model("single_model");
    std::string grass("grass");
    
    m_shader_storage.RegisterShader(
      grass,
      m_resman.GetResource<ResManager::FsPath>("grass.vert")->c_str(),
      m_resman.GetResource<ResManager::FsPath>("grass.frag")->c_str()
    );
    
    m_shader_storage.RegisterShader(
      select_single_model,
      m_resman.GetResource<ResManager::FsPath>("select_single_model.vert")->c_str(),
      m_resman.GetResource<ResManager::FsPath>("select_single_model.frag")->c_str()
    );

    m_shader_storage.RegisterShader(
      select_single_object,
      m_resman.GetResource<ResManager::FsPath>("select_single_color.vert")->c_str(),
      m_resman.GetResource<ResManager::FsPath>("select_single_color.frag")->c_str()
    );

    m_shader_storage.RegisterShader(
      frag_model,
      m_resman.GetResource<ResManager::FsPath>("vert_model.vert")->c_str(),
      m_resman.GetResource<ResManager::FsPath>("frag_model.frag")->c_str()
    );

    m_shader_storage.RegisterShader(
      shader_model_light_source,
      m_resman.GetResource<ResManager::FsPath>("simple_light_transform.vert")->c_str(),
      m_resman.GetResource<ResManager::FsPath>("simple_light_color.frag")->c_str()
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