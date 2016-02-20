#version 450 core 

layout (location = 0) in vec3 positions;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
uniform mat4 transform;
uniform mat4 perspective;
uniform mat4 camera;
uniform mat4 cameraRotation;

uniform vec3 lightPos;

out vec2 vs_texCoord; 
out vec4 vs_cameraCoord; 

out VS_OUT
{
  vec3 N;
  vec3 L;
  vec3 V;
}vs_out;

void main(void) 
{

  vec4 p = cameraRotation * camera * transform * vec4(positions.x, positions.y, positions.z,1.0); 

  vec4 n4 = vec4(normal.x, normal.y, normal.z,0.0);

  vs_out.N = (cameraRotation * camera * transform * n4).xyz;

  vs_out.L = (cameraRotation * camera * vec4(lightPos,1.0)).xyz - p.xyz; 

  vs_out.V = -p.xyz; 

  gl_Position = perspective * cameraRotation * camera * transform * vec4(positions.x, positions.y, positions.z,1.0);

  vs_cameraCoord = cameraRotation * camera * transform * vec4(positions.x, positions.y, positions.z,1.0);

  vs_texCoord = texCoord;

}

