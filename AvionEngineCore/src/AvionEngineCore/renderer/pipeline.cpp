#include "AvionEngineCore/renderer/pipeline.hpp"
#include "AvionEngineCore/core/resource_manager.hpp"

namespace avion::gfx {

    Pipeline::Pipeline(core::Scene& scene, core::resman::ResourceManager& resman)
    : scene_(scene)
    , m_resman(resman) 
    {}

    Pipeline::~Pipeline() {
        delete renderer_;
        AV_LOG_INFO("Pipeline::~Pipeline: pipeline is destroyed");
    }

    void Pipeline::Init(int width, int height) {
      m_resman.RegisterResource(core::resman::ResourceType::kTexture, "assets/textures");
      m_resman.RegisterResource(core::resman::ResourceType::kShader, "assets/shaders");


      // TODO: Initialization shaders and register them
      std::string simple_cube("lighting");
      std::string shader_model_light_source("shader_model_light_source");
  
      m_shaders_storage.RegisterShader(
          simple_cube, 
          m_resman.GetResource<std::filesystem::path>("simple_cube_transform.vert")->c_str(),
          m_resman.GetResource<std::filesystem::path>("lighting.frag")->c_str());

       m_shaders_storage.RegisterShader(
          shader_model_light_source,
          m_resman.GetResource<std::filesystem::path>("simple_light_transform.vert")->c_str(),
          m_resman.GetResource<std::filesystem::path>("simple_light_color.frag")->c_str());

      renderer_ = new Renderer(m_shaders_storage);

      glEnable(GL_BLEND);
      glEnable(GL_DEPTH_TEST);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      renderer_->Init();
      renderer_->SetPerspectiveProjection(45.f, width, height, 0.1f, 50.f);

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

    void Pipeline::TransferDataToFrameBuffer() noexcept 
    {
      const auto& objects_scene = scene_.GetAllObjects();
      const auto& lights_scene = scene_.GetAllSourceLights();
      
      ShaderType_t type_shader_t;

      // TODO: It is strange deal
      for (const auto& [curr_light, id, type_light,  light_color, light_size] : lights_scene) {  

        type_shader_t.name = "shader_model_light_source";
        AV_LOG_DEBUG("Pipeline::TransferDataToFrameBuffer: type shader of light " + type_shader_t.name);
        
        m_shaders_storage.PutData(type_shader_t.name, "light_color", light_color.color);

        Transform transform {
          .position = curr_light->GetGeometry(),
          .size = light_size.size,
          .axis = AxisRotate::NONE,
          .rotate = 0.f
        };

        RenderContext render_context{
            .type_shader = type_shader_t.type,
            .name_shader = type_shader_t.name,
            .transform   = transform,
            .key         = VertexObjectType::kLight
        };

        renderer_->Draw(render_context);
      } 
      
      for (const auto& [type, object] : objects_scene) {
        auto [position, size, color, mixing_color, material] = object.GetParams();

        type_shader_t.name = "lighting";

        AV_LOG_DEBUG("Pipeline::TransferDataToFrameBuffer: type shader: " + type_shader_t.name);
        
        AV_LOG_DEBUG(std::to_string(material.is_texture));
        bool is_texture = material.is_texture; 

        ProcessLight(is_texture);
       
        int idx_texture          = material.texture_diffuse;
        int idx_texture_specular = material.texture_specular;
        int idx_texture_emission = material.texture_emission;

        m_shaders_storage.PutData(type_shader_t.name, "material.fl_shininess", material.shininess);

        if (material.is_texture) 
        {
          m_shaders_storage.PutData(type_shader_t.name, "material_type.is_texture", true);
        }
        else 
        {
          m_shaders_storage.PutData(type_shader_t.name, "material.v3_ambient", material.ambient);
          m_shaders_storage.PutData(type_shader_t.name, "material.v3_diffuse", material.diffuse);
          m_shaders_storage.PutData(type_shader_t.name, "material.v3_specular",  material.specular);

          m_shaders_storage.PutData(type_shader_t.name, "material_type.is_texture", false);
          m_shaders_storage.PutData(type_shader_t.name, "material_type.is_prefab_material", true);
        }

        Transform transform {
            .position = position,
            .size = size,
            .axis = AxisRotate::NONE,
            .rotate = 0.f
        };

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
              .key = static_cast<VertexObjectType>(type)
        };

        renderer_->Draw(render_context);
      }
    }

