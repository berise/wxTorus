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


//	���� ���� �������̼ǵ��� �ʱ�ȭ ����
void TorusInternal::init( int nLevel, int nDifficulty )
{
	m_nLevel		=	nLevel;
	m_nDifficulty	=	nDifficulty;

	//	TRoller�� moveTorus���� ���� �ٸ� ������ ���ؼ�
	//	���� �õ带 ������ �Ѵ�.
	srand( (unsigned)time( NULL ) );


	initPin( nLevel );
	initPipe( nLevel );	
	initRoll( nLevel );

	//	!!
	//	Roller�� �� ���̸� �����ؾ� �Ѵ�.
	//	OnSize Ȥ�� BoxSizer �� ��ȭ�� ���� setRollerHeight�� ȣ���ؾ� �Ѵ�.
}


int TorusInternal::getLevel() { 	return m_nLevel; }
void TorusInternal::setLevel( int nLevel ) { 	setPipeLevel( nLevel ); setPinLevel( nLevel ); }




//
//	Implementation of Pipe Function Collections

//	init�� ���� �ʱ�ȭ �� ������ ���۽� ȣ��
//	�������� ������ �����ϰ� ������ ���θ� ����(clear)
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

	//	������ ���ο� ���� �䷯���� ��� ����.	
	for_each( m_pPV->begin(), m_pPV->end(), clear_pipe() );
	
}



//	full �� ���� �˻�� ���� ��������.. ó���ϸ� �ȴ�.
void TorusInternal::push_back(int aCol, int aTorusColor)
{
	TPipe	*aPipePtr = m_pPV->at( aCol );

	if( aPipePtr->isFull() != true )
	{
		aPipePtr->push_back( aTorusColor );

		m_pTorusView->draw();
		
		clearPipe( TORUS_CLEAR_ROW_BY_ROLLER );
		//	��� Invalidate(false)�� redraw�� ��ü.
//##		redraw();
	//	repaint( false );
	//	emit checkRow( TORUS_CLEAR_ROW_BY_ROLLER );
	}

}


