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

#ifndef __PARSER_XML_H__
#define __PARSER_XML_H__

#include <string>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include "score.h"
#include "block.h"
#include "smart_ptr.h"
#include "parser.h"


class CParserXML : public IParser
{
     friend class CSmartPtr<CParserXML>;

     private :
	std::string _Attribute( xmlNodePtr Node, const char *Name );
	void _ParseNote( xmlNodePtr Node, CSmartPtr<IEntity> & Entity );
	void _ParsePause( xmlNodePtr Node, CSmartPtr<IEntity> & Entity );
	void _ParseBlock( xmlNodePtr Node, CSmartPtr<IEntity> & Entity, CSmartPtr<CScore> & Score );

	CParserXML( const std::string & FileName );
	virtual ~CParserXML();

     public :
	static CSmartPtr<IParser> Create( const std::string & FileName )
	{
		return new CParserXML(FileName);
	}

	virtual CSmartPtr<CScore> Parse();
};

#endif //__PARSER_XML_H__
