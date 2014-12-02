#include <functional>
//#include <algorithm>

using namespace std;


#include <stdlib.h>
#include <time.h>

#include "global.h"
#include "torusinternal.h"
#include "torusview.h"

#include "pin.h"
#include "roller.h"
#include "pipe.h"
#include "torusfunctors.h"




TorusInternal::TorusInternal( TorusView *pView )
: m_pTorusView( pView )
{
	m_pTPin		= new TPin;
	m_pPV		= new TPipeVector;
	m_pRV		= new TRollerVector;

	m_nLevel = 0;
	m_nDifficulty = 0;


}


TorusInternal::~TorusInternal()
{
	zap( m_pTPin );

	destroyPipe();
	destroyRoll();
}


void TorusInternal::destroyPipe()
{
	for_each( m_pPV->begin(), m_pPV->end(), delete_object< TPipe >() );
	zap( m_pPV );
}

void TorusInternal::destroyRoll()
{
	for_each( m_pRV->begin(), m_pRV->end(), delete_object< TRoller >() );
	zap( m_pRV );
}


//	각종 내부 딜리게이션들의 초기화 수행
void TorusInternal::init( int nLevel, int nDifficulty )
{
	m_nLevel		=	nLevel;
	m_nDifficulty	=	nDifficulty;

	//	TRoller의 moveTorus에서 서로 다른 색상을 위해서
	//	랜덤 시드를 만들어야 한다.
	srand( (unsigned)time( NULL ) );


	initPin( nLevel );
	initPipe( nLevel );	
	initRoll( nLevel );

	//	!!
	//	Roller의 뷰 높이를 지정해야 한다.
	//	OnSize 혹은 BoxSizer 의 변화에 따라 setRollerHeight를 호출해야 한다.
}


int TorusInternal::getLevel() { 	return m_nLevel; }
void TorusInternal::setLevel( int nLevel ) { 	setPipeLevel( nLevel ); setPinLevel( nLevel ); }




//
//	Implementation of Pipe Function Collections

//	init은 최초 초기화 및 게임의 시작시 호출
//	파이프의 개수를 조절하고 파이프 내부를 비운다(clear)
void TorusInternal::initPipe( int nLevel )
{	
	
	while( (int)m_pPV->size() > nLevel )
	{
		removePipe();
	}

	while( (int)m_pPV->size() < nLevel )
	{
		insertPipe();
	}

	//	파이프 내부에 쌓인 토러스를 모두 없앰.	
	for_each( m_pPV->begin(), m_pPV->end(), clear_pipe() );
	
}



//	full 에 대한 검사는 상위 윈도에서.. 처리하면 된다.
void TorusInternal::push_back(int aCol, int aTorusColor)
{
	TPipe	*aPipePtr = m_pPV->at( aCol );

	if( aPipePtr->isFull() != true )
	{
		aPipePtr->push_back( aTorusColor );

		m_pTorusView->draw();
		
		clearPipe( TORUS_CLEAR_ROW_BY_ROLLER );
		//	모든 Invalidate(false)를 redraw로 교체.
//##		redraw();
	//	repaint( false );
	//	emit checkRow( TORUS_CLEAR_ROW_BY_ROLLER );
	}

}


//	Pin과 토러스를 주고 받는 함수
void TorusInternal::push_front(int aCol, int aTorusColor)
{
	TPipe	*aPipePtr = m_pPV->at( aCol );

	if( aPipePtr->isFull() != true )
	{
		aPipePtr->push_front( aTorusColor );

		m_pTorusView->draw();
		//	Pin의 행동이므로... clear row
		clearPipe( TORUS_CLEAR_ROW_BY_PIN );

//		redraw();
		//repaint( false );
		//emit checkRow( TORUS_CLEAR_ROW_BY_PIN );
	}
}


void TorusInternal::pop_front( int aCol )
{
	TPipe	*aPipePtr = m_pPV->at( aCol );
	aPipePtr->pop_front();

	m_pTorusView->draw();

	clearPipe( TORUS_CLEAR_ROW_BY_PIN );

//	redraw();
}

	


