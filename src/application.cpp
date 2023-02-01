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

#include <string>
#include <iostream>
#include "argstream.h"
#include "parser.h"
#include "score.h"
#include "smart_ptr.h"
#include "parser_factory.h"
#include "config.h"
#include "application.h"

using std::cerr;


CApplication::CApplication(){}

CApplication::~CApplication(){}

void CApplication::Run( int argc, char ** argv )
{
	std::string FileName;
	CSmartPtr<CScore> Score;

	argstream As(argc, argv);
	As >> parameter('f', "file", FileName, "Path to the score")
	   >> option('v', "verbose", CConfig::Instance()->Verbose, "Activate verbose mode")
	   >> help('h', "help", "Display this help");
	As.defaultErrorHandling();

	CSmartPtr<IParser> Parser(CParserFactory::Create(FileName));

	if( !Parser )
	{
		cerr << "error: file format currently not supported.\n";
		return;
	}

	if( !(Score = Parser->Parse()) )
	{
		cerr << "error: unable to parse the score.\n";
		return;
	}

	// We play the music !
	Score->Play();
}
