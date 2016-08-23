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
        meshObjects_.reserve(10);
        phongDirShaders_.resize(10);

        meshObjects_.push_back(Mesh());
        if(!import3DFromFile("../../meshes/quad.obj", meshObjects_[0]))
        {
          std::cout << "Assimp import failed." << std::endl; 
        }
        std::cout << "render verts: " << meshObjects_[0].model_.meshes_[0].vertices_.size() << std::endl; 

        meshObjects_[0].transform_.translation_ = i3d::Vec4(0.f, 1.8f, 0.f, 1.f);
        meshObjects_[0].transform_.translation_ = i3d::Vec4(0.f, 0.0f, 0.f, 0.f);
        meshObjects_[0].transform_.setRotationEuler(i3d::Vec3(0.0f, 0.0f, 0.0f));
        meshObjects_[0].initRender();

        meshObjects_.push_back(Mesh());
        if(!import3DFromFile("../../meshes/sphere.obj", meshObjects_[1]))
        {
          std::cout << "Assimp import failed." << std::endl; 
        }
        meshObjects_[1].transform_.translation_ = i3d::Vec4(0.0, 0.0, 0.0, 1);
        meshObjects_[1].transform_.setRotationEuler(i3d::Vec3(0.0, 0.0, 0.0));
        meshObjects_[1].initRender();
      }

  };

  class AssimpTest : public SDL_GL_application
  {
  public:
    AssimpTest()
    {
      setTitle(std::string("Assimp Test"));
    };

    virtual ~AssimpTest(){};

    void init()
    {
      SDL_GL_application::init();

      rm.loadAssets();

      perspective_.setPerspectiveTransform(50.f, static_cast<float>(getWindowWidth()), static_cast<float>(getWindowHeight()), 1.f, 60.f);
      camera_.initCamera(Vec3(0.f, 0.0f, -6.0f), Vec3(1.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f));

      glEnable(GL_DEPTH_TEST);

      textures_.reserve(10);

      shader_.initShader(*camera_.getPosPointer(), perspective_.getPerspectiveTransform(), camera_.getCameraTranslationTransform(), camera_.getCameraCoordinateTransform());

      normalShader_.initShader(*camera_.getPosPointer(), perspective_.getPerspectiveTransform(), camera_.getCameraTranslationTransform(), camera_.getCameraCoordinateTransform());

      ////////////////////////////////////////////////////////////////////////////////////////////////////////

      quad_.loadMesh("../../meshes/quad.obj");
      quad_.buildSmoothNormals();
      import3DFromFile( std::string("../../meshes/sphere.obj") );

      quad_.transform_.translation_ = i3d::Vec4(0.f, 1.8f, 0.f, 1.f);
      quad_.transform_.translation_ = i3d::Vec4(0.f, 0.0f, 0.f, 0.f);
      quad_.transform_.setRotationEuler(i3d::Vec3(0.0f, 0.0f, 0.0f));
      quad_.initRender();

      world_.loadMesh("../../meshes/sphere.obj");
      world_.buildSmoothNormals();
      world_.transform_.translation_ = i3d::Vec4(0.0, 0.0, 0.0, 1);
      world_.transform_.setRotationEuler(i3d::Vec3(0.0, 0.0, 0.0));
      world_.initRender();
    }

    bool import3DFromFile( const std::string& pFile)
    {
      // Check if file exists
      std::ifstream fin(pFile.c_str());
      if(!fin.fail())
      {
        fin.close();
      }
      else
      {
        return false;
      }

      scene = importer.ReadFile( pFile, aiProcessPreset_TargetRealtime_Quality);

      // If the import failed, report it
      if( !scene)
      {
        return false;
      }

      std::cout << "Assimp meshes: " << scene->mNumMeshes << std::endl; 
      std::cout << "Assimp vertices: " << scene->mMeshes[0]->mNumVertices << std::endl; 
      std::cout << "Assimp faces: " << scene->mMeshes[0]->mNumFaces << std::endl; 
      std::cout << "Assimp tcoords: " << scene->mMeshes[0]->mTextureCoords << std::endl; 

      std::cout << "Number of vertices: " << quad_.model_.meshes_.front().vertices_.size() << std::endl; 
      std::cout << "Number of faces: " << scene->mMeshes[0]->mNumFaces << std::endl; 
      std::cout << "Number of tcoords: " << quad_.model_.meshes_.front().orderedTexCoords_.size() << std::endl; 

      Mesh3D *mesh = &(quad_.model_.meshes_.front());
      mesh->vertices_.clear();
      mesh->vertexNormals_.clear();
      mesh->orderedTexCoords_.clear();

      unsigned numVerts = scene->mMeshes[0]->mNumFaces * 3;

      for(unsigned i(0); i < scene->mMeshes[0]->mNumFaces; ++i)
      {

        const aiFace &face = scene->mMeshes[0]->mFaces[i]; 

        for(unsigned j(0); j < 3; ++j)
        {
          aiVector3D p = scene->mMeshes[0]->mVertices[face.mIndices[j]];
          aiVector3D n = scene->mMeshes[0]->mNormals[face.mIndices[j]];
          aiVector3D t = scene->mMeshes[0]->mTextureCoords[0][face.mIndices[j]];

          mesh->vertices_.push_back(Vec3(p.x,p.y,p.z));
          mesh->vertexNormals_.push_back(Vec3(n.x,n.y,n.z));
          mesh->texCoords_.push_back(Vec2(t.x,t.y));
          
        }

      }

      // We're done. Everything will be cleaned up by the importer destructor
      return true;
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

      rm.meshObjects_[1].transform_.scale_.x = 1.0;
      rm.meshObjects_[1].transform_.scale_.y = 1.0;
      rm.meshObjects_[1].transform_.scale_.z = 1.0;

      shader_.setTransform(world_.transform_.getMatrix());
      shader_.setMatrices(perspective_.getPerspectiveTransform(),
        camera_.getCameraTranslationTransform(),
        camera_.getCameraCoordinateTransform()
      );

      shader_.eyePos_ = Vec3(0.8f, 0.0f, 0.0f);
      shader_.updateUniforms();
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      //world_.render();
      //rm.meshObjects_[1].render();

      world_.transform_.scale_.x = 1.01;
      world_.transform_.scale_.y = 1.01;
      world_.transform_.scale_.z = 1.01;
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      shader_.setTransform(world_.transform_.getMatrix());
      shader_.eyePos_ = Vec3(0.0f, 0.0f, 0.8f);
      shader_.updateUniforms();
      //rm.meshObjects_[1].render();
      world_.render();

//      shader_.setTransform(world_.transform_.getMatrix());
//      shader_.eyePos_ = Vec3(0.0f, 0.0f, 0.8f);
//      shader_.updateUniforms();
//      quad_.render();
//
//
//      normalShader_.bind();
//      normalShader_.setTransform(world_.transform_.getMatrix());
//      normalShader_.setMatrices(perspective_.getPerspectiveTransform(),
//      camera_.getCameraTranslationTransform(),
//      camera_.getCameraCoordinateTransform()
//      );
//
//      normalShader_.eyePos_ = Vec3(0.0f, 1.0f, 1.0f);
//      normalShader_.updateUniforms();
//      world_.renderNormals();


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
    SimpleShader shader_;
    NormalShader normalShader_;

    //TextureShader shader_;
    PhongDir shaderPhong_;
    
    std::vector<Texture> textures_;
    int size;
    int frame;
    Transform transform_;
    float time_;
    PerspectiveTransform perspective_;
    Camera camera_;
    Mesh quad_;
    Mesh world_;
    float speed_;
    Light light_;
    MyResourceManager rm;

  };
}

int main(int argc, char *argv[])
{

  i3d::AssimpTest app;

  app.init();

  app.run();

  return 0;
}