//	pop_back을 쓸일이 있는가?
void TorusInternal::pop_back( int aCol )
{
	TPipe	*aPipePtr = m_pPV->at( aCol );

	aPipePtr->pop_back();	

	m_pTorusView->draw();

	clearPipe( TORUS_CLEAR_ROW_BY_ROLLER  );

//	redraw();
}


bool TorusInternal::isPipeEmpty( int aCol )
{
	TPipe	*aPipePtr = m_pPV->at( aCol );

	return aPipePtr->isEmpty();
}

bool TorusInternal::isPipeFull( int aCol )
{
	TPipe	*aPipePtr = m_pPV->at( aCol );

	return aPipePtr->isFull();
}


int TorusInternal::front(int aCol)
{
	TPipe	*aPipePtr = m_pPV->at( aCol );

	return aPipePtr->front();

}

int TorusInternal::back( int aCol )
{
	TPipe	*aPipePtr = m_pPV->at( aCol );

	return aPipePtr->back();
}

int TorusInternal::getSize( int aCol )
{
	TPipe	*aPipePtr = m_pPV->at( aCol );

	return aPipePtr->getSize();
}


//	가장 오른쪽에 파이프를 삽입한다.
void TorusInternal::insertPipe()
{
	m_pPV->push_back( new TPipe );
}


//	가장 오른쪽의 파이프를 삭체한다.
void TorusInternal::removePipe()
{
	TPipe *aPipePtr = m_pPV->back();
	m_pPV->pop_back();

	zap( aPipePtr );
}

int TorusInternal::getPipeSize()
{
	return m_pPV->size();
}



//	get minimum pipe size to minimize checkPipe() time.
//	이 함수는 clearRow에서 사용된다.
int TorusInternal::getMinimumPipeSize()
{
	TPipeVector::iterator result = min_element( m_pPV->begin(), m_pPV->end(), minimum() ); 

	return (*result)->getSize();

}

int TorusInternal::getMaximumPipeSize()
{
	TPipeVector::iterator result = min_element( m_pPV->begin(), m_pPV->end(), maximum() );

	return (*result)->getSize();
}




//	This functor finds which pipe does not have the theColor at the Row.
//	다른 색상인가?
struct  not_equal_color_to : unary_function< TPipePtr, bool >
{
	not_equal_color_to( int Row, int Color ) : theRow( Row ), theColor( Color ) {}

	bool operator()( TPipePtr aPipePtr )
	{
		int aTorusColor = aPipePtr->getTorusColor( theRow );
		return	( aTorusColor != theColor );
	}

	int theRow, theColor;
};


//	This functor finds out which pipe in row have same color
struct  equal_color : binary_function< TPipePtr, TPipePtr, bool >
{
	equal_color( int Row ) : theRow(Row) {}

	//	bind2nd를 이용해 결합할 때 bool operator()는 const가 되어야 한다. 
	bool operator()( const TPipePtr a, const TPipePtr b ) const
	{ 
		return a->getTorusColor( theRow ) == b->getTorusColor( theRow );
	}

	int theRow;
};




/*! \fn void TorusInternal::clearPipe( ClearMethod aClearMethod  )
    \brief 파이프들을 가로로 조사하고, 토러스가 일렬로 배치되면 제거한다.
	\return 없음.
 */
