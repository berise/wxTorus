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

	$Id: pipe.cpp,v 1.1.1.1 2003/05/16 01:01:06 berise Exp $

*/
#include <assert.h>

#include "global.h"
#include "pipe.h"


//	이건 뭐지??
int PIPE_SUR=5;

TPipe::TPipe()
	:	m_Level( 3 ),
		m_BlockSize( 1 ),
		m_MaxSize( 7 )	//	 initial max size	
{
	m_TorusDequePtr = new TorusDeque;	
}

TPipe::~TPipe()
{
	//
	//	clear toruses left in the pipe.
	clear();

	zap( m_TorusDequePtr ); 
}


/*
 * 	push_front 호출 전에 overflow의 검사가 필요
 */
void TPipe::push_front( int aTorusColor )
{
	assert( isFull() != true );

	m_TorusDequePtr->push_front( aTorusColor );

	//repaint( false );

	//emit checkRow( TORUS_CLEAR_ROW_BY_PIN );
}

/*
 * 	호출 전에 overflow의 검사가 필요
 */
void TPipe::push_back( int aTorusColor )
{
	assert( isFull() != true );

	m_TorusDequePtr->push_back( aTorusColor );

	//repaint( false );

	//emit checkRow( TORUS_CLEAR_ROW_BY_ROLLER );
}



void TPipe::pop_front()
{
	assert( m_TorusDequePtr->empty() != true );

	/*
	 * 	pop it
	 */
	m_TorusDequePtr->pop_front(); 

	//repaint( false );

	///emit checkRow( TORUS_CLEAR_ROW_BY_PIN );
}


/*
 * 	unknown operation yet.
 */
void TPipe::pop_back()
{
	assert( m_TorusDequePtr->empty() != true );

	/*
	 * 	pop
	 */
	m_TorusDequePtr->pop_back();


	//repaint( false );

	//emit checkRow( TORUS_CLEAR_ROW_BY_ROLLER );
}

/*
 * 	front & back의 int 버전
 */
int TPipe::front() const
{
	return m_TorusDequePtr->front();
}


int TPipe::back() const
{
	return m_TorusDequePtr->back(); 
}


bool TPipe::isEmpty()
{
	return m_TorusDequePtr->empty();
} 

bool TPipe::isFull()
{
	//	
	//	size - guard space, fixed with pin.cpp 4.26
	return ( m_TorusDequePtr->size() >=  m_MaxSize );
}

/*
 * trick level set
 */
void TPipe::setBlockSize( int nBlockSize )
{
	m_MaxSize	= getLevel() + PIPE_SUR - nBlockSize;
	m_BlockSize = nBlockSize;

	/*
	 * ### this forces repaint.  
	 */
	//repaint( false );
}

int TPipe::getBlockSize()
{
	return m_BlockSize;
}



int TPipe::getTorusColor( const unsigned int Row )
{
//	qDebug("name : %s, list size : %d, Row : %d", name(), m_TorusDequePtr->size(), Row );
//	assert( m_TorusDequePtr->size() > (unsigned int) 0 );
//	assert( m_TorusDequePtr->size() >= (unsigned int) Row );

	return m_TorusDequePtr->at( Row );

	/*
	TorusDeque::iterator liter = m_TorusDequePtr->begin();

	for( unsigned int i = 0; i < Row; i++ )
	{
		liter++;
	} 

	return (*liter);	//	 errorneous line.  

  */
}



/*
void TPipe::initialize()
{
	clear();
}
*/

void TPipe::clear()
{
	m_TorusDequePtr->clear();

	/*
	 * 	repaint after clear causes segmentation fault.
	 */
//	repaint( false );
}


unsigned int TPipe::getSize()
{
	return m_TorusDequePtr->size();
}


//
//	Row must be in [0, size)
//	파이트의 Row 번째 토러스를 지운다.
//	TorusDeque에서 Row 번째의 항목을 삭제하면 된다.
//	TorusDeque.erase( TorusDeque.begin() + Row );
void TPipe::clearRow( const int Row )
{

	/*
	 * Erase TTorus object, internal part
	 */ 
	TorusDeque::iterator liter = m_TorusDequePtr->begin(); 
	for( int i = 0; i < Row; i++ )
	{
		liter++; 	
	} 
	
	//	#####
	m_TorusDequePtr->erase( liter);
}

//	level은 이 객체가 담을 수 있는 최대의 용량을 조절한다. 
void TPipe::setLevel( int nLevel )
{
	m_Level = nLevel;

	//	파이프의 레벨과 최대 토러스 개수역시 연관이 있다.
	//	레벨이 오름에 따라 최대 값이 늘어난다.
	m_MaxSize	= nLevel + PIPE_SUR - getBlockSize();
}

int TPipe::getLevel()
{
	return m_Level;
}
