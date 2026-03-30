#version 460 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;
};

struct Light {
    vec4 pos_dir;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;

uniform vec3 view_pos;

uniform Material material;
uniform Light light;

out vec4 FragColor;

void main()
{
    vec3 ambient = light.ambient * material.ambient;

    // diffuse light 
    vec3 norm = normalize(Normal);
    vec3 light_dir;

    if (light.pos_dir.w == 0.0) {
      vec3 direction = vec3(light.pos_dir.x, light.pos_dir.y, light.pos_dir.z);
      light_dir = normalize(-direction);
    } else if (light.pos_dir.w == 1.0) {
      vec3 position = vec3(light.pos_dir.x, light.pos_dir.y, light.pos_dir.y);
      light_dir = normalize(position - FragPos);
    }

    float diff = max(dot(norm, light_dir), 0.0);
    
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular
    vec3 view_dir = normalize(view_pos - FragPos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    
    vec3 specular = light.specular * (spec * material.specular);

    // Result 
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
