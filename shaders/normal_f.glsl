#version 450 core 
#pragma optimize (off)

out vec4 color; 

in vec2 texCoord0;                                                                  
in vec3 normal0;                                                                    
in vec3 worldPos0;

uniform vec3 model_col;

void main(void) 
{

  color = vec4(model_col,1.0);

}

