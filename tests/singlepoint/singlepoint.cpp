#include <sdl_gl_application.hpp>

#include<GL/gl.h>

#include <basicshader.hpp>

#include <shaderloader.hpp>

//#include <basicshader.hpp>

class Singlepoint : public SDL_GL_application
{
public:
  Singlepoint ()
  {
    setTitle(std::string("Single Point Render"));
  };

  virtual ~Singlepoint (){};

  void init()
  {
    SDL_GL_application::init();
    

    static const char* vs_source[] = 
    {
      "#version 450 core \n"
      " \n"
      "void main(void) \n"
      "{\n"
      "  const vec4 vertices[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),vec4(-0.25, -0.25, 0.5, 1.0),vec4(0.25, 0.25, 0.5, 1.0));\n"
      "  gl_Position = vertices[gl_VertexID];\n"
      "}\n"
    };

    static const char* fs_source[] = 
    {
      "#version 450 core \n"
      "out vec4 color; \n"
      "void main(void) \n"
      "{\n"
      "  color = vec4(0.0,0.8,1.0,1.0);\n"
      "}\n"
    };

    shader.initShader();
    shader.addVertexShader("triangle.vs");
    shader.addFragmentShader("triangle.fs");
    shader.linkShader();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

  }

  void render()
  {
    static const GLfloat red[]={0.0f, 0.2f, 0.0f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, red),

    shader.bind();

    glPointSize(40.0f);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    SDL_GL_SwapWindow(window);

  }

private:
  /* data */
  GLuint program;
  GLuint vao;
  BasicShader shader;
};

int main(int argc, char *argv[])
{

  Singlepoint app;

  app.init();

  app.run();

  return 0;
}

