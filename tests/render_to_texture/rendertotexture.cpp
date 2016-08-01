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

namespace i3d {

  class RenderToTexture : public SDL_GL_application
  {
    public:
      RenderToTexture()
      {
        setTitle(std::string("Render to texture test"));
        frame=0;
        time_=0;
        speed_ = 0.05f; 
      };

      virtual ~RenderToTexture(){};

      void init()
      {
        SDL_GL_application::init();
        
        perspective_.setPerspectiveTransform(50.f, static_cast<float>(getWindowWidth()), static_cast<float>(getWindowHeight()), 1.f, 60.f);
        camera_.initCamera(Vec3(0.f, 0.f, -6.0f), Vec3(1.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f));

        dLight_.ambientIntensity_ = 0.0f;
        dLight_.diffuseIntensity_ = 0.8f;
        dLight_.color_ = Vec3(1.0f, 1.0f, 1.0f);
        dLight_.dir_ = Vec3(0.0f, 0.0f, 1.0f);

        pLight_.color_    = Vec3(0.8f, 0.0f, 0.0f);
        pLight_.position_ = Vec3(1.0f,1.0f,1.0f);
        pLight_.ambientIntensity_ = 0.0f;
        pLight_.diffuseIntensity_ = 0.8f;
        pLight_.att_.constant_ = 0.2f;
        pLight_.att_.linear_ = 0.2f;
        pLight_.att_.exp_ = 0.2f;

        //pLight_.ambientIntensity_ = 0.2;
        //pLight_.diffuseIntensity_ = 0.8;
        //pLight_.att_.constant_ = 1.0;
        //pLight_.att_.linear_ = 0.0;
        //pLight_.att_.exp_ = 0.0;

        glEnable(GL_TEXTURE_2D);

        renderTex_.createRenderTexture(getWindowWidth(), getWindowHeight());

        glGenRenderbuffers(1, &renderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);

        // the renderbuffer needs also a depth component
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, getWindowWidth(), getWindowHeight());

        // get a framebuffer object
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex_.id_, 0);
        glEnable(GL_DEPTH_TEST); 

        //room_.loadMesh("../../meshes/room.obj");
        room_.loadMesh("../../meshes/quad.obj");
        room_.buildFakeVertexNormals();
        textures_.reserve(10);

        Texture r;
        r.createTextureFromImage("../../textures/wall_floor.png");
        textures_.push_back(std::move(r));
        Texture *t = &textures_[0];

        roomMat_ = PhongMaterial(0.0f, 200.0f, 1.0f, t);
        room_.setMaterial(&roomMat_);

        //renderManager.setupShaders(shaderList,shaderConfigurations,...);
        shader_.initShader(*camera_.getPosPointer(), perspective_.getPerspectiveTransform(), camera_.getCameraTranslationTransform(), camera_.getCameraCoordinateTransform(), roomMat_);
        shader_.setDirectionLight(&dLight_);
        shader_.setPointLight(&pLight_);
        room_.shader_ = &shader_;

        shaderPhong_.initShader(*camera_.getPosPointer(), perspective_.getPerspectiveTransform(), camera_.getCameraTranslationTransform(), camera_.getCameraCoordinateTransform(), roomMat_);
        shaderPhong_.setDirectionLight(&dLight_);
        shaderPhong_.setPointLight(&pLight_);
        room_.shader_ = &shaderPhong_;

        room_.transform_.translation_ = i3d::Vec4(0.f, 0.f, 0.f, 1.f);
        room_.transform_.setRotationEuler(i3d::Vec3(0.0f, 0.0f, 0.0f));

        room_.initRender();

        world_.loadMesh("../../meshes/world.obj");
        world_.buildSmoothNormals();

        Texture e;
        e.createTextureFromImage("../../textures/earth1.png");
        textures_.push_back(std::move(e));
        t = &textures_[1];

        worldMat_ = PhongMaterial(15.0f, 30.0f, 1.0f, t);
        world_.setMaterial(&worldMat_);

        world_.transform_.translation_ = i3d::Vec4(0.9f, -1.0f, 0.0f, 1.0f);
        world_.transform_.setRotationEuler(i3d::Vec3(0.0f, 0.0f, 0.0f));

        world_.initRender();

      }

      void shadowPass()
      {

        glClearDepth(1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);

        shaderPhong_.bind();
        room_.material_->bindTexture();
        //shader.setConfiguration(renderManager_.getConfiguration(GameObject* obj,...))
        shaderPhong_.setTransform(room_.transform_.getMatrix());
        shaderPhong_.material_ = room_.material_;
        shaderPhong_.setViewTransform(camera_.getCameraTranslationTransform(), camera_.getCameraCoordinateTransform());
        shaderPhong_.updateUniforms();

        room_.render();

      }

      void render()
      {

        static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        static const GLfloat ones[] = { 1.0f };
        static const GLfloat green[] = { 0.0f, 0.8f, 0.0f, 1.0f };

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glClearBufferfv(GL_COLOR, 0, black);
        glClearBufferfv(GL_DEPTH, 0, ones);

        shadowPass();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClearBufferfv(GL_COLOR, 0, green);
        glClearBufferfv(GL_DEPTH, 0, ones);

        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        float x = (float)rand() / (float)(RAND_MAX);

        const GLfloat col[] = { x, 0.f, 0.0f, 1.0f };

        //glPolygonMode(GL_FRONT_AND_BACK, renderMode_);

        //RenderManager.render();

        shader_.bind();

        renderTex_.bind();
        //shader.setConfiguration(renderManager_.getConfiguration(GameObject* obj,...))
        shader_.setTransform(room_.transform_.getMatrix());
        shader_.material_ = room_.material_;
        shader_.setViewTransform(camera_.getCameraTranslationTransform(), camera_.getCameraCoordinateTransform());
        shader_.updateUniforms();

        room_.render();

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
        //world_.transform_.translation_.x = 0.9 + std::sin(frame*0.01)*0.5;
        //light_.pos_.x = std::sin(frame*0.01);
        //room_.rotate(i3d::Vec3(0.0,frame*0.005,0.0));
        frame++;

      }

      void handleResizeEvent(SDL_Event &event)
      {
        SDL_GL_application::handleResizeEvent(event);
        perspective_.setPerspectiveTransform(70.0f, (float)getWindowWidth(), (float)getWindowHeight(), 0.0f, 100.0f);
      }

      void handleKeyPressEvent(SDL_Event &event)
      {
        switch(event.key.keysym.sym) {

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
            camera_.moveN(speed_);
            break;
          case SDLK_PAGEDOWN:
            camera_.moveN(-speed_);
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
              if(renderMode_==GL_FILL)
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
      TextureShader shader_;
      PhongDir shaderPhong_;
      Texture renderTex_;
      std::vector<Texture> textures_;
      int size;
      int frame;
      Transform transform_;
      float time_;
      PerspectiveTransform perspective_;
      Camera camera_;
      Mesh<> room_;
      Mesh<> world_;
      float speed_;
      Light light_;
      DirectionalLight dLight_;
      PointLight pLight_;
      PhongMaterial worldMat_;
      PhongMaterial roomMat_;
  };
}
int main(int argc, char *argv[])
{

  i3d::RenderToTexture app;

  app.init();

  app.run();

  return 0;
}

