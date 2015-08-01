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

//===================================================
//					INCLUDES
//===================================================

#include "3dmodel.h"
#include <aabb3.h>
#include <string>

namespace i3d {

  Model3D::Model3D(void)  
  {
    numMaterials_ = 0;
  }//end constructor

  Model3D::~Model3D(void)
  {
  }//end deconstructor

  Model3D::Model3D(const Model3D &pModel)
  {
    this->numMaterials_=pModel.numMaterials_;
    this->materials_= pModel.materials_;
    this->meshes_=pModel.meshes_;

    radius_ = pModel.radius_;
    box_   = pModel.box_;

  }

  void Model3D::GenerateBoundingBox()
  {
    int iVertsTotal = 0;
    for(unsigned int i = 0; i < meshes_.size();i++)
    {
      iVertsTotal+=meshes_[i].getNumVertices();
    }//end for

    std::vector<VECTOR3> Vec3Array;
    int iVert=0;
    for(unsigned int i = 0; i < meshes_.size();i++)
    {
      for(int j=0;j<meshes_[i].getNumVertices();j++)
      {

        Vec3Array.push_back(meshes_[i].vertices_[j]);
      }
    }//end for

    box_.initBox(Vec3Array);

  }

  void Model3D::AddMaterial(tMaterialInfo& pMatInfo)
  {

    //add the new material
    this->materials_.push_back(pMatInfo);
    this->numMaterials_++;

  }//end AddMaterial

  // Outputs the most important data of the model in a structered way
  void Model3D::OutputModelInfo(void)
  {
    int iVertsTotal = 0;
    for(unsigned int i = 0; i < meshes_.size();i++)
    {
      iVertsTotal+=meshes_[i].getNumVertices();
    }//end for
    std::cout<<"MODEL DATA INFO"<<std::endl;
    std::cout<<"--------------------------"<<std::endl;
    std::cout<<"Number of Subobjects: "<<this->meshes_.size()<<std::endl;
    std::cout<<"--------------------------"<<std::endl;
    std::cout<<"Object names: "<<std::endl;
    for(unsigned int i = 0; i < meshes_.size();i++)
    {
      std::cout<<"'"<<meshes_[i].fileName_<<"'"<<std::endl;
    }//end for
    std::cout<<"--------------------------"<<std::endl;
    std::cout<<"Number of Vertices total: "<<iVertsTotal<<std::endl;
    std::cout<<"--------------------------"<<std::endl;
    std::cout<<"Number of Materials: "<<this->numMaterials_<<std::endl;
    for(unsigned int i = 0; i < this->materials_.size();i++)
    {
      std::cout<<"'"<<materials_[i].strName<<"'"<<std::endl;
      std::cout<<"'"<<materials_[i].strFile<<"'"<<std::endl;
    }//end for
  }//end OutputModelInfo


  void Model3D::BuildVertexArrays(void)
  {

    //loop oover all the meshes and build their subobjects
    for(unsigned int i = 0; i < meshes_.size();i++)
    {
      meshes_[i].buildIndexArrays();
    }//end for

  }//end BuildVertexArrays


  void Model3D::CreateFrom(std::vector<VECTOR3 > &vVertices, std::vector<TriFace> &vFaces)
  {

    this->numMaterials_=0;
    Mesh3D mesh;
    mesh.textured_=false;
    mesh.valid_=true;
    mesh.matId_=-1;
    mesh.numFaces_=vFaces.size();
    mesh.numVerts_=vVertices.size();
    mesh.numTexCoords_=0;
    mesh.vertices_.clear();
    mesh.faces_.clear();

    std::vector<VECTOR3>::iterator vIter;
    std::vector<TriFace>::iterator tIter;

    int index=0;
    for(vIter=vVertices.begin();vIter!=vVertices.end();vIter++)
    {
      VECTOR3 vec=*vIter;
      mesh.vertices_[index]=vec;
      index++;
    }//end for

    index=0;
    for(tIter=vFaces.begin();tIter!=vFaces.end();tIter++)
    {
      TriFace tri=*tIter;
      mesh.faces_[index]=tri;
      index++;
    }//end for

    meshes_.push_back(mesh);

  }

  MeshIter Model3D::begin()
  {
    return meshes_.begin();
  }

  MeshIter Model3D::end()
  {
    return meshes_.end();
  }

  std::vector<Triangle3r> Model3D::GenTriangleVector()
  {
    std::vector<Triangle3r> vTriangles;
    MeshIter mIter = begin();
    int count = 0;
    for(;mIter != end();mIter++)
    {
      Mesh3D &mesh = *mIter;
      std::vector<TriFace>::iterator faceIter;

      for(faceIter=mesh.faces_.begin();faceIter!=mesh.faces_.end();faceIter++)
      {
        TriFace tri=*faceIter;

        //We loop through all triangular faces of the
        // model. This variable will hold the current face
        Triangle3r tri3(mesh.getVertices()[tri[0]],mesh.getVertices()[tri[1]],mesh.getVertices()[tri[2]]);
        tri3.idx_ = count++;
        vTriangles.push_back(tri3);
      }//end for
    }//end for
    return vTriangles;
  }

}
