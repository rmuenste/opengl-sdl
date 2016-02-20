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

    void initShader(Mat4 &perspective, Mat4 &cameraTranslation, Mat4 &cameraRotation)
    {

      BasicShader::initShader();

      addVertexShader("../../shaders/texture_shader.vert");
      addFragmentShader("../../shaders/texture_shader.frag");

      linkShader();

      addUniform(std::string("transform"));
      addUniform(std::string("perspective"));
      addUniform(std::string("camera"));
      addUniform(std::string("cameraRotation"));

      perspective_ = &perspective;
      cameraTranslation_ = &cameraTranslation;
      cameraRotation_ = &cameraRotation;

    }

    void updateUniforms();

    virtual ~TextureShader() {};

  };

}

#endif /* end of include guard: TEXTURESHADER_HPP_DFQJYGHL */
