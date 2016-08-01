#ifndef CAMERA_HPP_MZ1YSJ5P
#define CAMERA_HPP_MZ1YSJ5P

#include <vector3.h>
#include <matrix4x4.h>

namespace i3d {

  class Camera
  {
    public:
      Camera () {};
      virtual ~Camera () {};

      void initCamera(const Vec3 &pos, const Vec3 &u, const Vec3 &v, const Vec3 &n);

      void setLookAt(const Vec3 &pos, const Vec3 &look, const Vec3 &up);

      Mat4& getCameraCoordinateTransform();

      Mat4& getCameraTranslationTransform();

      Vec3  getPos(){return pos_;}
      Vec3* getPosPointer(){return &pos_;}

      Vec3 getU(){return u_;}

      Vec3 getV(){ return v_; }

      Vec3 getN(){return n_;}

      // rotate camera around the global 'up' axis
      void rotateY(float alpha);

      // rotate camera to look up/down
      void rotateX(float alpha);

      // rotate camera around the global 'up' axis
      inline void rotateHorizontal(float alpha)
      {
        rotateX(alpha);
      }

      // rotate camera to look up/down
      inline void rotateVertical(float alpha)
      {
        rotateY(alpha);
      }

      void moveU(float speed)
      {
        pos_ += speed * u_;
      }

      void moveV(float speed)
      {
        pos_ += speed * v_;
      }

      void moveN(float speed)
      {
        pos_ += speed * n_;
      }

      void outputCameraConfig()
      {
        std::cout << "---Camera UVN---" << std::endl;
        std::cout << "U(right): " << u_;
        std::cout << "U(norm): " << u_.mag() << std::endl;
        std::cout << "V(up): " << v_;
        std::cout << "V(norm): " << v_.mag() << std::endl;
        std::cout << "N(look): " << n_;
        std::cout << "N(norm): " << n_.mag() << std::endl;
        std::cout << "----------------" << std::endl;
        std::cout << std::endl;
      }

  private:
      /* data */
      // u is the 'right' vector
      Vec3 u_;

      // v is the 'up' vector
      Vec3 v_;

      // n is the 'forward' or 'lookat' vector
      Vec3 n_;
      //
      // pos is the position of the camera
      Vec3 pos_;

      // camera coordinate transformation matrix
      Mat4 matrix_;
      
      // camera translation transformation matrix
      Mat4 translationMatrix_;

  };

}



#endif /* end of include guard: CAMERA_HPP_MZ1YSJ5P */