void TorusInternal::clearPipe( ClearMethod aClearMethod  )
{
	int MinPipeSize = getMinimumPipeSize();
	bool IsMatched;

	/*
	 *
	 * check for all pipes
	 * row 2 -----+-----+-----+-----> 
	 * row 1 -----+-----+-----+-----> 
	 * row 0 -----+-----+-----+-----> c
	 *  ool    0     1     2     3
	 *
	 *  Checking row must be perform from top to bottom.
	 *  because clearing bottom first makes an unexpectable result, when
	 *  two Rows are matched.
	 */
	for( int row = MinPipeSize - 1; row >= 0; row-- )
	{
		IsMatched = true;

		TPipePtr aZerothPipePtr  = m_pPV->at( 0 ) ;
		int ZerothColor = aZerothPipePtr->getTorusColor(row);

		TPipeVector::iterator result = find_if(	m_pPV->begin(), 
												m_pPV->end(),
												//not_equal_color_to( row, ZerothColor ) );
												bind2nd( not2(equal_color(row)), aZerothPipePtr) );



		if( result == m_pPV->end() )
		{
			//	clear matched line(row)
			clearRow( row );		

			//	gainPoint는 clearRow후에 호출해야 isAllClear가 올바로 입력된다.:w
			m_pTorusView->gainPoint( row, aClearMethod, isAllClear() );						
		}



		/*
		TPipeVector::iterator result = find_if(	m_pPV->begin(), 
												m_pPV->end(),
												not_equal_color_to( row, ZerothColor ) );
												//not1 ( equal_color( row, ZerothColor ) ) );
												*/

		/*
		 * 	result indicates where the not_equal_color matched.
		 * 	result == end() means all equal.
		 
		if( result != m_pPV->end() )
		{
			IsMatched = false;
		}

		// Matched line operation
		if( IsMatched == true )
		{ 

			//	clear matched line(row)
			clearRow( row );

			//	###
			m_pTorusView->gainPoint( row, aClearMethod, isAllClear() );
			
		}
		*/
	}

}


//	파이프와 핀이 모두 빈 상태
bool TorusInternal::isAllClear()
{
	bool bPinClear = false;
	bool bPipeClear = false;

	
	//	get pin clear status
	bPinClear	= m_pTPin->isEmpty();

	
	//	get pipe clear status
	TPipeVector::iterator pvIter;	
	pvIter = find_if( m_pPV->begin(), m_pPV->end(), is_pipe_empty() );
	if( pvIter == m_pPV->end() )
	{
		bPipeClear = true;
	}
	else
	{
		bPipeClear = false;
	}

	return (	( bPinClear		== true ) &&
				( bPipeClear	== true ));	
}



void TorusInternal::clearRow( int nRow )
{
	int nMinimumPipeSize = getMinimumPipeSize();

	if( nRow <= nMinimumPipeSize )
	{
		for_each( m_pPV->begin(), m_pPV->end(), clear_row( nRow ) );
	}
}


bool TorusInternal::isPipeEmpty()
{
	TPipeVector::iterator pvIter;
	
	pvIter = find_if( m_pPV->begin(), m_pPV->end(), is_pipe_empty() );

	return pvIter == m_pPV->end();
}


void TorusInternal::setPipeLevel(int nLevel)
{
	for_each( m_pPV->begin(), m_pPV->end(), set_level( nLevel ) );
}



void TorusInternal::setPipeBlockSize(int nBlockSize)
{
	for_each( m_pPV->begin(), m_pPV->end(), set_block_size( nBlockSize ) );

//	redraw();
}

int TorusInternal::getPipeBlockSize()
{
	return m_pPV->at(0)->getBlockSize();
}




