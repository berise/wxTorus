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

	$Id: torus.h,v 1.1.1.1 2003/05/16 01:01:06 berise Exp $

*/
#ifndef TTORUS_H
#define TTORUS_H

#include "type.h"



__CORE_DECLARE_CLASS( TTorus );

/*! \class TTorus
 *  \brief torus is virtually generated in TRoller and then send to the pipe
 *  
 *  TTRorus is self drawable class(it was). But now TTorus only
 *  work for internal variables such as xy position, color and acceleration.
 */
class TTorus
{
	public:
			TTorus( int row, int color );
			TTorus( TTorusCref aTorusRef );
			~TTorus();

	public:
		int getImageIndex();
		void setY( int nY );

		void setColor( const int color );
		int getColor() const;

		
		int getX();
		int getY();
	//	QPoint & getPoint();

		/*
		 *	draw torus with m_Color at the specified pos.
		 *	non qobject draw..
		 */
		//virtual void draw( QPainter &PainterRef, int xPos = 0, int yPos = 0 );
		void setCenterTorus();


		/*
		 * 	roll() increase ImageIndex indicates an internal png number.
		 * 	This makes an image animate.
		 * 	And this function automatically rewind when it reaches to an
		 * 	end.
		 */
		void roll();

		void	move();


		TTorusRef operator=( TTorusCref aTorusRef );

	private:			
			int m_Color;    //       torus color
			int m_Accel;    //       torus acceleration

			int m_Y;

			int m_ImageIndex;
};


#endif
