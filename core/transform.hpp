#ifndef TRANSFORM_HPP_HKYGHV2N
#define TRANSFORM_HPP_HKYGHV2N

#include <vector4.h>
#include <matrix4x4.h>
#include <vector3.h>
#include <quaternion.h>

class Transform
{
public:
  Transform ();
  virtual ~Transform ();

  i3d::Vec4 translation_;
  i3d::Quat rotation_;
  i3d::Vec3 scale_;

  inline i3d::Mat4 getMatrix()
  {
    i3d::Mat4 translationMatrix;
    translationMatrix.setIdentity();
    translationMatrix.m_03 = translation_.x; 
    translationMatrix.m_13 = translation_.y; 
    translationMatrix.m_23 = translation_.z; 

    i3d::Mat4 rotationMatrix = rotation_.getMatrix4();

    i3d::Mat4 scaleMatrix;
    scaleMatrix.setIdentity();
    scaleMatrix.m_00 = scale_.x;
    scaleMatrix.m_11 = scale_.y;
    scaleMatrix.m_22 = scale_.z;

    return  translationMatrix * rotationMatrix * scaleMatrix;
  }

  inline void setRotationEuler(const i3d::Vec3 &eulerAngles)
  {
    rotation_.createFromEulerAngles(eulerAngles.y, eulerAngles.z, eulerAngles.x);
  }

private:
  /* data */
};

#endif /* end of include guard: TRANSFORM_HPP_HKYGHV2N */
