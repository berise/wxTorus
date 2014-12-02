#ifndef __Torus_internal_included__
#define __Torus_internal_included__


#include "global.h"	//	ClearMethod
#include "type.h"

#include <vector>

using namespace std;

class TPin;
class TRoller;

class TorusView;



//	Pipe components
__CORE_DECLARE_CLASS( TPipe );
DECLARE_VECTOR( TPipePtr, TPipeVector );


__CORE_DECLARE_CLASS( TRoller );
DECLARE_VECTOR( TRollerPtr, TRollerVector );

class TorusInternal
{
public:
	TorusInternal( TorusView *pView );
	~TorusInternal();


	//	Pipe Function Collections
public:


	void pop_front( int aCol );
	void pop_back( int aCol );
	void push_front( int aCol, int aTorusColor );
	void push_back( int aCol, int aTorusColor );



	//	Pipe related function
public:
//	void redraw();
//	void redraw( const VDC &vDCCref );
	void init( int nLevel, int nDifficulty );
	

	//	현재 레벨을 가져온다
	int getLevel();
	void setLevel( int nLevel );

	void	setDifficulty( int nDifficulty );
	int		getDifficulty();

	//	 파이프 초기화 
	void initPipe( int nLevel );
	void destroyPipe();

	//	게임 진행시 조절하는 블럭의 개수
	void setPipeBlockSize( int nBlockSize );
	int getPipeBlockSize();

	//	setLevel은 Init과는 비슷하지만, 담을 수 있는 공간을 조절한다.
	void setPipeLevel( int nLevel );

	//	파이프가 비었는가?
	bool isPipeEmpty();		
	bool isPipeEmpty( int aCol );

	//	가득찾는가?
	bool isPipeFull( int aCol );
	



	//	 clear functions.
	void clearRow( int nRow );
	void clearPipe( ClearMethod );
	bool isAllClear();



	//	토러스가 가장 많은 파이프의 토러스의 개수
	int getMaximumPipeSize();	
	int getMinimumPipeSize();

	//	첫번째 토러스 색상을 가져온다.
	int front( int aCol );
	int back( int aCol );

	//	현재 파이프의 개수
	int getPipeSize();
	int getSize( int aCol );


	//	개수 조절 함수
	void insertPipe();
	void removePipe();


	//	void setTorusWindow( TorusWindowPtr aTorusWindowPtr );	

	//
	// Pin functions
	void initPin( int nLevel );
	void setPinBlockSize( int nBlockSize );
	int		getPinBlockSize();

	void setPinLevel( int nLevel );

	void setPinPos( int nPinPos );
	int getPinPos();
	

	bool isPinFull();
	bool isPinEmpty();
	void popPin();
	void pushPin( int aTorusColor );
	int topPin();
	
	//	virtual functions.	
	void movePinRight();
	void movePinLeft();

	//	핀에 있는 토러스의 개수
	int getPinTorusSize();
	int getPinTorusColor( int nTh );



	//
	// Roller functions
	//	see comments in pipew.cpp
	void initRoll( int nLevel );
	void destroyRoll();


	
	//	Pipe 함수
	int getTorusColor(int aColumn, int nTh);

	//	Roller 함수
	int getTorusColor( int nColumn );
	int getTorusPosY( int nColumn );
	int getTorusImageIndex( int nColumn );


	int moveTorus();

	//	레벨 증가시 Roller 의 개수를 빼고
	void removeRoller();
	//	더하고 
	void insertRoller();

	//	뷰에서 롤러의 크기를 조절해야 한다. 레벨이 증가할 때 마다 크기 조절이 ㅣㅍㄹ요하다
	void	setRollerHeight( int nHeight );
	int		getRollerHeight();

	// 공통 함수
	void	increaseLevel();
	void	decreaseLevel();



private:
	//	pref
	int			m_bExtraTorus;
	int			m_nDifficulty;
	int			m_nLevel;

	TPin		*m_pTPin;


	//	레벨에 따라 달라지는 Roller 의 높이
	//	View와 관련된 인자이기 때문에.. Internal에 있다.
	int			m_nRollerHeight;

	//	Pipe STL Vector
	TPipeVectorPtr	m_pPV;

	//	Roll STL Vector, 2
	TRollerVectorPtr	m_pRV;

	//	View 클래스 포인터
	TorusView	*m_pTorusView;
};

#endif