    void Pipeline::ProcessLight(bool is_texture) noexcept
    {
      ShaderType_t type_shader_t;
      type_shader_t.name = "lighting";

      std::size_t count_point_light = 0;
      std::size_t count_spot_light  = 0;

      AV_LOG_DEBUG("Pipeline::ProcessLight: type shader of object" + type_shader_t.name);

      for (const auto& light_obj : scene_.GetAllSourceLights())
      {
        core::ILight* ptr_light  = light_obj.light.get();
        core::LightType tp_light = light_obj.type_light;

        if (ptr_light) {
        switch (tp_light)
        {
          case core::LightType::kSimpleLight:
          case core::LightType::kUnknownLight:
            break;

          case core::LightType::kDirLight:
          {
            m_shaders_storage.PutData(type_shader_t.name, "dir_light.ambient", ptr_light->GetAmbient());
            m_shaders_storage.PutData(type_shader_t.name, "dir_light.diffuse", ptr_light->GetDiffuse());
            m_shaders_storage.PutData(type_shader_t.name, "dir_light.specular", ptr_light->GetSpecular());
            m_shaders_storage.PutData(type_shader_t.name, "light_type.is_dir_light", true);
            m_shaders_storage.PutData(type_shader_t.name, "dir_light.direction", ptr_light->GetGeometry());
            break;
          }
          case core::LightType::kPointLight:
          {
            m_shaders_storage.PutData(type_shader_t.name, "point_light[" + std::to_string(count_point_light) + "].ambient", ptr_light->GetAmbient());
            m_shaders_storage.PutData(type_shader_t.name, "point_light[" + std::to_string(count_point_light) + "].diffuse", ptr_light->GetDiffuse());
            m_shaders_storage.PutData(type_shader_t.name, "point_light[" + std::to_string(count_point_light) + "].specular", ptr_light->GetSpecular());
            m_shaders_storage.PutData(type_shader_t.name, "point_light[" + std::to_string(count_point_light) + "].position", ptr_light->GetGeometry());

            if (auto* dc_ptr_light = dynamic_cast<core::PointLight*>(ptr_light)) 
            {
              m_shaders_storage.PutData(type_shader_t.name, "point_light[" + std::to_string(count_point_light) + "].constant", dc_ptr_light->GetConstant());
              m_shaders_storage.PutData(type_shader_t.name, "point_light[" + std::to_string(count_point_light) + "].linear", dc_ptr_light->GetLinear());
              m_shaders_storage.PutData(type_shader_t.name, "point_light[" + std::to_string(count_point_light) + "].quadratic", dc_ptr_light->GetQuadratic());
            }
            m_shaders_storage.PutData(type_shader_t.name, "light_type.is_point_light", true);
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

              m_shaders_storage.PutData(type_shader_t.name, "spot_light[" + std::to_string(count_spot_light) + "].direction", direction);
              m_shaders_storage.PutData(type_shader_t.name, "spot_light[" + std::to_string(count_spot_light) + "].cut_off", cutoff);
              m_shaders_storage.PutData(type_shader_t.name, "spot_light[" + std::to_string(count_spot_light) + "].position", position);
              m_shaders_storage.PutData(type_shader_t.name, "spot_light[" + std::to_string(count_spot_light) + "].outer_cut_off", outer_cut_off);
              
              m_shaders_storage.PutData(type_shader_t.name, "spot_light[" + std::to_string(count_spot_light) + "].ambient", spot_light_ptr->GetAmbient());
              m_shaders_storage.PutData(type_shader_t.name, "spot_light[" + std::to_string(count_spot_light) + "].diffuse", spot_light_ptr->GetDiffuse());
              m_shaders_storage.PutData(type_shader_t.name, "spot_light[" + std::to_string(count_spot_light) + "].specular", spot_light_ptr->GetSpecular());

              m_shaders_storage.PutData(type_shader_t.name, "spot_light[" + std::to_string(count_spot_light) + "].constant", spot_light_ptr->GetConstant());
              m_shaders_storage.PutData(type_shader_t.name, "spot_light[" + std::to_string(count_spot_light) + "].linear", spot_light_ptr->GetLinear());
              m_shaders_storage.PutData(type_shader_t.name, "spot_light[" + std::to_string(count_spot_light) + "].quadratic", spot_light_ptr->GetQuadratic());
             }

            m_shaders_storage.PutData(type_shader_t.name, "light_type.is_spot_light", true);
            ++count_spot_light;
            break;
          }
        }
      }
    }
    m_shaders_storage.PutData(type_shader_t.name, "number_point_lights", static_cast<int>(count_point_light));
    m_shaders_storage.PutData(type_shader_t.name, "number_spot_lights", static_cast<int>(count_spot_light)); 
  }
} // namespace avion::gfx
