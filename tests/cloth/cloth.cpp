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

namespace i3d {

  struct MeshTraits {
    std::vector<Vec3> v;
    std::vector<Vec3> f;
  };

  struct ConstraintSpring {
    int p1, p2;
    float rest_length;
    float Ks, Kd;
    int type;
  };

  int numX = 17, numY = 17;

  const int STRUCTURAL_SPRING = 0;
  const int SHEAR_SPRING = 1;
  const int BEND_SPRING = 2;
  int spring_count = 0;

  const float DEFAULT_DAMPING = -0.0125f;
  float	KsStruct = 0.5f, KdStruct = -0.25f;
  float	KsShear = 0.5f, KdShear = -0.25f;
  float	KsBend = 0.85f, KdBend = -0.25f;
  Vec3 gravity(0.0f, -0.00981f, 0.0f);
  float mass = 0.5f;

  std::vector<ConstraintSpring> springs;
  MeshTraits traits_;

  void AddSpring(int a, int b, float ks, float kd, int type) {
    ConstraintSpring spring;
    spring.p1 = a;
    spring.p2 = b;
    spring.Ks = ks;
    spring.Kd = kd;
    spring.type = type;
    //glm::vec3 deltaP = X[a] - X[b];
    //spring.rest_length = sqrt(glm::dot(deltaP, deltaP));
    springs.push_back(spring);
  }

  class SkyBoxTest : public SDL_GL_application
  {
  public:
    SkyBoxTest()
    {
      setTitle(std::string("SkyBox Test"));
      frame = 0;
      time_ = 0;
      speed_ = 0.05f;
    };

    virtual ~SkyBoxTest(){};

    void ComputeForces() {
      size_t i = 0;

      for (i = 0; i<springs.size(); i++) {
        traits_.f[i] = Vec3(0,0,0);

        //add gravity force only for non-fixed points
        if (i != 0 && i != numX)
          traits_.f[i] += gravity;

        //add force due to damping of velocity
        traits_.f[i] += DEFAULT_DAMPING*traits_.v[i];
      }

      //add spring forces
      for (i = 0; i<springs.size(); i++) {
        Vec3 p1 = quad_.model_.meshes_.front().vertices_[springs[i].p1];
        Vec3 p2 = quad_.model_.meshes_.front().vertices_[springs[i].p2];
        Vec3 v1 = traits_.v[springs[i].p1];
        Vec3 v2 = traits_.v[springs[i].p2];
        Vec3 deltaP = p1 - p2;
        Vec3 deltaV = v1 - v2;
        float dist = deltaP.mag();

        float leftTerm = -springs[i].Ks * (dist - springs[i].rest_length);
        float rightTerm = springs[i].Kd * (deltaV * deltaP) * (1.0/ dist);
        deltaP.normalize();
        Vec3 springForce = (leftTerm + rightTerm)*deltaP;

        if (springs[i].p1 != 0 && springs[i].p1 != numX)
          traits_.f[springs[i].p1] += springForce;
        if (springs[i].p2 != 0 && springs[i].p2 != numX)
          traits_.f[springs[i].p2] -= springForce;
      }
    }


    void IntegrateEuler(float deltaTime) {
      float deltaTimeMass = deltaTime / mass;
      size_t i = 0;

      for (i = 0; i<quad_.model_.meshes_.front().vertices_.size(); i++) {
        Vec3 oldV = traits_.v[i];
        traits_.v[i] += (traits_.v[i] * deltaTimeMass);
        quad_.model_.meshes_.front().vertices_[i] += deltaTime*oldV;

        //if (X[i].y <0) {
        //  X[i].y = 0;
        //}
      }
    }

