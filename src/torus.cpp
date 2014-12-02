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

	$Id: torus.cpp,v 1.1.1.1 2003/05/16 01:01:06 berise Exp $

*/
#include <math.h>

//#include <qpainter.h>
//#include <qbrush.h>
//#include <qcolor.h>


#include "global.h"
#include "torus.h"



/*! \class TTorus
 * \brief Default constructor
 */
TTorus::TTorus( int row, int color )
	:	m_Color( color ),
		m_Accel( 3 ),		
		m_Y( 0 ),
		m_ImageIndex(8)	//	flat torus first.
{
}


TTorus::TTorus( TTorusCref aTorusRef )
{
	if( this != &aTorusRef )
	{
		m_Color		= aTorusRef.m_Color;
		m_Accel		= aTorusRef.m_Accel;		
		m_Y			= aTorusRef.m_Y;
		m_ImageIndex= aTorusRef.m_ImageIndex;
	}
}

/*!
 *	Destructor
 */
TTorus::~TTorus()
{

}


TTorusRef TTorus::operator=( TTorusCref aTorusRef )
{	
	if( this != &aTorusRef )
	{		
		m_Color		= aTorusRef.m_Color;
		m_Accel		= aTorusRef.m_Accel;		
		m_Y			= aTorusRef.m_Y;
		m_ImageIndex= aTorusRef.m_ImageIndex;
	} 

	return *this;
}



/*! \fn void setColor( const int color );
 *  \brief set torus color
 *  \param color represented in a integer.
 *  \return nothing
 *	color determines torus´ acceleration
 *
 *	accel = (color+5) * 3 --> [ 15 ~ 30 ]
 *
 *	speed = base factor(15) - accel / 2^^;
 */
void TTorus::setColor( const int color )
{
	m_Color = color;
}

int TTorus::getColor() const
{
	return m_Color;
}


/*
 * 	rolling png image index
 */
void TTorus::roll()
{
	if( m_ImageIndex < MAX_IMAGE_COUNT-1 )
	{
		m_ImageIndex++;	//reroll
	}
	else
	{
		m_ImageIndex = 0;
	} 
}


void TTorus::setCenterTorus()
{
	m_ImageIndex = 8;
}

/*
void TTorus::draw( QPainter & PainterRef, int xPos, int yPos )
{ 
	ResourceCenter	*aRCPtr = &theResourceCenter; 

	QPixmap &aPixmapRef = aRCPtr->getTorusPixmap( m_Color, m_ImageIndex );

	if( &aPixmapRef == NULL )
	{
		qDebug("Fatal error! NULL pixmap found.");
	}

	PainterRef.drawPixmap( xPos, yPos, aPixmapRef, m_ImageIndex * 50, m_Color * 50, 50, 50 ); 
}
*/


int TTorus::getY()
{
	return m_Y;
}




void TTorus::move()
{
	m_Y += m_Color + 1;
}

void TTorus::setY(int nY)
{
	m_Y	=	nY;
}

int TTorus::getImageIndex()
{
	return m_ImageIndex;
}
