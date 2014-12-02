//	2004. torus uses wxWindows
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

	$Id: scorefile.h,v 1.1.1.1 2003/05/16 01:01:06 berise Exp $

*/
#ifndef _TSCORE_FILE_H
#define _TSCORE_FILE_H

#include <vector>

#include "type.h"

using namespace std;

class ScoreData
{
public:		
	ScoreData( const char * aName = "", int aScore = 0 )
	{
		setName(aName);
		setScore(aScore);
	};

	~ScoreData()	{	}

	const char * getName() const		{		return theName;	}
	void setName( const char * aName )	{		strcpy( theName, aName );	}

	int	getScore() const				{		return theScore;	}
	void setScore( const int aScore )	{		theScore = aScore;	}

private:
	char theName[128];
	int theScore;
};

__CORE_DECLARE_CLASS( ScoreData );

DECLARE_VECTOR( ScoreData, ScoreVector );


class TScoreFile
{
	public:
		TScoreFile();
		~TScoreFile();

	public:
		//void getScoreData( int nNumber, ScoreData *pSD );

		void	initialize();

		//	�÷��̾��� �̸��� ������ �ް� ���Ϳ� �����Ѵ�. ���� ScoreData�� �ѷ��ΰ�..
		void push_back( const char *pName, int nScore );

		//	������ �÷��̾��� �̸��� ��´�.
		int		getScore( int Number );
		void	getPlayer( int aNumber, char *aPlayerPtr );

		//	���� ���� ������ ��´�.
		//	�̴� �÷��̾��� �̸��� ����ϱ� ���� �����ϴ� ���� �Լ��̴�.
		//	�÷��̾�� ������ ����� �� ������ ....
		int		getMinScore();


		//	HIGH_SCORE ���Ͽ� �����Ѵ�. �о�´�.
		void read();
		void write();

	private:
		ScoreVector	theScoreVector;
		bool		m_bDirty;
};


#endif