    void init()
    {
      SDL_GL_application::init();

      perspective_.setPerspectiveTransform(70.f, static_cast<float>(getWindowWidth()), static_cast<float>(getWindowHeight()), 1.f, 60.f);
      camera_.initCamera(Vec3(0.f, 1.8f, -6.0f), Vec3(1.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f));

      dLight_.ambientIntensity_ = 0.0f;
      dLight_.diffuseIntensity_ = 0.8f;
      dLight_.color_ = Vec3(0.0f, 0.0f, 0.0f);
      //dLight_.color_ = Vec3(0.0f, 0.0f, 0.0f);
      dLight_.dir_ = Vec3(0.25f, -0.5f, 1.0f);
      dLight_.dir_ = Vec3(1.0f, -0.75f, 1.0f);
      dLight_.dir_ = Vec3(-1.0f, 0.0f, 0.0f);

      pLight_.color_ = Vec3(0.8f, 0.8f, 0.8f);
      //pLight_.color_ = Vec3(0.0f, 0.0f, 0.0f);
      pLight_.position_ = Vec3(0.0f, 2.5f, 1.0f);
      pLight_.ambientIntensity_ = 3.0f;
      pLight_.diffuseIntensity_ = 5.0f;
      pLight_.att_.constant_ = 1.0f;
      pLight_.att_.linear_ = 1.0f;
      pLight_.att_.exp_ = 0.2f;

      sLight_.color_ = Vec3(0.95f, 0.7f, 0.4f);
      sLight_.color_ = Vec3(0.0f, 0.0f, 0.0f);
      sLight_.position_ = Vec3(0.0f, 1.0f, -2.0f);
      sLight_.dir_ = Vec3(0.f, 0.0f, 1.0f);

      sLight_.ambientIntensity_ = 0.0f;
      sLight_.diffuseIntensity_ = 0.5f;
      sLight_.cutoff_ = 0.925f;
      sLight_.att_.constant_ = 0.09f;
      sLight_.att_.linear_ = 0.09f;
      sLight_.att_.exp_ = 0.1f;

      cameraShadow_.setLookAt(Vec3(0.f, 1.0f, -6.0f), Vec3(0.f, 0.f, 1.f), Vec3(0.f, 1.f, 0.f));

      renderTex_.createDepthTexture(getWindowWidth(), getWindowHeight());

      glGenRenderbuffers(1, &renderBuffer);
      glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);

      // the renderbuffer needs also a depth component
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, getWindowWidth(), getWindowHeight());