//	Pin�� �䷯���� �ְ� �޴� �Լ�
void TorusInternal::push_front(int aCol, int aTorusColor)
{
	TPipe	*aPipePtr = m_pPV->at( aCol );

	if( aPipePtr->isFull() != true )
	{
		aPipePtr->push_front( aTorusColor );

		m_pTorusView->draw();
		//	Pin�� �ൿ�̹Ƿ�... clear row
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

	


//	pop_back�� ������ �ִ°�?
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


//	���� �����ʿ� �������� �����Ѵ�.
void TorusInternal::insertPipe()
{
	m_pPV->push_back( new TPipe );
}


//	���� �������� �������� ��ü�Ѵ�.
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
//	�� �Լ��� clearRow���� ���ȴ�.
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
//	�ٸ� �����ΰ�?
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

	//	bind2nd�� �̿��� ������ �� bool operator()�� const�� �Ǿ�� �Ѵ�. 
	bool operator()( const TPipePtr a, const TPipePtr b ) const
	{ 
		return a->getTorusColor( theRow ) == b->getTorusColor( theRow );
	}

	int theRow;
};




/*! \fn void TorusInternal::clearPipe( ClearMethod aClearMethod  )
    \brief ���������� ���η� �����ϰ�, �䷯���� �Ϸķ� ��ġ�Ǹ� �����Ѵ�.
	\return ����.
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

			//	gainPoint�� clearRow�Ŀ� ȣ���ؾ� isAllClear�� �ùٷ� �Էµȴ�.:w
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


//	�������� ���� ��� �� ����
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


	//	vxcl������ ���� ���۸�(double buffering) ����.
	//	������ �� �˴ٽ���. flicker free�� ���ϸ��̼��� �����ϱ� ���� ���̰�.
	//	CreateCompatibleBiamap API�� �Ἥ. VBitmap�� �켱 �ʱ�ȭ ��Ų��.
	//	MSDN�� ������ CreateCompatibleBiamap�� �־��� DC�� ���� monocromȤ�� colored �� �����ȴ�.
	//	����, �ʱ� ���ڷ� dc.GetHandle()�� �־�� �Ѵ�.
	//	���⼭ mdc.GetHandle()�� �ָ� Monocrom�� �����ȴ�.
	VBitmap		aBufferBitmap;
	VBrush		aBlackBrush;
	aBufferBitmap = CreateCompatibleBitmap( vDCCref.GetHandle(), rClient.GetWidth(), rClient.GetHeight() );

	aBlackBrush.CreateSolid( COLORREF( 0x00000000 ) );
	mdc.SelectObject( aBufferBitmap.GetHandle() );
	mdc.FillRect( rClient, aBlackBrush.GetHandle() );



	//	visible areas in the transparent png image.	 

	//	������ ���� �׸��� ��ƾ
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
							TORUS_WIDTH * (MAX_IMAGE_COUNT/2),	//	������ �䷯�� ��ġ
							nTorusColor * TORUS_WIDTH + 20 );
			
							
				
			//
			//	���� �´� �䷯���� ȭ�鿡 �׸���. 
			//	���̻� torus.draw�� ȣ������ ����.
			//	LeftTop �ڸ��� ���� pin ��ġ�� ���� �ٸ��� �־�� �Ѵ�.
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
	//	���� ������ �ִ� ���� �ȿ���
	if( m_pTPin->getPinPos() < m_pTPin->getLevel() - 1 )		//	'-'��  c index Ư��
	{
		m_pTPin->moveRight();	
	}
}


void TorusInternal::movePinLeft()
{
	//	�� �̻� �� �� ����.
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


//	�ܺ� �������̽�
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
	//	���ο� �ѷ��� ���� ���� ��, ���̸� ������ �־�� �Ѵ�.
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



//	TorusInternal���� TRollerVector ������ ���� �䷯���� �����δ�.
//	�䷯���� �ٴڿ� �ݾ��� ���,
//	PipeW�� ��� �ִ����� �˻��ϰ�,
//		���� �� ��� GameOver
//		��� �ִ� ��� PipeW->push_back

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

	//	������  �䷯���� �̵�.
	for_each( m_pRV->begin(), m_pRV->end(), move_toruses() );

	
	VRect rClient;
	GetClientRect( rClient );

	int nHeight = rClient.GetHeight();
	*/
	

	//	hit test.
	/**	2002.8.2  �̻��� ������ �߻��Ѵ�.
		�� ������ RollerVector�� new �� �Ҵ�� TRoller�� �̻��� �޸𸮷� �����õǴ� ���̴�.
		������ �� ������ ������ �� ���ٴ� ���̴�. ������ �߻��ϱ⵵ �ϰ�, �߻����� �ʱ⵵ �ϰ�.
		����� Nondeterministic �ϴٴ� ���� �� ���� �� ����.

		���� ��Ȳ���� ���� �ǽɰ��� ���� MSVC�� STL ������ ����� �������� �ʴ� �ٴ� ���ε�...
		������ ������ ���������� �����̴�.

		2002.8.7  ������ �������� begin(), end() �� ������� �ʰ�, for ���� ����Ѵ�.
	
	TRollerVector::iterator	rvIter = m_pRV->begin();
	while( rvIter != m_pRV->end() )
	{
		TRollerPtr	aRollerPtr = (*rvIter);

		//	�ٴڿ� �ݾҴٸ�
		if( aRollerPtr->getTorusYPoint() > nHeight )
		{
			//	���� ������ ���� ������, PipeWindow�� �����ϰ�
			if( m_PipeWindowPtr->isFull( rvIter - m_pRV->begin() ) == false )
			{
				m_PipeWindowPtr->push_back( rvIter - m_pRV->begin(), aRollerPtr->getTorusColor() );

				//	���´�.
				aRollerPtr->wrapTorus();
			}
			else
			{
				//	�ѷ��� �䷯�� �� �ϳ��� �ٴڿ� ��Ҵٸ�
				//	���� ���� �ϰ�, �� �̻� �̵� ��Ű�� �ȵȴ�.
				//	segfault �Ͼ(02.7.8)				
				theTorusWindowPtr->OnGameOver();

				//	02.7.10 
				//	return �ؼ� movetorus�� �θ��� ������, segfault ����.
				return;
			}
		}
		
		rvIter++;
	}

  */


	//	������  �䷯���� �̵�.
	for_each( m_pRV->begin(), m_pRV->end(), move_toruses( getDifficulty() ) );

	int nRVSize	=	m_pRV->size();
	for( int i = 0; i < nRVSize; i++ )
	{		
		//	�ٴڿ� �ݾҴٸ�
		if( getTorusPosY( i ) > getRollerHeight() )
		{
			//	���� ������ ���� ������, PipeWindow�� �����ϰ�
			if( isPipeFull( i ) == false )
			{
				push_back( i, getTorusColor( i ) );

				//	���´�.
				m_pRV->at(i)->wrapTorus();				
			}
			else
			{
				//	02.7.10 
				//	return �ؼ� movetorus�� �θ��� �ʾƾ�... segfault ���� �� �ִ�.
				// return 1;
				
				// now return # of overflown pipe
				return i;
			}
		}

	}

	return -1;	//	everything ok.	
}


//	���������� aColumn�� nTh��°�� ������ �����´�.
int TorusInternal::getTorusColor(int aColumn, int nTh)
{
	return m_pPV->at( aColumn )->getTorusColor( nTh );
}


//	�ѷ����� aColumn�� ������ �����´�.
int TorusInternal::getTorusColor(int aColumn)
{
	return m_pRV->at( aColumn )->getTorusColor();
}


//	 ���� �ʱ�ȭ 
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



//	�信�� ���� �ѷ��� ũ��
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



//	���� �Լ�
void	TorusInternal::increaseLevel()
{
	//	level�� �̸��� �ٲܱ�?
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