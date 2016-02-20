#include <basicshader.hpp>
#include <shaderloader.hpp>
#include <stdlib.h>
#include <vector3.h>
#include <matrix4x4.h>
#include <light.hpp>
#include <shadowmapshader.hpp>

namespace i3d
{

  void ShadowMapShader::updateUniforms()
  {

    setUniform(std::string("perspective"), *perspective_);
    setUniform(std::string("camera"), *cameraTranslation_);
    setUniform(std::string("cameraRotation"), *cameraRotation_);
    //setUniform(std::string("lightPos"), *lightPos_);

  }

}
