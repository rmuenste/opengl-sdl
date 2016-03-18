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

struct Attenuation
{
  float constant;
  float linear;
  float exp;
};

struct PointLight
{
  vec3 color;
  float ambientIntensity;
  float diffuseIntensity;
  vec3 position;
  Attenuation att;
};


uniform float specularIntensity;
uniform float specularExponent;

uniform vec3 eyePos;
uniform DirectionalLight dirLight;

uniform PointLight pointLight;

float base_intensity = 0.3;

uniform sampler2D sampler;

vec4 ambientLight = vec4(0.8,0.8,0.8,1);

vec4 lightKernel(vec3 d, vec3 n, vec3 lightColor, float lightAmbIntensity, float lightDiffIntensity)
{
  // ambient part
  vec4 light = vec4( lightAmbIntensity * lightColor, 1.0f);

  vec3 N = normalize(n);

  vec3 L = d;

  vec3 R = normalize(reflect(L,N));

  vec3 V = normalize(eyePos-worldPos0);

  // Diffuse light: calculate the angle between the surface normal and the reversed light vector
  vec3 diffuse = lightColor * max(dot(N,-L), 0.0) * lightDiffIntensity;

  // Specular light: calculate the angle between the reflected light and eye vector
  vec3 specular = lightColor * pow(max(dot(R, V), 0.0), specularExponent) * specularIntensity;

  return light + vec4(diffuse,1.0f) + vec4(specular,1.0f);
}

vec4 getDirectionalLight(vec3 normal)
{
  return lightKernel(dirLight.dir, normal, dirLight.color, dirLight.ambientIntensity, dirLight.diffuseIntensity);
}

vec4 getPointLight(vec3 normal)
{

  vec3 n = normalize(normal);
  vec3 dir   = worldPos0 - pointLight.position; 
  float dist = length(dir);
  dir        = normalize(dir);

  vec4 color = lightKernel(dir, n, pointLight.color, pointLight.ambientIntensity, pointLight.diffuseIntensity);

  float attenuation = pointLight.att.constant + pointLight.att.linear * dist + 
                      pointLight.att.exp * dist * dist; 

  return color / attenuation;

}

void main(void) 
{

  vec4 totalLight = getDirectionalLight(normal0);

  totalLight += getPointLight(normal0);

  color = texture2D(sampler, texCoord0.xy) * totalLight;

}

