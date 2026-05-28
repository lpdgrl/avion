#include "AvionEngineCore/renderer/pipeline.hpp"
#include "AvionEngineCore/renderer/pipeline_queue.hpp"

#include "AvionEngineCore/core/resource_manager.hpp"

// TODO: THIS IS WRONG!!! VERY WRONGG. THE LEAK ABSTRACTION FROM EDITOR
#include "../../../../AvionEngineEditor/includes/AvionEngineEditor/editor/editor_context.hpp"

namespace avion::gfx {

    Pipeline::Pipeline(core::Scene& scene, core::resman::ResourceManager& resman, PipelineQueue& pl_queue, RenderState& render_state)
    : scene_(scene)
    , m_resman(resman) 
    , m_pl_queue(pl_queue)
    , renderer_(new Renderer(m_shaders_storage, render_state))
    {
      
    }

    Pipeline::~Pipeline() {
        delete renderer_;
        AV_LOG_INFO("Pipeline::~Pipeline: pipeline is destroyed");
    }

    void Pipeline::Init(int width, int height) {
      using resman = core::resman::ResourceManager;

      m_resman.RegisterResource(core::resman::ResourceType::kTexture, "assets/textures");
      m_resman.RegisterResource(core::resman::ResourceType::kShader,  "assets/shaders");
      m_resman.RegisterResource(core::resman::ResourceType::kModel,   "assets/models");
      m_resman.RegisterResource(core::resman::ResourceType::kConfig,  "assets/config");

      // TODO: Initialization shaders and register them
      std::string simple_cube("lighting");
      std::string shader_model_light_source("shader_model_light_source");
      std::string frag_model("model");
      std::string select_single_object("single_object");
      std::string select_single_model("single_model");
      std::string grass("grass");
      
      m_shaders_storage.RegisterShader(
        grass,
        m_resman.GetResource<resman::FsPath>("grass.vert")->c_str(),
        m_resman.GetResource<resman::FsPath>("grass.frag")->c_str()
      );

      m_shaders_storage.RegisterShader(
        select_single_model,
        m_resman.GetResource<resman::FsPath>("select_single_model.vert")->c_str(),
        m_resman.GetResource<resman::FsPath>("select_single_model.frag")->c_str()
      );

      m_shaders_storage.RegisterShader(
        select_single_object,
        m_resman.GetResource<resman::FsPath>("select_single_color.vert")->c_str(),
        m_resman.GetResource<resman::FsPath>("select_single_color.frag")->c_str()
      );

      m_shaders_storage.RegisterShader(
        frag_model,
        m_resman.GetResource<core::resman::ResourceManager::FsPath>("vert_model.vert")->c_str(),
        m_resman.GetResource<core::resman::ResourceManager::FsPath>("frag_model.frag")->c_str()
      );
  
      m_shaders_storage.RegisterShader(
          simple_cube, 
          m_resman.GetResource<core::resman::ResourceManager::FsPath>("simple_cube_transform.vert")->c_str(),
          m_resman.GetResource<core::resman::ResourceManager::FsPath>("lighting.frag")->c_str());

       m_shaders_storage.RegisterShader(
          shader_model_light_source,
          m_resman.GetResource<core::resman::ResourceManager::FsPath>("simple_light_transform.vert")->c_str(),
          m_resman.GetResource<core::resman::ResourceManager::FsPath>("simple_light_color.frag")->c_str());

      renderer_->Init();
      renderer_->SetPerspectiveProjection(45.f, width, height, 0.1f, 100.f);

      // TODO: Initialization texture and get id for them
      auto& list_textures = m_resman.GetListTexture();
      for (const auto& name_texture : list_textures)
      {
        if (auto* p_res = m_resman.GetResource<core::Texture>(name_texture); p_res != nullptr)
        {
          renderer_->LoadTexture2D(p_res->GetId(), p_res->GetWidth(), p_res->GetHeight(), p_res->GetBuffer(), p_res->GetColorChannels());
        }
      }
      
      m_shaders_storage.PutData(simple_cube, "material.s2d_diffuse", 0);
      m_shaders_storage.PutData(simple_cube, "material.s2d_specular", 1);
      m_shaders_storage.PutData(simple_cube, "material.s2d_emission", 2);
      m_shaders_storage.UseShader(simple_cube);

      // renderer_->InitTexture();
    }

