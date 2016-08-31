#ifndef SIMPLESHADER_HPP_8CERZIYR
#define SIMPLESHADER_HPP_8CERZIYR

#include <stdlib.h>
#include <iostream>
#include <string>
#include <basicshader.hpp>
#include <matrix4x4.h>
#include <vector3.h>
#include <material.hpp>

namespace i3d
{
  class MaterialShader : public BasicShader
  {
  public:

    MaterialShader() : BasicShader()
    {

    };

    void initShader(Vec3 &pos, Mat4 &perspective, Mat4 &cameraTranslation, Mat4 &cameraRotation)
    {

      BasicShader::initShader();

      addVertexShader("../../shaders/simple_v.glsl");
      addFragmentShader("../../shaders/simple_f.glsl");

      linkShader();

      addUniform(std::string("transform"));
      addUniform(std::string("perspective"));
      addUniform(std::string("camera"));
      addUniform(std::string("cameraRotation"));

      addUniform(std::string("model_col"));

      perspective_ = &perspective;
      cameraTranslation_ = &cameraTranslation;
      cameraRotation_ = &cameraRotation;
      eyePos_ = pos;

    }

    void updateUniforms() override;

    void bindMaterial(Material *m) override
    {
      material_ = m;

      glActiveTexture(GL_TEXTURE0 + 0);
      shadowTex_->bind();
      glBindSampler(0, shadowTex_->id_);

      glActiveTexture(GL_TEXTURE0 + 2);
      material_->bindTexture();
      glBindSampler(2, material_->textures_.front()->id_);

      //glActiveTexture(GL_TEXTURE0 + 0);
      //material_->bindTexture();
      //glBindSampler(0, material_->textures_.front()->id_);

      //glActiveTexture(GL_TEXTURE0 + 2);
      //shadowTex_->bind();
      //glBindSampler(2, shadowTex_->id_);



    }

    virtual ~MaterialShader() {};

  };

}

#endif /* end of include guard: SIMPLESHADER_HPP_8CERZIYR */