/*
// vwcl & vxcl codes
void TorusInternal::redraw( const VDC &vDCCref )
{
	VDC	mdc;	//	memory DC

	VRect rClient;
	GetClientRect( rClient );

	vDCCref.CreateCompatible( mdc );


	//	vxcl에서의 더블 버퍼링(double buffering) 구현.
	//	목적은 잘 알다시피. flicker free의 에니메이션을 구현하기 위한 것이고.
	//	CreateCompatibleBiamap API를 써서. VBitmap을 우선 초기화 시킨다.
	//	MSDN에 따르면 CreateCompatibleBiamap은 주어진 DC에 따라 monocrom혹은 colored 가 결정된다.
	//	따라서, 초기 인자로 dc.GetHandle()을 주어야 한다.
	//	여기서 mdc.GetHandle()을 주면 Monocrom이 생성된다.
	VBitmap		aBufferBitmap;
	VBrush		aBlackBrush;
	aBufferBitmap = CreateCompatibleBitmap( vDCCref.GetHandle(), rClient.GetWidth(), rClient.GetHeight() );

	aBlackBrush.CreateSolid( COLORREF( 0x00000000 ) );
	mdc.SelectObject( aBufferBitmap.GetHandle() );
	mdc.FillRect( rClient, aBlackBrush.GetHandle() );



	//	visible areas in the transparent png image.	 

	//	파이프 내부 그리기 루틴
	//for_each( m_pPV->begin(), m_pPV->begin(), mem_ptr( drawTorus ) );

	VDC aMDC;
	vDCCref.CreateCompatible( aMDC );
	VBitmap	vbmSidebar( MAKEINTRESOURCE(IDB_SIDEBAR) );
	

	for( int aCol = 0; aCol < m_pPV->size(); aCol++ )
	{
		TPipe	*aPipePtr = m_pPV->at( aCol );

		//	### level change code here
		//	trick level draw... draw from top to bottom	
		int i;

		aMDC.SelectObject( vbmSidebar.GetHandle() );
		for( i = 0; i < aPipePtr->getBlockSize(); i++ )
		{			
			mdc.BitBlt(	TORUS_WIDTH * aCol,
						TORUS_HEIGHT * i,
						TORUS_WIDTH * aCol + TORUS_WIDTH, 
						TORUS_HEIGHT * i + TORUS_HEIGHT,
						aMDC,
						0,
						0 );


			
			mdc.Rectangle( TORUS_WIDTH * aCol, TORUS_HEIGHT * i, TORUS_WIDTH * aCol + TORUS_WIDTH, TORUS_HEIGHT * i + TORUS_HEIGHT );
			
			//aPainter.drawPixmap( 0, TORUS_HEIGHT * i, aSideBarRef );
		}
		
	
		int HOffset = rClient.GetHeight() - TORUS_HEIGHT; // first blocked area	//	 20 is transparent area in the png image.
		

		VBitmap	bmTorus( MAKEINTRESOURCE(IDB_TORUS) );
		aMDC.SelectObject( bmTorus.GetHandle() );

		//	draw toruses from bottom to top.
		int nTorus = aPipePtr->getSize();
		for(  i = 0; i < nTorus; i++ )	
		{
			int nTorusColor = aPipePtr->getTorusColor( i );

			//	char szDebug[256];

			//	wsprintf( szDebug, "[%d:%d] color is : %d, h offset : %d", aCol, i, nTorusColor, nTorusColor * TORUS_HEIGHT + 0 );
			//MessageBox( szDebug );
			
			//	Windows NT: Requires version 5.0 or later.
				//	 Windows: Requires Windows 98 or later.
				//  Header: Declared in wingdi.h.
				//	Import Library: Included as a resource in msimg32.dll.
			
			  
				
				  // comment out below func. T~B~
			TransparentBlt( mdc.GetHandle(),
								TORUS_WIDTH * aCol,	//	lefttop
								HOffset,	//		righttop
								TORUS_WIDTH,
								TORUS_HEIGHT,
								memDC2.GetHandle(),
								TORUS_WIDTH * 8,
								nTorusColor * TORUS_WIDTH + 20,
								TORUS_WIDTH,
								TORUS_HEIGHT,
								0x000000 );	//	black
								
			

			
			

				mdc.BitBlt( TORUS_WIDTH * aCol,	//	lefttop
							HOffset,	//		righttop
							TORUS_WIDTH,
							TORUS_HEIGHT,
							aMDC,
							TORUS_WIDTH * (MAX_IMAGE_COUNT/2),	//	납작한 토러스 위치
							nTorusColor * TORUS_WIDTH + 20 );
			
							
				
			//
			//	색상에 맞는 토러스를 화면에 그린다. 
			//	더이상 torus.draw를 호출하지 않음.
			//	LeftTop 자리는 현재 pin 위치에 따라 다르게 주어야 한다.
			//	COMMENT OUT BELOW FUNC CALL
			mdc.Rectangle(TORUS_WIDTH * aCol,	//	lefttop
						HOffset,	//		righttop
						TORUS_WIDTH * aCol + TORUS_WIDTH,	//	leftbottom
						HOffset + TORUS_HEIGHT //	flat torus height
						);
						
			HOffset -= TORUS_HEIGHT; 
		}
	}


	//	blitting
	vDCCref.BitBlt( 0, 0, rClient.GetWidth(), rClient.GetHeight(), mdc );
}
*/


/*
void TorusInternal::redraw()
{
	VClientDC	vDC( VREF_THIS );

	redraw( vDC );
}
*/




