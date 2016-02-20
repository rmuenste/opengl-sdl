#ifndef TWOPASSSHADOW_HPP_DFQJYGHL
#define TWOPASSSHADOW_HPP_DFQJYGHL

#include <stdlib.h>
#include <iostream>
#include <string>
#include <basicshader.hpp>
#include <matrix4x4.h>
#include <vector3.h>
#include <material.hpp>

namespace i3d
{
  class TwoPassShadow : public BasicShader
  {
  public:

    TwoPassShadow() : BasicShader()
    {

    };

    void initShader(Vec3 &lightPos, Mat4 &perspective, Mat4 &cameraTranslation, Mat4 &cameraRotation)
    {

      BasicShader::initShader();

      addVertexShader("../../shaders/two_pass_shadow.vert");
      addFragmentShader("../../shaders/two_pass_shadow.frag");

      linkShader();

      addUniform(std::string("transform"));
      addUniform(std::string("perspective"));
      addUniform(std::string("camera"));
      addUniform(std::string("cameraRotation"));
      addUniform(std::string("lightPos"));
      //addUniform(std::string("shadowMatrix"));

      addUniform(std::string("specularIntensity"));
      addUniform(std::string("specularExponent"));
      addUniform(std::string("diffuseIntensity"));

      perspective_ = &perspective;
      cameraTranslation_ = &cameraTranslation;
      cameraRotation_ = &cameraRotation;
      lightPos_ = &lightPos;

    }

    void updateUniforms();

    void setMaterial(PhongMaterial &mat)
    {
      material_ = &mat;
    }

    virtual ~TwoPassShadow() {};

  };

}

#endif /* end of include guard: TWOPASSSHADOW_HPP_DFQJYGHL */
