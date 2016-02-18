#version 450 core 

out vec4 color; 
in vec2 vs_texCoord;

in VS_OUT
{
  vec3 N;
  vec3 L;
  vec3 V;
} fs_in;

uniform float diffuseIntensity;
uniform float specularIntensity;
uniform float specularExponent;

float base_intensity = 0.3;

uniform sampler2D sampler;

vec4 ambientLight = vec4(0.8,0.8,0.8,1);

struct DirectionalLight
{
  vec3 color;
  float ambientIntensity;
  float diffuseIntensity;
  vec3 dir;
};

uniform DirectionalLight dirLight;

void main(void) 
{

  vec3 N = normalize(fs_in.N);
  vec3 L = normalize(fs_in.L);
  vec3 V = normalize(fs_in.V);

  vec4 totalLight = ambientLight;

  color = texture2D(sampler, vs_texCoord.xy);
  vec3 base = color.xyz;

  vec3 R = reflect(-L,N);

  vec3 diffuse =  vec3(1.0,1.0,1.0) * max(dot(N,-L), 0.0) * base_intensity * diffuseIntensity;

  vec3 specular = vec3(1.0,1.0,1.0) * pow(max(dot(R, V), 0.0), specularExponent) * base_intensity * specularIntensity;

  totalLight+=(vec4(diffuse + specular, 1.0));

  color = color * (totalLight);
}

