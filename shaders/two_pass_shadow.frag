#version 450 core 
#pragma optimize (off)

out vec4 color; 

in vec2 texCoord0;                                                                  
in vec3 normal0;                                                                    
in vec3 worldPos0;

uniform sampler2D sampler;
uniform sampler2D sampler1;

vec4 ambientLight = vec4(0.8,0.8,0.8,1);

void main(void) 
{

  vec4 totalLight = vec4(0,0,0,1); //getDirectionalLight(normal0);

  float d = texture(sampler,texCoord0.xy).x;
  //d = 1.0 - (1.0 - d) * 25.0f;
  d = pow(d,5);

  if(worldPos0.x < 0.0f)
  {
    color = texture2D(sampler,texCoord0.xy);
  }
  else
  {
    color = texture2D(sampler1,texCoord0.xy);
  }

}

