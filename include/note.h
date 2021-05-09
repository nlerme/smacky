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

#ifndef __NOTE_H__
#define __NOTE_H__

#include "smart_ptr.h"
#include "impulse.h"


enum ENote
{
	NOTE_C       = 0,
	NOTE_C_SHARP = 1,
	NOTE_D       = 2,
	NOTE_D_SHARP = 3,
	NOTE_E       = 4,
	NOTE_F       = 5,
	NOTE_F_SHARP = 6,
	NOTE_G       = 7,
	NOTE_G_SHARP = 8,
	NOTE_A       = 9,
	NOTE_A_SHARP = 10,
	NOTE_B       = 11
};

class CNote : public IImpulse
{
     friend class CSmartPtr<CNote>;

     private :
	ushort m_Volume, m_Pitch;

	CNote( const ushort Volume, const ENote Note, const ushort Octave, const uint Duration );

     public :
	static CSmartPtr<IImpulse> Create( const ushort Volume, const ENote Note, const ushort Octave, const uint Duration )
	{
		return new CNote(Volume, Note, Octave, Duration);
	}

	virtual ~CNote();

	static float DefaultTempoFactor()
	{
		return 1.0f;
	}

	static ushort DefaultVolume()
	{
		return 100;
	}

	const ushort & Volume() const;
	const ushort & Pitch() const;
	virtual void Play() const;
	virtual EEntityType Type() const;
};

#endif //__NOTE_H__
