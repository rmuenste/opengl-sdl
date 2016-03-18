#version 450 core 

out vec4 color; 
in vec2 vs_texCoord;
in vec4 vs_cameraCoord; 

in eye_coordS_OUT
{
  vec3 eye_normal;
  vec3 L;
  vec3 eye_coord;
  vec3 world_coord;
  vec4 shadow_coord;
} fs_in;


uniform float diffuseIntensity;
uniform float specularIntensity;
uniform float specularExponent;
float base_intensity = 0.3;

uniform sampler2D sampler;
uniform sampler2D depthTexture;

struct Attenuation
{
  float constant;
  float linear;
  float exponent;
};

vec4 ambientLight = vec4(0.8,0.8,0.8,1);

void main(void) 
{

  vec3 eye_normal = normalize(fs_in.eye_normal);
  vec3 L = normalize(fs_in.L);
  vec3 eye_coord = normalize(fs_in.eye_coord);

  vec4 totalLight = ambientLight;

  color = texture2D(sampler, vs_texCoord.xy);
  vec3 base = color.xyz;

  vec3 R = reflect(-L,eye_normal);

  vec3 diffuse =  vec3(1.0,1.0,1.0) * max(dot(eye_normal,-L), 0.0) * base_intensity * diffuseIntensity;

  vec3 specular = vec3(1.0,1.0,1.0) * pow(max(dot(R, eye_coord), 0.0), specularExponent) * base_intensity * specularIntensity;

  vec3 shadowMapCoord = (fs_in.shadow_coord.xyz/fs_in.shadow_coord.w) * vec3(0.5) + vec3(0.5);

  float f = texture2D(depthTexture,shadowMapCoord.xy).x;

  float shadow = 1.0;
  if(shadowMapCoord.z > f)
    shadow = 0.0;

  totalLight += shadow * (vec4(diffuse + specular, 1.0));

  color = color * (totalLight);
  color.r = fs_in.shadow_coord.x;	
  color.g = fs_in.shadow_coord.y;	

}

