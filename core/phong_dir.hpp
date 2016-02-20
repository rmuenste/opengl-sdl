#ifndef PHONG_DIR_HPP_MKPFO97H
#define PHONG_DIR_HPP_MKPFO97H

#include <stdlib.h>
#include <iostream>
#include <string>
#include <basicshader.hpp>
#include <matrix4x4.h>
#include <vector3.h>
#include <material.hpp>

namespace i3d
{
  class PhongDir : public BasicShader
  {
  public:

    PhongDir() : BasicShader()
    {

    };

    void initShader(Vec3 &pos, Mat4 &perspective, Mat4 &cameraTranslation, Mat4 &cameraRotation, PhongMaterial &mat)
    {

      BasicShader::initShader();

      addVertexShader("../../shaders/phong_dir.vert");
      addFragmentShader("../../shaders/phong_dir.frag");

      linkShader();

      addUniform(std::string("transform"));
      addUniform(std::string("perspective"));
      addUniform(std::string("camera"));
      addUniform(std::string("cameraRotation"));

      addUniform(std::string("specularIntensity"));
      addUniform(std::string("specularExponent"));
      //addUniform(std::string("diffuseIntensity"));

      addUniform(std::string("eyePos"));

      addUniform(std::string("dirLight.diffuseIntensity"));
      addUniform(std::string("dirLight.dir"));
      addUniform(std::string("dirLight.color"));
      addUniform(std::string("dirLight.ambientIntensity"));

      material_ = &mat;

      perspective_ = &perspective;
      cameraTranslation_ = &cameraTranslation;
      cameraRotation_ = &cameraRotation;
      eyePos_ = &pos;

    }

    void updateUniforms() override;

    virtual ~PhongDir() {};

  };

}

#endif /* end of include guard: PHONG_DIR_HPP_MKPFO97H */
