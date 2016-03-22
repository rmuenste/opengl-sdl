#include <basicshader.hpp>
#include <shaderloader.hpp>
#include <stdlib.h>


namespace i3d
{
  void BasicShader::linkShader()
  {
    glLinkProgram(program_);

    char log[1024];
    int length;
    GLint status;
    glGetProgramiv(program_, GL_LINK_STATUS, &status);
    if (status == 0)
    {
      glGetProgramInfoLog(vertexShader_, sizeof(log), &length, log);
      std::cout << "Program linking failed." << std::endl;
      std::exit(EXIT_FAILURE);
    }

    glGetProgramiv(program_, GL_VALIDATE_STATUS, &status);
    if (status == 0)
    {
      std::cout << "Program validation failed." << std::endl;
      std::exit(EXIT_FAILURE);
    }

  }

  void BasicShader::addVertexShader(std::string fileName)
  {

    ShaderLoader loader;
    loader.load(fileName);
    vertexShader_ = glCreateShader(GL_VERTEX_SHADER);

    if (vertexShader_ == 0)
    {
      std::cout << "Vertex shader creation failed." << std::endl;
      std::exit(EXIT_FAILURE);
    }

    glShaderSource(vertexShader_, 1, loader.getShaderSource(), NULL);

    glCompileShader(vertexShader_);

    char log[1024];
    int length;
    GLint status;
    glGetShaderiv(vertexShader_, GL_COMPILE_STATUS, &status);
    if (status == 0)
    {
      glGetShaderInfoLog(vertexShader_, sizeof(log), &length, log);
      std::cout << "Vertex shader compilation failed." << std::endl;
      std::cout << log << std::endl;
      std::exit(EXIT_FAILURE);
    }

    glAttachShader(program_, vertexShader_);

  }

  void BasicShader::addFragmentShader(std::string fileName)
  {
    ShaderLoader loader;
    loader.load(fileName);
    fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);

    if (fragmentShader_ == 0)
    {
      std::cout << "Fragment shader creation failed." << std::endl;
      std::exit(EXIT_FAILURE);
    }

    glShaderSource(fragmentShader_, 1, loader.getShaderSource(), NULL);

    glCompileShader(fragmentShader_);

    char log[1024];
    int length;
    GLint status;
    glGetShaderiv(fragmentShader_, GL_COMPILE_STATUS, &status);
    if (status == 0)
    {
      glGetShaderInfoLog(fragmentShader_, sizeof(log), &length, log);
      std::cout << "Fragment shader compilation failed." << std::endl;
      std::cout << log << std::endl;
      std::exit(EXIT_FAILURE);
    }

    glAttachShader(program_, fragmentShader_);
  }

  void BasicShader::addGeometryShader(std::string fileName)
  {
    ShaderLoader loader;
    loader.load(fileName);
    geometryShader_ = glCreateShader(GL_GEOMETRY_SHADER);

    if (geometryShader_ == 0)
    {
      std::cout << "Geometry shader creation failed." << std::endl;
      std::exit(EXIT_FAILURE);
    }

    glShaderSource(geometryShader_, 1, loader.getShaderSource(), NULL);

    glCompileShader(geometryShader_);

    char log[1024];
    int length;
    GLint status;
    glGetShaderiv(geometryShader_, GL_COMPILE_STATUS, &status);
    if (status == 0)
    {
      glGetShaderInfoLog(geometryShader_, sizeof(log), &length, log);
      std::cout << "Geometry shader compilation failed." << std::endl;
      std::exit(EXIT_FAILURE);
    }

    glAttachShader(program_, geometryShader_);
  }

  void BasicShader::addTesselationShader(std::string fileName)
  {
  }

  void BasicShader::addUniform(std::string name)
  {

    int uniformLoc = glGetUniformLocation(program_, name.c_str());

    if (uniformLoc == -1)
    {
      std::cout << "Error: could not find uniform: " << name << std::endl;
      std::exit(EXIT_FAILURE);
    }

    uniforms_.insert({ name, uniformLoc });

  }

  template <>
  void BasicShader::setUniform<DirectionalLight>(std::string name, DirectionalLight _uniform)
  {
    int loc = (*(uniforms_.find(name))).second;
    //glUniformMatrix4fv(loc, 1, GL_TRUE, _uniform.get());
    //std::cout << _uniform << std::endl;

  }

  template <typename T>
  void setUniform(std::string name, T _uniform)
  {

  }

}
