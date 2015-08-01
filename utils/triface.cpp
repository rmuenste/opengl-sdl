#include "triface.h"

namespace i3d {

  TriFace::TriFace(void)
  {

  }

  TriFace::~TriFace(void)
  {

  }

  TriFace::TriFace(int vertexIndex[3])
  {

    for(int i = 0; i < 3; i++)
    {
      m_VertIndices[i] = vertexIndex[i];
    }//end for

  }

  TriFace::TriFace(int vertexIndex[3], int texIndex[3])
  {

    for(int i = 0; i < 3; i++)
    {
      m_VertIndices[i] = vertexIndex[i];
      m_TexIndices[i] = texIndex[i];
    }//end for

  }

  TriFace::TriFace(const TriFace& f)
  {

    for(int i=0;i<3;i++)
    {
      m_VertIndices[i]=f[i];
      m_TexIndices[i] = f.m_TexIndices[i];
    }//end for

  }//end  

  void TriFace::InitFace(const int vertexIndex[3])
  {

    for(int i = 0; i < 3; i++)
    {
      m_VertIndices[i] = vertexIndex[i];
    }//end for

  }//end initFace

  void TriFace::InitFace(int vertexIndex[3])
  {

    for(int i = 0; i < 3; i++)
    {
      m_VertIndices[i] = vertexIndex[i];
    }//end for

  }//end initFace

  void TriFace::InitFace(int vertexIndex[3], int texIndex[3])
  {

    for(int i = 0; i < 3; i++)
    {
      m_VertIndices[i] = vertexIndex[i];
      m_TexIndices[i] = texIndex[i];
    }//end for

  }//end initFace

}

