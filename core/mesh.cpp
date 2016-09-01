#include <mesh.hpp>

namespace i3d {

  void Mesh::rotate(const Vec3 &v)
  {
    transform_.setRotationEuler(v);
  }

  void Mesh::loadMesh(std::string fileName)
  {
    ObjLoader loader;
    loader.readModelFromFile(&model_, fileName.c_str());
    model_.prepareNonIndexedRendering();
  }

  void Mesh::buildSmoothNormals()
  {
    model_.buildSmoothNormals();
  }

  void Mesh::buildFakeVertexNormals()
  {
    model_.buildFakeVertexNormals();
  }

  void Mesh::loadTexture(std::string fileName)
  {
    //texture_.createTextureFromImage(fileName);
    hasTexture_ = true;
  }

  void Mesh::setMaterial(Material *m)
  {
    material_ = m;
    hasTexture_ = true;
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
    drawVertices_ = 3 * model_.meshes_.front().faces_.size();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(4, &buffers[0]);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, model_.meshes_[0].vertices_.size() * 3 * sizeof(float),
      model_.meshes_.front().vertices_.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    if (hasTexture_)
    {
      glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
      glBufferData(GL_ARRAY_BUFFER, model_.meshes_.front().texCoords_.size() * 2 * sizeof(float),
        model_.meshes_.front().texCoords_.data(), GL_STATIC_DRAW);

      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, model_.meshes_[0].vertexNormals_.size() * 3 * sizeof(float),
      model_.meshes_.front().vertexNormals_.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    for (unsigned i=0; i < model_.meshes_.front().vertices_.size(); ++i)
    {

      model_.meshes_.front().normalLines_.push_back(model_.meshes_.front().vertices_[i]);

      model_.meshes_.front().normalLines_.push_back(model_.meshes_.front().vertices_[i] +
                                                    0.125f * model_.meshes_.front().vertexNormals_[i]);

    }

    /////////////////////////////////////////////////////////////////////////////////////////
    glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
    glBufferData(GL_ARRAY_BUFFER, model_.meshes_[0].normalLines_.size() * 3 * sizeof(float),
      model_.meshes_.front().normalLines_.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  }

  void Mesh::initNonIndexedRender()
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

  void Mesh::initNonIndexedRender(int a0, int a1, int a2)
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

  void Mesh::render(const Mat4 &perspective, const Mat4 &cameraTrans, const Mat4 &cameraCoord, const Vec3 &cameraPos)
  {

    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);

    if (hasTexture_)
    {

      glEnableVertexAttribArray(1);

    }

    glEnableVertexAttribArray(2);

    shader_->setUniform(std::string("transform"), transform_.getMatrix());
    shader_->setUniform(std::string("perspective"), perspective);
    shader_->setUniform(std::string("camera"), cameraTrans);
    shader_->setUniform(std::string("cameraRotation"), cameraCoord);

    glDrawArrays(GL_TRIANGLES, 0, drawVertices_);
    //glDrawArrays(primitiveMode_, 0, drawVertices_);

    glDisableVertexAttribArray(0);

    if (hasTexture_)
    {
      glDisableVertexAttribArray(1);
    }

    glDisableVertexAttribArray(2);

  }

  void Mesh::shadowRender()
  {

    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);

    shader_->bind();

    shader_->setUniform(std::string("transform"), transform_.getMatrix());
    shader_->updateUniforms();

    glDrawArrays(GL_TRIANGLES, 0, drawVertices_);

    glDisableVertexAttribArray(0);

  }

  void Mesh::renderNonIndexed(const Mat4 &perspective, const Mat4 &cameraTrans, const Mat4 &cameraCoord, const Vec3 &cameraPos)
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

    glDrawArrays(GL_TRIANGLES, 0, drawVertices_);

    glDisableVertexAttribArray(0);

    if (hasTexture_)
    {

      glDisableVertexAttribArray(1);

    }

    glDisableVertexAttribArray(2);

  }

  void Mesh::renderNonIndexed(const Mat4 &perspective, const Mat4 &cameraTrans, const Mat4 &cameraCoord, const Vec3 &cameraPos, int a0, int a1, int a2)
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

    glDrawArrays(GL_TRIANGLES, 0, drawVertices_);

    glDisableVertexAttribArray(a0);

    if (hasTexture_)
    {

      glDisableVertexAttribArray(a1);

    }

    glDisableVertexAttribArray(a2);

  }

  void Mesh::render()
  {

    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);

    if(hasTexture_)
    {
      glEnableVertexAttribArray(1);
    }

    glEnableVertexAttribArray(2);

    glDrawArrays(GL_TRIANGLES, 0, drawVertices_);

    glDisableVertexAttribArray(0);

    if (hasTexture_)
    {
      glDisableVertexAttribArray(1);
    }

    glDisableVertexAttribArray(2);

  }

  void Mesh::renderNormals()
  {

    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);

    if (hasTexture_)
    {

      glEnableVertexAttribArray(1);
    }

    glEnableVertexAttribArray(2);

    glEnableVertexAttribArray(3);

    glDrawArrays(GL_LINES, 0, model_.meshes_[0].normalLines_.size());

    glDisableVertexAttribArray(0);

    if (hasTexture_)
    {
      glDisableVertexAttribArray(1);
    }

    glDisableVertexAttribArray(2);

    glDisableVertexAttribArray(3);

  }

}
