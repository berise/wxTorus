/*
	Torus for win32

	Copyright (C) 2002 JaeSung Lee

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

	$Id: global.h,v 1.1.1.1 2003/05/16 01:01:06 berise Exp $

*/
#ifndef _GLOBAL_H
#define _GLOBAL_H

//
//	Red, Green, Blue, Violat
#define MAX_TORUS		4

//#define MAX_PIN_SIZE	7	/*	PinSize + BottomGuard	*/
#define MAX_PIPE_SIZE	9	/*	PipeSize + TopGuard		*/

#define MAX_PIN_BOTTOM_SIZE		4
#define MIN_PIN_BOTTOM_SIZE		1

#define MAX_PIPE_TOP_SIZE		4
#define MIN_PIPE_TOP_SIZE		4

//
//	dimension
#define TORUS_WIDTH			50
#define	TORUS_HEIGHT			10	//	누웠을 때인데...

//
//	obsolete
#define TORUS_WIDTH_ALPHA		50
#define	TORUS_HEIGHT_ALPHA		10


#define MAX_IMAGE_COUNT			17

//
//	Color index, also used for speed factor
//	speed factor = 1 / TORUS_COLOR
#define TORUS_COLOR_RED			0
#define TORUS_COLOR_GREEN		1
#define TORUS_COLOR_BLUE		2
#define TORUS_COLOR_MAGENTA		3
#define TORUS_COLOR_YELLOW		4


#define TORUS_IMAGE_LOGO		500
#define TORUS_IMAGE_PIN			600
#define TORUS_IMAGE_HIT			700
#define TORUS_IMAGE_SIDEBAR		800


//	윈도우 크기 관련.
#define	TORUS_MAX_HEIGHT		600
#define TORUS_MAX_WIDTH			500



/*
 *	clear method for scoring
 */ 
typedef enum {	
	TORUS_CLEAR_ROW_BY_PIN = 0,
	TORUS_CLEAR_ROW_BY_ROLLER }
ClearMethod;



#endif
