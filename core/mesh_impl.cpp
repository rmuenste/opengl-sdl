



  template<>
  void Mesh<array_render>::rotate(const Vec3 &v)
  {
    transform_.setRotationEuler(v);
  }

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
    //texture_.createTextureFromImage(fileName);
    hasTexture_ = true;
  }

  template<>
  void Mesh<array_render>::setMaterial(Material *m)
  {
    material_ = m;
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

    std::cout << "render: " << model_.meshes_[0].vertices_.size() << std::endl; 

    glGenBuffers(4, &buffers[0]);
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

  template<>
  void Mesh<array_render>::shadowRender()
  {

    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);

    shader_->bind();

    shader_->setUniform(std::string("transform"), transform_.getMatrix());
    shader_->updateUniforms();

    glDrawArrays(GL_TRIANGLES, 0, drawVertices_);

    glDisableVertexAttribArray(0);

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

    glDrawArrays(GL_TRIANGLES, 0, drawVertices_);

    glDisableVertexAttribArray(a0);

    if (hasTexture_)
    {

      glDisableVertexAttribArray(a1);

    }

    glDisableVertexAttribArray(a2);

  }

  template<>
  void Mesh<array_render>::render()
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

  template<>
  void Mesh<array_render>::renderNormals()
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
