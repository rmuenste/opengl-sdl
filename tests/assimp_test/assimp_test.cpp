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

#include <assimp/Importer.hpp>	//OO version Header!
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

namespace i3d {

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
      import3DFromFile( std::string("../../meshes/big_room.obj") );
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

      // We're done. Everything will be cleaned up by the importer destructor
      return true;
    }

    void render()
    {


    }

    void handleResizeEvent(SDL_Event &event)
    {

    }

    void handleKeyPressEvent(SDL_Event &event)
    {

    }

  private:
    /* data */
    // Create an instance of the Importer class
    Assimp::Importer importer;
    const aiScene* scene = nullptr;

  };
}
int main(int argc, char *argv[])
{

  i3d::AssimpTest app;

  app.init();

  app.run();

  return 0;
}

