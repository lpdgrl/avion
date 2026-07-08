#include "AvionEngineCore/api/opengl/opengl_renderer.hpp"

namespace avion::api::backend::opengl
{

  OpenglRenderer::OpenglRenderer(ShaderStorage& shader_storage)
  : m_shader_storage(shader_storage)
  {
  }

  void OpenglRenderer::Init(RenderState state)
  {
    m_depth_state     = state.depth_state;
    m_stencil_state   = state.stencil_state;
    m_color_state     = state.color_state;
    m_viewport_state  = state.viewport_state;
    m_blend_state     = state.blend_state;

    ApplyDepthState();
    ApplyStencilState();
    ApplyBlendingState();
  }

  // Change current opengl state
  void OpenglRenderer::ApplyCurrentState(RenderState& render_state) noexcept
  {
    // ApplyDepthState();
    // ApplyStencilState();
    // ApplyBlendingState();
    // m_color_state = render_state.color_state;
    // ApplyViewportState();
    if (render_state.depth_state != m_depth_state)
    {
      m_depth_state = render_state.depth_state;
      ApplyDepthState();
    }

    if (render_state.stencil_state != m_stencil_state)
    {
      m_stencil_state = render_state.stencil_state;
      ApplyStencilState();
    }

    if (render_state.blend_state != m_blend_state)
    {
      m_blend_state = render_state.blend_state;
      ApplyBlendingState();
    }

    if (render_state.color_state != m_color_state)
    {
      m_color_state = render_state.color_state;
    }

    if (render_state.viewport_state != m_viewport_state)
    {
      m_viewport_state = render_state.viewport_state;
      ApplyViewportState();
    }
  }

  void OpenglRenderer::PrepareDraw() const noexcept
  {
    auto [red, blue, green, alpha] = m_color_state;
    glClearColor(red, blue, green, alpha);
    // TODO: Clear buffers only when he enabled
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  }

  void OpenglRenderer::Draw(const RenderItem& item) const noexcept
  {
    using RenderOption = api::backend::detail::RenderOption;

    auto&& buffer_id = item.model_handler.id;
    auto&& transform = item.transform;
    auto&& mesh_range = item.mesh_range;
    auto&& material_range = item.material_range;
    auto&& view_matrix = item.view_matrix;
    auto&& view_position = item.view_position;
    auto&& model_matrix = transform.GetMatrix();
    auto&& solid_color = item.solid_color;
    auto&& render_option = item.render_item_option;

    m_shader_storage.PutData("model", "material_type.is_texture", 
      render_option == RenderOption::kSolidColorMaterial ? false : true
    );

    if (render_option == RenderOption::kSolidColorMaterial) 
    {
      m_shader_storage.PutData("model", "solid_color", solid_color);
      m_shader_storage.PutData("model", "material.fl_shininess", 32.f);
    }

    m_shader_storage.PutData("model", "view", view_matrix);
    m_shader_storage.PutData("model", "view_pos", view_position);
    m_shader_storage.PutData("model", "model", model_matrix);
    m_shader_storage.UseShader("model");

    auto it = m_buffer_storage.find(buffer_id);
    assert(!(it == m_buffer_storage.end()) && "OpenglRenderer::Draw(RenderItem item): buffer id isn't exists");

    if (render_option == RenderOption::kTextureMaterial)
    {
      BindTexture2D(material_range);
    }
   
    auto&& buffer = it->second;
    glBindVertexArray(buffer.GetIdVao());

    for (auto m : mesh_range)
    {
      glDrawElements(GL_TRIANGLES, m.index_count, GL_UNSIGNED_INT, 
        std::bit_cast<void*>(m.first_index * sizeof(std::uint32_t))
      );
    }
    
    if (render_option == RenderOption::kTextureMaterial)
    {
      UnBindTexture2D(material_range);
    }
    glBindVertexArray(0);
  }

  void OpenglRenderer::BindTexture2D(const MaterialRange& range) const noexcept
  {
    int i = 1;
    for (auto& material : range)
    {
      glActiveTexture(GL_TEXTURE0 + i);

      std::string name_texture("material.");

      switch(material.type)
      {
        case TextureType::kDiffuse:
        {
          name_texture.append("diffuse");
          name_texture.append(std::to_string(i));
          break;
        }
        case TextureType::kSpecular:
        { 
          name_texture.append("specular");
          name_texture.append(std::to_string(i));
          break;
        }
        case TextureType::kEmission:
        {
          name_texture.append("emission");
          name_texture.append(std::to_string(i));
          break;
        }
      }

      m_shader_storage.PutData("model", name_texture, i++);
      m_shader_storage.ExecuteAfterUse("model");
      m_texture2d_storage.find(material.id)->second.Bind();
    }
  }

  void OpenglRenderer::UnBindTexture2D(const MaterialRange& range) const noexcept
  {
    for (auto& material : range)
    {
      m_texture2d_storage.find(material.id)->second.UnBind();
    }
  }

  OpenglRenderer::BufferId OpenglRenderer::CreateBuffer(MeshData mesh_data)
  {
    auto [it, _] = m_buffer_storage.emplace(++m_last_buffer_id, std::move(OpenglBuffer()));
    auto& buffer = it->second;

    buffer.SetupBuffer(mesh_data);
    return it->first;
  }

