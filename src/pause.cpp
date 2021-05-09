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

#include <iostream>
#include <sstream>
#include <cstdlib>
#include "utils.h"
#include "config.h"
#include "pause.h"

using std::cout;


CPause::CPause( const ushort Duration ) : IImpulse(Duration){}

CPause::~CPause(){}

void CPause::Play() const
{
	#ifndef __WIN32__
		std::ostringstream oss;
		oss << "beep -f 1 -l " << ToString(m_Duration);

		if( CConfig::Instance()->Verbose )
			cout << "Pause -> (Duration=" << m_Duration << "ms)\n";

		int dummy = system(oss.str().c_str());
		dummy++;
	#else
		Beep(m_Pitch, m_Duration);
	#endif
}

EEntityType CPause::Type() const
{
	return ET_PAUSE;
}
