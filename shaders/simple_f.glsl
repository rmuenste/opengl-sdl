#version 450 core 
#pragma optimize (off)

out vec4 color; 

in vec2 texCoord0;                                                                  
in vec3 normal0;                                                                    
in vec3 worldPos0;

//uniform vec3 model_col;
uniform sampler2D sampler;

void main(void) 
{

  //color = vec4(1,0,0,1.0);
  color = texture2D(sampler, texCoord0.xy);

}

