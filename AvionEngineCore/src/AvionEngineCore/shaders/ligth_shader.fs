#version 460 core

out vec4 FragColor;

uniform vec3 ligthColor;

void main()
{
    FragColor = vec4(ligthColor, 1.0); // set all 4 vector values to 1.0
}