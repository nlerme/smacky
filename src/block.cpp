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
#include "block.h"


CBlock::CBlock( const ushort Loops, const uint Tempo ) : m_Loops(Loops), m_Tempo(Tempo), m_NbNotes(0), m_NbPauses(0), m_NbBlocks(0){}

CBlock::~CBlock(){}

void CBlock::AddEntity( const CSmartPtr<IEntity> & Entity )
{
	Assert(Entity);
	m_Entities.push_back(Entity);

	if( Entity->Type() == ET_NOTE )
		m_NbNotes++;
	else if( Entity->Type() == ET_PAUSE )
		m_NbPauses++;
	else
	{
		m_NbNotes  += static_cast<CBlock*>(GetImpl(Entity))->NbNotes();
		m_NbPauses += static_cast<CBlock*>(GetImpl(Entity))->NbPauses();

		m_NbBlocks++;
	}
}

void CBlock::Purge()
{
	TEntitiesIt It(m_Entities.begin());


	while( It < m_Entities.end() )
		m_Entities.erase(It++);

	m_NbNotes  = 0;
	m_NbPauses = 0;
	m_NbBlocks = 0;
}

void CBlock::Play() const
{
	ushort Count(0);


	while( Count < m_Loops )
	{
		TEntitiesCIt It(m_Entities.begin());

		while( It < m_Entities.end() )
		{
			(*It)->Play();
			It++;
		}

		Count++;
	}
}

EEntityType CBlock::Type() const
{
	return ET_BLOCK;
}

const ushort & CBlock::Loops() const
{
	return m_Loops;
}

void CBlock::Loops( const ushort Loops )
{
	m_Loops = Loops;
}

const uint & CBlock::Tempo() const
{
	return m_Tempo;
}

void CBlock::Tempo( const uint Tempo )
{
	m_Tempo = Tempo;
}

const CBlock::TEntities & CBlock::Entities() const
{
	return m_Entities;
}

const uint CBlock::NbNotes() const
{
	return (m_Loops * m_NbNotes);
}

const uint CBlock::NbPauses() const
{
	return (m_Loops * m_NbPauses);
}

const uint & CBlock::NbBlocks() const
{
	return m_NbBlocks;
}
