#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float scr_aspect;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 Position;

void main()
{
    TexCoords = tex_coords;
    gl_Position = projection * view * model * vec4(position, 1.0);
}