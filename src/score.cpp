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
#include <stack>
#include "exceptions.h"
#include "config.h"
#include "score.h"

using std::cout;


CScore::CScore( const std::string & Name, const uint Tempo ) : m_Name(Name), m_Tempo(Tempo), m_NbBlocks(0), m_NbNotes(0), m_NbPauses(0){}

CScore::~CScore(){}

const std::string & CScore::Name() const
{
	return m_Name;
}

const uint & CScore::Tempo() const
{
	return m_Tempo;
}

void CScore::AddEntity( const CSmartPtr<IEntity> & Entity )
{
	Assert(Entity);
	m_Entities.push_back(Entity);
}

void CScore::Purge()
{
	TEntitiesIt It(m_Entities.begin());


	while( It < m_Entities.end() )
		m_Entities.erase(It++);
}

void CScore::Play() const
{
	TEntitiesCIt It(m_Entities.begin());


	if( CConfig::Instance()->Verbose )
	{
		std::string Message(" Score name: ");
		int NbSpaces(80);

		cout << '+' << std::string(NbSpaces, '-') << "+\n"
		     << '|' << std::string(NbSpaces, ' ') << "|\n"
		     << '|' << Message << m_Name << std::string(NbSpaces-m_Name.size()-Message.size(), ' ') << "|\n"
		     << '|' << std::string(NbSpaces, ' ') << "|\n"
		     << '+' << std::string(NbSpaces, '-') << "+\n\n";

		cout << "Total number of blocks -> " << static_cast<CBlock*>(GetImpl(*It))->NbBlocks() << '\n'
		     << "Total number of notes  -> " << static_cast<CBlock*>(GetImpl(*It))->NbNotes() << '\n'
		     << "Total number of pauses -> " << static_cast<CBlock*>(GetImpl(*It))->NbPauses() << "\n\n";
	}

	while( It < m_Entities.end() )
	{
		(*It)->Play();
		It++;
	}
}
