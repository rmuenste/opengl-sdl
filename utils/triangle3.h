/***************************************************************************
 *   Copyright (C) 2006 by Raphael Mnster   *
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

#ifndef _TRIANGLE3D_H
#define _TRIANGLE3D_H

//===================================================
//					DEFINITIONS
//===================================================


//===================================================
//					INCLUDES
//===================================================
#include <iostream>
#include <vector3.h>

namespace i3d {

/**
 * @brief A triangle class that stores the coordinates of the three vertices
 */
template<class T>
class Triangle3
{

public:

  int idx_;

	/* constructors */
	Triangle3(void);

	Triangle3(const Vector3<T> &vV0, const Vector3<T> &vV1, const Vector3<T> &vV2);

	Triangle3(const Triangle3 &triT);

	/* deconstructors */
	~Triangle3(void){};

	inline Vector3<T> Get(int i)
	{

		if(i==0)return m_vV0;
		else if(i==1)return m_vV1;
		else if(i==2)return m_vV2;
		else return Vector3<T>();

	}//end Get

	inline Vector3<T> Get(int i) const
	{

		if(i==0)return m_vV0;
		else if(i==1)return m_vV1;
		else if(i==2)return m_vV2;
		else return Vector3<T>();

	}//end Get

	inline void operator=(const Triangle3<T> &triT)
	{
		m_vV0 = triT.m_vV0;
		m_vV1 = triT.m_vV1;
		m_vV2 = triT.m_vV2;
	}//end operator=

	inline Vector3<T> GetCenter()
	{
		Vector3<T> vCenter = (m_vV0 + m_vV1 + m_vV2) * (1.0/3.0);
		return vCenter;
	};
  
  inline T GetArea()
  {
    T area = 0.5 * Vector3<T>::Cross((m_vV1 - m_vV0),(m_vV2 - m_vV0)).mag();
    return fabs(area);
  };
  
  inline Vector3<T> GetNormal()
  { 
    Vector3<T> normal = Vector3<T>::Cross((m_vV1 - m_vV0),(m_vV2 - m_vV0));
    normal.Normalize();
    return normal;      
  }

	Vector3<T> m_vV0;	
	Vector3<T> m_vV1;	
	Vector3<T> m_vV2;	
};

template<class T>
Triangle3<T>::Triangle3() : idx_(-1)
{
}//end constructor

template<class T>
Triangle3<T>::Triangle3(const Vector3<T> &vV0, const Vector3<T> &vV1, const Vector3<T> &vV2) 
: m_vV0(vV0), m_vV1(vV1), m_vV2(vV2), idx_(-1)
{

}//end constructor

template<class T>
Triangle3<T>::Triangle3(const Triangle3<T> &triT)
{
	m_vV0 = triT.m_vV0;
	m_vV1 = triT.m_vV1;
	m_vV2 = triT.m_vV2;
  idx_ = triT.idx_;
}//end constructor

typedef Triangle3<float> Triangle3f;
typedef Triangle3<double> Triangle3d;
typedef Triangle3<Real> Triangle3r;

}

#endif
