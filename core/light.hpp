#ifndef LIGHT_HPP_5LMS1ZES
#define LIGHT_HPP_5LMS1ZES

#include <vector3.h>
#include <matrix4x4.h>

namespace i3d {

  class Light
  {
    public:

      Light() {};

      Light(const Vec3 &pos, const Vec3 &color, float intensity) : pos_(pos), color_(color), intensity_(intensity)
      {

      };

      Light(const Light &copy) : pos_(copy.pos_), color_(copy.color_), intensity_(copy.intensity_)
      {

      }

      virtual ~Light() {};

      Vec3& getPos()
      {
        return pos_;
      }

      void setPos(const Vec3 &pos)
      {
        pos_ = pos;
      }

      Vec3& getColor()
      {
        return color_;
      }

      void setColor(const Vec3 &color)
      {
        color_ = color;
      }

      float getIntensity()
      {
        return intensity_;
      }

      void setIntensity(float intensity)
      {
        intensity_ = intensity;
      }

      Vec3 pos_; 

      Vec3 color_;

      float intensity_;

  };

  struct DirectionalLight {

    Vec3 color_;
    float ambientIntensity_;
    Vec3 dir_;
    float diffuseIntensity_;

  };

  struct Attenuation
  {
    float constant_;
    float linear_;
    float exp_;
  };

  struct PointLight {

    Vec3 color_;
    float ambientIntensity_;
    float diffuseIntensity_;

    Vec3 position_; 

    Attenuation att_;

  };

  //class DirectionalLight
  //{
  //  public:

  //    Vec3 dir_;

  //    Light light_;

  //    DirectionalLight(const Vec3 &dir, const Light &light);

  //    ~DirectionalLight();

  //    Vec3& getDir()
  //    {
  //      return dir_;
  //    }

  //    void setDir(const Vec3 &dir)
  //    {
  //      dir_ = dir;
  //    }

  //  private:

  //};



}

#endif // !LIGHT_HPP