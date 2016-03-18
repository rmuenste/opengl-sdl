#include <basicshader.hpp>
#include <shaderloader.hpp>
#include <stdlib.h>
#include <vector3.h>
#include <matrix4x4.h>
#include <light.hpp>
#include <phong.hpp>

namespace i3d
{

  void Phong::updateUniforms()
  {

    setUniform(std::string("transform"), transform_);
    setUniform(std::string("perspective"), *perspective_);
    setUniform(std::string("camera"), *cameraTranslation_);
    setUniform(std::string("cameraRotation"), *cameraRotation_);
    setUniform(std::string("eyePos"), *eyePos_);

    
    setUniform(std::string("specularIntensity"), material_->getSpecularIntensity());
    setUniform(std::string("specularExponent"), material_->getSpecularExponent());

    setUniform(std::string("dirLight.color"), dirLight_->color_);
    setUniform(std::string("dirLight.ambientIntensity"), dirLight_->ambientIntensity_);
    setUniform(std::string("dirLight.diffuseIntensity"), dirLight_->diffuseIntensity_);
    setUniform(std::string("dirLight.dir"), dirLight_->dir_);

  }

}
