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


	//	�ѷ��� ������ ���� ���̰� �޶�����. 
	//	�޶����� ���� �������� ���� ��ŭ �پ���.
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
	//	Roller �ϳ��� TTorus �ϳ�, �� 1:1 ����
	//	Roller object �� ������  ���̸� �ȴ�.
	//TTorus	theTorus;

	//
	//	TTorus�� X,Y ��ǥ�� �ִ� �� ����, Roller�� Torus�� ��ǥ�� �����ϸ�
	//	���? 
	//	1. TTorus�� xy ��ǥ�� ������ ��.
	//		Roller�� �ܼ��� TTorus.move �� ȣ���ϸ� �ȴ�.
	//		���� ����ÿ��� TTorus.setLevel( N )���� �ֱ�ȭ �ʿ�.
	//	2. Roller�� TTorus�� y ��ǥ�� �����ϸ�...
	//		speed ���ڸ� ������ �� Roller.setLevel(N)���� �ذ�,
	//		TTorus���� �μ� ������ �ʿ䰡 ��������.
	double m_TorusYPoint;
	
	//	TorusColor
	int m_TorusColor;

	//	Torus �̹���(���ư��� �������� �䷯�� �̹���
	int m_TorusImageIndex;

	//	������ ���� ����Ǵ� �ѷ��� ���� ����.
	int		m_RollerHeight;
	
};


#endif
