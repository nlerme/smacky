/*
    Smacky - Music emulator on speakers
    Copyright (C) 2006 - Nicolas Lermé

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 3 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <cstdio>
#include "singleton_base.h"
#include "singleton_manager.h"


template <typename T> class CSingleton : public CSingletonBase
{
     private :
	static T * m_Instance;

	void _Kill()
	{
		delete m_Instance;
		m_Instance = NULL;
	}

     protected :
	CSingleton(){}
	virtual ~CSingleton(){}

     public :
	static T * Instance()
	{
		if( !m_Instance )
		{
			m_Instance = new T;
			CSingletonManager::Add(m_Instance);
		}

		return m_Instance;
	}
};

template <typename T> T * CSingleton<T>::m_Instance = NULL;

#endif //__SINGLETON_H__
