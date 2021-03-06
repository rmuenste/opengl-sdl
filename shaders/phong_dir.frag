#version 450 core 
#pragma optimize (off)

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

struct SpotLight
{
  vec3 color;
  vec3 position;
  vec3 dir;

  float ambientIntensity;
  float diffuseIntensity;
  float cutoff;

  Attenuation att;
};

uniform float specularIntensity;
uniform float specularExponent;

uniform vec3 eyePos;
uniform DirectionalLight dirLight;

uniform PointLight pointLight;
uniform SpotLight spotLight;;

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

vec4 getSpotLight(vec3 normal)
{

  vec4 color = vec4(0,0,0,0);

  vec3 cone = worldPos0 - spotLight.position;
  float dist = length(cone);
  cone = normalize(cone);

  vec3 dirLight = spotLight.dir;
  dirLight = normalize(dirLight);
  float coneAngle = dot(cone, dirLight);

  if(coneAngle > spotLight.cutoff)
  {

    vec3 n = normalize(normal);

    color = lightKernel(dirLight, n, spotLight.color, spotLight.ambientIntensity, spotLight.diffuseIntensity);

    float attenuation = spotLight.att.constant + spotLight.att.linear * dist + 
      spotLight.att.exp * dist * dist; 

    //color = color / attenuation * (1.0 - (1.0-coneAngle)/(1.0 - spotLight.cutoff));
    color = color / attenuation;// * (1.0 - (1.0-coneAngle)/(1.0 - spotLight.cutoff));

    //color = vec4(1,1,1,1);
    //color = vec4(spotLight.color,1.0) * spotLight.ambientIntensity * spotLight.diffuseIntensity;
    //color = color * spotLight.att.constant * spotLight.att.linear * spotLight.att.exp;
  }

  return color;

}

void main(void) 
{
  vec4 totalLight = getDirectionalLight(normal0);

  totalLight += getPointLight(normal0);

  totalLight += getSpotLight(normal0);

  color = texture2D(sampler, texCoord0.xy) * totalLight;
}

