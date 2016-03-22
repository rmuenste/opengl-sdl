#ifndef MATERIAL_HPP_XJ2A6HC5
#define MATERIAL_HPP_XJ2A6HC5

#include <unordered_map>
#include <texture.hpp>
#include <vector>

namespace i3d
{

  class Material
  {
  public:

    std::vector<Texture*> textures_;

    //void addTexture()

    virtual float getSpecularIntensity(){ return 0.0f; };
    virtual float getSpecularExponent(){ return 0.0f; };
    virtual float getDiffuseIntensity(){ return 0.0f; };

    virtual void bindTexture()
    {
      if (!textures_.empty())
      {
        textures_.front()->bind();
      }
    }

    Material()=default;

    virtual ~Material();

  private:

  };

  class PhongMaterial : public Material
  {
  public:

    float specularIntensity_;
    float specularExponent_;
    float diffuseIntensity_;

    explicit PhongMaterial(float si, float se, float di) : specularIntensity_(si), specularExponent_(se), diffuseIntensity_(di)
    {

    }

    explicit PhongMaterial(float si, float se, float di, Texture *tex) : specularIntensity_(si), specularExponent_(se), diffuseIntensity_(di)
    {
      textures_.push_back(tex);
    }

    PhongMaterial() = default;

    virtual ~PhongMaterial()
    {
    }

    float getSpecularIntensity() override
    {
      return specularIntensity_;
    }

    float getSpecularExponent() override
    {
      return specularExponent_;
    }

    float getDiffuseIntensity() override
    {
      return diffuseIntensity_;
    }

    void bindTexture() override
    {
      if (!textures_.empty())
      {
        textures_.front()->bind();
      }
    }

  private:

  };

}


#endif /* end of include guard: MATERIAL_HPP_XJ2A6HC5 */
