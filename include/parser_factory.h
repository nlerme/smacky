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

#ifndef __PARSER_FACTORY_H__
#define __PARSER_FACTORY_H__

#include <string>
#include "parser.h"
#include "parser_xml.h"
#include "file.h"


class CParserFactory
{
     public :
	static CSmartPtr<IParser> Create( const std::string & FileName )
	{
		std::string Extension;
		Extension.assign(CFile(FileName).Extension());

		if( Extension.compare("xml") == 0 )
			return CParserXML::Create(FileName);
		else
			return NULL;
	}
};

#endif //__PARSER_FACTORY_H__
