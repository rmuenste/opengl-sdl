#include <basicshader.hpp>
#include <shaderloader.hpp>
#include <stdlib.h>
#include <vector3.h>
#include <matrix4x4.h>
#include <light.hpp>
#include <twopassshadow.hpp>

namespace i3d
{

  void TwoPassShadow::updateUniforms()
  {

    setUniform(std::string("transform"), transform_);
    setUniform(std::string("perspective"), *perspective_);
    setUniform(std::string("camera"), *cameraTranslation_);
    setUniform(std::string("cameraRotation"), *cameraRotation_);

    setUniform(std::string("sampler"), 0);
    setUniform(std::string("sampler1"), 2);

  }

}
