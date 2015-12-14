#include <sdl_gl_application.hpp>
#include <GL/gl.h>
#include <basicshader.hpp>
#include <shaderloader.hpp>
#include <stdlib.h>
#include <objloader.h>
#include <3dmodel.h>
#include <3dmesh.h>
#include <texture.hpp>
#include <transform.hpp>
#ifndef WIN32
#include <sys/time.h>
#endif
#include <perspectivetransform.hpp>
#include <cmath>
#include <camera.hpp>
#include <mesh.hpp>

namespace i3d {

  class LightTest : public SDL_GL_application
  {
  public:
    LightTest()
    {
      setTitle(std::string("Light Test"));
      frame = 0;
      time_ = 0;
      speed_ = 0.05f;
    };

    virtual ~LightTest(){};

    void init()
    {
      SDL_GL_application::init();

      perspective_.setPerspectiveTransform(50.0, getWindowWidth(), getWindowHeight(), 0.0, 100.0);
      camera_.initCamera(Vec3(0, 1.8, -6.0), Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1));

      glEnable(GL_TEXTURE_2D);

      room_.loadMesh("../../meshes/room.obj");

      // Fake vertex normals are face normals copied to the vertices
      room_.buildFakeVertexNormals();
      room_.loadTexture("../../textures/wall_floor.png");

      shader_.initShader();
      shader_.addVertexShader("../../shaders/phong_world.vert");
      shader_.addFragmentShader("../../shaders/phong_world.frag");

      shader_.linkShader();
      shader_.addUniform(std::string("transform"));
      shader_.addUniform(std::string("perspective"));
      shader_.addUniform(std::string("camera"));
      shader_.addUniform(std::string("cameraRotation"));

      room_.shader_ = &shader_;

      room_.transform_.translation_ = i3d::Vec4(0, 0, 0, 1);
      room_.transform_.setRotationEuler(i3d::Vec3(0.0, 0.0, 0.0));

      room_.initRender();

    }

    void render()
    {

      static const GLfloat black[] = { 0.0f, 0.0, 0.0f, 1.0f };
      static const GLfloat ones[] = { 1.0f };
      glClearBufferfv(GL_COLOR, 0, black);
      glClearBufferfv(GL_DEPTH, 0, ones);

      glEnable(GL_CULL_FACE);
      glFrontFace(GL_CCW);

      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);

      float x = (float)rand() / (float)(RAND_MAX);

      const GLfloat col[] = { x, 0.f, 0.0f, 1.0f };

      glPolygonMode(GL_FRONT_AND_BACK, renderMode_);

      room_.render(perspective_.getPerspectiveTransform(),
        camera_.getCameraTranslationTransform(),
        camera_.getCameraCoordinateTransform(),
        camera_.getPos());

#ifndef _MSC_VER
      struct timeval start, end;

      long mtime, seconds, useconds;

      gettimeofday(&start, NULL);
#endif

      SDL_GL_SwapWindow(window);

#ifndef _MSC_VER
      gettimeofday(&end, NULL);

      seconds = end.tv_sec - start.tv_sec;
      useconds = end.tv_usec - start.tv_usec;

      mtime = ((seconds)* 1000 + useconds / 1000.0) + 0.5;
#endif
      //room_.rotate(i3d::Vec3(0.0,std::sin(frame*0.001)*3.14,0.0));
      //room_.rotate(i3d::Vec3(0.0,frame*0.005,0.0));
      frame++;

    }

    void handleResizeEvent(SDL_Event &event)
    {
      SDL_GL_application::handleResizeEvent(event);
      perspective_.setPerspectiveTransform(70.0, getWindowWidth(), getWindowHeight(), 0.0, 100.0);
    }

    void handleKeyPressEvent(SDL_Event &event)
    {
      switch (event.key.keysym.sym) {

      case SDLK_RIGHT:
        camera_.getPos() += speed_ * camera_.getU();
        break;
      case SDLK_LEFT:
        camera_.getPos() -= speed_ * camera_.getU();
        break;
      case SDLK_UP:
        camera_.getPos() += speed_ * camera_.getN();
        break;
      case SDLK_DOWN:
        camera_.getPos() -= speed_ * camera_.getN();
        break;
      case SDLK_a:
        camera_.rotateY(-speed_);
        break;
      case SDLK_d:
        camera_.rotateY(speed_);
        break;
      case SDLK_w:
        camera_.rotateX(-speed_);
        std::cout << camera_.getN() << std::endl;
        break;
      case SDLK_s:
        camera_.rotateX(speed_);
        std::cout << camera_.getN() << std::endl;
        break;
      case SDLK_0:
      {
        if (renderMode_ == GL_FILL)
        {
          renderMode_ = GL_LINE;
        }
        else
        {
          renderMode_ = GL_FILL;
        }
      }
        break;
      }
    }

  private:
    /* data */
    GLuint program;
    GLuint vao;
    GLuint iao;
    GLuint tiao;
    GLuint buffers[3];
    BasicShader shader_;
    BasicShader worldShader_;
    Texture earth;
    int size;
    int frame;
    Transform transform_;
    float time_;
    PerspectiveTransform perspective_;
    Camera camera_;
    Mesh<array_render> room_;
    float speed_;
  };
}
int main(int argc, char *argv[])
{

  i3d::LightTest app;

  app.init();

  app.run();

  return 0;
}