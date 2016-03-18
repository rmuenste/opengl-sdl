#version 450                                                                        
                                                                                    
in vec2 TexCoordOut; 
out vec4 color; 
uniform sampler2D sampler; 
void main()                                                                         
{                                                                                   
  float d = texture(sampler, TexCoordOut).x;                       
  //d = 1.0 - (1.0 - d) * 10.0; 
  //d = 1.0 - sqrt(d); 
  //d = pow(d,64);
  color = vec4(d);
}
