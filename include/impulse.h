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

#ifndef __IMPULSE_H__
#define __IMPULSE_H__

#include "entity.h"
#include "types.h"


class IImpulse : public IEntity
{
     protected :
	uint m_Duration;

     public :
	IImpulse( const uint Duration );
	virtual ~IImpulse();

	const uint & Duration() const;
	virtual void Play() const = 0;
	virtual EEntityType Type() const = 0;
};

#endif //__IMPULSE_H__
