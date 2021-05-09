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

#include "tokenizer.h"


//----------------------------------------- CSpace class -----------------------------------------
bool CSpace::operator () ( char C ) const
{
	return (C == ' ');
}


//---------------------------------------- CNewLine class ----------------------------------------
bool CNewLine::operator () ( char C ) const
{
	return (C == '\n');
}


//----------------------------------------- CComma class -----------------------------------------
bool CComma::operator () ( char C ) const
{
	return (C == ',');
}


//--------------------------------------- CSeparator class ---------------------------------------
CSeparator::CSeparator( const std::string & String ) : m_String(String){}

bool CSeparator::operator () ( char C ) const
{
	return (m_String.find(C) != std::string::npos);
}
