#ifndef PERSPECTIVETRANSFORM_HPP_MGN84WK1
#define PERSPECTIVETRANSFORM_HPP_MGN84WK1

#include <transform.hpp>
#include <matrix4x4.h>
#include <cmath>

class PerspectiveTransform : public Transform
{
public:
  PerspectiveTransform (){};
  virtual ~PerspectiveTransform (){};

  void setPerspectiveTransform(float fov, float w, float h, float zNear, float zFar)
  {
    const float pi = 4.0 * std::atan(1.0);
    float myFov = fov * pi/(180.0);
    pMat_.setZero();
    float tanHalfAlpha = std::tan(myFov * 0.5);
    float zRange = zNear - zFar;
    float ar = w/h;
    pMat_.m_Entries[0]  = 1.0/(tanHalfAlpha*ar); pMat_.m_Entries[1] =  0;              pMat_.m_Entries[2] = 0;    pMat_.m_Entries[3] = 0; 
    pMat_.m_Entries[4]  = 0;                     pMat_.m_Entries[5] = 1.0/tanHalfAlpha;pMat_.m_Entries[6] = 0;    pMat_.m_Entries[7] = 0; 
    pMat_.m_Entries[8]  = 0;                     pMat_.m_Entries[9] = 0;               pMat_.m_Entries[10] = (-zNear-zFar)/zRange;    pMat_.m_Entries[11] = 2.0*zFar*zNear/zRange; 
    pMat_.m_Entries[12] = 0;                     pMat_.m_Entries[13] = 0;              pMat_.m_Entries[14] = 1;    pMat_.m_Entries[15] = 0; 
  }

  i3d::Mat4& getPerspectiveTransform()
  {
    return pMat_;
  }

private:
  /* data */
  i3d::Mat4 pMat_;
};


#endif /* end of include guard: PERSPECTIVETRANSFORM_HPP_MGN84WK1 */
