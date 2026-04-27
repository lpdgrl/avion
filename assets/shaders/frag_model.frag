#version 460 core



struct Material {
  sampler2D diffuse1;
  sampler2D specular1;
  sampler2D emission;

  float fl_shininess;
};

struct SimpleLight {
  vec3 position;
  
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct DirLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;
  
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

struct SpotLight {
  vec3 position;
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;

  float cut_off;
  float outer_cut_off;
};

struct MaterialType {
  bool is_texture;
  bool is_prefab_material;
};

struct LightType {
  bool is_dir_light;
  bool is_point_light;
  bool is_spot_light;
};

#define NR_POINT_LIGHT 10
#define NR_SPOT_LIGHT  10

in vec3 fr_normal;
in vec3 fr_frag_position;
in vec2 fr_texture_coordinates;

out vec4 frag_color;

uniform vec3 view_pos;
uniform Material material;

// Lights 
uniform DirLight dir_light;
uniform PointLight point_light[NR_POINT_LIGHT];
uniform SpotLight spot_light[NR_SPOT_LIGHT];

uniform MaterialType material_type;
uniform LightType light_type;

uniform int number_point_lights;
uniform int number_spot_lights;

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 view_dir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 frag_pos, vec3 view_dir);
float CalculateAttenuation(SpotLight light, vec3 frag_pos);
float CalculateAttenuation(PointLight light, vec3 frag_pos);

void main()
{
  vec3 normal = normalize(fr_normal);
  vec3 view_dir = normalize(view_pos - fr_frag_position);
  vec3 result;
  
  if (light_type.is_dir_light)
  {  
    result += CalculateDirLight(dir_light, normal, view_dir);
  }

  if (light_type.is_point_light)
  {
    for (int i = 0; i < number_point_lights; ++i)
    {
      result += CalculatePointLight(point_light[i], normal, fr_frag_position, view_dir);
    }
  }

  if (light_type.is_spot_light)
  {  
    for (int i = 0; i < number_spot_lights; ++i)
    {
      result += CalculateSpotLight(spot_light[i], normal, fr_frag_position, view_dir);
    }
  }

  frag_color = vec4(result, 1.0);
}


vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 view_dir)
{
  vec3 result;
  vec3 light_dir = normalize(-light.direction);
  float diff = max(dot(normal, light_dir), 0.0);
  
  vec3 reflect_dir = reflect(-light_dir, normal);
  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.fl_shininess);
  
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  ambient  = light.ambient * vec3(texture(material.diffuse1, fr_texture_coordinates));
  diffuse  = light.diffuse * diff * vec3(texture(material.diffuse1, fr_texture_coordinates));
  specular = light.specular * spec * vec3(texture(material.specular1, fr_texture_coordinates));


  result = ambient + diffuse + specular;
  return result;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
  vec3 result;
  vec3 light_dir = normalize(light.position - frag_pos);
  float diff = max(dot(normal, light_dir), 0.0);

  vec3 reflect_dir = reflect(-light_dir, normal);
  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.fl_shininess);

  float attenuation = CalculateAttenuation(light, frag_pos);
  
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  ambient = light.ambient * vec3(texture(material.diffuse1, fr_texture_coordinates));
  diffuse = light.diffuse * diff * vec3(texture(material.diffuse1, fr_texture_coordinates));
  specular = light.specular * spec * vec3(texture(material.specular1, fr_texture_coordinates));

  ambient  *= attenuation;
  diffuse  *= attenuation;
  specular *= attenuation;
  
  result = ambient + diffuse + specular;
  return result;
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
  vec3 result;
  vec3 light_dir = normalize(light.position - frag_pos);
  float diff = max(dot(normal, light_dir), 0.0);

  vec3 reflect_dir = reflect(-light_dir, normal);
  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.fl_shininess);

  float attenuation = CalculateAttenuation(light, frag_pos);
  
  // spotlight intesity
  float theta = dot(light_dir, normalize(-light.direction));
  float epsilon = light.cut_off - light.outer_cut_off;
  float intesity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  ambient  = light.ambient * vec3(texture(material.diffuse1, fr_texture_coordinates));
  diffuse  = light.diffuse * diff * vec3(texture(material.diffuse1, fr_texture_coordinates));
  specular = light.specular * spec * vec3(texture(material.specular1, fr_texture_coordinates));

  ambient  *= attenuation * intesity;
  diffuse  *= attenuation * intesity;
  specular *= attenuation * intesity; 

  result = ambient + diffuse + specular;
  return result;
}

float CalculateAttenuation(SpotLight light, vec3 frag_pos)
{
  float _distance = length(light.position - frag_pos);
  float attenuation = 1.0 / (light.constant + light.linear * _distance + light.quadratic * (_distance * _distance));

  return attenuation;
}

float CalculateAttenuation(PointLight light, vec3 frag_pos)
{
  float _distance  = length(light.position - frag_pos);
  float attenuation = 1.0 / (light.constant + light.linear * _distance  + light.quadratic * (_distance * _distance));

  return attenuation;
}
