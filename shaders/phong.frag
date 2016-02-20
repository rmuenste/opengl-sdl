#version 450 core 

out vec4 color; 
in vec2 vs_texCoord;

in VS_OUT
{
  vec3 N;
  vec3 L;
  vec3 V;
} fs_in;

vec3 diffuse_albedo = vec3(0.5, 0.2, 0.7);
vec3 specular_albedo = vec3(1.0,1.0,1.0);
float specular_power = 20.0;

uniform sampler2D sampler;

void main(void) 
{

  vec3 N = normalize(fs_in.N);
  vec3 L = normalize(fs_in.L);
  vec3 V = normalize(fs_in.V);

  vec4 ambientLight = vec4(0.4,0.4,0.4,1);
  color = texture2D(sampler, vs_texCoord.xy);

  vec3 R = reflect(-L,N);

  vec3 diffuse = max(dot(N,L), 0.0) * color.xyz;
  diffuse += color.xyz * vec3(ambientLight);

  vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * specular_albedo;

  color = vec4(diffuse + specular, 1.0); 

}

