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

  class AmbientLight : public SDL_GL_application
  {
    public:
      AmbientLight ()
      {
        setTitle(std::string("Transformed Triangle Render"));
        frame=0;
        time_=0;
      };

      virtual ~AmbientLight (){};

      void init()
      {
        SDL_GL_application::init();

        perspective_.setPerspectiveTransform(70.0, getWindowWidth(), getWindowHeight(), 0.0, 100.0);
        camera_.initCamera(Vec3(0,0,-3.0), Vec3(1,0,0), Vec3(0,1,0), Vec3(0,0,1));

        glEnable(GL_TEXTURE_2D);

        quad_.loadMesh("../../meshes/earth_plane.obj"); 
        quad_.loadTexture("../../textures/test.png");

        shader_.initShader();
        shader_.addVertexShader("../../shaders/phong.vert");
        shader_.addFragmentShader("../../shaders/phong.frag");

        shader_.linkShader();
        shader_.addUniform(std::string("transform"));
        shader_.addUniform(std::string("perspective"));
        shader_.addUniform(std::string("camera"));
        shader_.addUniform(std::string("cameraRotation"));

        quad_.shader_ = &shader_;

        quad_.transform_.translation_ = i3d::Vec4(0,0,0,1);
        quad_.transform_.setRotationEuler(i3d::Vec3(0.0,0.0,0.0));
        
        quad_.initRender();

      }

      void render()
      {

        static const GLfloat black[]={0.0f, 0.0, 0.0f, 1.0f};
        glClearBufferfv(GL_COLOR, 0, black);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        float x = (float)rand()/(float)(RAND_MAX);

        const GLfloat col[]={x, 0.f, 0.0f, 1.0f};

        quad_.render(perspective_.getPerspectiveTransform(), 
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

        seconds  = end.tv_sec  - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;

        mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
#endif
        //quad_.rotate(i3d::Vec3(0.0,0.0,std::sin(frame*0.001)*3.14));
        frame++;

      }

      void handleResizeEvent(SDL_Event &event)
      {
        SDL_GL_application::handleResizeEvent(event);
        perspective_.setPerspectiveTransform(70.0, getWindowWidth(), getWindowHeight(), 0.0, 100.0);
      }

      void handleKeyPressEvent(SDL_Event &event)
      {
        switch(event.key.keysym.sym) {

          case SDLK_RIGHT:
            camera_.getPos() += 0.01f * camera_.getU();
            break;
          case SDLK_LEFT:
            camera_.getPos() -= 0.01f * camera_.getU();
            break;
          case SDLK_UP:
            camera_.getPos() += 0.01f * camera_.getN();
            break;
          case SDLK_DOWN:
            camera_.getPos() -= 0.01f * camera_.getN();
            break;
          case SDLK_a:
            camera_.rotateY(-0.01f);
            break;
          case SDLK_d:
            camera_.rotateY(0.01f);
            break;
          case SDLK_w:
            camera_.rotateX(-0.01f);
            std::cout << camera_.getN() << std::endl;
            break;
          case SDLK_s:
            camera_.rotateX(0.01f);
            std::cout << camera_.getN() << std::endl;
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
      Texture earth;
      int size;
      int frame;
      Transform transform_;
      float time_;
      PerspectiveTransform perspective_;
      Camera camera_;
      Mesh quad_;
  };
}
int main(int argc, char *argv[])
{

  i3d::AmbientLight app;

  app.init();

  app.run();

  return 0;
}

