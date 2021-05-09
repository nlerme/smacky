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

#ifndef __PARSER_H__
#define __PARSER_H__

#include <map>
#include <string>
#include "score.h"
#include "note.h"


class IParser
{
     protected :
	std::string m_FileName;
	std::map<const std::string, ENote> m_Conversions;

     public :
	IParser( const std::string & FileName );
	virtual ~IParser();
	virtual CSmartPtr<CScore> Parse() = 0;
};

#endif //__PARSER_H__
