#version 460 core

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 ligthColor;
uniform vec3 ligthPos;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 ligthDir = normalize(ligthPos - FragPos);

    // Diffuse ligth
    float diff = max(dot(norm, ligthDir), 0.0);
    vec3 diffuse = diff * ligthColor;

    // Ambient ligth
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * ligthColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}