    void Pipeline::ChangeCameraPosition(CameraMovement direction, GLfloat delta_time) const noexcept {
        renderer_->ChangeCameraPosition(direction, delta_time);
    }

    void Pipeline::ProcessMouseMovement(double xoffset, double yoffset) const noexcept {
        renderer_->ProcessMouseMovement(xoffset, yoffset);
    }

    void Pipeline::DrawObjects() noexcept 
    {
      const auto& objects_scene = scene_.GetAllObjects();
      const auto& lights_scene = scene_.GetAllSourceLights();
      
      ShaderType_t type_shader_t;

      glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
      glStencilFunc(GL_ALWAYS, 1, 0xFF);

      // TODO: It is strange deal
      for (const auto& [curr_light, id, type_light,  light_color, light_size, is_selectable] : lights_scene) {  

        type_shader_t.name = "shader_model_light_source";
        // AV_LOG_DEBUG("Pipeline::TransferDataToFrameBuffer: type shader of light " + type_shader_t.name);
        
        m_shaders_storage.PutData(type_shader_t.name, "light_color", light_color.color);

        Transform transform {
          .position = curr_light->GetGeometry(),
          .rotation{0.f},
          .size = light_size.size,
          .value_rotate{0.f},
          .axis = AxisRotate::NONE,
        };

        RenderContext render_context{
            .type_shader = type_shader_t.type,
            .name_shader = type_shader_t.name,
            .transform   = transform,
            .key         = VertexObjectType::kLight
        };

        if (is_selectable)
        { 
          glStencilMask(0xFF);
        } 
        else 
        {
          glStencilMask(0x00);
        }

        renderer_->Draw(render_context);
      } 
      
      for (auto& obj_scn : objects_scene) {
        auto [_, color, mixing_color, material] = obj_scn.object.GetParams();

        auto& transform = obj_scn.object.GetTransform();
        
        type_shader_t.name = (obj_scn.type == core::ObjectType::kGrass ? "grass" : "lighting");

        // AV_LOG_DEBUG("Pipeline::TransferDataToFrameBuffer: type shader: " + type_shader_t.name);
        
        bool is_texture = material.is_texture; 

        ProcessLights(type_shader_t.name);

        int idx_texture          = material.texture_diffuse;
        int idx_texture_specular = material.texture_specular;
        int idx_texture_emission = material.texture_emission;

        m_shaders_storage.PutData(type_shader_t.name, "material.fl_shininess", material.shininess);

        if (material.is_texture) 
        {
          m_shaders_storage.PutData(type_shader_t.name, "material_type.is_texture", true);
          m_shaders_storage.PutData(type_shader_t.name, "material_type.is_prefab_material", false);
        }
        else 
        {
          m_shaders_storage.PutData(type_shader_t.name, "material.v3_ambient",  material.ambient);
          m_shaders_storage.PutData(type_shader_t.name, "material.v3_diffuse",  material.diffuse);
          m_shaders_storage.PutData(type_shader_t.name, "material.v3_specular", material.specular);

          m_shaders_storage.PutData(type_shader_t.name, "material_type.is_texture", false);
          m_shaders_storage.PutData(type_shader_t.name, "material_type.is_prefab_material", true);
        }

        TransferMaterial mat_tex {
          .is_texture = is_texture,
          .idx_texture = idx_texture,
          .idx_texture_specular = idx_texture_specular,
          .idx_texture_emission = idx_texture_emission
        };

        RenderContext render_context{
          .type_shader = type_shader_t.type,
          .name_shader = type_shader_t.name,
          .transform   = transform,
          .mat_tex     = mat_tex,
          .key = static_cast<VertexObjectType>(obj_scn.type)
        };

        if (obj_scn.is_selectable)
        { 
          glStencilMask(0xFF);
        } 
        else 
        {
          glStencilMask(0x00);
        }

        renderer_->Draw(render_context);
      }

      ProcessModels();
    }

