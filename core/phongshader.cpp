#include <basicshader.hpp>
#include <shaderloader.hpp>
#include <stdlib.h>
#include <vector3.h>
#include <matrix4x4.h>
#include <light.hpp>
#include <phongshader.hpp>

namespace i3d
{

  void PhongShader::updateUniforms()
  {

    setUniform(std::string("transform"), transform_);
    setUniform(std::string("perspective"), *perspective_);
    setUniform(std::string("camera"), *cameraTranslation_);
    setUniform(std::string("cameraRotation"), *cameraRotation_);
    setUniform(std::string("lightPos"), *lightPos_);

    
    setUniform(std::string("specularIntensity"), material_->getSpecularIntensity());
    setUniform(std::string("specularExponent"), material_->getSpecularExponent());
    setUniform(std::string("diffuseIntensity"), material_->getDiffuseIntensity());

  }

}
