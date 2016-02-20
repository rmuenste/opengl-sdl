#include <sdl_gl_application.hpp>

#include<GL/gl.h>

#include <basicshader.hpp>

#include <shaderloader.hpp>

#include <stdlib.h>

//#include <basicshader.hpp>

class Singlepoint : public SDL_GL_application
{
public:
  Singlepoint ()
  {
    setTitle(std::string("Single Point Render"));
    frame=0;
  };

  virtual ~Singlepoint (){};

  void init()
  {
    SDL_GL_application::init();

    shader.initShader();
    shader.addVertexShader("../../shaders/triangle.vs");
    shader.addFragmentShader("../../shaders/triangle.fs");
    shader.linkShader();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

  }

  void render()
  {
    static const GLfloat red[]={0.0f, 0.25f, 0.0f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, red);

    float x = (float)rand()/(float)(RAND_MAX);

    const GLfloat col[]={1.0f, 0.f, 0.0f, 1.0f};

    shader.bind();

    glVertexAttrib4fv(0, col);
    glPointSize(40.0f);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    SDL_GL_SwapWindow(window);

    frame++;

  }

private:
  /* data */
  GLuint program;
  GLuint vao;
  BasicShader shader;
  int frame;
};

int main(int argc, char *argv[])
{

  Singlepoint app;

  app.init();

  app.run();

  return 0;
}

