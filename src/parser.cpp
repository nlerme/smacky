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

#include "parser.h"


IParser::IParser( const std::string & FileName ) : m_FileName(FileName)
{
	m_Conversions["C"]  = NOTE_C;
	m_Conversions["C#"] = NOTE_C_SHARP;
	m_Conversions["D"]  = NOTE_D;
	m_Conversions["D#"] = NOTE_D_SHARP;
	m_Conversions["E"]  = NOTE_E;
	m_Conversions["F"]  = NOTE_F;
	m_Conversions["F#"] = NOTE_F_SHARP;
	m_Conversions["G"]  = NOTE_G;
	m_Conversions["G#"] = NOTE_G_SHARP;
	m_Conversions["A"]  = NOTE_A;
	m_Conversions["A#"] = NOTE_A_SHARP;
	m_Conversions["B"]  = NOTE_B;
}

IParser::~IParser(){}
