#include <sdl_gl_application.hpp>
#include<GL/gl.h>
#include <basicshader.hpp>
#include <shaderloader.hpp>
#include <stdlib.h>
#include <objloader.h>
#include <3dmodel.h>
#include <3dmesh.h>
#include <texture.hpp>
#include <transform.hpp>

class TransformedTriangle : public SDL_GL_application
{
public:
  TransformedTriangle ()
  {
    setTitle(std::string("Transformed Triangle Render"));
    frame=0;
  };

  virtual ~TransformedTriangle (){};

  void init()
  {
    SDL_GL_application::init();

    shader.initShader();
    shader.addVertexShader("../../shaders/transform.vert");
    shader.addFragmentShader("../../shaders/transform.frag");
    shader.linkShader();
    shader.addUniform(std::string("transform"));
    transform_.translation_ = i3d::Vec4(0.0f, 0.0f, 0.0f, 1.0f);
    transform_.setRotationEuler(i3d::Vec3(0.0,0.0,3.13));

    glEnable(GL_TEXTURE_2D);

    earth.createTextureFromImage(std::string("../../textures/australia.png"));
    //earth.makeCheckImage();

    i3d::ObjLoader loader;
    i3d::Model3D model;
    loader.readModelFromFile(&model, "../../meshes/earth_tri2.obj");
    model.BuildVertexArrays();

    for(auto v : model.meshes_.front().vertices_)
    {
      std::cout << v << std::endl;
    }

    for(auto t : model.meshes_.front().texCoords_)
    {
      std::cout << t << std::endl;
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(3, &buffers[0]);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, model.meshes_[0].vertices_.size() * 3 * sizeof(float),
                                  model.meshes_.front().vertices_.data() , GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, model.meshes_.front().orderedTexCoords_.size() * 2 * sizeof(float),
                                  model.meshes_.front().orderedTexCoords_.data() , GL_STATIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    size = 3 * model.meshes_.front().faces_.size(); 

    glGenBuffers(1, &iao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iao);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.meshes_.front().faces_.size() * 3 * sizeof(int),
                                          model.meshes_.front().indices_, GL_STATIC_DRAW); 

  }

  void render()
  {

    static const GLfloat red[]={0.0f, 0.25f, 0.0f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, red);

    float x = (float)rand()/(float)(RAND_MAX);

    const GLfloat col[]={x, 0.f, 0.0f, 1.0f};

    shader.bind();
    shader.setUniform(std::string("transform"), transform_.getMatrix());

    earth.bind();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iao);
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

    SDL_GL_SwapWindow(window);

    frame++;

  }

private:
  /* data */
  GLuint program;
  GLuint vao;
  GLuint iao;
  GLuint tiao;
  GLuint buffers[3];
  BasicShader shader;
  Texture earth;
  int size;
  int frame;
  Transform transform_;
};

int main(int argc, char *argv[])
{

  TransformedTriangle app;

  app.init();

  app.run();

  return 0;
}

