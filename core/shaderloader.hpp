#ifndef SHADERLOADER_HPP_E2N5FDP0
#define SHADERLOADER_HPP_E2N5FDP0

#include <string>
#include <cstring>


class ShaderLoader
{
public:
  ShaderLoader () : shaderSource_(nullptr) {};

  virtual ~ShaderLoader ()  
  {
  };

  bool load(std::string fileName);

  std::string getShaderCode(){ return shaderCode_;};

  const char** getShaderSource()
  {
//    if(shaderSource_!=nullptr)
//    {
//      delete[] shaderSource_;
//      shaderSource_=nullptr;
//    }
//
//    std::strcpy(shaderSource_, shaderCode_.c_str());
//    const char **val = const_cast<const char**>(&shaderSource_);
    shaderSource_ = shaderCode_.c_str();
    return &shaderSource_;
  };

private:
  /* data */
  std::string shaderCode_;
  const char* shaderSource_;
};

#endif /* end of include guard: SHADERLOADER_HPP_E2N5FDP0 */








