#version 460 core

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 ligthColor;
uniform vec3 ligthPos;
uniform vec3 view_pos;

out vec4 FragColor;

void main()
{
    float specular_strength = 0.5f;
    vec3 view_dir = normalize(view_pos - FragPos);

    vec3 norm = normalize(Normal);
    vec3 ligthDir = normalize(ligthPos - FragPos);

    vec3 reflect_dir = reflect(-ligthDir, norm);

    // Diffuse ligth
    float diff = max(dot(norm, ligthDir), 0.0);
    vec3 diffuse = diff * ligthColor;

    // Ambient ligth
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * ligthColor;

    // Specular ligth
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), 32);
    vec3 specular = specular_strength * spec * ligthColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
