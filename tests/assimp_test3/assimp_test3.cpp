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
        textures_.reserve(10);
        gameObjects_.push_back(GameObject());
        meshObjects_.push_back(Mesh());

        if(!import3DFromFile("../../meshes/tquad.obj", meshObjects_[0]))
        {
          std::cout << "Assimp import failed." << std::endl; 
        }
        std::cout << "render verts: " << meshObjects_[0].model_.meshes_[0].vertices_.size() << std::endl; 

        meshObjects_[0].transform_.translation_ = i3d::Vec4(0.f, 1.8f, 0.f, 1.f);
        meshObjects_[0].transform_.translation_ = i3d::Vec4(0.f, 0.0f, 0.f, 0.f);
        meshObjects_[0].transform_.setRotationEuler(i3d::Vec3(0.0f, 0.0f, 0.0f));
        meshObjects_[0].initRender();
        gameObjects_[0].meshObject_ = &meshObjects_[0];

        gameObjects_.push_back(GameObject());
        meshObjects_.push_back(Mesh());
        if(!import3DFromFile("../../meshes/sphere.obj", meshObjects_[1]))
        {
          std::cout << "Assimp import failed." << std::endl; 
        }
        meshObjects_[1].transform_.translation_ = i3d::Vec4(0.0, 0.0, 0.0, 1);
        meshObjects_[1].transform_.setRotationEuler(i3d::Vec3(0.0, 0.0, 0.0));
        meshObjects_[1].initRender();
        gameObjects_[1].meshObject_ = &meshObjects_[1];

        gameObjects_.push_back(GameObject());
        meshObjects_.push_back(Mesh());

        //if(!import3DFromFile("../../meshes/tri_faces.obj", meshObjects_[2]))
        //{
        //  std::cout << "Assimp import failed." << std::endl; 
        //}
        //meshObjects_[2].transform_.translation_ = i3d::Vec4(0.0, 0.0, 0.0, 1);
        //meshObjects_[2].transform_.setRotationEuler(i3d::Vec3(0.0, 0.0, 0.0));
        //meshObjects_[2].initRender();
        //gameObjects_[2].meshObject_ = &meshObjects_[2];

        gameObjects_[2].meshObject_ = &meshObjects_[2];
        Mesh &m = meshObjects_.back();

        m.model_.meshes_.push_back(Mesh3D());
        Mesh3D &mesh3d = m.model_.meshes_.back();

        Vec3 v(-2,-2,0);
        float length=4.0f;
        int divs = 20;
        float gridSize=length/float(divs);
        for(unsigned y(0); y <= divs; ++y)
        {
          mesh3d.vertices_.push_back(Vec3(v.x, 0, v.y + y*gridSize));
          mesh3d.vertices_.push_back(Vec3(v.x + length, 0, v.y + y*gridSize));
        }
        for(unsigned x(0); x <= divs; ++x)
        {
          mesh3d.vertices_.push_back(Vec3(v.x + x*gridSize, 0, v.y));
          mesh3d.vertices_.push_back(Vec3(v.x + x*gridSize, 0, v.y + length));
        }

        for (unsigned i(0); i < mesh3d.vertices_.size(); ++i)
        {
          mesh3d.texCoords_.push_back(Vec2(0,0));
          mesh3d.vertexNormals_.push_back(Vec3(0, 1, 0));
        }

        m.hasTexture_ = true;;
        m.primitiveMode_=GL_LINES;

        meshObjects_[2].transform_.translation_ = i3d::Vec4(0.0, 0.0, 0.0, 1);
        meshObjects_[2].transform_.setRotationEuler(i3d::Vec3(0.0, 0.0, 0.0));
        meshObjects_[2].initRender();
        gameObjects_[2].meshObject_ = &meshObjects_[2];
        
      }

  };

  class AssimpTest3 : public SDL_GL_application
  {
  public:
    AssimpTest3()
    {
      setTitle(std::string("Assimp Texture"));
      speed_ = 0.05f;
    };

    virtual ~AssimpTest3(){};

    void init()
    {
      SDL_GL_application::init();
      
      glEnable(GL_TEXTURE_2D);

      rm.loadAssets();

      perspective_.setPerspectiveTransform(50.f, static_cast<float>(getWindowWidth()), static_cast<float>(getWindowHeight()), 1.f, 60.f);

      camera_.initCamera(Vec3(0.f, 1.0f, -6.0f), Vec3(1.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f));
      
      rm.phongDirShaders_.push_back(SimpleShader());
      rm.phongDirShaders_[0].initShader(*camera_.getPosPointer(), perspective_.getPerspectiveTransform(), camera_.getCameraTranslationTransform(), camera_.getCameraCoordinateTransform());
      normalShader_.initShader(*camera_.getPosPointer(), perspective_.getPerspectiveTransform(), camera_.getCameraTranslationTransform(), camera_.getCameraCoordinateTransform());

      rm.gameObjects_[0].shader_ = &rm.phongDirShaders_[0];
      rm.gameObjects_[1].shader_ = &rm.phongDirShaders_[0];
      rm.gameObjects_[2].shader_ = &rm.phongDirShaders_[0];

      glEnable(GL_DEPTH_TEST);

    }

    void render()
    {

      static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
      static const GLfloat ones[] = { 1.0f };
      static const GLfloat green[] = { 0.0f, 0.8f, 0.0f, 1.0f };
      static const GLfloat grey[] = { 0.3f, 0.3f, 0.3f, 1.0f };

      glClearBufferfv(GL_COLOR, 0, black);
      glClearBufferfv(GL_DEPTH, 0, ones);

      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      glClearBufferfv(GL_COLOR, 0, grey);
      glClearBufferfv(GL_DEPTH, 0, ones);

      glEnable(GL_CULL_FACE);
      glFrontFace(GL_CCW);

      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);

      float x = (float)rand() / (float)(RAND_MAX);

      const GLfloat col[] = { x, 0.3f, 0.3f, 0.3f };

      rm.gameObjects_[0].shader_->bind();

      rm.gameObjects_[1].meshObject_->transform_.scale_.x = 1.0;
      rm.gameObjects_[1].meshObject_->transform_.scale_.y = 1.0;
      rm.gameObjects_[1].meshObject_->transform_.scale_.z = 1.0;


      rm.gameObjects_[1].shader_->setTransform(rm.meshObjects_[1].transform_.getMatrix());
      rm.gameObjects_[1].shader_->setMatrices(perspective_.getPerspectiveTransform(),
        camera_.getCameraTranslationTransform(),
        camera_.getCameraCoordinateTransform()
      );

      rm.gameObjects_[1].shader_->eyePos_ = Vec3(0.8f, 0.0f, 0.0f);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glEnable(GL_TEXTURE_2D);

      rm.materials_[0].bindTexture();

      rm.gameObjects_[0].shader_->setTransform(rm.meshObjects_[0].transform_.getMatrix());
      rm.gameObjects_[0].shader_->updateUniforms();
      rm.meshObjects_[0].render();
      
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

      rm.gameObjects_[1].shader_->setTransform(rm.meshObjects_[1].transform_.getMatrix());
      rm.gameObjects_[1].shader_->eyePos_ = Vec3(0.0f, 0.0f, 0.8f);
      rm.gameObjects_[1].shader_->updateUniforms();
      rm.meshObjects_[1].render();

      normalShader_.bind();
      normalShader_.setTransform(rm.meshObjects_[2].transform_.getMatrix());
      normalShader_.setMatrices(perspective_.getPerspectiveTransform(),
        camera_.getCameraTranslationTransform(),
        camera_.getCameraCoordinateTransform()
      );

      normalShader_.eyePos_ = Vec3(0.6f, 0.6f, 0.6f);
      normalShader_.updateUniforms();
      rm.gameObjects_[2].render();

      //rm.materials_[2].bindTexture();
      //rm.gameObjects_[2].shader_->setTransform(rm.meshObjects_[2].transform_.getMatrix());
      //rm.gameObjects_[2].shader_->eyePos_ = Vec3(0.0f, 0.0f, 0.8f);
      //rm.gameObjects_[2].shader_->updateUniforms();
      //rm.meshObjects_[2].render();

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
//      case SDLK_UP:
//        camera_.moveV(speed_);
//        break;
//      case SDLK_DOWN:
//        camera_.moveV(-speed_);
//        break;
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
    NormalShader normalShader_;

  };

}

int main(int argc, char *argv[])
{

  i3d::AssimpTest3 app;

  app.init();

  app.run();

  return 0;
}

