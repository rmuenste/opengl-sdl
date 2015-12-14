#version 450 core 

out vec4 color; 
in vec2 vs_texCoord;
in vec4 vs_cameraCoord; 

in VS_OUT
{
  vec3 N;
  vec3 L;
  vec3 V;
} fs_in;


vec3 diffuse_albedo = vec3(1.0,1.0,1.0);
vec3 specular_albedo = vec3(1.0,1.0,1.0);
float specular_power = 30.0;
float base_intensity = 0.3;

uniform sampler2D sampler;

struct Attenuation
{
  float constant;
  float linear;
  float exponent;
};

vec3 point_color;
vec3 point_pos = vec3(0.0,0.0,3.0);
float point_intensity;
Attenuation point_attenuation;

vec4 ambientLight = vec4(0.8,0.8,0.8,1);

float calcPointLightAtt(vec3 normal)
{
  
  vec3 dir = vs_cameraCoord.xyz - point_pos;

  float dist = length(dir);
  dir = normalize(dir);

  float attenuation = point_attenuation.constant + point_attenuation.linear * dist + point_attenuation.exponent * dist * dist;
  return attenuation;

}

vec3 directLightCalc(vec3 N, vec3 L, vec3 V, vec3 tcolor, float baseIntensity, vec3 diffAlbedo, vec3 specAlbedo, float specPower)
{
  vec3 R = reflect(-L,N);

  vec3 diffuse = max(dot(N,-L), 0.0) * baseIntensity * diffAlbedo;
  diffuse += tcolor * vec3(ambientLight);

  vec3 specular = pow(max(dot(R, V), 0.0), specPower) * specAlbedo;

  vec3 color = vec3(diffuse + specular); 

  return color;
}

void main(void) 
{

  vec3 N = normalize(fs_in.N);
  vec3 L = normalize(fs_in.L);
  vec3 V = normalize(fs_in.V);

  vec4 totalLight = ambientLight;

  color = texture2D(sampler, vs_texCoord.xy);
  vec3 base = color.xyz;

  vec3 R = reflect(-L,N);

  vec3 diffuse = max(dot(N,-L), 0.0) * base_intensity * diffuse_albedo;
  //diffuse += color.xyz * ambientLight.xyz;

  vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * 5.0 * specular_albedo;

  totalLight+=(vec4(diffuse + specular, 1.0));
  //color = (vec4(diffuse + specular, 1.0)); 
  color = color * (totalLight);
}

