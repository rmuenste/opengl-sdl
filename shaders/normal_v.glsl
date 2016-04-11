#version 450 core 
#pragma optimize (off)

layout (location = 0) in vec3 positions;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout(location = 3) in vec3 normalLines;

uniform mat4 transform;
uniform mat4 perspective;
uniform mat4 camera;
uniform mat4 cameraRotation;

out vec2 texCoord0;                                                                 
out vec3 normal0;                                                                   
out vec3 worldPos0; 

void main(void) 
{

  vec4 n4 = vec4(normal.x, normal.y, normal.z,0.0);

  normal0 = (perspective * cameraRotation * camera * transform * n4).xyz;

  worldPos0 = (perspective * cameraRotation * camera * transform * vec4(positions.x, positions.y, positions.z,1.0)).xyz;

  texCoord0 = texCoord;

  gl_Position = (perspective * cameraRotation * camera * transform * vec4(normalLines.x, normalLines.y, normalLines.z,1.0));

}
