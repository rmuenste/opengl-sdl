#include <resourcemanager.hpp>
#include <fstream>
#include <mesh.hpp>

namespace i3d {

  void ResourceManager::loadAssets()
  {

  }

  void ResourceManager::loadScene()
  {

  }

  bool ResourceManager::import3DFromFile(const std::string fileName, Mesh &meshObject)
  {
    // Check if file exists
    std::ifstream fin(fileName.c_str());
    if(!fin.fail())
    {
      fin.close();
    }
    else
    {
      std::cout << "File: " << fileName << " could not be loaded." << std::endl; 
      return false;
    }

    scene = importer.ReadFile( fileName, aiProcessPreset_TargetRealtime_Quality);

    // If the import failed, report it
    if( !scene)
    {
      std::cout << "Scene import failed." << std::endl; 
      return false;
    }

    std::cout << "Assimp meshes: " << scene->mNumMeshes << std::endl; 
    std::cout << "Assimp vertices: " << scene->mMeshes[0]->mNumVertices << std::endl; 

    if(scene->mMeshes[0]->HasNormals())
      std::cout << "Assimp normals present" << std::endl; 
    
    std::cout << "Assimp faces: " << scene->mMeshes[0]->mNumFaces << std::endl; 
    std::cout << "Assimp tcoords: " << scene->mMeshes[0]->mTextureCoords << std::endl; 

    meshObject.model_.meshes_.reserve(scene->mNumMeshes);

    for(unsigned k(0); k < scene->mNumMeshes; ++k)
    {

      meshObject.model_.meshes_.push_back(Mesh3D());
      Mesh3D &mesh = meshObject.model_.meshes_[k];

      unsigned numVerts = scene->mMeshes[k]->mNumFaces * 3;
      mesh.vertices_.reserve(numVerts);
      mesh.vertexNormals_.reserve(numVerts);
      mesh.texCoords_.reserve(numVerts);

      unsigned count = 0;
      for(unsigned i(0); i < scene->mMeshes[k]->mNumFaces; ++i)
      {

        const aiFace &face = scene->mMeshes[k]->mFaces[i]; 
        int vi[3];
        int ti[3];
        for(unsigned j(0); j < 3; ++j)
        {
          aiVector3D p = scene->mMeshes[k]->mVertices[face.mIndices[j]];
          aiVector3D n = scene->mMeshes[k]->mNormals[face.mIndices[j]];
          aiVector3D t = scene->mMeshes[k]->mTextureCoords[0][face.mIndices[j]];

          mesh.vertices_.push_back(Vec3(p.x,p.y,p.z));
          mesh.vertexNormals_.push_back(Vec3(n.x,n.y,n.z));
          mesh.texCoords_.push_back(Vec2(t.x,t.y));
          vi[j] = count;
          ti[j] = count;
          count++;
        }
        mesh.faces_.push_back(TriFace(vi, ti));
      }
    }
    // We're done. Everything will be cleaned up by the importer destructor
    return true;
  }
}
