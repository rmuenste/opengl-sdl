#version 450 core 

out vec4 color; 
in vec2 vs_texCoord;

uniform sampler2D sampler;

void main(void) 
{
  color = texture2D(sampler, vs_texCoord.xy);
}

