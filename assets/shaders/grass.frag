#version 460 core

struct Material {
  vec3 v3_ambient;
  vec3 v3_diffuse;
  vec3 v3_specular;

  sampler2D s2d_diffuse;
  sampler2D s2d_specular;
  sampler2D s2d_emission;

  float fl_shininess;
};

struct MaterialType {
  bool is_texture;
  bool is_prefab_material;
};

in vec2 TexCoords;
out vec4 FragColor;

uniform Material material;
uniform MaterialType material_type;
 

void main()
{   
    vec4 result;
    if (material_type.is_prefab_material)
    {
      result = vec4(material.v3_diffuse, 1.0);
    }
    else
    {
      result = texture(material.s2d_diffuse, TexCoords);
      if(result.a < 0.1)
          discard;
    }

    FragColor = result;
}