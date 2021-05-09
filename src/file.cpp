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

#include "file.h"


CFile::CFile( const std::string & Name, std::ios_base::openmode Mode ) : std::fstream(Name.c_str(), Mode), m_Name(Name) {}

const std::string & CFile::FullName() const
{
	return m_Name;
}

const std::string CFile::FileName() const
{
	std::string::size_type Pos(m_Name.find_last_of("\\/"));
	return (Pos != std::string::npos) ? m_Name.substr(Pos + 1) : m_Name;
}

const std::string CFile::ShortFileName() const
{
	std::string FN(FileName());
	return FN.substr(0, FN.find_last_of("."));
}

const std::string CFile::Extension() const
{
	std::string::size_type Pos = m_Name.find_last_of(".");
	return (Pos != std::string::npos) ? m_Name.substr(Pos + 1) : "";
}
