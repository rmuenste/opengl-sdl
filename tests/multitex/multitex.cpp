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
#include <shadowmapshader.hpp>
#include <twopassshadow.hpp>

namespace i3d {

  class MultiTextureTest : public SDL_GL_application
  {
  public:
    MultiTextureTest()
    {
      setTitle(std::string("Multi Texture Test"));
      frame = 0;
      time_ = 0;
      speed_ = 0.05f;
    };

    virtual ~MultiTextureTest() {};

    void init()
    {
      SDL_GL_application::init();

      perspective_.setPerspectiveTransform(50.f, static_cast<float>(getWindowWidth()), static_cast<float>(getWindowHeight()), 1.f, 60.f);
      camera_.initCamera(Vec3(0.f, 1.8f, -6.0f), Vec3(1.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f));

      dLight_.ambientIntensity_ = 0.0f;
      dLight_.diffuseIntensity_ = 0.0f;
      dLight_.color_ = Vec3(0.0f, 0.0f, 0.0f);
      dLight_.dir_ = Vec3(0.25f, 0.0f, 1.0f);

      pLight_.color_ = Vec3(0.0f, 0.0f, 0.0f);
      pLight_.position_ = Vec3(2.5f, 1.0f, 2.0f);
      pLight_.ambientIntensity_ = 0.0f;
      pLight_.diffuseIntensity_ = 0.5f;
      pLight_.att_.constant_ = 0.02f;
      pLight_.att_.linear_ = 0.02f;
      pLight_.att_.exp_ = 0.03f;

      sLight_.color_ = Vec3(0.95f, 0.7f, 0.4f);
      sLight_.position_ = Vec3(0.0f, -0.5f, 2.0f);
      sLight_.dir_ = Vec3(-0.2f, 0.0f, 1.0f);

      sLight_.ambientIntensity_ = 0.0f;
      sLight_.diffuseIntensity_ = 1.0f;
      sLight_.cutoff_ = 0.8f;
      sLight_.att_.constant_ = 0.02f;
      sLight_.att_.linear_ = 0.02f;
      sLight_.att_.exp_ = 0.03f;

      cameraShadow_.initCamera(Vec3(0.f, 1.1f, -6.0f), Vec3(1.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f));

      renderTex_.createDepthTexture(getWindowWidth(), getWindowHeight());

      glEnable(GL_DEPTH_TEST);

      textures_.reserve(10);

      Texture r;
      r.createTextureFromImage("../../textures/wall_floor.png");
      textures_.push_back(std::move(r));

      roomMat_ = PhongMaterial(15.0f, 30.0f, 1.0f, &textures_.back());

      Texture e;
      e.createTextureFromImage("../../textures/earth1.png");
      textures_.push_back(std::move(e));

      worldMat_ = PhongMaterial(15.0f, 30.0f, 1.0f, &textures_.back());

      //renderManager.setupShaders(shaderList,shaderConfigurations,...);
      shader_.initShader(camera_.getPos(), perspective_.getPerspectiveTransform(), camera_.getCameraTranslationTransform(), camera_.getCameraCoordinateTransform(), worldMat_);
      shader_.setDirectionLight(&dLight_);
      shader_.setPointLight(&pLight_);

      ////////////////////////////////////////////////////////////////////////////////////////////////////////

      quad_.loadMesh("../../meshes/quad.obj");
      quad_.buildSmoothNormals();

      quad_.setMaterial(&worldMat_);

      quad_.transform_.translation_ = i3d::Vec4(0.f, 1.8f, 0.f, 1.f);
      quad_.transform_.setRotationEuler(i3d::Vec3(0.0f, 0.0f, 0.0f));

      quad_.initRender();

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
      shader_.shadowTex_ = roomMat_.textures_.front();
      shader_.shadowTex_ = quad_.material_->textures_.front();

      //shader_.bindMaterial(quad_.material_);
      shader_.bindMaterial(&roomMat_);

      shader_.setTransform(quad_.transform_.getMatrix());
      shader_.setMatrices(perspective_.getPerspectiveTransform(),
        camera_.getCameraTranslationTransform(),
        camera_.getCameraCoordinateTransform()
        );
      shader_.updateUniforms();
      quad_.render();

      glActiveTexture(GL_TEXTURE0);

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
    TwoPassShadow shader_;

    //TextureShader shader_;
    PhongDir shaderPhong_;
    Texture renderTex_;
    Texture *floor_;
    
    ShadowMapShader shaderShadow_;
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
    DirectionalLight dLight_;
    PointLight pLight_;
    SpotLight sLight_;
    PhongMaterial worldMat_;
    PhongMaterial roomMat_;
    PhongMaterial monkeyMat_;

  };
}
int main(int argc, char *argv[])
{

  i3d::MultiTextureTest app;

  app.init();

  app.run();

  return 0;
}