//	Implementation of Pin functions
void TorusInternal::movePinRight()
{
	//	현재 레벨의 최대 범위 안에서
	if( m_pTPin->getPinPos() < m_pTPin->getLevel() - 1 )		//	'-'는  c index 특성
	{
		m_pTPin->moveRight();	
	}
}


void TorusInternal::movePinLeft()
{
	//	더 이상 갈 수 없어.
	if( m_pTPin->getPinPos() > 0 )
	{
		m_pTPin->moveLeft();
//		Invalidate( false );
	}

}

void TorusInternal::pushPin(int aTorusColor)
{
	m_pTPin->push( aTorusColor );
//	Invalidate( false );
}

void TorusInternal::popPin()
{
	m_pTPin->pop();
//	Invalidate( false );

}

bool TorusInternal::isPinEmpty()
{
	//int nSize = m_pTPin->getSize();
	return m_pTPin->isEmpty();
}

bool TorusInternal::isPinFull()
{
	return m_pTPin->isFull();
}

int TorusInternal::topPin()
{
	return m_pTPin->top();
}


//	외부 인터페이스
int TorusInternal::getPinPos()
{
	return m_pTPin->getPinPos();
}

void TorusInternal::setPinLevel(int nLevel)
{
	m_pTPin->setLevel( nLevel );
}

void TorusInternal::setPinBlockSize(int nBlockSize)
{
	m_pTPin->setBlockSize( nBlockSize );

	//	window invalidation
//	Invalidate( false );

}

int TorusInternal::getPinBlockSize()
{
	return m_pTPin->getBlockSize();
}

void TorusInternal::initPin( int nLevel )
{
	m_pTPin->init( nLevel );
}

void TorusInternal::setPinPos(int /* nPinPos */)
{
	
}



int TorusInternal::getPinTorusSize()
{
	return m_pTPin->getSize();
}


int TorusInternal::getPinTorusColor( int nTh )
{
	return m_pTPin->getTorusColor( nTh );
}






//
//	Implementation of Roller Functions
void TorusInternal::insertRoller()	//insertRoller( int nHeight )
{
	//	새로운 롤러를 집어 넣을 때, 높이를 설정해 주어야 한다.
	//	## see rollw.cpp, previous project
	TRollerPtr	aRollerPtr	= new TRoller;

	aRollerPtr->setHeight( getRollerHeight() );		//	 ###  sample 500 height 
		
	m_pRV->push_back( aRollerPtr );

//	redraw();
}

void TorusInternal::removeRoller()
{
	TRoller *aRollerPtr = m_pRV->back();
	m_pRV->pop_back();

	zap( aRollerPtr );
}



//	TorusInternal안의 TRollerVector 각각에 대해 토러스를 움직인다.
//	토러스가 바닥에 닫았을 경우,
//	PipeW에 비어 있는지를 검사하고,
//		가득 찬 경우 GameOver
//		비어 있는 경우 PipeW->push_back

/*! \func moveTorus()
/* \brief Timer or Thread in the torusview periodically call moveTorus.
 *        This function moves torus in the roller at the give speed(difficulty)
 *        If one of toruses hit the upper part of pipe then it call gameover.
 * \param NONE
 * \return -1, if game over, otherwise return value indicates number of overflowed pipe
 */
