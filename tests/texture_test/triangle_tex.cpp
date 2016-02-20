#include <sdl_gl_application.hpp>
#include<GL/gl.h>
#include <basicshader.hpp>
#include <shaderloader.hpp>
#include <stdlib.h>
#include <objloader.h>
#include <3dmodel.h>
#include <3dmesh.h>
#include <texture.hpp>


//#include <basicshader.hpp>

class Triangle : public SDL_GL_application
{
public:
  Triangle ()
  {
    setTitle(std::string("Single Point Render"));
    frame=0;
  };

  virtual ~Triangle (){};

  void init()
  {
    SDL_GL_application::init();

    shader.initShader();
    shader.addVertexShader("../../shaders/triangle_tex.vert");
    shader.addFragmentShader("../../shaders/triangle_tex.frag");
    shader.linkShader();

    glEnable(GL_TEXTURE_2D);

    earth.createTextureFromImage(std::string("../../textures/australia.png"));
    //earth.makeCheckImage();

    i3d::ObjLoader loader;
    i3d::Model3D model;
    loader.readModelFromFile(&model, "earth_tri.obj");

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

    static const GLfloat vertex_positions[] = {
      0.25f, -0.25f, 0.5f,
     -0.25f, -0.25f, 0.5f,
      0.25f,  0.25f, 0.5f
    };

    static const GLfloat colors[] = {               
                               1.0, 0.0, 0.0, 1.0,                
                               0.0, 1.0, 0.0, 1.0,                
                               0.0, 0.0, 1.0, 1.0
                               };                

    glGenBuffers(3, &buffers[0]);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, model.meshes_[0].vertices_.size() * 3 * sizeof(float),
                                  model.meshes_.front().vertices_.data() , GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, model.meshes_[0].texCoords_.size() * 2 * sizeof(float),
                                  model.meshes_[0].texCoords_.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL); 
    glEnableVertexAttribArray(2);

  }

  void render()
  {
    static const GLfloat red[]={0.0f, 0.25f, 0.0f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, red);

    float x = (float)rand()/(float)(RAND_MAX);

    const GLfloat col[]={x, 0.f, 0.0f, 1.0f};


    shader.bind();
    earth.bind();

    glDrawArrays(GL_TRIANGLES, 0, 3);

    SDL_GL_SwapWindow(window);

    frame++;

  }

private:
  /* data */
  GLuint program;
  GLuint vao;
  GLuint buffers[3];
  BasicShader shader;
  Texture earth;
  int frame;
};

int main(int argc, char *argv[])
{

  Triangle app;

  app.init();

  app.run();

  return 0;
}

