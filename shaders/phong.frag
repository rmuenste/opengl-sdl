#version 450 core 

out vec4 color; 

in vec2 texCoord0;                                                                  
in vec3 normal0;                                                                    
in vec3 worldPos0;

struct DirectionalLight
{
  vec3 color;
  float ambientIntensity;
  float diffuseIntensity;
  vec3 dir;
};


uniform float specularIntensity;
uniform float specularExponent;

uniform vec3 eyePos;
uniform DirectionalLight dirLight;

float base_intensity = 0.3;

uniform sampler2D sampler;

vec4 ambientLight = vec4(0.8,0.8,0.8,1);

void main(void) 
{

  ambientLight = vec4( dirLight.ambientIntensity * dirLight.color, 1.0f);

  vec4 totalLight = ambientLight;

  vec3 N = normalize(normal0);

  vec3 L = dirLight.dir;

  color = texture2D(sampler, texCoord0.xy);

  vec3 R = normalize(reflect(L,N));

  vec3 V = normalize(eyePos-worldPos0);

  // Diffuse light: calculate the angle between the surface normal and the reversed light vector
  vec3 diffuse = vec3(1.0,1.0,1.0) * max(dot(N,-L), 0.0) * dirLight.diffuseIntensity;

  // Specular light: calculate the angle between the reflected light and eye vector
  vec3 specular = vec3(1.0,1.0,1.0) * pow(max(dot(R, V), 0.0), specularExponent) * specularIntensity * dirLight.diffuseIntensity;

  totalLight+=(vec4(diffuse + specular, 1.0));

  color = color * (totalLight);

}