    void Pipeline::ProcessMesh() noexcept
    {
      while (!m_pl_queue.IsEmpty()) 
      {
        Mesh *mesh = m_pl_queue.Dequeue();
        renderer_->RegisterMesh(mesh);
      }
    }

    void Pipeline::ProcessObjects() noexcept 
    {

    }

    void Pipeline::ProcessModels() noexcept
    {
      auto& models = scene_.GetModels();
      if (models.empty())
      {
        return;
      }

      ShaderType_t type_shader_t;
      type_shader_t.name = "model";

      ProcessMesh();
      ProcessLights(type_shader_t.name);

      // AV_LOG_DEBUG("Pipeline::ProcessModels: type shader of object " + type_shader_t.name);

      m_shaders_storage.PutData(type_shader_t.name, "material.fl_shininess", 32.f);

      RenderContext render_context
      {
        .type_shader = type_shader_t.type,
        .name_shader = type_shader_t.name,
        .transform   = {},
        .mat_tex     = {},
        .key         = VertexObjectType::kModel
      };
      
      for (auto& ptr_model : models)
      {
        render_context.transform = ptr_model->model.GetTransform();
        renderer_->SetRenderContext(render_context);
        auto& meshs = ptr_model->model.GetMeshs();

        if (ptr_model->is_selectable)
        {
          glStencilMask(0xFF);
        }
        else 
        {
          glStencilMask(0x00);
        }

        for (auto& mesh : meshs)
        {
          renderer_->Draw(mesh, type_shader_t.name);
        }
      }
    }

    void Pipeline::ProcessLights(const std::string& name_shader) noexcept
    {
      ShaderType_t type_shader_t;
      type_shader_t.name = "lighting";

      std::size_t count_point_light = 0;
      std::size_t count_spot_light  = 0;

      // AV_LOG_DEBUG("Pipeline::ProcessLight: type shader of object " + type_shader_t.name);

      for (const auto& light_obj : scene_.GetAllSourceLights())
      {
        core::ILight* ptr_light  = light_obj.light.get();
        core::LightType tp_light = light_obj.type;

        if (ptr_light) {
        switch (tp_light)
        {
          case core::LightType::kSimpleLight:
          case core::LightType::kUnknownLight:
            break;

          case core::LightType::kDirLight:
          {
            m_shaders_storage.PutData(name_shader, "dir_light.ambient", ptr_light->GetAmbient());
            m_shaders_storage.PutData(name_shader, "dir_light.diffuse", ptr_light->GetDiffuse());
            m_shaders_storage.PutData(name_shader, "dir_light.specular", ptr_light->GetSpecular());
            m_shaders_storage.PutData(name_shader, "light_type.is_dir_light", true);
            m_shaders_storage.PutData(name_shader, "dir_light.direction", ptr_light->GetGeometry());
            break;
          }
          case core::LightType::kPointLight:
          {
            m_shaders_storage.PutData(name_shader, "point_light[" + std::to_string(count_point_light) + "].ambient", ptr_light->GetAmbient());
            m_shaders_storage.PutData(name_shader, "point_light[" + std::to_string(count_point_light) + "].diffuse", ptr_light->GetDiffuse());
            m_shaders_storage.PutData(name_shader, "point_light[" + std::to_string(count_point_light) + "].specular", ptr_light->GetSpecular());
            m_shaders_storage.PutData(name_shader, "point_light[" + std::to_string(count_point_light) + "].position", ptr_light->GetGeometry());

            if (auto* dc_ptr_light = dynamic_cast<core::PointLight*>(ptr_light)) 
            {
              m_shaders_storage.PutData(name_shader, "point_light[" + std::to_string(count_point_light) + "].constant", dc_ptr_light->GetConstant());
              m_shaders_storage.PutData(name_shader, "point_light[" + std::to_string(count_point_light) + "].linear", dc_ptr_light->GetLinear());
              m_shaders_storage.PutData(name_shader, "point_light[" + std::to_string(count_point_light) + "].quadratic", dc_ptr_light->GetQuadratic());
            }
            m_shaders_storage.PutData(name_shader, "light_type.is_point_light", true);
            ++count_point_light;
            break;
          }
          case core::LightType::kSpotLight:
          {
            if (auto* spot_light_ptr = dynamic_cast<core::SpotLight*>(ptr_light))
            {
              glm::vec3 position = spot_light_ptr->GetPosition(); 
              glm::vec3 direction = spot_light_ptr->GetDirection();
              
              float cutoff = glm::cos(glm::radians(spot_light_ptr->GetCutOff()));
              float outer_cut_off = glm::cos(glm::radians(spot_light_ptr->GetOuterCutOff()));

              m_shaders_storage.PutData(name_shader, "spot_light[" + std::to_string(count_spot_light) + "].direction", direction);
              m_shaders_storage.PutData(name_shader, "spot_light[" + std::to_string(count_spot_light) + "].cut_off", cutoff);
              m_shaders_storage.PutData(name_shader, "spot_light[" + std::to_string(count_spot_light) + "].position", position);
              m_shaders_storage.PutData(name_shader, "spot_light[" + std::to_string(count_spot_light) + "].outer_cut_off", outer_cut_off);
              
              m_shaders_storage.PutData(name_shader, "spot_light[" + std::to_string(count_spot_light) + "].ambient", spot_light_ptr->GetAmbient());
              m_shaders_storage.PutData(name_shader, "spot_light[" + std::to_string(count_spot_light) + "].diffuse", spot_light_ptr->GetDiffuse());
              m_shaders_storage.PutData(name_shader, "spot_light[" + std::to_string(count_spot_light) + "].specular", spot_light_ptr->GetSpecular());

              m_shaders_storage.PutData(name_shader, "spot_light[" + std::to_string(count_spot_light) + "].constant", spot_light_ptr->GetConstant());
              m_shaders_storage.PutData(name_shader, "spot_light[" + std::to_string(count_spot_light) + "].linear", spot_light_ptr->GetLinear());
              m_shaders_storage.PutData(name_shader, "spot_light[" + std::to_string(count_spot_light) + "].quadratic", spot_light_ptr->GetQuadratic());
             }

            m_shaders_storage.PutData(name_shader, "light_type.is_spot_light", true);
            ++count_spot_light;
            break;
          }
        }
      }
    }
    m_shaders_storage.PutData(name_shader, "number_point_lights", static_cast<int>(count_point_light));
    m_shaders_storage.PutData(name_shader, "number_spot_lights", static_cast<int>(count_spot_light)); 
  }

