#include "AvionEngineCore/renderer/pipeline.hpp"
#include "AvionEngineCore/core/resource_manager.hpp"

namespace avion::gfx {

    Pipeline::Pipeline(core::Scene& scene)
    : scene_(scene)
    , m_resman(std::filesystem::canonical("/proc/self/exe").c_str()) 
    {}

    Pipeline::~Pipeline() {
        delete renderer_;
        AV_LOG_INFO("Pipeline::~Pipeline: pipeline is destroyed");
    }

    void Pipeline::Init(int width, int height) {
        m_resman.RegisterResource(core::resman::ResourceType::kTexture, "assets/textures");
        m_resman.RegisterResource(core::resman::ResourceType::kShader, "assets/shaders");
        
        InitShadersStructs();

        renderer_ = new Renderer(m_resman, m_shaders_storage);

        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        renderer_->Init();
        renderer_->SetPerspectiveProjection(45.f, width, height, 0.1f, 50.f);

        // // TODO: Need implementation resource manager!! 
        // queue_textures_.emplace_back("./AvionEngineCore/resources/textures/container2.png");
        // auto& tex_1 = queue_textures_.back();

        // if (tex_1.LoadTexture()) {
        //     std::string success;
        //     success += "Success loaded texture ";
        //     success += tex_1.GetPath();
        //     AV_LOG_DEBUG(success);

        //     res_.emplace("container2.png", &tex_1);
        // } else {
        //     queue_textures_.pop_back();
        // }

        //  // TODO: Need implementation resource manager!!
        //  queue_textures_.emplace_back("./AvionEngineCore/resources/textures/container.jpg");
        //  auto& tex_2 = queue_textures_.back(); 

        //  if (tex_2.LoadTexture()) {
        //      std::string success;
        //      success += "Success loaded texture ";
        //      success += tex_2.GetPath();
        //      AV_LOG_DEBUG(success);

        //      res_.emplace("container.jpg", &tex_2);
        //  } else {
        //      queue_textures_.pop_back();
        //  }
    
        // queue_textures_.emplace_back("./AvionEngineCore/resources/textures/container2_specular.png");
        // auto& tex_3 = queue_textures_.back(); 

        // if (tex_3.LoadTexture()) {
        //     std::string success;
        //     success += "Success loaded texture ";
        //     success += tex_3.GetPath();
        //     AV_LOG_DEBUG(success);

        //     res_.emplace("container2_specular.png", &tex_3);
        // } else {
        //     queue_textures_.pop_back();
        // }

        // queue_textures_.emplace_back("./AvionEngineCore/resources/textures/matrix.jpg");
        // auto& tex_4 = queue_textures_.back();

        // if (tex_4.LoadTexture()) {
        //     std::string success;
        //     success += "Success loaded texture ";
        //     success += tex_4.GetPath();
        //     AV_LOG_DEBUG(success);

        //     res_.emplace("matrix.jpg", &tex_4);
        // } else {
        //     queue_textures_.pop_back();
        // }

        // queue_textures_.emplace_back("./AvionEngineCore/resources/textures/test1_glossymap.jpg");
        // auto& tex_5 = queue_textures_.back();

        // if (tex_5.LoadTexture()) {
        //     std::string success;
        //     success += "Success loaded texture ";
        //     success += tex_5.GetPath();
        //     AV_LOG_DEBUG(success);

        //     res_.emplace("test1_glossymap.jpg", &tex_5);
        // } else {
        //     queue_textures_.pop_back();
        // }
        
        // queue_textures_.emplace_back("./AvionEngineCore/resources/textures/RubicDiffuse.jpg");
        // auto& tex_6 = queue_textures_.back();

        // if (tex_6.LoadTexture()) {
        //     std::string success;
        //     success += "Success loaded texture ";
        //     success += tex_6.GetPath();
        //     AV_LOG_DEBUG(success);

        //     res_.emplace("RubicDiffuse.jpg", &tex_6);   
        // } else {
        //     queue_textures_.pop_back();
        // }

        // queue_textures_.emplace_back("./AvionEngineCore/resources/textures/RubicEmissive.jpg");
        // auto& tex_7 = queue_textures_.back();
        
        // if (tex_7.LoadTexture()) {
        //     std::string success;
        //     success += "Success loaded texture ";
        //     success += tex_7.GetPath();
        //     AV_LOG_DEBUG(success);

        //     res_.emplace("RubicEmissive.jpg", &tex_7);
        // } else {
        //     queue_textures_.pop_back();
        // }

        // queue_textures_.emplace_back("./AvionEngineCore/resources/textures/lighting_maps_specular_color.png");
        // auto& tex_8 = queue_textures_.back();

        // if (tex_8.LoadTexture()) {
        //     std::string success;
        //     success += "Success loaded texture ";
        //     success += tex_8.GetPath();
        //     AV_LOG_DEBUG(success);

        //     res_.emplace("lighting_maps_specular_color.png", &tex_8);
        // } else {
        //     queue_textures_.pop_back();
        // }

        renderer_->InitTexture();
    }

