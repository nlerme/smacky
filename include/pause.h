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

#ifndef __PAUSE_H__
#define __PAUSE_H__

#include "impulse.h"
#include "smart_ptr.h"
#include "types.h"


class CPause : public IImpulse
{
     friend class CSmartPtr<CPause>;

     private :
	CPause( const ushort Duration );

     public :
	static CSmartPtr<IImpulse> Create( const ushort Duration )
	{
		return new CPause(Duration);
	}

	virtual ~CPause();

	static float DefaultTempoFactor()
	{
		return 1.0f;
	}

	virtual void Play() const;
	virtual EEntityType Type() const;
};

#endif //__PAUSE_H__
