#include <stdio.h>
#include <iostream>
#include "vectorn.h"
#include <string.h>

namespace i3d {

template<class T>
VectorN<T>::VectorN(void)
{
	m_Data=NULL;
	m_n=0;
}

template<class T>
VectorN<T>::VectorN(int n)
{
	m_n = n;
	m_Data= new T[n];
	memset(m_Data,(T)0.0,sizeof(T)*m_n);
}


template<class T>
VectorN<T>::~VectorN(void)
{
	if(m_Data != NULL)
	{
		delete[] m_Data;
		m_Data=NULL;
	}
}

template<class T>
VectorN<T>::VectorN(int n, const T *pData) : m_n(n)
{
	m_Data = new T[n];
	memcpy(m_Data,pData,n*sizeof(T));
}

template<class T>
void VectorN<T>::OutputVector()
{
	std::cout<<"------------------------------"<<std::endl;
	std::cout<<"Vector output: "<<std::endl;
	for(int j=0;j<m_n;j++)
	{
		if(m_Data[j]<0)
			std::cout<<m_Data[j]<<" ";
		else
			std::cout<<" "<<m_Data[j]<<" ";
	std::cout<<std::endl;
	}
	std::cout<<"------------------------------"<<std::endl;
	std::cout<<std::endl;	
}

//----------------------------------------------------------------------------
// Explicit instantiation.
//----------------------------------------------------------------------------
template class VectorN<float>;

template class VectorN<double>;
//----------------------------------------------------------------------------

}