  OpenglRenderer::Texture2dId OpenglRenderer::CreateTexture2D(Texture2dData data)
  {
    auto [it, _] = m_texture2d_storage.emplace(++m_last_texture_id, std::move(OpenglTexture2D()));
    auto& texture2d = it->second;

    texture2d.SetupTexture(data);
    return it->first;
  }

  void OpenglRenderer::CreateFrameBuffer(const std::string& name, float width, float height)
  {
    auto [it, result] = m_framebuffer_storage.try_emplace(name, width, height);
    auto& framebuffer = it->second;

    framebuffer.SetupFrameBuffer();
  }

  void OpenglRenderer::ApplyDepthState() const noexcept
  {
    if (!m_depth_state.enabled)
    {
      glDisable(GL_DEPTH_TEST);
      return;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthMask(m_depth_state.depth_mask);
    glDepthFunc(detail::ToOpenglCompareFunc(m_depth_state.depth_func));
  }

  void OpenglRenderer::ApplyStencilState() const noexcept
  {
    if (!m_stencil_state.enabled)
    {
      glDisable(GL_STENCIL_TEST);
      return;
    }

    auto [_, stencil_func,
      stencil_fail, stencil_depth_fail, stencil_depth_pass,
      stencil_ref, stencil_func_mask, stencil_mask] = m_stencil_state;

    glEnable(GL_STENCIL_TEST);
    glStencilOp(
      detail::ToOpenglStencilAction(stencil_fail),
      detail::ToOpenglStencilAction(stencil_depth_fail),
      detail::ToOpenglStencilAction(stencil_depth_pass)
    );
    glStencilFunc(
      detail::ToOpenglCompareFunc(stencil_func),
      stencil_ref,
      stencil_func_mask
    );
    glStencilMask(stencil_mask);
  }

  void OpenglRenderer::ApplyBlendingState() const noexcept
  {
    if (!m_blend_state.enabled)
    {
      glDisable(GL_BLEND);
      return;
    }

    auto [_, blend_source_factor, blend_destination_factor, bledn_equation] = m_blend_state;
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  void OpenglRenderer::ApplyViewportState() const noexcept
  {
    glViewport(
      m_viewport_state.x_viewport, 
      m_viewport_state.y_viewport, 
      m_viewport_state.width_viewport,
      m_viewport_state.height_viewport
    );
  }

  void OpenglRenderer::ApplyColorState() const noexcept
  {

  }

  void OpenglRenderer::SetOrthoProjection(OrthoProjection& projection) noexcept
  {
    m_projection = glm::ortho(projection.left,
      projection.width, projection.bottom, projection.height, projection.z_near, projection.z_far);

    SubmitProjectionMatrixToShader();
  }

  void OpenglRenderer::SetPerspectiveProjection(PerspectiveProjection& projection) noexcept
  {
    m_projection = glm::perspective(glm::radians(projection.fov),
      static_cast<float>(projection.width) / static_cast<float>(projection.height), projection.near, projection.far);

    SubmitProjectionMatrixToShader();
  }

  void OpenglRenderer::SubmitProjectionMatrixToShader() noexcept
  {
    for (const auto& [_, shader] : m_shader_storage.GetStorage())
    {
      shader->PutData("projection", m_projection);
    }
  }

  OpenglFrameBuffer& OpenglRenderer::GetFrameBuffer(const std::string name)
  {
    auto it = m_framebuffer_storage.find(name);
    // assert(it != m_framebuffer_storage.end() && "OpenglRenderer::GetFrameBuffer: framebuffer isn't exists!");
    return it->second;
  }

} // namespace avion::api::backend::opengl

namespace avion::api::backend::opengl::detail
{
  GLenum ToOpenglCompareFunc(backend::detail::CompareFunc func)
  {
    namespace bkd = backend::detail;

    switch (func)
    {
      case bkd::CompareFunc::Always:
      {
        break;
      }
      case bkd::CompareFunc::Equal:
      {
        return GL_EQUAL;
      }
      case bkd::CompareFunc::Gequal:
      {
        return GL_GEQUAL;
      }
      case bkd::CompareFunc::Greater:
      {
        return GL_GREATER;
      }
      case bkd::CompareFunc::Lequal:
      {
        return GL_LEQUAL;
      }
      case bkd::CompareFunc::Less:
      {
        return GL_LESS;
      }
      case bkd::CompareFunc::Never:
      {
        return GL_NEVER;
      }
      case bkd::CompareFunc::Notequal:
      {
        return GL_NOTEQUAL;
      }
    }
    return GL_ALWAYS;
  }

  GLenum ToOpenglStencilAction(backend::detail::StencilAction action)
  {
    namespace bkd = backend::detail;

    switch (action)
    {
      case bkd::StencilAction::Zero:
      {
        return GL_ZERO;
      }
      case bkd::StencilAction::Replace:
      {
        return GL_REPLACE;
      }
      case bkd::StencilAction::Keep:
      {
        break;
      }
      case bkd::StencilAction::Invert:
      {
        return GL_INVERT;
      }
      case bkd::StencilAction::IncrementWrap:
      {
        return GL_INCR_WRAP;
      }
      case bkd::StencilAction::Increment:
      {
        return GL_INCR;
      }
      case bkd::StencilAction::DecrementWrap:
      {
        return GL_DECR_WRAP;
      }
      case bkd::StencilAction::Decrement:
      {
        return GL_DECR;
      }
    }
    return GL_KEEP;
  }
} // namespace avion::api::backend::opengl