  void Pipeline::DrawSelectableObjects(SelectionContext& selection_ctx) noexcept
  {
    ShaderType_t type_shader_t;
    type_shader_t.name = "lighting";

    RenderContext render_ctx{
      .type_shader = type_shader_t.type,
      .name_shader = type_shader_t.name,
      .transform{},
      .mat_tex{},
      .key{}
    };

    if (selection_ctx.primitive.is_select)
    {
      if (auto p_object = scene_.GetObject(selection_ctx.primitive.id); p_object != nullptr)
      { 
        render_ctx.transform = p_object->object.GetTransform();
        render_ctx.key = static_cast<VertexObjectType>(p_object->type);
        renderer_->DrawOutline(render_ctx);
      }
    }
    else if (selection_ctx.light.is_select)
    {
      if (auto p_object = scene_.GetLight(selection_ctx.light.id); p_object != nullptr)
      {
        Transform transform {
          .position = p_object->light->GetGeometry(),
          .rotation{0.f},
          .size = p_object->size.size,
          .value_rotate{0.f},
          .axis = AxisRotate::NONE
        };

        render_ctx.name_shader = "shader_model_light_source";
        render_ctx.transform = transform;
        render_ctx.key = VertexObjectType::kLight;

        renderer_->DrawOutline(render_ctx);
      }
    }
    else if (selection_ctx.model)
    {
      if (decltype(auto) p_object = scene_.GetModel(selection_ctx.model.id); p_object != nullptr)
      {
        renderer_->DrawOutlineModel(p_object->model);
      }
    }
  }

  glm::vec3 Pipeline::GetCameraPosition() const noexcept
  {
    return renderer_->GetCameraPosition();
  }
} // namespace avion::gfx
