#include <resourcemanager.hpp>
#include <fstream>
#include <mesh.hpp>

#define i3dProcessPreset_TargetRealtime_Quality ( \
  aiProcess_CalcTangentSpace | \
  aiProcess_GenSmoothNormals | \
  aiProcess_JoinIdenticalVertices | \
  aiProcess_ImproveCacheLocality | \
  aiProcess_LimitBoneWeights | \
  aiProcess_RemoveRedundantMaterials | \
  aiProcess_SplitLargeMeshes | \
  aiProcess_GenUVCoords | \
  aiProcess_SortByPType | \
  aiProcess_FindDegenerates | \
  aiProcess_FindInvalidData | \
  0)

namespace i3d {

  void ResourceManager::loadAssets()
  {

  }

  bool ResourceManager::loadScene(std::string fileName)
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

    size_t pos = fileName.find_last_of(".");
    std::string sType = fileName.substr(pos);

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

    std::cout << "Assimp faces: " << scene->mMeshes[0]->mNumFaces << std::endl; std::cout << "Assimp tcoords: " << scene->mMeshes[0]->mTextureCoords << std::endl; 
    std::cout << "Material index: " << scene->mMeshes[0]->mMaterialIndex << std::endl; 
    std::cout << "Number of materials: " << scene->mNumMaterials << std::endl; 
    textures_.reserve(scene->mNumMaterials);

