#version 450 core 

layout (location = 0) in vec3 positions;
layout (location = 1) in vec4 in_color;
layout (location = 2) in vec2 texCoord;

out vec4 vs_color; 
out vec2 vs_texCoord; 

void main(void) 
{
  gl_Position = vec4(positions.x, positions.y, positions.z,1.0);
  vs_color = in_color;
  vs_texCoord = texCoord;
}

