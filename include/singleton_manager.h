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

#ifndef __SINGLETON_MANAGER_H__
#define __SINGLETON_MANAGER_H__

#include <stack>
#include "types.h"


class CSingletonBase;


class CSingletonManager
{
     private :
	std::stack<CSingletonBase *> m_InstancesList;

	CSingletonManager(){}

	~CSingletonManager()
	{
		while( !m_InstancesList.empty() )
		{
			m_InstancesList.top()->_Kill();
			m_InstancesList.pop();
		}
	}

     public :
	static void Add( CSingletonBase * Singleton )
	{
		static CSingletonManager Instance;
		Instance.m_InstancesList.push(Singleton);
	}

	static const uint NumberOfInstances()
	{
		static CSingletonManager Instance;
		return Instance.m_InstancesList.size();
	}
};

#endif //__SINGLETON_MANAGER_H__
