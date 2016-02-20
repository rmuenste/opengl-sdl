#include <material.hpp>

namespace i3d
{

  Material::Material()
  {
  }

  Material::~Material()
  {
  }

  float PhongMaterial::getSpecularIntensity()
  {
    return specularIntensity_;
  }

  float PhongMaterial::getSpecularExponent()
  {
    return specularExponent_;
  }

  float PhongMaterial::getDiffuseIntensity()
  {
    return diffuseIntensity_;
  }



}