#ifndef RESOURCEMANAGER_HPP_HYE1LHJ6
#define RESOURCEMANAGER_HPP_HYE1LHJ6

#include <vector2.h>
#include <vector3.h>
#include <mesh.hpp>
#include <phongshader.hpp>
#include <phong_dir.hpp>
#include <assimp/Importer.hpp>	
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <simpleshader.hpp>

namespace i3d {

  class ResourceManager {

    public:

      std::vector<Mesh> meshObjects_;
      std::vector<Texture> textures_;
      std::vector<PhongMaterial> materials_;
      std::vector<SimpleShader> phongDirShaders_;
      Assimp::Importer importer;
      const aiScene* scene = nullptr;

      ResourceManager(){};
      ~ResourceManager(){};

      virtual void loadAssets();

      virtual void loadScene();

      bool import3DFromFile(const std::string fileName, Mesh &meshObject);

  };

};

#endif /* end of include guard: RESOURCEMANAGER_HPP_HYE1LHJ6 */
