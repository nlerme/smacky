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

#include "exceptions.h"
#include <sstream>

using std::endl;


//------------------------------------- General exception -------------------------------------
CException::CException( const std::string & Message ) : m_Message(Message){}

CException::~CException() throw() {}

const char * CException::what() const throw()
{
	return m_Message.c_str();
}


//------------------------------------ Assertion exception ------------------------------------
CAssertException::CAssertException( const std::string & File, uint Line, const std::string & Message )
{
	std::ostringstream oss;
	oss << "[ Assertion ] File `" << File << "` (" << Line << ") - " << Message << endl;
	m_Message = oss.str();
}


//----------------------------------- OutOfMemory exception -----------------------------------
COutOfMemory::COutOfMemory( const std::string & Message )
{
	std::ostringstream oss;
	oss << "[ Out Of Memory ] " << Message << endl;
	m_Message = oss.str();
}
