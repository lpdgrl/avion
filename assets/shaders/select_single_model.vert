#version 460 core

layout (location = 0) in vec3 ve_position;
layout (location = 1) in vec3 ve_normal;
layout (location = 2) in vec2 ve_texture_coordinates;

out vec2 fr_texture_coordinates;
out vec3 fr_normal;
out vec3 fr_frag_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    fr_texture_coordinates = ve_texture_coordinates;    
    fr_frag_position = vec3(model * vec4(ve_position, 1.0f));
    fr_normal = mat3(transpose(inverse(model))) * ve_normal;

    gl_Position = projection * view * model * vec4(ve_position, 1.0);
}