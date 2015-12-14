#ifndef MESH_HPP_5LMS1ZES
#define MESH_HPP_5LMS1ZES

#include <3dmodel.h>
#include <basicshader.hpp>
#include <transform.hpp>
#include <texture.hpp>
#include <vector>
#include <matrix4x4.h>
#include <GL/gl.h>
#include <objloader.h>

namespace i3d {

  enum NormalGeneration
  {
    averaged_normals,
    fake_vertex
  };

  enum RenderMode
  {
    element_render,
    array_render 
  };

  // export from blender
  // Z up, Y forward -> Cull counter-clockwise
  // load obj: swap y with z
  template<int rendermode = array_render>
  class Mesh
  {
    public:
      Mesh ()
      {
        this->shader_ = nullptr;
        hasTexture_ = false;
      };

      virtual ~Mesh (){};

      void loadMesh(std::string fileName);

      void loadTexture(std::string fileName);

      void render();

      void setVertexShader(std::string fileName);

      void setFragmentShader(std::string fileName);

      void initRender();

      void initNonIndexedRender();

      void initNonIndexedRender(int a0, int a1, int a2);
       
      void rotate(const Vec3 &v);
      
      void buildSmoothNormals();

      void buildFakeVertexNormals();

      void render(const Mat4 &perspective, const Mat4 &cameraTrans, const Mat4 &cameraCoord, const Vec3 &cameraPos);

      void renderNonIndexed(const Mat4 &perspective, const Mat4 &cameraTrans, const Mat4 &cameraCoord, const Vec3 &cameraPos);

      void renderNonIndexed(const Mat4 &perspective, const Mat4 &cameraTrans, const Mat4 &cameraCoord, const Vec3 &cameraPos, int a0, int a1, int a2);

      Transform transform_;

      BasicShader* shader_;

      Texture texture_;

      Model3D model_;

      bool hasTexture_;

    private:
      /* data */
      GLuint vao;
      GLuint iao;
      GLuint buffers[3];
      int drawVertices_;
  };

  template<>
  void Mesh<array_render>::loadMesh(std::string fileName)
  {
    ObjLoader loader;
    loader.readModelFromFile(&model_, fileName.c_str());
    model_.prepareNonIndexedRendering();
  }

  template<>
  void Mesh<array_render>::buildSmoothNormals()
  {
    model_.buildSmoothNormals();
  }

  template<>
  void Mesh<array_render>::buildFakeVertexNormals()
  {
    model_.buildFakeVertexNormals();
  }

  template<>
  void Mesh<array_render>::loadTexture(std::string fileName)
  {
    texture_.createTextureFromImage(fileName);
    hasTexture_ = true;
  }

  template<>
  void Mesh<array_render>::setFragmentShader(std::string fileName)
  {
    shader_->addFragmentShader(fileName);
  }

  template<>
  void Mesh<array_render>::setVertexShader(std::string fileName)
  {
    shader_->addVertexShader(fileName);
  }

