#include <algorithm>	//	min_element
#include <functional>
using namespace std;

#include "scorefile.h"
#include "roller.h"
#include "pipe.h"

//	��ü�� ����� 
template<class T>
struct delete_object : public unary_function<const T*, void>
{
	void operator()( const T* ptr ) const
	{
		delete ptr;
	}
};

struct  set_level
{
	set_level( int nLevel ) : theLevel( nLevel ) {}

	void operator()( TPipePtr aPipePtr )
	{
		aPipePtr->setLevel( theLevel );
	}


	int theLevel;
};



//	minimum retrieve minimum pipe size(# of torus)
struct minimum : public binary_function< TPipe *, TPipe *,  bool >
{
	bool operator() ( TPipe	*aPipePtr1, TPipe	*aPipePtr2  )
	{
		return ( aPipePtr1->getSize() < aPipePtr2->getSize() );
	}
};

struct maximum :  public binary_function< TPipe *, TPipe *,  bool >
{
	bool operator() ( TPipe	*aPipePtr1, TPipe	*aPipePtr2  )
	{
		return ( aPipePtr1->getSize() > aPipePtr2->getSize() );
	}
};




/*
//	���� �����ΰ�?
struct  equal_color: unary_function< TPipePtr, bool >
{
		equal_color( int Row, int Color ) : theRow( Row ), theColor( Color ) {}

		bool operator()( TPipePtr aPipePtr )
		{
			int aTorusColor = aPipePtr->getTorusColor( theRow );

			//	use != in order to avoid int to bool warning
			return	( aTorusColor == theColor );// != 0 ;
		}

		int	theRow, theColor;

};

*/

//	clear_row functor call TPipePtr->clearRow
//	�־��� ���� �����ϴ� ��ü 
struct  clear_row
{
		clear_row( int Row ) : theRow( Row ) {}

		void operator()( TPipePtr aPipePtr )
		{
//			assert( aPipePtr != NULL );

			aPipePtr->clearRow( theRow );
		}

		int theRow;
};


struct  is_pipe_empty: unary_function< TPipePtr, bool >
{
	bool operator()( TPipePtr aPipePtr )
	{
		return	( aPipePtr->isEmpty() == false );
	}
};


struct  set_block_size
{
	set_block_size( int nBlockSize ) : theBlockSize( nBlockSize ) {}

	void operator()( TPipePtr aPipePtr )
	{
		aPipePtr->setBlockSize( theBlockSize );
	}


	int theBlockSize;
};



struct  clear_pipe: public unary_function<const TPipePtr, void>
{
	void operator()( const TPipePtr ptr )
	{
		ptr->clear();
	}
};

// end of pipew functors.




struct equal_score: public binary_function< ScoreDataCref, ScoreDataCref, bool >
{	
	bool	operator()( ScoreDataCref x, int nScore) const
	{
		return ( x.getScore() == nScore );
	} 
};

/*
struct ScoreDisplay: public unary_function< ScoreDataCref, bool >
{
	bool	operator()( ScoreDataCref x ) const
	{
		cerr << x.getName() << '\t' << x.getScore() << endl;
		return true;
	} 
};
*/


// end of scorefile functors


//	rollw functors
//	������ �䷯���� �����̴� �Լ� ��ü
struct  move_toruses: public unary_function< TRoller *, void >
{
	move_toruses( int nLevel ) : m_nLevel( nLevel ) {}
	void operator()( TRoller * aRollerPtr )
	{
		aRollerPtr->roll( m_nLevel );
	}

	int m_nLevel;
};


//	������ ���� ������ ��
//	������ �䷯���� ����.(�ٽ� ������ ��)
struct  wrap_torus: public unary_function< TRoller *, void >
{
	void operator()( TRoller * aRollerPtr ) 
	{
		aRollerPtr->wrapTorus();
	}
};


//	�䷯���� �ٴ��� �ƴ���...�� ���� �˻�. hit test
/*
struct  hit_bottom: public unary_function< TRoller *, void >
{
	hit_bottom( const PipeWindow *aPipeWindowPtr ) : m_PipeWindowPtr( aPipeWindowPtr )
	{
	}


	void operator()( TRoller * aRollerPtr ) const
	{
		if( aRollerPtr->getTorusYPoint() > m_RollerHeight )
		{
			m_PipeWindowPtr->push_back( 
		}
	}
private:
	PipeWindow *m_PipeWindowPtr;
	int			m_RollerHeight;
};
*/


//	TRoller�� ���̸� ���ߴ� �Լ� ��ü
//	6.18, ������ ũ�� Ȥ�� ������ ����� ��� ������ ũ�⸦ �ٽ� �����ؾ� ��.
struct  set_roller_height: public unary_function< TRoller *, void >
{
	set_roller_height( int nHeight ): m_nHeight( nHeight )
	{
	}

	void operator()( TRoller * aRollerPtr )
	{
		aRollerPtr->setHeight( m_nHeight );
	}

	int m_nHeight;
};