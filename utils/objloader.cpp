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

#include "objloader.h"
#include "3dmodel.h"
#include <fstream>
#include <sstream>
#include <string.h>

namespace i3d {

  ObjLoader::ObjLoader(void)
  {

    uv_ = false;	
    normals_ = false;

  }//end constructor

  ObjLoader::~ObjLoader(void)
  {

  }//end deconstructor

  void ObjLoader::readModelFromFile(Model3D *pModel,const char *strFileName)
  {

    ifstream in(strFileName);

    char strLine[256];
    string first;

    model_ = pModel;
    Mesh3D mesh;

    if(!in.is_open())
    {
      std::cerr<<"Unable to open file: "<<strFileName<<std::endl;
      exit(0);
    }
    int faces=0;
    int line=0;
    while(!in.eof())
    {    
      in>>first;
      if(first == string("#"))
      {
        in.getline(strLine,256);
        line++;
        continue;
      }
      else if(first == string(""))
      {
        in.getline(strLine,256);
        line++;
        continue;
      }
      //case: Vertex
      else if(first == string("v"))
      {
        //readVertex(in,strLine);
        Vec3 vec;
        in >> vec.x;
        in >> vec.y;
        in >> vec.z;
        float t = vec.y;
        vec.y=vec.z;
        vec.z=t;
        in.getline(strLine,256);
        mesh.vertices_.push_back(vec);
        line++;
      }
      else if(first == string("vn"))
      {
        normals_=true;
        //readVertex(in,strLine);
        Vector3f vec;
        in >> vec.x;
        in >> vec.y;
        in >> vec.z;
        in.getline(strLine,256);
        mesh.vertexNormals_.push_back(vec);
        line++;
      }
      //case: TexCoord
      else if(first == string("vt"))
      {
        readTexCoord(mesh, in, strLine);
        uv_ = true;
        mesh.setIsTextured(true);
      }
      //case: Face
      else if(first == string("f"))
      {
//        std::cout << "face: " << faces  << std::endl;
//        std::cout << "line: " << line  << std::endl;
        readFaceTex(mesh, in, strLine);
        in.getline(strLine, 256);
//        printf("strLine: %s\n",strLine);
        faces++;
        line++;
      }
      //case: Object groupd
      else if(first == string("o"))
      {
        std::cerr<<"Found Mesh(es) in non-supported OBJ object format. Please use the OBJ group format."<<std::endl;
        exit(0);
      }
      //case: Object groupd
      else if(first == string("g"))
      {
        in.getline(strLine,256);
        std::cout << "Found obj group mode format" << std::endl;
        line++;
      }        
      //default
      else
      {
        in.getline(strLine,256);
        line++;
      }

    }//end while

    cout <<"Number of vertices: "<<mesh.vertices_.size()<<endl;
    cout <<"Number of normals: "<<mesh.vertexNormals_.size()<<endl;
    cout <<"Number of faces: "<<mesh.faces_.size()<<endl;
    cout <<"Number of texVertex: "<<mesh.texCoords_.size()<<endl;


    model_->meshes_.push_back(mesh);

    //add a dummy material
    tMaterialInfo info;
    char f[255]="COWLOFTH.bmp";
    strcpy(info.strFile,f);
    info.texureId = 0;
    pModel->addMaterial(info);
    pModel->meshes_[0].setMaterialId(0);

  }//end ReadModelFromFile

  void ObjLoader::readMultiMeshFromFile(Model3D *pModel,const char *strFileName)
  {

    ifstream in(strFileName);

    char strLine[256];
    string first;

    model_ = pModel;
    Mesh3D mesh;

    if(!in.is_open())
    {
      std::cerr<<"Unable to open file: "<<strFileName<<std::endl;
      exit(0);
    }

    //count the number of sub-meshes
    int subMeshes = 0;
    offset_ = 0;
    while(!in.eof())
    {
      in>>first;
      if(first == string("g"))
      {
        subMeshes++;
        in.getline(strLine,256);
      }
      else
        in.getline(strLine,256);
    }//end while

    in.clear();
    in.seekg(0,ios::beg);

    for(int i=0;i< subMeshes;i++)
    {
      Mesh3D mesh;
      readSubMesh(in,&mesh);
      pModel->meshes_.push_back(mesh);
      offset_+=mesh.getNumVerts();
    }

  }//end ReadMultiMeshFromFile

