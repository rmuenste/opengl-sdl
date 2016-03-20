#ifndef TEXTURESHADER_HPP_DFQJYGHL
#define TEXTURESHADER_HPP_DFQJYGHL

#include <stdlib.h>
#include <iostream>
#include <string>
#include <basicshader.hpp>
#include <matrix4x4.h>
#include <vector3.h>
#include <material.hpp>

namespace i3d
{
  class TextureShader : public BasicShader
  {
  public:

    TextureShader() : BasicShader()
    {

    };

    void initShader(Vec3 &pos, Mat4 &perspective, Mat4 &cameraTranslation, Mat4 &cameraRotation, PhongMaterial &mat)
    {

      BasicShader::initShader();

      addVertexShader("../../shaders/texture_shader.vert");
      addFragmentShader("../../shaders/texture_shader.frag");

      linkShader();

      addUniform(std::string("transform"));
      addUniform(std::string("perspective"));
      addUniform(std::string("camera"));
      addUniform(std::string("cameraRotation"));

      addUniform(std::string("specularIntensity"));
      addUniform(std::string("specularExponent"));

      addUniform(std::string("eyePos"));

      addUniform(std::string("dirLight.diffuseIntensity"));
      addUniform(std::string("dirLight.dir"));
      addUniform(std::string("dirLight.color"));
      addUniform(std::string("dirLight.ambientIntensity"));

      addUniform(std::string("pointLight.color"));
      addUniform(std::string("pointLight.ambientIntensity"));
      addUniform(std::string("pointLight.diffuseIntensity"));
      addUniform(std::string("pointLight.position"));

      addUniform(std::string("pointLight.att.constant"));
      addUniform(std::string("pointLight.att.linear"));
      addUniform(std::string("pointLight.att.exp"));

      material_ = &mat;

      perspective_ = &perspective;
      cameraTranslation_ = &cameraTranslation;
      cameraRotation_ = &cameraRotation;
      eyePos_ = &pos;

    }

    void updateUniforms() override;

    virtual ~TextureShader() {};

  };

}

#endif /* end of include guard: TEXTURESHADER_HPP_DFQJYGHL */
