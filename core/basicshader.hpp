#ifndef BASICSHADER_HPP_DFQJYGHL
#define BASICSHADER_HPP_DFQJYGHL

#include <stdlib.h>
#include <iostream>
#include <string>
#include <GL/gl3w.h>
#include <GL/gl.h>
#include <shaderloader.hpp>
#include <unordered_map>
#include <vector3.h>
#include <matrix4x4.h>
#include <light.hpp>
#include <material.hpp>

//GLuint glCreateShader(GLenum type);
//GLuint glCreateProgram();

namespace i3d
{



  class BasicShader
  {
  public:

    Vec3 *lightPos_;
    Vec3 *eyePos_;
    Mat4 *perspective_;
    Mat4 *cameraTranslation_;
    Mat4 *cameraRotation_;
    Mat4 transform_;

    Mat4 *shadowMatrix_;
    Material *material_;

    DirectionalLight *dirLight_;

    void setDirectionLight(DirectionalLight *l)
    {
      dirLight_ = l;
    }

    void setTransform(const Mat4 &t)
    {
      transform_ = t;
    }

    void setMaterial(Material *m)
    {
      material_ = m;
    }

    void setShadowMatrix(Mat4 &shadowMatrix)
    {
      shadowMatrix_ = &shadowMatrix;
    }

    void setViewTransform(Mat4 &cameraTranslation, Mat4 &cameraRotation)
    {
      cameraTranslation_ = &cameraTranslation;
      cameraRotation_ = &cameraRotation;
    }

    void setPerspectiveTransform(Mat4 &perspective)
    {
      perspective_ = &perspective;
    }

    BasicShader() :
      program_(0), vertexShader_(0), fragmentShader_(0), geometryShader_(0), tesselationShader_(0)
    {


    };

    virtual ~BasicShader() {};

    virtual void updateUniforms()
    {

    }

    virtual void initShader()
    {

      program_ = glCreateProgram();

      if (program_ == 0)
      {
        std::cerr << "Could not create shader program." << std::endl;
      }

    };

    virtual void initShader(Vec3 &lightPos, Mat4 &perspective, Mat4 &cameraTranslation, Mat4 &cameraRotation)
    {

      BasicShader::initShader();

      addVertexShader("../../shaders/phong_world.vert");
      addFragmentShader("../../shaders/phong_world.frag");

      linkShader();

      addUniform(std::string("transform"));
      addUniform(std::string("perspective"));
      addUniform(std::string("camera"));
      addUniform(std::string("cameraRotation"));
      addUniform(std::string("lightPos"));

      perspective_ = &perspective;
      cameraTranslation_ = &cameraTranslation;
      cameraRotation_ = &cameraRotation;
      lightPos_ = &lightPos;

    }

    void addUniform(std::string name);

    void addVertexShader(std::string fileName);
    void addFragmentShader(std::string fileName);
    void addGeometryShader(std::string fileName);
    void addTesselationShader(std::string fileName);
    void linkShader();

    template <typename T>
    void setUniform(std::string name, T _uniform);

    void bind()
    {
      glUseProgram(program_);
    };

    //void bindTexture(int unit, GLuint id, std::string name)
    //{
    //  glActiveTexture(GL_TEXTURE1);
    //  glBindTexture(GL_TEXTURE_2D, id);
    //  setUniform(name, 1);
    //}

    /* data */
    GLuint program_;
    GLuint vertexShader_;
    GLuint fragmentShader_;
    GLuint geometryShader_;
    GLuint tesselationShader_;
    std::unordered_map<std::string, int> uniforms_;
  };

  template <>
  inline void BasicShader::setUniform<int>(std::string name, int _uniform)
  {
    int loc = (*(uniforms_.find(name))).second;
    glUniform1i(loc, _uniform);
  }

  template <>
  inline void BasicShader::setUniform<GLuint>(std::string name, GLuint _uniform)
  {
    int loc = (*(uniforms_.find(name))).second;
    glUniform1i(loc, _uniform);
  }

  template <>
  inline void BasicShader::setUniform<float>(std::string name, float _uniform)
  {
    int loc = (*(uniforms_.find(name))).second;
    glUniform1f(loc, _uniform);
  }

  template <>
  inline void BasicShader::setUniform<Vec3>(std::string name, i3d::Vec3 _uniform)
  {
    int loc = (*(uniforms_.find(name))).second;
    glUniform3f(loc, _uniform.x, _uniform.y, _uniform.z);
  }

  template <>
  inline void BasicShader::setUniform<Mat4>(std::string name, i3d::Mat4 _uniform)
  {
    int loc = (*(uniforms_.find(name))).second;
    glUniformMatrix4fv(loc, 1, GL_TRUE, _uniform.get());
    //std::cout << _uniform << std::endl;

  }

}



#endif /* end of include guard: BASICSHADER_HPP_DFQJYGHL */