      // get a framebuffer object
      glGenFramebuffers(1, &fbo);
      glBindFramebuffer(GL_FRAMEBUFFER, fbo);
      glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, renderTex_.id_, 0);
      glEnable(GL_DEPTH_TEST);

      room_.loadMesh("../../meshes/big_room.obj");
      room_.buildFakeVertexNormals();
      textures_.reserve(10);

      Texture r;
      r.createTextureFromImage("../../textures/wall_floor2.png");
      textures_.push_back(std::move(r));

      roomMat_ = PhongMaterial(0.0f, 100.0f, 1.0f, &textures_.back());
      room_.setMaterial(&roomMat_);

      shaderShadow_.initShader(*camera_.getPosPointer(), perspective_.getPerspectiveTransform(), camera_.getCameraTranslationTransform(), camera_.getCameraCoordinateTransform(), roomMat_);

      shaderShadow_.setDirectionLight(&dLight_);
      shaderShadow_.setPointLight(&pLight_);
      shaderShadow_.setSpotLight(&sLight_);

      //renderManager.setupShaders(shaderList,shaderConfigurations,...);
      shader_.initShader(*camera_.getPosPointer(), perspective_.getPerspectiveTransform(), camera_.getCameraTranslationTransform(), camera_.getCameraCoordinateTransform(), roomMat_);
      shader_.setDirectionLight(&dLight_);
      shader_.setPointLight(&pLight_);
      room_.shader_ = &shader_;

      shaderPhong_.initShader(*camera_.getPosPointer(), perspective_.getPerspectiveTransform(), camera_.getCameraTranslationTransform(), camera_.getCameraCoordinateTransform(), roomMat_);
      shaderPhong_.setDirectionLight(&dLight_);
      shaderPhong_.setPointLight(&pLight_);
      shaderPhong_.setSpotLight(&sLight_);
      room_.shader_ = &shaderPhong_;

      room_.transform_.translation_ = i3d::Vec4(0.f, 0.f, 0.f, 1.f);
      room_.transform_.setRotationEuler(i3d::Vec3(0.0f, 0.0f, 0.0f));

      room_.initRender();

      ////////////////////////////////////////////////////////////////////////////////////////////////////////

      quad_.loadMesh("../../meshes/cloth1.obj");
      quad_.buildSmoothNormals();

      Texture e;
      //e.createTextureFromImage("../../textures/earth1.png");
      e.createTextureFromImage("../../textures/ground.png");
      textures_.push_back(std::move(e));

      planeMat_ = PhongMaterial(15.0f, 30.0f, 1.0f, &textures_.back());
      quad_.setMaterial(&planeMat_);

      quad_.transform_.translation_ = i3d::Vec4(0.f, 0.0f, 0.f, 1.f);
      quad_.transform_.setRotationEuler(i3d::Vec3(0.0f, 0.0f, 0.0f));

      quad_.initRender();

      ////////////////////////////////////////////////////////////////////////////////////////////////////////

      wall_.loadMesh("../../meshes/wall.obj");
      //world_.loadMesh("../../meshes/sphere0.obj");
      wall_.buildSmoothNormals();
      //world_.buildFakeVertexNormals();

      Texture w;
      w.createTextureFromImage("../../textures/brick.png");
      textures_.push_back(std::move(w));

      worldMat_ = PhongMaterial(5.0f, 30.0f, 1.0f, &textures_.back());
      wall_.setMaterial(&worldMat_);

      wall_.transform_.translation_ = i3d::Vec4(0.0, 0.0, 0.0, 1);
      //world_.transform_.translation_ = i3d::Vec4(0.0, 1.0, 0.0, 1);
      wall_.transform_.setRotationEuler(i3d::Vec3(0.0, 0.0, 0.0));

      wall_.initRender();

      wall2_.loadMesh("../../meshes/wall_r.obj");
      //world_.loadMesh("../../meshes/sphere0.obj");
      wall2_.buildSmoothNormals();
      //world_.buildFakeVertexNormals();

      wall2_.setMaterial(&worldMat_);

      wall2_.transform_.translation_ = i3d::Vec4(0.0, 0.0, 0.0, 1);
      //world_.transform_.translation_ = i3d::Vec4(0.0, 1.0, 0.0, 1);
      wall2_.transform_.setRotationEuler(i3d::Vec3(0.0, 0.0, 0.0));

      wall2_.initRender();

      //setup springs
      // Horizontal
      int l1, l2;
      int u = 17;
      int v = 17;
      for (l1 = 0; l1 < v; ++l1)	// v
        for (l2 = 0; l2 < (u - 1); ++l2) {
          AddSpring((l1 * u) + l2, (l1 * u) + l2 + 1, KsStruct, KdStruct, STRUCTURAL_SPRING);
        }

      // Vertical
      for (l1 = 0; l1 < (u); l1++)
      for (l2 = 0; l2 < (v - 1); l2++) {
        AddSpring((l2 * u) + l1, ((l2 + 1) * u) + l1, KsStruct, KdStruct, STRUCTURAL_SPRING);
      }
      // Shearing Springs
      for (l1 = 0; l1 < (v - 1); l1++)
      for (l2 = 0; l2 < (u - 1); l2++) {
        AddSpring((l1 * u) + l2, ((l1 + 1) * u) + l2 + 1, KsShear, KdShear, SHEAR_SPRING);
        AddSpring(((l1 + 1) * u) + l2, (l1 * u) + l2 + 1, KsShear, KdShear, SHEAR_SPRING);
      }


      // Bend Springs
      for (l1 = 0; l1 < (v); l1++) {
        for (l2 = 0; l2 < (u - 2); l2++) {
          AddSpring((l1 * u) + l2, (l1 * u) + l2 + 2, KsBend, KdBend, BEND_SPRING);
        }
        AddSpring((l1 * u) + (u - 3), (l1 * u) + (u - 1), KsBend, KdBend, BEND_SPRING);
      }
      for (l1 = 0; l1 < (u); l1++) {
        for (l2 = 0; l2 < (v - 2); l2++) {
          AddSpring((l2 * u) + l1, ((l2 + 2) * u) + l1, KsBend, KdBend, BEND_SPRING);
        }
        AddSpring(((v - 3) * u) + l1, ((v - 1) * u) + l1, KsBend, KdBend, BEND_SPRING);
      }

      int s = quad_.model_.meshes_.front().vertices_.size();
      traits_.v.reserve(s);
      traits_.f.reserve(s);
      for (unsigned i(0); i < s; ++i)
      {
        traits_.v.push_back(Vec3(0, 0, 0));
        traits_.f.push_back(Vec3(0, 0, 0));
      }

    }

    void shadowPass()
    {

      glClearDepth(1.0f);
      glClear(GL_DEPTH_BUFFER_BIT);

      shaderPhong_.bind();
      shaderPhong_.setMatrices(perspective_.getPerspectiveTransform(),
        cameraShadow_.getCameraTranslationTransform(),
        cameraShadow_.getCameraCoordinateTransform()
        );

      //shader.setConfiguration(renderManager_.getConfiguration(GameObject* obj,...), obj.getMaterial(), obj.getTransform())
      //shaderPhong_.setTransform(room_.transform_.getMatrix());
      //shaderPhong_.bindMaterial(room_.material_);
      //shaderPhong_.updateUniforms();
      //room_.render();

      shaderPhong_.setTransform(quad_.transform_.getMatrix());
      shaderPhong_.bindMaterial(quad_.material_);
      shaderPhong_.updateUniforms();
      quad_.render();

      shaderPhong_.setTransform(wall_.transform_.getMatrix());
      shaderPhong_.bindMaterial(wall_.material_);
      shaderPhong_.updateUniforms();
      wall_.render();

      shaderPhong_.setTransform(wall2_.transform_.getMatrix());
      shaderPhong_.bindMaterial(wall2_.material_);
      shaderPhong_.updateUniforms();
      wall2_.render();

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
      //glDepthFunc(GL_LEQUAL);

      float x = (float)rand() / (float)(RAND_MAX);

      const GLfloat col[] = { x, 0.f, 0.0f, 1.0f };

      //glPolygonMode(GL_FRONT_AND_BACK, renderMode_);

      //RenderManager.render();


      //shadowPass();
  
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      shaderShadow_.bind();
      shaderShadow_.shadowTex_ = &renderTex_;
      shaderShadow_.eyePos_ = camera_.getPos();
      shaderShadow_.setMatrices(perspective_.getPerspectiveTransform(),
        camera_.getCameraTranslationTransform(),
        camera_.getCameraCoordinateTransform()
        );

      shaderShadow_.lightMatrix_ = perspective_.getPerspectiveTransform() * cameraShadow_.getCameraCoordinateTransform() * cameraShadow_.getCameraTranslationTransform();
      //shaderShadow_.lightMatrix_ = perspective_.getPerspectiveTransform() * cameraShadow_.getCameraTranslationTransform() * cameraShadow_.getCameraCoordinateTransform();
      //shaderShadow_.lightMatrix_.setIdentity();

      //shaderShadow_.setTransform(room_.transform_.getMatrix());
      //shaderShadow_.bindMaterial(room_.material_);
      //shaderShadow_.updateUniforms();
      //room_.render();

      //shaderShadow_.setTransform(wall_.transform_.getMatrix());
      //shaderShadow_.bindMaterial(wall_.material_);
      //shaderShadow_.updateUniforms();
      //wall_.render();

      //shaderShadow_.setTransform(wall2_.transform_.getMatrix());
      //shaderShadow_.bindMaterial(wall2_.material_);
      //shaderShadow_.updateUniforms();
      //wall2_.render();

      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


      shaderShadow_.setTransform(quad_.transform_.getMatrix());
      shaderShadow_.bindMaterial(quad_.material_);
      shaderShadow_.updateUniforms();
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
    ShadowMapShader shaderShadow_;
    std::vector<Texture> textures_;
    int size;
    int frame;
    Transform transform_;
    float time_;
    PerspectiveTransform perspective_;
    Camera camera_;
    Camera cameraShadow_;
    Mesh room_;
    Mesh monkey_;
    Mesh quad_;
    Mesh wall_;
    Mesh wall2_;
    float speed_;
    Light light_;
    DirectionalLight dLight_;
    PointLight pLight_;
    SpotLight sLight_;
    PhongMaterial worldMat_;
    PhongMaterial roomMat_;
    PhongMaterial monkeyMat_;
    PhongMaterial planeMat_;
    
  };
}
int main(int argc, char *argv[])
{

  i3d::SkyBoxTest app;

  app.init();

  app.run();

  return 0;
}