    for(unsigned k(0); k < scene->mNumMaterials; ++k)
    {
      aiMaterial *mat = scene->mMaterials[k]; 
      aiString name;
      mat->Get(AI_MATKEY_NAME,name);
      std::cout << "Material name: " << name.C_Str() << std::endl; 
      std::cout << "Number of textures: " << mat->GetTextureCount(aiTextureType_DIFFUSE) << std::endl; 
      aiString path;
      if(mat->GetTexture(aiTextureType_DIFFUSE, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
      {

        std::cout << "Texture path: " << path.C_Str() << std::endl; 
        std::cout << "Texture path: " << sType << std::endl; 
        std::string texName;
        if(sType == ".dae")
        {
          std::string dir("../../textures/");
          std::string basename(path.C_Str());
          texName = std::string(dir + basename);
        }
        else
        {
          texName = std::string(path.C_Str());
        }

        Texture tex;
        tex.createTextureFromImage(i3dTexType::i3d_tex_diffuse_map, texName);
        textures_.push_back(std::move(tex));

      }
      else
      {
        std::cout << "Default Texture path: " << std::endl; 
        std::string texName("../../textures/greyhalf.png");
        Texture tex;
        tex.createTextureFromImage(i3dTexType::i3d_tex_diffuse_map, texName);
        textures_.push_back(std::move(tex));
      }
      float specularExponent;
      if(mat->Get(AI_MATKEY_SHININESS, specularExponent) == AI_SUCCESS)
      {
        specularExponent = 40.0f;
        std::cout << "Specular Exponent: " << specularExponent << std::endl; 
      }
      else
      {
        specularExponent = 20.0f;
        std::cout << "Specular Exponent: " << specularExponent << std::endl; 
      }
      float specularIntensity;
      if(mat->Get(AI_MATKEY_SHININESS_STRENGTH, specularIntensity) == AI_SUCCESS)
        std::cout << "Specular Intensity: " << specularIntensity << std::endl; 
      else
      {
        specularIntensity = 0.8f;
        std::cout << "Specular Intensity: " << specularIntensity << std::endl; 
      }
      aiVector3D col_diffuse;
      float diffuseIntensity(0.0f);
      if(mat->Get(AI_MATKEY_COLOR_DIFFUSE, col_diffuse) == AI_SUCCESS)
      {
        std::cout << "Diffuse color: " << col_diffuse.x << std::endl; 
      }
      else
      {
        diffuseIntensity = 1.0f;
      }

      PhongMaterial phong;
      phong.specularIntensity_ = specularIntensity;
      phong.specularExponent_ = specularExponent;
      phong.diffuseIntensity_ = 0.8f;
      phong.textures_.push_back(&(textures_.back()));
      materials_.push_back(phong);

    }

    meshObjects_.reserve(scene->mNumMeshes);

    for(unsigned k(0); k < scene->mNumMeshes; ++k)
    {

      gameObjects_.push_back(GameObject());
      GameObject &go = gameObjects_.back();

      meshObjects_.push_back(Mesh());
      Mesh &meshObj = meshObjects_.back();
      go.meshObject_ = &meshObj;

      meshObj.polygonMode_ = GL_FILL; 
      meshObj.texFormat_   = tCoordFormat::i3d_format_assimp; 

      meshObj.model_.meshes_.push_back(Mesh3D());
      Mesh3D &mesh = meshObj.model_.meshes_.front();

      unsigned numVerts = scene->mMeshes[k]->mNumFaces * 3;

      mesh.vertices_.reserve(numVerts);

      if(scene->mMeshes[k]->HasNormals())
        mesh.vertexNormals_.reserve(numVerts);

      mesh.texCoords_.reserve(numVerts);
      std::cout << "Loading mesh with: " << numVerts << " vertices." << std::endl;
      std::cout << "Loading " << scene->mMeshes[k]->mName.C_Str() << "." << std::endl;

      
      if(scene->mMeshes[k]->mMaterialIndex < scene->mNumMaterials)
      {
        meshObj.hasTexture_ = true;
        go.material_ = &materials_[scene->mMeshes[k]->mMaterialIndex];
      }

      unsigned count = 0;

      if(scene->mMeshes[k]->mFaces[0].mNumIndices > 3)
      {
        std::cerr << "Cannot handle faces with more than 3 vertices: file: " << __FILE__ <<
          " line: " << __LINE__ << std::endl;
        std::exit(EXIT_FAILURE);
      }

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

          mesh.vertices_.push_back(Vec3(p.x,p.z,p.y));

          if(scene->mMeshes[k]->HasNormals())
            mesh.vertexNormals_.push_back(Vec3(n.x,n.z,n.y));

          mesh.texCoords_.push_back(Vec2(t.x,t.y));
          vi[j] = count;
          ti[j] = count;
          count++;
        }
        mesh.faces_.push_back(TriFace(vi, ti));
      }
      meshObj.transform_.translation_ = i3d::Vec4(0.f, 0.0f, 0.f, 0.f);
      meshObj.transform_.setRotationEuler(i3d::Vec3(0.0f, 0.0f, 0.0f));
      meshObj.initRender();
    }
    // We're done. Everything will be cleaned up by the importer destructor
    return true;
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
  } std::cout << "Assimp meshes: " << scene->mNumMeshes << std::endl; std::cout << "Assimp vertices: " << scene->mMeshes[0]->mNumVertices << std::endl; if(scene->mMeshes[0]->HasNormals()) std::cout << "Assimp normals present" << std::endl; std::cout << "Assimp faces: " << scene->mMeshes[0]->mNumFaces << std::endl; std::cout << "Assimp tcoords: " << scene->mMeshes[0]->mTextureCoords << std::endl; 
  std::cout << "Material index: " << scene->mMeshes[0]->mMaterialIndex << std::endl; 
  std::cout << "Number of materials: " << scene->mNumMaterials << std::endl; 

  for(unsigned k(0); k < scene->mNumMaterials; ++k)
  {
    aiMaterial *mat = scene->mMaterials[k]; 
    aiString name;
    mat->Get(AI_MATKEY_NAME,name);
    std::cout << "Material name: " << name.C_Str() << std::endl; 
    std::cout << "Number of textures: " << mat->GetTextureCount(aiTextureType_DIFFUSE) << std::endl; 
    aiString path;
    if(mat->GetTexture(aiTextureType_DIFFUSE, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
    {
      std::cout << "Texture path: " << path.C_Str() << std::endl; 
      std::string fileName(path.C_Str());
      Texture tex;
      tex.createTextureFromImage(i3dTexType::i3d_tex_diffuse_map, fileName);
      textures_.push_back(std::move(tex));
      meshObject.hasTexture_=true;

    }
    else
    {
      std::cout << "Default Texture path: " << std::endl; 
      std::string fileName("../../textures/earth1.png");
      Texture tex;
      tex.createTextureFromImage(i3dTexType::i3d_tex_diffuse_map, fileName);
      textures_.push_back(std::move(tex));
      meshObject.hasTexture_=true;
    }
    float specularExponent;
    if(mat->Get(AI_MATKEY_SHININESS, specularExponent) == AI_SUCCESS)
    {
      specularExponent = 40.0f;
      std::cout << "Specular Exponent: " << specularExponent << std::endl; 
    }
    else
    {
      specularExponent = 20.0f;
      std::cout << "Specular Exponent: " << specularExponent << std::endl; 
    }
    float specularIntensity;
    if(mat->Get(AI_MATKEY_SHININESS_STRENGTH, specularIntensity) == AI_SUCCESS)
      std::cout << "Specular Intensity: " << specularIntensity << std::endl; 
    else
    {
      specularIntensity = 0.8f;
      std::cout << "Specular Intensity: " << specularIntensity << std::endl; 
    }
    aiVector3D col_diffuse;
    float diffuseIntensity(0.0f);
    if(mat->Get(AI_MATKEY_COLOR_DIFFUSE, col_diffuse) == AI_SUCCESS)
    {
      std::cout << "Diffuse color: " << col_diffuse.x << std::endl; 
    }
    else
    {
      diffuseIntensity = 1.0f;
      //        specularExponent = 20.0f;
      //        std::cout << "Specular Exponent: " << specularExponent << std::endl; 
    }

    PhongMaterial phong;
    phong.specularIntensity_ = specularIntensity;
    phong.specularExponent_ = specularExponent;
    phong.diffuseIntensity_ = 0.8f;
    phong.textures_.push_back(&(textures_.back()));
    materials_.push_back(phong);
  }

  meshObject.model_.meshes_.reserve(scene->mNumMeshes);
  meshObject.polygonMode_ = GL_FILL; 
  meshObject.texFormat_ = tCoordFormat::i3d_format_assimp; 

  for(unsigned k(0); k < scene->mNumMeshes; ++k)
  {

    meshObject.model_.meshes_.push_back(Mesh3D());
    Mesh3D &mesh = meshObject.model_.meshes_[k];

    unsigned numVerts = scene->mMeshes[k]->mNumFaces * 3;
    mesh.vertices_.reserve(numVerts);

    if(scene->mMeshes[k]->HasNormals())
      mesh.vertexNormals_.reserve(numVerts);

    mesh.texCoords_.reserve(numVerts);

    unsigned count = 0;

    if(scene->mMeshes[k]->mFaces[0].mNumIndices > 3)
    {
      std::cerr << "Cannot handle faces with more than 3 vertices: file: " << __FILE__ <<
        " line: " << __LINE__ << std::endl;
      std::exit(EXIT_FAILURE);
    }

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

        mesh.vertices_.push_back(Vec3(p.x,p.z,p.y));

        if(scene->mMeshes[k]->HasNormals())
          mesh.vertexNormals_.push_back(Vec3(n.x,n.z,n.y));

        mesh.texCoords_.push_back(Vec2(t.x,t.y));
        vi[j] = count;
        ti[j] = count;
        count++;
      }
      mesh.faces_.push_back(TriFace(vi, ti));
    }
  }

  void ResourceManager::cpTriFaces(const aiMesh &aimesh, Mesh3D &mesh)
  {
    unsigned count = 0;
    for (unsigned i(0); i < aimesh.mNumFaces; ++i)
    {
      const aiFace &face = aimesh.mFaces[i];
      int vi[3];
      int ti[3];
      for (unsigned j(0); j < 3; ++j)
      {
        aiVector3D p = aimesh.mVertices[face.mIndices[j]];
        aiVector3D n = aimesh.mNormals[face.mIndices[j]];
        aiVector3D t = aimesh.mTextureCoords[0][face.mIndices[j]];

        mesh.vertices_.push_back(Vec3(p.x, p.z, p.y));

        if (aimesh.HasNormals())
          mesh.vertexNormals_.push_back(Vec3(n.x, n.z, n.y));

        mesh.texCoords_.push_back(Vec2(t.x, t.y));
        vi[j] = count;
        ti[j] = count;
        count++;
      }
      mesh.faces_.push_back(TriFace(vi, ti));
    }
  }

  void ResourceManager::cpQuadFaces(const aiMesh &aimesh, Mesh3D &mesh)
  {
    unsigned count = 0;
    for (unsigned i(0); i < aimesh.mNumFaces; ++i)
    {
      const aiFace &face = aimesh.mFaces[i];
      int vi[4];
      int ti[4];
      for (unsigned j(0); j < 4; ++j)
      {
        aiVector3D p = aimesh.mVertices[face.mIndices[j]];
        aiVector3D n = aimesh.mNormals[face.mIndices[j]];
        aiVector3D t = aimesh.mTextureCoords[0][face.mIndices[j]];

        mesh.vertices_.push_back(Vec3(p.x, p.z, p.y));

        if (aimesh.HasNormals())
          mesh.vertexNormals_.push_back(Vec3(n.x, n.z, n.y));

        mesh.texCoords_.push_back(Vec2(t.x, t.y));
        vi[j] = count;
        ti[j] = count;
        count++;
      }
      mesh.faces_.push_back(TriFace(vi, ti));
    }
  }

=======
  // We're done. Everything will be cleaned up by the importer destructor
  return true;
}
>>>>>>> Added and tested loading of a 'scene'
}