    void Pipeline::InitShadersStructs() noexcept 
    {
    //         shader_light_.light_color.name_param = "ligthColor";
    //         shader_light_.model_matrix.name_param = "model";
    //         shader_light_.view_pos.name_param = "view_pos";
    //         shader_light_.view_matrix.name_param = "view";
            
    //         shader_object_.mat_light_ambient.name_param = "material.ambient";
    //         shader_object_.mat_light_diffuse.name_param = "material.diffuse";
    //         shader_object_.mat_light_specular.name_param = "material.specular";
    //         shader_object_.mat_light_shininess.name_param = "material.shininess";

    //         shader_object_.screen_aspect.name_param = "scr_aspect";

    //         shader_object_.model_matrix.name_param = "model";
    //         shader_object_.view_matrix.name_param = "view";
    //         shader_object_.view_pos.name_param = "view_pos";
            
    //         shader_object_.light_ambient.name_param = "light.ambient";
    //         shader_object_.light_diffuse.name_param = "light.diffuse";
    //         shader_object_.light_specular.name_param = "light.specular";
    //         shader_object_.light_position.name_param = "light.position";
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
      
      core::ILight* light = nullptr;
      ShaderType_t type_shader_t;
      core::LightType tp_light = core::LightType::kUnknownLight;

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
        
        light = curr_light.get();
        tp_light = type_light;
      } 

      for (const auto& [type, object] : objects_scene) {
        auto [position, size, color, mixing_color, material] = object.GetParams();

        type_shader_t.name = "lighting";

        AV_LOG_DEBUG("Pipeline::TransferDataToFrameBuffer: type shader of object" + type_shader_t.name);
        
        if (light) {
          m_shaders_storage.PutData(type_shader_t.name, "light.ambient", light->GetAmbient());
          m_shaders_storage.PutData(type_shader_t.name, "light.diffuse", light->GetDiffuse());
          m_shaders_storage.PutData(type_shader_t.name, "light.specular", light->GetSpecular());
          m_shaders_storage.PutData(type_shader_t.name, "light.position", light->GetGeometry());
          
          if (tp_light == core::LightType::kSimpleLight) 
          {
            glm::vec3 position = light->GetGeometry();
            glm::vec4 pos_dir(position.x, position.y, position.z, 1.f);
            m_shaders_storage.PutData(type_shader_t.name, "light.pos_dir", pos_dir);      
          } else if (tp_light == core::LightType::kDirLight) 
          {
            glm::vec3 direction = light->GetGeometry();
            glm::vec4 pos_dir(direction.x, direction.y, direction.z, 0.f);
            m_shaders_storage.PutData(type_shader_t.name, "light.pos_dir", pos_dir);
          }
        }
        
        m_shaders_storage.PutData(type_shader_t.name, "material.ambient", material.ambient);
        m_shaders_storage.PutData(type_shader_t.name, "material.diffuse", material.diffuse);
        m_shaders_storage.PutData(type_shader_t.name, "material.specular",  material.specular);
        m_shaders_storage.PutData(type_shader_t.name, "material.shininess", material.shininess);

        bool is_texture = ((material.texture != nullptr) ? true : false);
        int idx_texture = ((material.texture != nullptr) ? material.texture->GetId(): -1); 
        int idx_texture_specular = ((material.texture_specular != nullptr) ? material.texture_specular->GetId() : -1);
        int idx_texture_emission = ((material.texture_emission != nullptr) ? material.texture_emission->GetId() : -1);

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
              .key = static_cast<VertexObjectType>(type)
        };

        renderer_->Draw(render_context);
    }

    }

    Pipeline::ResTextures& Pipeline::GetLoadedResource() noexcept {
        return res_;
    }

    ShaderType_t Pipeline::GetShaderTypeByLightType(core::LightType type_light) const noexcept 
    {
      ShaderType_t type_shader;
      switch(type_light) {
        case core::LightType::kSimpleLight:
        {
          type_shader.type = ShaderLight::kShSimpleLight;
          type_shader.name = "simple_cube_SL_prefab_material";
          break;
        }
        case core::LightType::kDirLight:
        {
          type_shader.type = ShaderLight::kShDirLight;
          type_shader.name = "shader_cube_DL_prefab_material";
          break;
        }
        case core::LightType::kPointLight:
        {
          type_shader.type = ShaderLight::kShPointLight;
          type_shader.name = "shader_obj_light";
          break;
        }
      }
      return type_shader;
    }

} // namespace avion::gfx
