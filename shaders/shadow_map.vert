#version 450                                                                        
                                                                                    
layout (location = 0) in vec3 positions;                                             
layout (location = 1) in vec2 texCoord;                                             
layout (location = 2) in vec3 normal;                                               
                                                                                    
uniform mat4 mvp;                                                                  
out vec2 TexCoordOut;                                                               
uniform mat4 transform;                                                                  
uniform mat4 perspective;                                                                  
uniform mat4 camera;                                                                  
uniform mat4 cameraRotation;                                                                  

void main()                                                                         
{                                                                                   
    mat4 mvp = perspective * cameraRotation * camera * transform;
    gl_Position = mvp * vec4(positions, 1.0);                                       
    TexCoordOut = texCoord;                                                         
}
