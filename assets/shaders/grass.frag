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

out vec4 FragColor;
uniform Material material;

in vec2 TexCoords;


void main()
{             
    vec4 texColor = texture(material.s2d_diffuse, TexCoords);
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;
}