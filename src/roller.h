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

	$Id: roller.h,v 1.1.1.1 2003/05/16 01:01:06 berise Exp $

*/
#if !defined( TROLLER_H )
#define TROLLER_H


#include "type.h"

//#include "torus.h" 


/*
 *	The TRoll is not a troll resides in a dungeon, but a Tee-Roll class
 *
 *	The TRoller animates torus rolling from top to bottom.
 *	And when the torus hits the bottom, and then it will emit a signal
 *	called fire. fire( row, color );
 *
 *	TRoller makes a top class of the Torus Class.
 *	The Torus Class is TRoller -- TPipe -- TPin set.
 *
 *	[ TRoller * ] set
 *	[ TPipe   * ] set
 *	[ TPin      ]
 *
 *
 * 	TRoller has a TTorus, which moves from top to bottom of TRoller
 * 	itself.
 * 	TRoller have to moves torus' and reposition.
 */
class TRoller
{
	
public:
	TRoller();
	~TRoller();

public:
	

	void	wrapTorus();
	void	initTorus();
	int		getImageIndex();
	
	double	getTorusYPoint();
	int		getTorusColor();

	/*
	 *	level variation. 
	 *	The level not only game difficulty but also rows to fire.
	 *	(shortly its same as max column)
	 */
	void setLevel( const int Level );
	int getLevel() const;


	//	롤러는 레벨에 따라서 높이가 달라진다. 
	//	달라지는 양은 파이프의 높이 만큼 줄어든다.
	void	setHeight( const int nHeight );
	int		getHeight();

	/*!
	 *	Moves torus to bottom.
	 *
	 * 	For every timeout signal, timer slot will use moveTorus.
	 */
	void	roll( int nDifficulty );	



private: 

	int	m_Level;		//	difficulties.

	//
	//	Roller 하나에 TTorus 하나, 즉 1:1 대응
	//	Roller object 를 적당히  늘이면 된다.
	//TTorus	theTorus;

	//
	//	TTorus에 X,Y 좌표를 주는 것 보다, Roller가 Torus의 좌표를 관리하면
	//	어떨까? 
	//	1. TTorus에 xy 좌표를 가지는 것.
	//		Roller는 단순히 TTorus.move 만 호출하면 된다.
	//		레벨 적용시에는 TTorus.setLevel( N )으로 최기화 필요.
	//	2. Roller가 TTorus의 y 좌표를 관리하면...
	//		speed 인자를 적용할 때 Roller.setLevel(N)으로 해결,
	//		TTorus까지 인수 전달할 필요가 없어진다.
	double m_TorusYPoint;
	
	//	TorusColor
	int m_TorusColor;

	//	Torus 이미지(돌아가며 떨어지는 토러스 이미지
	int m_TorusImageIndex;

	//	레벨에 따라 변경되는 롤러의 높이 저장.
	int		m_RollerHeight;
	
};


#endif
