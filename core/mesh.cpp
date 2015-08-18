#include <mesh.hpp>
#include <objloader.h>


namespace i3d {

  void Mesh::loadMesh(std::string fileName)
  {

    ObjLoader loader;
    loader.readModelFromFile(&model_, fileName.c_str());
    model_.prepareIndexing();

  }

  void Mesh::loadTexture(std::string fileName)
  {
    texture_.createTextureFromImage(fileName);
  }

  void Mesh::setFragmentShader(std::string fileName)
  {
    shader_->addFragmentShader(fileName);
  } 

  void Mesh::setVertexShader(std::string fileName)
  {
    shader_->addVertexShader(fileName);
  } 

  void Mesh::initRender()
  {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(3, &buffers[0]);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, model_.meshes_[0].vertices_.size() * 3 * sizeof(float),
        model_.meshes_.front().vertices_.data() , GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    if(model_.meshes_.front().getIsTextured())
    {

      glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
      glBufferData(GL_ARRAY_BUFFER, model_.meshes_.front().orderedTexCoords_.size() * 2 * sizeof(float),
          model_.meshes_.front().orderedTexCoords_.data() , GL_STATIC_DRAW);

      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
      glEnableVertexAttribArray(1);
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, model_.meshes_[0].vertexNormals_.size() * 3 * sizeof(float),
        model_.meshes_.front().vertexNormals_.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    drawVertices_ = 3 * model_.meshes_.front().faces_.size(); 

    glGenBuffers(1, &iao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iao);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model_.meshes_.front().faces_.size() * 3 * sizeof(int),
        model_.meshes_.front().indices_, GL_STATIC_DRAW); 

  }

  void Mesh::render(const Mat4 &perspective, const Mat4 &cameraTrans, const Mat4 &cameraCoord, const Vec3 &cameraPos)
  {
    shader_->bind();

    shader_->setUniform(std::string("transform"), transform_.getMatrix());
    shader_->setUniform(std::string("perspective"), perspective);
    shader_->setUniform(std::string("camera"), cameraTrans);
    shader_->setUniform(std::string("cameraRotation"), cameraCoord);

    texture_.bind();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iao);
    glDrawElements(GL_TRIANGLES, drawVertices_, GL_UNSIGNED_INT, 0);

  }

  void Mesh::rotate(const Vec3 &v)
  {
    transform_.setRotationEuler(v);
  }
      
}
