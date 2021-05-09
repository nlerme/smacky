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

#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include <functional>
#include <string>
#include <algorithm>
#include <vector>


class CSpace : public std::unary_function<char, bool>
{
     public :
	bool operator () ( char C ) const;
};

class CNewLine : public std::unary_function<char, bool>
{
     public :
	bool operator () ( char C ) const;
};

class CComma : public std::unary_function<char, bool>
{
     public :
	bool operator () ( char C ) const;
};

class CSeparator : public std::unary_function<char, bool>
{
     private :
	std::string m_String;

     public :
	CSeparator( const std::string & String );
	bool operator () ( char C ) const;
};

template <typename Predicate> class CTokenizer
{
     public :
	static std::vector<std::string> Tokenize( const std::string & String, const Predicate & P = Predicate() )
	{
		std::vector<std::string> Tokens;
		std::string::const_iterator It(String.begin());
		std::string::const_iterator ItTokenEnd(String.begin());


		while( It != String.end() )
		{
			while( P(*It) )
				It++;

			ItTokenEnd = find_if(It, String.end(), P);

			if( It < ItTokenEnd )
				Tokens.push_back(std::string(It, ItTokenEnd));

			It = ItTokenEnd;
		}

		return Tokens;
	}
};

// Practical aliases
typedef CTokenizer<CSpace> CSpaceTokenizer;
typedef CTokenizer<CComma> CCommaTokenizer;
typedef CTokenizer<CNewLine> CNewLineTokenizer;

#endif //__TOKENIZER_H__