  template<>
  void Mesh<array_render>::initRender()
  {
    drawVertices_ = 3 * model_.meshes_.front().faces_.size();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(3, &buffers[0]);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, model_.meshes_[0].vertices_.size() * 3 * sizeof(float),
      model_.meshes_.front().vertices_.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    if (hasTexture_)
    {
      glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
      glBufferData(GL_ARRAY_BUFFER, model_.meshes_.front().orderedTexCoords_.size() * 2 * sizeof(float),
        model_.meshes_.front().orderedTexCoords_.data(), GL_STATIC_DRAW);

      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, model_.meshes_[0].vertexNormals_.size() * 3 * sizeof(float),
      model_.meshes_.front().vertexNormals_.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  }

  template<>
  void Mesh<array_render>::initNonIndexedRender()
  {

    drawVertices_ = 3 * model_.meshes_.front().faces_.size();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(3, &buffers[0]);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, model_.meshes_[0].vertices_.size() * 3 * sizeof(float),
      model_.meshes_.front().vertices_.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    if (hasTexture_)
    {

      glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
      glBufferData(GL_ARRAY_BUFFER, model_.meshes_.front().orderedTexCoords_.size() * 2 * sizeof(float),
        model_.meshes_.front().orderedTexCoords_.data(), GL_STATIC_DRAW);

      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, model_.meshes_[0].vertexNormals_.size() * 3 * sizeof(float),
      model_.meshes_.front().vertexNormals_.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  }

  template<>
  void Mesh<array_render>::initNonIndexedRender(int a0, int a1, int a2)
  {

    drawVertices_ = 3 * model_.meshes_.front().faces_.size();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(3, &buffers[0]);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, model_.meshes_[0].vertices_.size() * 3 * sizeof(float),
      model_.meshes_.front().vertices_.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(a0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    if (hasTexture_)
    {

      glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
      glBufferData(GL_ARRAY_BUFFER, model_.meshes_.front().orderedTexCoords_.size() * 2 * sizeof(float),
        model_.meshes_.front().orderedTexCoords_.data(), GL_STATIC_DRAW);

      glVertexAttribPointer(a1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, model_.meshes_[0].vertexNormals_.size() * 3 * sizeof(float),
      model_.meshes_.front().vertexNormals_.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(a2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  }

  template<>
  void Mesh<array_render>::render(const Mat4 &perspective, const Mat4 &cameraTrans, const Mat4 &cameraCoord, const Vec3 &cameraPos)
  {

    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);

    if (hasTexture_)
    {

      glEnableVertexAttribArray(1);

    }

    glEnableVertexAttribArray(2);

    shader_->bind();

    shader_->setUniform(std::string("transform"), transform_.getMatrix());
    shader_->setUniform(std::string("perspective"), perspective);
    shader_->setUniform(std::string("camera"), cameraTrans);
    shader_->setUniform(std::string("cameraRotation"), cameraCoord);

    texture_.bind();

    glDrawArrays(GL_TRIANGLES, 0, drawVertices_);

    glDisableVertexAttribArray(0);

    if (hasTexture_)
    {
      glDisableVertexAttribArray(1);
    }

    glDisableVertexAttribArray(2);

  }

  template<>
  void Mesh<array_render>::render()
  {

    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);

    if (hasTexture_)
    {

      glEnableVertexAttribArray(1);

    }

    glEnableVertexAttribArray(2);

    shader_->bind();

    shader_->setUniform(std::string("transform"), transform_.getMatrix());
    shader_->updateUniforms();

    texture_.bind();

    glDrawArrays(GL_TRIANGLES, 0, drawVertices_);

    glDisableVertexAttribArray(0);

    if (hasTexture_)
    {
      glDisableVertexAttribArray(1);
    }

    glDisableVertexAttribArray(2);

  }

  template<>
  void Mesh<array_render>::renderNonIndexed(const Mat4 &perspective, const Mat4 &cameraTrans, const Mat4 &cameraCoord, const Vec3 &cameraPos)
  {

    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);

    if (hasTexture_)
    {

      glEnableVertexAttribArray(1);

    }

    glEnableVertexAttribArray(2);

    shader_->bind();

    shader_->setUniform(std::string("transform"), transform_.getMatrix());
    shader_->setUniform(std::string("perspective"), perspective);
    shader_->setUniform(std::string("camera"), cameraTrans);
    shader_->setUniform(std::string("cameraRotation"), cameraCoord);

    texture_.bind();

    glDrawArrays(GL_TRIANGLES, 0, drawVertices_);

    glDisableVertexAttribArray(0);

    if (hasTexture_)
    {

      glDisableVertexAttribArray(1);

    }

    glDisableVertexAttribArray(2);

  }

  template<>
  void Mesh<array_render>::renderNonIndexed(const Mat4 &perspective, const Mat4 &cameraTrans, const Mat4 &cameraCoord, const Vec3 &cameraPos, int a0, int a1, int a2)
  {

    glBindVertexArray(vao);
    glEnableVertexAttribArray(a0);

    if (hasTexture_)
    {

      glEnableVertexAttribArray(a1);

    }

    glEnableVertexAttribArray(a2);

    shader_->bind();

    shader_->setUniform(std::string("transform"), transform_.getMatrix());
    shader_->setUniform(std::string("perspective"), perspective);
    shader_->setUniform(std::string("camera"), cameraTrans);
    shader_->setUniform(std::string("cameraRotation"), cameraCoord);

    texture_.bind();

    glDrawArrays(GL_TRIANGLES, 0, drawVertices_);

    glDisableVertexAttribArray(a0);

    if (hasTexture_)
    {

      glDisableVertexAttribArray(a1);

    }

    glDisableVertexAttribArray(a2);

  }

  template<>
  void Mesh<array_render>::rotate(const Vec3 &v)
  {
    transform_.setRotationEuler(v);
  }

}

#endif /* end of include guard: MESH_HPP_5LMS1ZES */
