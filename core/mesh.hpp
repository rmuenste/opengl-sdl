#ifndef MESH_HPP_5LMS1ZES
#define MESH_HPP_5LMS1ZES

#include <3dmodel.h>
#include <basicshader.hpp>
#include <transform.hpp>
#include <texture.hpp>
#include <vector>
#include <matrix4x4.h>
#include <GL/gl.h>

namespace i3d {

  // export from blender
  // Z up, Y forward -> Cull counter-clockwise
  // load obj: swap y with z
  class Mesh
  {
    public:
      Mesh ()
      {
        this->shader_ = nullptr;
        hasTexture_ = false;
      };

      virtual ~Mesh (){};

      void loadMesh(std::string fileName, bool indexed = true);

      void loadTexture(std::string fileName);

      void render();

      void setVertexShader(std::string fileName);

      void setFragmentShader(std::string fileName);

      void initRender();

      void initNonIndexedRender();
       
      void rotate(const Vec3 &v);

      void render(const Mat4 &perspective, const Mat4 &cameraTrans, const Mat4 &cameraCoord, const Vec3 &cameraPos);

      void renderNonIndexed(const Mat4 &perspective, const Mat4 &cameraTrans, const Mat4 &cameraCoord, const Vec3 &cameraPos);

      Transform transform_;

      BasicShader* shader_;

      Texture texture_;

      Model3D model_;

      bool hasTexture_;

    private:
      /* data */
      GLuint vao;
      GLuint iao;
      GLuint buffers[3];
      int drawVertices_;
  };

}

#endif /* end of include guard: MESH_HPP_5LMS1ZES */
