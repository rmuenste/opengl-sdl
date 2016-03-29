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

    virtual void bindMaterial(Material *m) override
    {
      material_ = m;
      material_->bindTexture();
    }

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

      addUniform(std::string("spotLight.color"));
      addUniform(std::string("spotLight.position"));
      addUniform(std::string("spotLight.dir"));
      addUniform(std::string("spotLight.ambientIntensity"));
      addUniform(std::string("spotLight.diffuseIntensity"));
      addUniform(std::string("spotLight.cutoff"));
                              
                              
      addUniform(std::string("spotLight.att.constant"));
      addUniform(std::string("spotLight.att.linear"));
      addUniform(std::string("spotLight.att.exp"));

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
