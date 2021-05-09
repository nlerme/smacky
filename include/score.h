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

#ifndef __TAB_H__
#define __TAB_H__

#include <string>
#include <vector>
#include "types.h"
#include "block.h"
#include "entity.h"


class CScore
{
     friend class CSmartPtr<CScore>;

     private :
	std::string m_Name;
	uint m_Tempo, m_NbBlocks, m_NbNotes, m_NbPauses;
	std::vector<CSmartPtr<IEntity> > m_Entities;

	typedef std::vector<CSmartPtr<IEntity> > TEntities;
	typedef TEntities::iterator              TEntitiesIt;
	typedef TEntities::const_iterator        TEntitiesCIt;

	CScore( const std::string & Name, const uint Tempo );

     public :
	static CSmartPtr<CScore> Create( const std::string & Name, const uint Tempo )
	{
		return new CScore(Name, Tempo);
	}

	virtual ~CScore();

	static std::string DefaultName()
	{
		return "My music score";
	}

	static uint DefaultTempo()
	{
		return 500;
	}

	const std::string & Name() const;
	const uint & Tempo() const;
	void AddEntity( const CSmartPtr<IEntity> & Entity );
	void Purge();
	void Play() const;
};

#endif //__TAB_H__
