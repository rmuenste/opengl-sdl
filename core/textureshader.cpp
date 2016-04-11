#include <basicshader.hpp>
#include <shaderloader.hpp>
#include <stdlib.h>
#include <vector3.h>
#include <matrix4x4.h>
#include <light.hpp>
#include <textureshader.hpp>

namespace i3d
{

  void TextureShader::updateUniforms()
  {

    setUniform(std::string("transform"), transform_);
    setUniform(std::string("perspective"), *perspective_);
    setUniform(std::string("camera"), *cameraTranslation_);
    setUniform(std::string("cameraRotation"), *cameraRotation_);
    setUniform(std::string("eyePos"), eyePos_);

    setUniform(std::string("specularIntensity"), material_->getSpecularIntensity());
    setUniform(std::string("specularExponent"), material_->getSpecularExponent());

    setUniform(std::string("dirLight.color"), dirLight_->color_);
    setUniform(std::string("dirLight.ambientIntensity"), dirLight_->ambientIntensity_);
    setUniform(std::string("dirLight.diffuseIntensity"), dirLight_->diffuseIntensity_);
    setUniform(std::string("dirLight.dir"), dirLight_->dir_);

    setUniform(std::string("pointLight.color"), pointLight_->color_);
    setUniform(std::string("pointLight.ambientIntensity"), pointLight_->ambientIntensity_);
    setUniform(std::string("pointLight.diffuseIntensity"), pointLight_->diffuseIntensity_);
    setUniform(std::string("pointLight.position"), pointLight_->position_);

    setUniform(std::string("pointLight.att.constant"), pointLight_->att_.constant_);
    setUniform(std::string("pointLight.att.linear"), pointLight_->att_.linear_);
    setUniform(std::string("pointLight.att.exp"), pointLight_->att_.exp_);

    setUniform(std::string("sampler"), 0);
    setUniform(std::string("sampler1"), 2);

  }

}
