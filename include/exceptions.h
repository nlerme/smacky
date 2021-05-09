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

#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <exception>
#include <string>
#include <sstream>
#include "types.h"

using std::endl;


//------------------------------------- General exception -------------------------------------
class CException : public std::exception
{
     protected :
        std::string m_Message;

     public :
        CException( const std::string & Message = "" );
        virtual ~CException() throw();
        virtual const char * what() const throw();
};


//------------------------------------ Assertion exception ------------------------------------
struct CAssertException : public CException
{
	CAssertException( const std::string & File, uint Line, const std::string & Message );
};

#define Assert(condition) if (!(condition)) throw CAssertException(__FILE__, __LINE__, "Condition non satisfaite\n\n" #condition)


//---------------------------------- Out of memory exception ----------------------------------
struct COutOfMemory : public CException
{
	COutOfMemory( const std::string & Message );
};

#endif //__EXCEPTION_H__
