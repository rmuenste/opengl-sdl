/***************************************************************************
 *   Copyright (C) 2006 by Raphael Münster   *
 *   raphael@Cortez   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _OBJLOADER_H_
#define _OBJLOADER_H_


#include <vector>
#include <vector3.h>
#include <vector2.h>

namespace i3d {

  class Model3D;
  class Mesh3D;

  using namespace std;

  ///@cond HIDDEN_SYMBOLS
  typedef struct
  {

    int VertexIndex[3];
    int TexIndex[3];

  }tObjFace;
  ///@cond 

  typedef std::vector<VECTOR3> VertArray;
  typedef std::vector<tObjFace>  FaceArray;
  typedef std::vector<VECTOR2> TexCoordArray;

  /**
   * @brief A class that loads .obj files
   *
   * A class that loads .obj files and converts them into a Model3D
   * the internally used file format in this library
   *
   */
  class ObjLoader 
  {
    public:
      ObjLoader(void);
      ~ObjLoader(void);

      /* reads the .obj file specified in strFileName */
      void readModelFromFile(Model3D *pModel,const char *strFileName);


      /**
       * @brief Load a .obj file
       *
       * If you export from Blender use these export settings:
       * 'triangulate faces', forward axis:-Y, up axis: Z
       * 'triangulate faces', forward axis:-Z, up axis: Y
       * 'export objects as obj groups' is preferred
       */
      void readMultiMeshFromFile(Model3D *pModel,const char *strFileName);

      void readModelFromFile(char *strFileName){};

      /**
       * @brief Returns the vertices of the reader's mesh structure
       * 
       * Returns the vertices of the reader's mesh structure
       */
      const std::vector<VECTOR3>& getVertices() const;

      /**
       * @brief Returns the faces of the reader's mesh structure
       * 
       * Returns the vertices of the reader's mesh structure
       */  
      const FaceArray& getFaces() const;

      /**
       * @brief Returns the normals of the reader's mesh structure
       * 
       * Returns the vertices of the reader's mesh structure
       */  
      const std::vector<VECTOR3>& getNormals() const;

      /**
       * @brief Returns the texture coordinates of the reader's mesh structure
       * 
       * Returns the texture coordinates of the reader's mesh structure
       */    
      const std::vector<VECTOR2>& getTexCoords(void) const;

      bool hasUV(void) const;

    private:

      /**
       * @brief Reads the vertex section from the input stream
       * 
       * Reads the vertex section from the input stream
       */    
      void readVertex(ifstream &in, char strLine[]);

      /**
       * @brief Reads the vertex section from the input stream
       * 
       * Reads the vertex section from the input stream
       */      
      void readVertices(ifstream &in, char strLine[]);

      /**
       * @brief Reads the face section from the input stream
       * 
       * Reads the face section from the input stream
       */      
      void readFaces(ifstream &in, char strLine[]);

      /**
       * @brief Reads a single face from the input stream
       * 
       * Reads a single face from the input stream
       */      
      void readFace(ifstream &in, char strLine[]);

      /**
       * @brief Reads a single texture coordinate from the input stream
       * 
       * Reads a single texture coordinate from the input stream
       */      
      void readTexCoord(Mesh3D &mesh, ifstream &in, char strLine[]);

      /**
       * @brief Reads the texture coordinates for a face from the stream
       * 
       * Reads the texture coordinates for a face from the stream
       */      
      void readFaceTex(Mesh3D &pMesh, ifstream &in, char strLine[]);

      /**
       * @brief Initiates reading of a sub mesh from the stream
       * 
       * Initiates reading of a sub mesh from the stream
       */      
      void readSubMesh(ifstream &in, Mesh3D *pMesh);

      /* private member variables */

      VertArray vertices_;

      TexCoordArray texCoords_;

      FaceArray faces_;

      bool uv_;

      bool normals_;

      Model3D *model_;

      std::string type_;

      int offset_;

  };

}

#endif
