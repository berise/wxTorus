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

	$Id: pin.cpp,v 1.1.1.1 2003/05/16 01:01:06 berise Exp $

*/
#include <assert.h>

#include "global.h"	//	 T_W, T_H
#include "pin.h"


int PIN_SUR = 2;	// sur

TPin::TPin()
	:	m_BlockSize( 1 ),
		m_PinPos( 0 ),
		m_Level( 3 )	//	starting level
{
	m_TorusVectorPtr  = new TorusVector;

//	m_nCapacity = MAX_PIN_SIZE 
	m_nCapacity = m_Level + PIN_SUR - m_BlockSize;
}

TPin::~TPin()
{
	//
	//	 clear toruses left in the pin.
	clear();
	zap( m_TorusVectorPtr  );
}



void TPin::push( int aTorusColor )
{
	assert( isFull() != true );
	m_TorusVectorPtr ->push_back( aTorusColor ); 

	//repaint( false );
}

void TPin::pop()
{
	assert( isEmpty() != true );
	m_TorusVectorPtr ->pop_back();

	//repaint( false );
}

int TPin::top()
{
	assert( isEmpty() != true );

	return m_TorusVectorPtr ->back();
}


bool TPin::isEmpty()
{
	return m_TorusVectorPtr ->empty();
}

bool TPin::isFull()
{
//	not only == but >=, Fixed 4.26
	return ( m_TorusVectorPtr ->size() >= m_nCapacity );
}

int TPin::getSize() const
{
	return m_TorusVectorPtr->size();

}

/*
 * 	obsolete
 */
void TPin::setSize( const int /* PinSize */ )
{
//	m_MaxPin = PinSize;
}

void TPin::setBlockSize( int nBlockSize)
{
//	assert needs
	m_BlockSize = nBlockSize; 
	
	//	update available pin size. 	
	m_nCapacity = m_Level + PIN_SUR - m_BlockSize;

}

void TPin::debug()
{ 
//	qDebug("TPin::pin size %d, pos %d, limit %d", m_TorusVectorPtr ->size(), m_PinPos, m_nCapacity );
}

//
int TPin::getPinPos() const
{
	return m_PinPos;
}

void TPin::moveLeft()
{
	m_PinPos--; 
	//repaint( false );
}

void TPin::moveRight()
{
	m_PinPos++; 
	//repaint( false );
}

void TPin::setLevel( const int Level )
{
	m_Level = Level;

	//	레벨은 핀에 넣을 수 있는 토러스의 개수와 연관이 있다.
	m_nCapacity = m_Level + PIN_SUR - m_BlockSize;
}

int TPin::getLevel() const
{
	return m_Level;
}


//	init으로 setPinPos를 없앨 수 있다.
void TPin::init( int nLevel )
{
	m_Level = nLevel;

	//	레벨은 핀에 넣을 수 있는 토러스의 개수와 연관이 있다.
	m_nCapacity = m_Level + PIN_SUR - m_BlockSize;


	m_PinPos = 0;	//	reset pin pos.  fixed 01.04.25
	clear();
}

void TPin::clear()
{
	m_TorusVectorPtr->clear();
}



void TPin::append()
{
	m_Level++;
}


void TPin::remove()
{
	if( m_Level > 3 )
	{
		m_Level--;
	}
}

int TPin::getBlockSize()
{
	return m_BlockSize;

}

int TPin::getTorusColor( int nRow )
{
	return m_TorusVectorPtr->at( nRow );
}


void TPin::setCapacity( const int Level )
{
	//	레벨은 핀에 넣을 수 있는 토러스의 개수
	m_nCapacity = Level + PIN_SUR - getBlockSize();
}

int TPin::getCapacity() const
{
	return m_nCapacity;
}

