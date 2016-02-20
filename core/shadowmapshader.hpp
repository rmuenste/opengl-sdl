#ifndef SHADOWMAPSHADER_HPP_DFQJYGHL
#define SHADOWMAPSHADER_HPP_DFQJYGHL

#include <stdlib.h>
#include <iostream>
#include <string>
#include <basicshader.hpp>
#include <matrix4x4.h>
#include <vector3.h>
#include <material.hpp>

namespace i3d
{
  class ShadowMapShader : public BasicShader
  {
  public:

    ShadowMapShader() : BasicShader()
    {

    };

    void initShader(Vec3 &lightPos, Mat4 &perspective, Mat4 &cameraTranslation, Mat4 &cameraRotation)
    {

      BasicShader::initShader();

      addVertexShader("../../shaders/shadow_map.vert");
      addFragmentShader("../../shaders/shadow_map.frag");

      linkShader();

      addUniform(std::string("transform"));
      addUniform(std::string("perspective"));
      addUniform(std::string("camera"));
      addUniform(std::string("cameraRotation"));
      //addUniform(std::string("lightPos"));

      perspective_ = &perspective;
      cameraTranslation_ = &cameraTranslation;
      cameraRotation_ = &cameraRotation;
      lightPos_ = &lightPos;

    }

    void updateUniforms();

    virtual ~ShadowMapShader() {};

  };

}

#endif /* end of include guard: SHADOWMAPSHADER_HPP_DFQJYGHL */