int TorusInternal::moveTorus()
{
	/*

	//	각각의  토러스를 이동.
	for_each( m_pRV->begin(), m_pRV->end(), move_toruses() );

	
	VRect rClient;
	GetClientRect( rClient );

	int nHeight = rClient.GetHeight();
	*/
	

	//	hit test.
	/**	2002.8.2  이상한 에러가 발생한다.
		이 에러는 RollerVector에 new 로 할당된 TRoller가 이상한 메모리로 포인팅되는 것이다.
		문제는 이 에러가 예측할 수 없다는 것이다. 에러가 발생하기도 하고, 발생하지 않기도 하고.
		영어로 Nondeterministic 하다는 말이 딱 맞을 것 같다.

		현재 상황에서 제일 의심가는 놈은 MSVC의 STL 버전이 제대로 동작하지 않는 다는 것인데...
		여전히 문제는 오리무중인 상태이다.

		2002.8.7  오늘을 기점으로 begin(), end() 를 사용하지 않고, for 문을 사용한다.
	
	TRollerVector::iterator	rvIter = m_pRV->begin();
	while( rvIter != m_pRV->end() )
	{
		TRollerPtr	aRollerPtr = (*rvIter);

		//	바닥에 닫았다면
		if( aRollerPtr->getTorusYPoint() > nHeight )
		{
			//	넣을 공간이 남아 있으면, PipeWindow로 전달하고
			if( m_PipeWindowPtr->isFull( rvIter - m_pRV->begin() ) == false )
			{
				m_PipeWindowPtr->push_back( rvIter - m_pRV->begin(), aRollerPtr->getTorusColor() );

				//	감는다.
				aRollerPtr->wrapTorus();
			}
			else
			{
				//	롤러의 토러스 중 하나라도 바닥에 닿았다면
				//	게임 종료 하고, 더 이상 이동 시키면 안된다.
				//	segfault 일어남(02.7.8)				
				theTorusWindowPtr->OnGameOver();

				//	02.7.10 
				//	return 해서 movetorus를 부르지 않으면, segfault 막음.
				return;
			}
		}
		
		rvIter++;
	}

  */


	//	각각의  토러스를 이동.
	for_each( m_pRV->begin(), m_pRV->end(), move_toruses( getDifficulty() ) );

	int nRVSize	=	m_pRV->size();
	for( int i = 0; i < nRVSize; i++ )
	{		
		//	바닥에 닫았다면
		if( getTorusPosY( i ) > getRollerHeight() )
		{
			//	넣을 공간이 남아 있으면, PipeWindow로 전달하고
			if( isPipeFull( i ) == false )
			{
				push_back( i, getTorusColor( i ) );

				//	감는다.
				m_pRV->at(i)->wrapTorus();				
			}
			else
			{
				//	02.7.10 
				//	return 해서 movetorus를 부르지 않아야... segfault 막을 수 있다.
				// return 1;
				
				// now return # of overflown pipe
				return i;
			}
		}

	}

	return -1;	//	everything ok.	
}


//	파이프에서 aColumn의 nTh번째의 색상을 가져온다.
int TorusInternal::getTorusColor(int aColumn, int nTh)
{
	return m_pPV->at( aColumn )->getTorusColor( nTh );
}


//	롤러에서 aColumn의 색상을 가져온다.
int TorusInternal::getTorusColor(int aColumn)
{
	return m_pRV->at( aColumn )->getTorusColor();
}


//	 게임 초기화 
void TorusInternal::initRoll( int nLevel )
{
	
	while( (int) m_pRV->size() > nLevel )
	{
		removeRoller();
	}

	while( (int) m_pRV->size() < nLevel )
	{
		insertRoller();
	}

	for_each( m_pRV->begin(), m_pRV->end(), wrap_torus() );
}



//	뷰에서 볼때 롤러의 크기
void	TorusInternal::setRollerHeight( int nHeight )
{
	m_nRollerHeight = nHeight;
}

int		TorusInternal::getRollerHeight()
{
	return m_nRollerHeight;
}


int TorusInternal::getTorusPosY( int nColumn )
{
	//	YPoint= ==> PosY
	return m_pRV->at( nColumn )->getTorusYPoint();
}

int TorusInternal::getTorusImageIndex( int nColumn )
{
	return m_pRV->at( nColumn )->getImageIndex();
}



//	공통 함수
void	TorusInternal::increaseLevel()
{
	//	level로 이름을 바꿀까?
	insertPipe();
	insertRoller();

	m_nLevel++;

	setPinLevel( m_nLevel );
	setPipeLevel( m_nLevel );
	
}

void	TorusInternal::decreaseLevel()
{
	m_nLevel--;

	setPinLevel( m_nLevel );
	setPipeLevel( m_nLevel );

	removePipe();
	removeRoller();

}


void	TorusInternal::setDifficulty( int nDifficulty )
{
	m_nDifficulty = nDifficulty;
}


int		TorusInternal::getDifficulty()
{
	return	m_nDifficulty;
}