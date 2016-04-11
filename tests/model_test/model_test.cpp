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
#include <light.hpp>
#include <phong_dir.hpp>
#include <textureshader.hpp>
#include <simpleshader.hpp>
#include <twopassshadow.hpp>
#include <normallines.hpp>

namespace i3d {

  class ModelTest : public SDL_GL_application
  {
  public:
    ModelTest()
    {
      setTitle(std::string("Normals Test"));
      frame = 0;
      time_ = 0;
      speed_ = 0.05f;
    };

    virtual ~ModelTest() {};

    void init()
    {
      SDL_GL_application::init();

      perspective_.setPerspectiveTransform(50.f, static_cast<float>(getWindowWidth()), static_cast<float>(getWindowHeight()), 1.f, 60.f);
      camera_.initCamera(Vec3(0.f, 0.0f, -6.0f), Vec3(1.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f));

      glEnable(GL_DEPTH_TEST);

      textures_.reserve(10);

      shader_.initShader(camera_.getPos(), perspective_.getPerspectiveTransform(), camera_.getCameraTranslationTransform(), camera_.getCameraCoordinateTransform());

      normalShader_.initShader(camera_.getPos(), perspective_.getPerspectiveTransform(), camera_.getCameraTranslationTransform(), camera_.getCameraCoordinateTransform());

      ////////////////////////////////////////////////////////////////////////////////////////////////////////

      quad_.loadMesh("../../meshes/quad.obj");
      quad_.buildSmoothNormals();

      quad_.transform_.translation_ = i3d::Vec4(0.f, 1.8f, 0.f, 1.f);
      quad_.transform_.setRotationEuler(i3d::Vec3(0.0f, 0.0f, 0.0f));

      quad_.initRender();

      world_.loadMesh("../../meshes/sphere0.obj");
      world_.buildSmoothNormals();

      world_.transform_.translation_ = i3d::Vec4(0.0, 0.0, 0.0, 1);
      world_.transform_.setRotationEuler(i3d::Vec3(0.0, 0.0, 0.0));

      world_.initRender();

    }

    void render()
    {

      static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
      static const GLfloat ones[] = { 1.0f };
      static const GLfloat green[] = { 0.0f, 0.8f, 0.0f, 1.0f };

      glClearBufferfv(GL_COLOR, 0, black);
      glClearBufferfv(GL_DEPTH, 0, ones);

      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      glClearBufferfv(GL_COLOR, 0, green);
      glClearBufferfv(GL_DEPTH, 0, ones);

      glEnable(GL_CULL_FACE);
      glFrontFace(GL_CCW);

      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);

      float x = (float)rand() / (float)(RAND_MAX);

      const GLfloat col[] = { x, 0.f, 0.0f, 1.0f };

      shader_.bind();

      world_.transform_.scale_.x = 1.0;
      world_.transform_.scale_.y = 1.0;
      world_.transform_.scale_.z = 1.0;
      shader_.setTransform(world_.transform_.getMatrix());
      shader_.setMatrices(perspective_.getPerspectiveTransform(),
        camera_.getCameraTranslationTransform(),
        camera_.getCameraCoordinateTransform()
      );

      shader_.eyePos_ = Vec3(0.8f, 0.0f, 0.0f);
      shader_.updateUniforms();
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      world_.render();


      world_.transform_.scale_.x = 1.01;
      world_.transform_.scale_.y = 1.01;
      world_.transform_.scale_.z = 1.01;
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      shader_.setTransform(world_.transform_.getMatrix());
      shader_.eyePos_ = Vec3(0.0f, 0.0f, 0.8f);
      shader_.updateUniforms();
      world_.render();

      ////////////////////////////////////////////////////////////////////////////////////////////////////////

      normalShader_.bind();
      normalShader_.setTransform(world_.transform_.getMatrix());
      normalShader_.setMatrices(perspective_.getPerspectiveTransform(),
        camera_.getCameraTranslationTransform(),
        camera_.getCameraCoordinateTransform()
        );

      normalShader_.eyePos_ = Vec3(0.0f, 1.0f, 1.0f);
      normalShader_.updateUniforms();
      world_.renderNormals();


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

      frame++;

    }

    void handleResizeEvent(SDL_Event &event)
    {
      SDL_GL_application::handleResizeEvent(event);
      perspective_.setPerspectiveTransform(70.0f, (float)getWindowWidth(), (float)getWindowHeight(), 0.0f, 100.0f);
    }

    void handleKeyPressEvent(SDL_Event &event)
    {
      switch (event.key.keysym.sym) {

      case SDLK_RIGHT:
        camera_.moveU(speed_);
        break;
      case SDLK_LEFT:
        camera_.moveU(-speed_);
        break;
      case SDLK_UP:
        camera_.moveV(speed_);
        break;
      case SDLK_DOWN:
        camera_.moveV(-speed_);
        break;
      case SDLK_PAGEUP:
        camera_.moveW(speed_);
        break;
      case SDLK_PAGEDOWN:
        camera_.moveW(-speed_);
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

    GLuint fbo;
    GLuint renderBuffer;

    GLuint tiao;
    GLuint buffers[3];
    SimpleShader shader_;
    NormalShader normalShader_;

    //TextureShader shader_;
    PhongDir shaderPhong_;
    Texture renderTex_;
    Texture *floor_;
    
    std::vector<Texture> textures_;
    int size;
    int frame;
    Transform transform_;
    float time_;
    PerspectiveTransform perspective_;
    Camera camera_;
    Camera cameraShadow_;
    Mesh<> room_;
    Mesh<> monkey_;
    Mesh<> quad_;
    Mesh<> world_;
    float speed_;
    Light light_;

  };
}
int main(int argc, char *argv[])
{

  i3d::ModelTest app;

  app.init();

  app.run();

  return 0;
}

