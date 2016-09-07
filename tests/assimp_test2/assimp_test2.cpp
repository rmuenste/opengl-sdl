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
#include <fstream>
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
#include <shadowmapshader.hpp>
#include <simpleshader.hpp>
#include <twopassshadow.hpp>
#include <normallines.hpp>

#include <assimp/Importer.hpp>	//OO version Header!
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <resourcemanager.hpp>

namespace i3d {

  class MyResourceManager : public ResourceManager {

    public:

      virtual void loadAssets()
      {

        if(!ResourceManager::loadScene("../../meshes/test_scene.dae"))
        {
          std::cerr << "Loading Scene failed..." << std::endl;
          std::exit(EXIT_FAILURE);
        }

      }

  };

  class AssimpTest2 : public SDL_GL_application
  {
  public:
    AssimpTest2()
    {
      setTitle(std::string("Assimp Test"));
      speed_ = 0.05f;
      time_ = 0;
    };

    virtual ~AssimpTest2(){};

    void init()
    {
      SDL_GL_application::init();

      rm.loadAssets();

      perspective_.setPerspectiveTransform(70.f, static_cast<float>(getWindowWidth()), static_cast<float>(getWindowHeight()), 1.f, 60.f);
      camera_.initCamera(Vec3(0.f, 1.8f, -6.0f), Vec3(1.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f));

      rm.phongDirShaders_.push_back(SimpleShader());
      rm.phongDirShaders_[0].initShader(*camera_.getPosPointer(), perspective_.getPerspectiveTransform(), camera_.getCameraTranslationTransform(), camera_.getCameraCoordinateTransform());

      for(auto &go : rm.gameObjects_)
      {
        go.shader_ = &rm.phongDirShaders_[0];
      }

      glEnable(GL_DEPTH_TEST);

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

      rm.gameObjects_[0].shader_->bind();
      for(auto &go : rm.gameObjects_)
      {
        go.material_->bindTexture();
        go.shader_->setTransform(go.meshObject_->transform_.getMatrix());
        go.shader_->setMatrices(perspective_.getPerspectiveTransform(),
          camera_.getCameraTranslationTransform(),
          camera_.getCameraCoordinateTransform()
        );

        go.shader_->eyePos_ = Vec3(0.8f, 0.0f, 0.0f);
        go.shader_->updateUniforms();
        go.render();
      }



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
      //case SDLK_UP:
      //  camera_.moveV(speed_);
      //  break;
      //case SDLK_DOWN:
      //  camera_.moveV(-speed_);
      //  break;
      case SDLK_PAGEUP:
        camera_.moveN(speed_);
        break;
      case SDLK_PAGEDOWN:
        camera_.moveN(-speed_);
        break;
      case SDLK_a:
        camera_.rotateY(-speed_);
        camera_.outputCameraConfig();
        break;
      case SDLK_d:
        camera_.rotateY(speed_);
        camera_.outputCameraConfig();
        break;
      case SDLK_w:
        //camera_.rotateX(-speed_);
        camera_.moveN(speed_);
        camera_.outputCameraConfig();
        break;
      case SDLK_s:
        //camera_.rotateX(speed_);
        camera_.moveN(-speed_);
        camera_.outputCameraConfig();
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
    // Create an instance of the Importer class
    Assimp::Importer importer;
    const aiScene* scene = nullptr;

    /* data */
    GLuint program;
    GLuint vao;
    GLuint iao;

    GLuint fbo;
    GLuint renderBuffer;

    GLuint tiao;
    GLuint buffers[3];
    PerspectiveTransform perspective_;
    Camera camera_;
    int size;
    int frame;
    Transform transform_;
    float time_;
    float speed_;
    MyResourceManager rm;

  };

}

int main(int argc, char *argv[])
{

  i3d::AssimpTest2 app;

  app.init();

  app.run();

  return 0;
}

