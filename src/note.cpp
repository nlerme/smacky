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

#ifdef _WIN32
	#include <windows.h>
#else
	#include <sstream>
#endif
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "config.h"
#include "utils.h"
#include "note.h"

using std::cout;
using std::flush;


CNote::CNote( const ushort Volume, const ENote Note, const ushort Octave, const uint Duration ) : IImpulse(Duration), m_Volume(Volume)
{
	m_Pitch = ushort(16.3516 * std::pow(2.0, double(Octave) + double(Note) / 12.0));
}

CNote::~CNote(){}

const ushort & CNote::Volume() const
{
	return m_Volume;
}

const ushort & CNote::Pitch() const
{
	return m_Pitch;
}

void CNote::Play() const
{
	#ifndef __WIN32__
		std::ostringstream oss;
		oss << "beep -f " << ToString(m_Pitch) << " -l " << ToString(m_Duration);

		if( CConfig::Instance()->Verbose )
			cout << "Note -> (Pitch=" << m_Pitch << ", Duration=" << m_Duration << "ms)\n";

		int dummy = system(oss.str().c_str());
		dummy++;
	#else
		Beep(m_Pitch, m_Duration);
	#endif
}

EEntityType CNote::Type() const
{
	return ET_NOTE;
}
