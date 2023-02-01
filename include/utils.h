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

#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <sstream>
#include "smart_ptr.h"
#include "types.h"


//----- String conversions -----
template <typename T> std::string ToString( const T & Value )
{
	std::ostringstream Oss;
	Oss << Value;
	return Oss.str();
}

template <typename T> bool FromString( const std::string & String, T & Destination )
{
	std::istringstream Iss(String);
	return bool(Iss >> Destination) != 0;
}

// Swap between two variables
template <typename T> void Swap( T & A, T & B )
{
	T C(A);
	A = B;
	B = C;
}

#endif //__UTILS_H__
