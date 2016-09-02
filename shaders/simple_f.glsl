#version 450 core 
#pragma optimize (off)

out vec4 color; 

in vec2 texCoord0;                                                                  
in vec3 normal0;                                                                    
in vec3 worldPos0;

//------------------------------------------------------------------------------------
//
//                          Structure Definition
//
//------------------------------------------------------------------------------------

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

struct PhongMaterial
{
  float diffuseIntensity; 
  float specularIntensity; 
  float specularExponent; 
};

//------------------------------------------------------------------------------------
//
//                                Uniform Variables
//
//------------------------------------------------------------------------------------

//uniform vec3 model_col;
uniform sampler2D sampler;

void main(void) 
{

  //color = vec4(1,0,0,1.0);
  color = texture2D(sampler, texCoord0.xy);

}

