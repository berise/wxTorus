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

	$Id: roller.cpp,v 1.1.1.1 2003/05/16 01:01:06 berise Exp $

*/
#include <stdlib.h>		//	rand()
#include <math.h>
//#include <assert.h>


#include "global.h"
//#include "torus.h"
#include "roller.h"



TRoller::TRoller()
	:	m_Level( 3 )		//	basic level
{
	initTorus();
}



TRoller::~TRoller()
{	
}


//	토러스의 위치를 초기화.
void TRoller::initTorus()
{
	wrapTorus();
}

//	토러스를 다시 감는다? 말은 이상하지만, 다시 돌리기 위해 위쪽으로 올린다는 뜻
//	위치 초기화와 동일
void TRoller::wrapTorus()
{
	m_TorusYPoint		= -30.0;
	m_TorusColor		= rand() % MAX_TORUS;
	
	m_TorusImageIndex	= (MAX_IMAGE_COUNT/2);	//	flat torus first.
}


//
//	
void TRoller::setLevel( const int Level )
{
	m_Level = Level; 
}


int TRoller::getLevel() const
{
	return m_Level;
}




//	높이 설정
void TRoller::setHeight( const int nHeight )
{
	m_RollerHeight = nHeight;
}

int TRoller::getHeight()
{
	return m_RollerHeight;
}




int TRoller::getTorusColor()
{
	//return theTorus.getColor();
	return m_TorusColor;
}

double	TRoller::getTorusYPoint()
{
	//return theTorus.getY();
	return m_TorusYPoint;
}


int TRoller::getImageIndex()
{
	return m_TorusImageIndex;
}



void TRoller::roll( int nDifficulty )
{
	//	Y point 굴림
	//m_TorusYPoint	=	getTorusYPoint() + ( 6 - getTorusColor() );
#ifdef __BORLANDC__
	m_TorusYPoint += (nDifficulty >> 2 ) + sqrt( powl( (float)6-getTorusColor(), 2.0 ) );
#else
	m_TorusYPoint += (nDifficulty >> 2 ) + sqrt( powf( (float)6-getTorusColor(), 2.0 ) );
#endif
	
	//	이미지 굴림
	if( m_TorusImageIndex < MAX_IMAGE_COUNT-1 )
	//if( m_TorusImageIndex < 9-1 )
	{
		m_TorusImageIndex++;	//reroll
	}
	else
	{
		m_TorusImageIndex = 0;
	} 
}