  void ObjLoader::readSubMesh(ifstream &in, Mesh3D *pMesh)
  {

    char strLine[256];
    while(!in.eof())
    {
      in>>type_;
      if(type_ == string("#"))
      {
        in.getline(strLine,256);
        continue;
      }
      //case: Vertex
      else if(type_ == string("v"))
        readVertices(in,strLine);
      //case: TexCoord
      //case: Face
      else if(type_ == string("f"))
      {
        readFaces(in, strLine);
        break;
      }
      //default
      else
        in.getline(strLine,256);
    }//end while

    //assign number of vertices
    pMesh->vertices_.clear();
    pMesh->numVerts_=vertices_.size();
    pMesh->numFaces_=faces_.size();
    pMesh->numTexCoords_=texCoords_.size();
    pMesh->texCoords_.clear();
    pMesh->faces_.clear();

    for(unsigned int i=0;i<vertices_.size();i++)
    {
      pMesh->vertices_.push_back(vertices_[i]);
    }

    for(unsigned int i=0;i<texCoords_.size();i++)
    {
      pMesh->texCoords_.push_back(texCoords_[i]);
    }

    for(unsigned int i=0;i<faces_.size();i++)
    {
      pMesh->faces_.push_back(TriFace(faces_[i].VertexIndex));
    }//end for

    //reset the vectors
    faces_.clear();
    texCoords_.clear();
    vertices_.clear();

  }

  void ObjLoader::readVertices(ifstream &in, char strLine[])
  {

    while(!in.eof() && type_==string("v"))
    {
      readVertex(in,strLine);
      in >> type_;
    }
  }

  void ObjLoader::readFaces(ifstream &in, char strLine[])
  {
    while(!in.eof() && type_==string("f"))
    {
      readFace(in,strLine);
      in >> type_;
    }
  }

  void ObjLoader::readVertex(ifstream &in, char strLine[])
  {

    Vector3f vec;
    in >> vec.x;
    in >> vec.y;
    in >> vec.z;
    //vec.y=-vec.y;
    in.getline(strLine,256);
    vertices_.push_back(vec);

  }//end ReadVertex

  void ObjLoader::readFace(ifstream &in, char strLine[])
  {

    tObjFace Face;

    for(int i = 0; i < 3; i++)
    {
      in >> Face.VertexIndex[i];
      Face.VertexIndex[i]-=(offset_+1);
    }

    in.getline(strLine, 256);
    faces_.push_back(Face);

  }//end ReadFace

  void ObjLoader::readTexCoord(Mesh3D &mesh, ifstream &in, char strLine[])
  {

    VECTOR2 vec;
    in >> vec.x;
    in >> vec.y;
//    vec.x = 1.0f - vec.x;
//    vec.y = 1.0f - vec.y;
    mesh.texCoords_.push_back(vec);
    in.getline(strLine,256);

  }//end ReadTexCoord

  void ObjLoader::readFaceTex(Mesh3D &mesh, ifstream &in, char strLine[])
  {
    tObjFace Face;

    string s;

    basic_string<char> vertIndex;
    basic_string<char> texIndex;
    int vi;
    int ti;

    for(int i = 0; i < 3; i++)
    {
      // Format for a face is vertexIndex/texture index vertexIndex/textureIndex vertexIndex/texture index 
      // Format for a face is vI//tI vI//tI vI//tI 
      // Format for a face is vI/tI/nI vI/tI/nI vI/tI/nI 
      if(uv_ && normals_)
      {
        in >> s;
        std::cout << "String: " << s  << std::endl;
        if(s.empty()) return;
        // find separator 
        basic_string<char>::size_type index = s.find("/");

        // extract indices
        vertIndex = s.substr(0,index);
        texIndex = s.substr(index+1,index+2);

        // convert indices from string to int
        istringstream VertStream(vertIndex);
        istringstream TexStream(texIndex);
        
        VertStream >> vi;
        TexStream  >> ti;

        //assign the values to the face structure
        Face.VertexIndex[i] = vi-1;
        Face.TexIndex[i]    = ti-1;		
      }
      else if(normals_)
      {
        in >> s;
        //std::cout << "String: " << s  << std::endl;
        if(s.empty()) return;
        // find separator 
        basic_string<char>::size_type index = s.find("/");

        // extract indices
        vertIndex = s.substr(0,index);
        texIndex = s.substr(index+2,index+3);

        // convert indices from string to int
        istringstream VertStream(vertIndex);
        istringstream TexStream(texIndex);
        
        VertStream >> vi;
        TexStream  >> ti;

        //assign the values to the face structure
        Face.VertexIndex[i] = vi-1;
        Face.TexIndex[i]    = ti-1;		
      }
    }

    mesh.faces_.push_back(TriFace(Face.VertexIndex, Face.TexIndex));
//    std::cout << "Face.TexIndex:0:" << mesh.faces_.back().m_TexIndices[0] << std::endl;
//    std::cout << "Face.TexIndex:1:" << mesh.faces_.back().m_TexIndices[1] << std::endl;
//    std::cout << "Face.TexIndex:2:" << mesh.faces_.back().m_TexIndices[2] << std::endl;

  }//end ReadFaceTex

  const VertArray& ObjLoader::getVertices() const
  {

    return vertices_;

  }//end GetVertices

  const FaceArray& ObjLoader::getFaces() const
  {

    return faces_;

  }//end GetVertices

  bool ObjLoader::hasUV(void) const
  {
    return uv_;
  }

  const std::vector<Vec2>& ObjLoader::getTexCoords(void) const
  {
    return texCoords_;
  }//end GetTexCoords

}
