#include <camera.hpp>
#include <quaternion.h>

namespace i3d {

  void Camera::initCamera(const Vec3 &pos, const Vec3 &u, const Vec3 &v, const Vec3 &n)
  {
    pos_ = pos;
    u_   = u;
    v_   = v;
    n_   = n;
    u_.normalize();
    v_.normalize();
    n_.normalize();
  }

  /*
  * Vec3 up tells us where the global 'up' axis is
  *
  */
  void Camera::setLookAt(const Vec3 &pos, const Vec3 &look, const Vec3 &up)
  {
    pos_ = pos;
    n_ = look;
    n_.normalize();

    // look x up gives us a right vector
    u_ = Vec3::Cross(n_, up);

    // look x right gives us the camera 'up' vectors
    v_ = Vec3::Cross(u_, n_);
  }

  Mat4& Camera::getCameraCoordinateTransform()
  {
    matrix_.m_Entries[0]  = u_.x; matrix_.m_Entries[1] = u_.y; matrix_.m_Entries[2]  = u_.z; matrix_.m_Entries[3]  = 0; 
    matrix_.m_Entries[4]  = v_.x; matrix_.m_Entries[5] = v_.y; matrix_.m_Entries[6]  = v_.z; matrix_.m_Entries[7]  = 0; 
    matrix_.m_Entries[8]  = n_.x; matrix_.m_Entries[9] = n_.y; matrix_.m_Entries[10] = n_.z; matrix_.m_Entries[11] = 0; 
    matrix_.m_Entries[12] = 0; matrix_.m_Entries[13]   = 0; matrix_.m_Entries[14]    = 0; matrix_.m_Entries[15]    = 1; 
    return matrix_;
  }

  Mat4& Camera::getCameraTranslationTransform()
  {
    translationMatrix_.setIdentity();
    translationMatrix_.m_03 = -pos_.x;
    translationMatrix_.m_13 = -pos_.y;
    translationMatrix_.m_23 = -pos_.z;
    return translationMatrix_;
  }

  void Camera::rotateY(float alpha)
  {
    
    Vec3 axis(0,1,0);

    n_ = Quat::rotate(n_, axis, alpha);
    n_.normalize();

    u_ = Vec3::Cross(axis,n_);
    u_.normalize();

  }

  
  void Camera::rotateX(float alpha)
  {

    n_ = Quat::rotate(n_, u_, alpha);
    n_.normalize();

    v_ = Vec3::Cross(n_,u_);
    v_.normalize();

  }

}



