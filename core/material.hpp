#ifndef MATERIAL_HPP_XJ2A6HC5
#define MATERIAL_HPP_XJ2A6HC5

#include <unordered_map>
#include <texture.hpp>

namespace i3d
{

  class Material
  {
  public:

    std::unordered_map<std::string, Texture> textures_;

    //void addTexture()

    float getSpecularIntensity();
    float getSpecularExponent();
    float getDiffuseIntensity();

    Material();
    ~Material();

  private:

  };



  class PhongMaterial : public Material
  {
  public:

    float specularIntensity_;
    float specularExponent_;
    float diffuseIntensity_;

    PhongMaterial(float si, float se, float di) : specularIntensity_(si), specularExponent_(se), diffuseIntensity_(di)
    {

    }

    PhongMaterial()
    {
    }

    ~PhongMaterial()
    {
    }

    float PhongMaterial::getSpecularIntensity();

    float PhongMaterial::getSpecularExponent();

    float PhongMaterial::getDiffuseIntensity();

  private:

  };


}


#endif /* end of include guard: MATERIAL_HPP_XJ2A6HC5 */