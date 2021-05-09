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

#ifndef __FILE_H__
#define __FILE_H__

#include <string>
#include <iostream>
#include <fstream>
#include "types.h"


class CFile : public std::fstream
{
     private :
	std::string m_Name;

     public :
	CFile( const std::string & Name, std::ios_base::openmode Mode = ios_base::in | ios_base::out );
	const std::string & FullName() const;
	const std::string FileName() const;
	const std::string ShortFileName() const;
	const std::string Extension() const;
};

#endif //__FILE_H__
