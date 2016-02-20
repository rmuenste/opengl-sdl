#ifndef PHONGSHADER_HPP_DFQJYGHL
#define PHONGSHADER_HPP_DFQJYGHL

#include <stdlib.h>
#include <iostream>
#include <string>
#include <basicshader.hpp>
#include <matrix4x4.h>
#include <vector3.h>
#include <material.hpp>

namespace i3d
{
  class PhongShader : public BasicShader
  {
  public:

    PhongShader() : BasicShader()
    {

    };

    void initShader(Vec3 &lightPos, Mat4 &perspective, Mat4 &cameraTranslation, Mat4 &cameraRotation, PhongMaterial &mat)
    {

      BasicShader::initShader();

      addVertexShader("../../shaders/phong_world.vert");
      addFragmentShader("../../shaders/phong_world.frag");

      linkShader();

      addUniform(std::string("transform"));
      addUniform(std::string("perspective"));
      addUniform(std::string("camera"));
      addUniform(std::string("cameraRotation"));
      addUniform(std::string("lightPos"));

      addUniform(std::string("specularIntensity"));
      addUniform(std::string("specularExponent"));
      addUniform(std::string("diffuseIntensity"));

      material_ = &mat;

      perspective_ = &perspective;
      cameraTranslation_ = &cameraTranslation;
      cameraRotation_ = &cameraRotation;
      lightPos_ = &lightPos;

    }

    void updateUniforms() override;

    virtual ~PhongShader() {};

  };

}

#endif /* end of include guard: PHONGSHADER_HPP_DFQJYGHL */
