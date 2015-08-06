#version 450 core 

layout (location = 0) in vec3 positions;
layout (location = 1) in vec2 texCoord;
uniform mat4 transform;
out vec2 vs_texCoord; 

void main(void) 
{
  gl_Position = transform * vec4(positions.x, positions.y, positions.z,1.0);
  vs_texCoord = texCoord;
}

