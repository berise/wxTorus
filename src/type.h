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

	$Id: type.h,v 1.1.1.1 2003/05/16 01:01:06 berise Exp $

*/
#ifndef _TYPE_H
#define _TYPE_H

#define	DECLARE_TYPE( __decl, __name ) \
		typedef __decl			__name;			\
		typedef __name *		__name##Ptr;	\
		typedef __name &		__name##Ref;	\
		typedef const __name *	__name##Cptr;	\
		typedef const __name &	__name##Cref;


#define	__CORE_DECLARE_CLASS( __class ) \
		class __class;							\
		typedef __class *		__class##Ptr;	\
		typedef __class &		__class##Ref;	\
		typedef const __class *	__class##Cptr;	\
		typedef const __class &	__class##Cref;


/*! \brief No more type mismatch. Underlying library(Qt, vwcl or wxWidget)
 * will do
 */
/*
DECLARE_TYPE( bool, Bool );
DECLARE_TYPE( int, Int );
DECLARE_TYPE( char, Char );
DECLARE_TYPE( unsigned char, Uchar );
DECLARE_TYPE( unsigned int, Uint );


typedef void	Void;
typedef void *	VoidPtr;
typedef const void * VoidCptr;
*/


#define DECLARE_OBJ( __name )	\
		typedef __name *		__name##Ptr;	\
		typedef __name &		__name##Ref;	\
		typedef const __name *	__name##Cptr;	\
		typedef const __name &	__name##Cref;

//
//	stl objects
#define	DECLARE_STACK( __type, __name )	\
		typedef stack< __type > __name;	\
		DECLARE_OBJ( __name );

#define	DECLARE_DEQUE( __type, __name )	\
		typedef deque< __type > __name;	\
		DECLARE_OBJ( __name );

#define	DECLARE_VECTOR( __type, __name )	\
		typedef vector< __type > __name;	\
		DECLARE_OBJ( __name );

#define	DECLARE_LIST( __type, __name )	\
		typedef list< __type > __name;	\
		DECLARE_OBJ( __name );



		/*
//
//	stl objects
#define	DECLARE_STACK( __type, __name )	\
		typedef std::stack< __type > __name;	\
		DECLARE_OBJ( __name );

#define	DECLARE_DEQUE( __type, __name )	\
		typedef std::deque< __type > __name;	\
		DECLARE_OBJ( __name );

#define	DECLARE_VECTOR( __type, __name )	\
		typedef std::vector< __type > __name;	\
		DECLARE_OBJ( __name );

#define	DECLARE_LIST( __type, __name )	\
		typedef std::list< __type > __name;	\
		DECLARE_OBJ( __name );
		*/

/*
 * 	memory allocation & deallocation
 * 	from internet source
 */

#define zap( __Z_OBJECT__ )	 \
		if( __Z_OBJECT__ != NULL ) \
		{ \
			delete __Z_OBJECT__; \
			__Z_OBJECT__ = NULL; \
		}
		
#endif
