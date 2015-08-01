#version 450 core 

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 col;

out vec4 vs_color; 

void main(void) 
{
//  const vec4 vertices[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),
//                                   vec4(-0.25, -0.25, 0.5, 1.0),
//                                   vec4(0.25, 0.25, 0.5, 1.0));

  vec4 pos(position.x, position.y, position.z, 1.0);

//  const vec4 colors[3] = vec4[3](               
//                               vec4(1.0, 0.0, 0.0, 1.0),                
//                               vec4(0.0, 1.0, 0.0, 1.0),                
//                               vec4(0.0, 0.0, 1.0, 1.0));                

  //gl_Position = vertices[gl_VertexID];
  gl_Position = position;
  vs_color = col;
}

