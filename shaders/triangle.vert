#version 450 core 

layout (location = 0) in vec3 positions;
layout (location = 1) in vec4 in_color;

out vec4 vs_color; 

void main(void) 
{
  gl_Position = vec4(positions.x, positions.y, positions.z,1.0);
  vs_color = in_color;
}

