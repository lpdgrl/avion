#version 460 core

layout (location = 0) in vec3 ve_position;
layout (location = 1) in vec3 ve_normal;
layout (location = 2) in vec2 ve_texture_coordinates;
layout (location = 3) in ivec4 bones_ids;
layout (location = 4) in vec4 weights;

out vec2 fr_texture_coordinates;
out vec3 fr_normal;
out vec3 fr_frag_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 300;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 final_bones_matrices[MAX_BONES];
uniform bool has_animation;

void main()
{
    vec4 total_position = vec4(0.0f);
    vec3 total_normal = vec3(0.0f);

    if (has_animation)
    {
      for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
      {
        if (bones_ids[i] < 0)
        {
          continue;
        }
        if (bones_ids[i] >= MAX_BONES)
        {
          total_position = vec4(ve_position, 1.0f);
          break;
        }
        vec4 local_position = final_bones_matrices[bones_ids[i]] * vec4(ve_position, 1.0f);
        total_position += local_position * weights[i];
        total_normal += mat3(final_bones_matrices[bones_ids[i]]) * ve_normal * weights[i];
      }
      total_normal = normalize(total_normal);
    }
    else 
    {
      total_position = vec4(ve_position, 1.0f);
      total_normal = ve_normal;
    }

    fr_texture_coordinates = ve_texture_coordinates;    
    if (has_animation)
    {
      fr_frag_position = vec3(model * total_position);
    }
    else 
    {
      fr_frag_position = vec3(model * vec4(ve_position, 1.0f));
    }
    
    fr_normal = mat3(transpose(inverse(model))) * total_normal;


    gl_Position = projection * view * model * total_position;
    
}