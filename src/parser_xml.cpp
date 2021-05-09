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
#include <vector>
#include "note.h"
#include "pause.h"
#include "utils.h"
#include "tokenizer.h"
#include "parser_xml.h"

using std::cout;
using std::cerr;


std::string CParserXML::_Attribute( xmlNodePtr Node, const char *Name )
{
	char *Attribute=NULL;

	return (!(Attribute = (char*)xmlGetProp(Node, BAD_CAST Name)) ? "" : std::string(Attribute));
}

void CParserXML::_ParseNote( xmlNodePtr Node, CSmartPtr<IEntity> & Entity )
{
	std::string ValueStr(_Attribute(Node, "value"));
	std::string DurationStr(_Attribute(Node, "duration"));
	std::string VolumeStr(_Attribute(Node, "volume"));
	ushort Volume(CNote::DefaultVolume());
	float Duration(CNote::DefaultTempoFactor());


	if( ValueStr.empty() )
	{
		cerr << "error: note value is empty.\n";
		return;
	}

	if( !DurationStr.empty() && !FromString(DurationStr, Duration) )
	{
		cerr << "error: wrong format of the duration for the note `" << ValueStr << "'.\n";
		return;
	}

	if( !VolumeStr.empty() && !FromString(VolumeStr, Volume) )
	{
		cerr << "error: wrong format of the volume for the note `" << ValueStr << "'.\n";
		return;
	}

	std::vector<std::string> Chunks(CCommaTokenizer::Tokenize(ValueStr));

	if( Chunks.size() != 2 || m_Conversions.find(Chunks[0]) == m_Conversions.end() )
	{
		cerr << "error: wront format for the value of the note `" << ValueStr << "'.\n";
		return;
	}

	ushort Octave;

	if( !FromString(Chunks[1], Octave) )
	{
		cerr << "error: wrong format for the octave of the note `" << ValueStr << "'.\n";
		return;
	}

	ENote Note(m_Conversions[Chunks[0]]);

	// Here, we get the tempo of the parent node (always a block)
	Entity->AddEntity(CNote::Create(Volume, Note, Octave, uint(Duration * static_cast<CBlock*>(GetImpl(Entity))->Tempo())));
}

void CParserXML::_ParsePause( xmlNodePtr Node, CSmartPtr<IEntity> & Entity )
{
	std::string DurationStr(_Attribute(Node, "duration"));
	float Duration(CPause::DefaultTempoFactor());


	if( !DurationStr.empty() && !FromString(DurationStr, Duration) )
	{
		cerr << "error: wrong format for the duration of a pause.\n";
		return;
	}

	// Here, we get the tempo of the parent node (always a block)
	Entity->AddEntity(CPause::Create(uint(Duration * static_cast<CBlock*>(GetImpl(Entity))->Tempo())));
}

void CParserXML::_ParseBlock( xmlNodePtr Node, CSmartPtr<IEntity> & Entity, CSmartPtr<CScore> & Score )
{
	std::string LoopsStr(_Attribute(Node, "loops"));
	std::string TempoStr(_Attribute(Node, "tempo"));
	ushort Loops(CBlock::DefaultLoops());
	uint Tempo(Score->Tempo()); // Here, we get the tempo of the score


	if( !LoopsStr.empty() && !FromString(LoopsStr, Loops) )
	{
		cerr << "error: wrong format for the number of loops in a block.\n";
		return;
	}

	if( !TempoStr.empty() && !FromString(TempoStr, Tempo) )
	{
		cerr << "error: wrong format for the tempo in a block.\n";
		return;
	}

	CSmartPtr<IEntity> Block(CBlock::Create(Loops, Tempo));
	Node = Node->xmlChildrenNode;

	while( Node )
	{
		if( !xmlStrcmp(Node->name, BAD_CAST "note") )
			_ParseNote(Node, Block);
		else if( !xmlStrcmp(Node->name, BAD_CAST "pause") )
			_ParsePause(Node, Block);
		else if( !xmlStrcmp(Node->name, BAD_CAST "block") )
			_ParseBlock(Node, Block, Score);

		Node = Node->next;
	}

	Entity->AddEntity(Block);
}

CParserXML::CParserXML( const std::string & FileName ) : IParser(FileName){}

CParserXML::~CParserXML(){}

CSmartPtr<CScore> CParserXML::Parse()
{
	CSmartPtr<CScore> Score(NULL);
	CSmartPtr<IEntity> Entity(NULL);
	xmlDocPtr Doc   = NULL;
	xmlNodePtr Node = NULL;


	if( (Doc = xmlParseFile(m_FileName.c_str())) )
	{
		if( (Node = xmlDocGetRootElement(Doc)) && !xmlStrcmp(Node->name, BAD_CAST "score") )
		{
			std::string TempoStr(_Attribute(Node, "tempo"));
			uint Tempo(CScore::DefaultTempo());

			if( !TempoStr.empty() && !FromString(TempoStr, Tempo) )
			{
				cerr << "error: wrong format for the tempo of the score.\n";
				return NULL;
			}

			std::string Name(_Attribute(Node, "name"));

			if( Name.empty() )
				Name = CScore::DefaultName();

			Score  = CScore::Create(Name, Tempo);
			Entity = CBlock::Create(CBlock::DefaultLoops(), Tempo); // Here, we build a fake block as root tree with the tempo of the score (given or not)
			Node   = Node->xmlChildrenNode;

			while( Node )
			{
				if( !xmlStrcmp(Node->name, BAD_CAST "note") )
					_ParseNote(Node, Entity);
				else if( !xmlStrcmp(Node->name, BAD_CAST "pause") )
					_ParsePause(Node, Entity);
				else if( !xmlStrcmp(Node->name, BAD_CAST "block") )
					_ParseBlock(Node, Entity, Score);

				Node = Node->next;
			}

			Score->AddEntity(Entity);
			xmlFreeNode(Node);
		}

		xmlFreeDoc(Doc);
	}

	xmlCleanupParser();
	xmlMemoryDump();

	return Score;
}
