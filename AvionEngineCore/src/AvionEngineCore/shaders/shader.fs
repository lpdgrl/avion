#version 460 core

struct Material {
    // vec3 ambient;
    
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;

    // vec3 diffuse;
    // vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 view_pos;

uniform Material material;
uniform Light light;

uniform float delta;

out vec4 FragColor;

void main()
{

    // ambient light 
    // vec3 ambient = light.ambient * material.ambient;

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // diffuse light 
    vec3 norm = normalize(Normal);
    vec3 light_dir = normalize(light.position - FragPos);
    float diff = max(dot(norm, light_dir), 0.0);

    // vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb; 

    // specular
    vec3 view_dir = normalize(view_pos - FragPos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    // vec3 specular = light.specular * (spec * material.specular);
    vec3 specularMap = vec3(texture(material.specular, TexCoords));

    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
   
    vec3 emission = vec3(0.0);
    if (texture(material.specular, TexCoords).g == 0.0) {
        // vec2 tex_coord = TexCoords * delta;
        emission = texture(material.emission, TexCoords).rgb;
    }

    vec3 result = ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0);
}
