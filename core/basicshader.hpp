#ifndef BASICSHADER_HPP_DFQJYGHL
#define BASICSHADER_HPP_DFQJYGHL

#include <stdlib.h>
#include <iostream>
#include <string>
#include <GL/gl3w.h>
#include <GL/gl.h>
#include <shaderloader.hpp>

//GLuint glCreateShader(GLenum type);
//GLuint glCreateProgram();

class BasicShader
{
public:
  BasicShader () : 
    program_(0), vertexShader_(0), fragmentShader_(0), geometryShader_(0), tesselationShader_(0)
  {


  };

  virtual ~BasicShader () {};

  void initShader()
  {

    program_ = glCreateProgram();

    if(program_ == 0)
    {
      std::cerr << "Could not create shader program." << std::endl;
    }

  };
  void addVertexShader(std::string fileName);
  void addFragmentShader(std::string fileName);
  void addGeometryShader(std::string fileName);
  void addTesselationShader(std::string fileName);
  void linkShader();

  void bind()
  {
    glUseProgram(program_);
  };

private:
  /* data */
  GLuint program_;
  GLuint vertexShader_;
  GLuint fragmentShader_;
  GLuint geometryShader_;
  GLuint tesselationShader_;
};

#endif /* end of include guard: BASICSHADER_HPP_DFQJYGHL */