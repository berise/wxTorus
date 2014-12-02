/*
	Torus for Linux (Windows)

	Copyright (C) 2002 JaeSung Lee (berise@dreamwiz.com)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.	

	$Id: scorefile.cpp,v 1.1.1.1 2003/05/16 01:01:06 berise Exp $
*/

#include <algorithm>
#include <functional>

//#include <string.h>
//#include <stdio.h>


#include "scorefile.h"
//#include "torusfunctors.h"


#include "wx/file.h"
//#include "wx"

static const char * TORUS_SCORE_FILE = "highscore";

//	Max count of Torus Top Players
const int TORUS_MAX_TOP_PLAYERS	=	10;




//
//	scorefile functors.
//	helper functors such as Minimum score, 
//	binary_function의 템플릿 인자에는 const 를 쓰지 않는다. Effective STL p258-259
//template <class T>
struct MinScore: public binary_function< ScoreData, ScoreData, bool >
//struct MinScore: public binary_function< T, T, bool >
{ 
	bool	operator()( const ScoreData & x, const ScoreData & y ) const
	//bool	operator()( const T & x, const T& y ) const
	{
		return ( x.getScore() < y.getScore() );
	} 
};


struct MaxScore: public binary_function< ScoreData, ScoreData, bool >
{
	bool	operator()( ScoreDataCref x, ScoreDataCref y ) const
	{
		return ( x.getScore() > y.getScore() );
	} 
};






TScoreFile::TScoreFile( )
:m_bDirty( false )
{
	bool is_exist = false;

	is_exist = wxFile::Exists( TORUS_SCORE_FILE );

	if( is_exist == true )
	{
		//	파일을 읽고 난 후, 리스트 뷰에 출력을 위해서
		//	최대값 정렬이 필요하다.(이게 더 편함)
		read();		
	}
	else
	{
		initialize();
		write();		
	}
}

TScoreFile::~TScoreFile()
{
//	if( m_bDirty == true )
//	{
//		write();
//	}
	theScoreVector.clear();
}



//	TORUS_SCORE_FILE 파일이 없을 경우
//	기본 점수로 모두 채운다.
void	TScoreFile::initialize()
{
	/*
	 * 	predefined top 10 players
	 */
	theScoreVector.clear();

	push_back( "The Wizard!",		500000 );
	push_back( "Torus Guru",		400000 );
	push_back( "Torus Fanatic",		300000 );
	push_back( "Professional",		240000 );
	push_back( "Torus Master",		180000 );

	push_back( "Discipline",		140000 );
	push_back( "Apprentice",		80000 );
	push_back( "Intermediate",		40000 );
	push_back( "Beginner",			20000 );
	push_back( "Novice",			10000 );
}




void TScoreFile::push_back( const char *pName, int nScore )
{
	
	theScoreVector.push_back( ScoreData( pName, nScore )  );

	//	최소값을 삭제하기 위해
	//	erase 함수와 더불어 사용하면 편함
	sort( theScoreVector.begin(), theScoreVector.end(), MaxScore() );
	m_bDirty = true;
}


int TScoreFile::getScore( int Number )
{
	return theScoreVector[ Number ].getScore();
}

void TScoreFile::getPlayer( int aNumber, char *aPlayerPtr )
{
	strcpy( aPlayerPtr, theScoreVector[ aNumber ].getName() );
}

void TScoreFile::write()
{
	wxFile f;

	if( f.Open(TORUS_SCORE_FILE, wxFile::write) == true )
	{
		int j = 0; 

		for( int i = 0; i < TORUS_MAX_TOP_PLAYERS; i++ )
		{
			f.Write( ( void *) &(theScoreVector[i]), sizeof( ScoreData ) );
			j++;
		}	

		f.Close();

		theScoreVector.clear();
	}
}

void TScoreFile::read()
{
	wxFile f;

	if( f.Open(TORUS_SCORE_FILE, wxFile::read) == true )
	{
		ScoreData sd;

		while( f.Eof() != true )
		{		
			f.Read( (void *) &sd, sizeof( ScoreData ) );
			push_back( sd.getName(), sd.getScore() );
		}

		int bb = theScoreVector.size() ;

		f.Close(); 
	}
}

int TScoreFile::getMinScore()
{	
	ScoreVector::iterator	iter = min_element( theScoreVector.begin(), 
												theScoreVector.end(), 
												//not2(MinScore<struct ScoreData>()) );
												MinScore() );
	return (*iter).getScore();
}
