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

	$Id: pipe.h,v 1.1.1.1 2003/05/16 01:01:06 berise Exp $

*/
#if !defined( PIPE_H )
#define PIPE_H

/*
 *	some deque implementation got a warning line.
 *	for example, linux 6.x series gets a warning line 
 *	while comparing signed with unsigned.
 */
#pragma hdrstop
#include <deque>

using namespace std;

#include "type.h"
#include "global.h"

//#include "torus.h"

//DECLARE_CLASS( TTorus );
__CORE_DECLARE_CLASS( TPipe );


//DECLARE_LIST( TTorus, TorusDeque );

DECLARE_DEQUE( int, TorusDeque );



/*
 *	A TPipe manages each single pipe line in the TPipeSet
 *
 *	It assumes that top of the pipe is a front and bottom is a back.
 *
 *	Any changes( push, pop ) in the pipe will emit a checkRow signal
 *	which connected to the TPipeSet.
 */
class TPipe
{	

	public:
		TPipe( );
		~TPipe();

	public:
		int getLevel();
		void setLevel( int nLevel );
		int m_Level;
		/*
		 *	push Torus into the TPipe
		 *
		 *	This function emits checkRow() signal
		 */
		virtual void push_front( int aTorusColor );
		virtual void push_back( int aTorusColor );
	
		/*
		 *	pop torus from the pipe.
		 *
		 *	This function emits checkRow() signal
		 */
		virtual void pop_front();
		virtual void pop_back();


		//	See what is at the edge of the pipe.
		int front() const;
		int back() const;

		/*
		 *	get color in Row_th in the pipe
		 *	For checking routine in pipeset
		 */
		int getTorusColor( const unsigned int Row );
		

		//	clear row[th] torus in the pipe
		void clearRow( const int Row );

		void clearPipe( ClearMethod aClearMethod );

		void isAllClear();

		//	clear toruses left in the pin.
		void clear();
		void initialize();


		bool isEmpty();
		bool isFull();


		//	hardeness.
		void setBlockSize( int nBlockSize );
		int getBlockSize();
		
		//	get deque size
		unsigned int getSize();

		void debug();


		//	3.24
		//void drawTorus( QPainter &aPainterRef, int aTorusColor, int yPos );
		/*

	signals:
		void checkRow( ClearMethod );


	public slots:
		void slotHitTime() ;

	protected:
		void resizeEvent( QResizeEvent * );
		void paintEvent( QPaintEvent * );

  */

	
	protected:
		TorusDequePtr				m_TorusDequePtr; 
		TorusDeque::size_type		m_MaxSize;
	

		int				m_MaxPipe;

		int				m_BlockSize;	//	 파이프 위쪽에 위치한 블럭 (difficulty)

		/*
		 * 	HitTimer shows a hit line(matched torus line.
		 * 	It allows use to confirm which line matched. (configurable)
		 */
		//QTimer			m_HitTimer;
};


#endif
