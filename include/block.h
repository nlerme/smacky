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

#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <vector>
#include "smart_ptr.h"
#include "entity.h"
#include "impulse.h"


class CBlock : public IEntity
{
     friend class DefaultSPStorage<CBlock>;

     private :
	ushort m_Loops;
	uint m_Tempo, m_NbNotes, m_NbPauses, m_NbBlocks;
	std::vector<CSmartPtr<IEntity> > m_Entities;

	typedef std::vector<CSmartPtr<IEntity> > TEntities;
	typedef TEntities::iterator              TEntitiesIt;
	typedef TEntities::const_iterator        TEntitiesCIt;

	CBlock( const ushort Loops, const uint Tempo );

     public :
	static CSmartPtr<CBlock> Create( const ushort Loops, const uint Tempo )
	{
		return new CBlock(Loops, Tempo);
	}

	virtual ~CBlock();

	static ushort DefaultLoops()
	{
		return 1;
	}

	virtual void AddEntity( const CSmartPtr<IEntity> & Entity );
	virtual void Purge();
	virtual void Play() const;
	virtual EEntityType Type() const;

	const ushort & Loops() const;
	void Loops( const ushort Loops );
	const uint & Tempo() const;
	void Tempo( const uint Tempo );
	const TEntities & Entities() const;
	const uint NbNotes() const;
	const uint NbPauses() const;
	const uint & NbBlocks() const;
};

#endif //__BLOCK_H__
