#version 460 core

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 ligthColor;


void main()
{
    FragColor = vec4(ligthColor * objectColor, 1.0);
}
