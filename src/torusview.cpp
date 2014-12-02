#include "torusinternal.h"
#include "torusview.h"
#include "wxtorus.h"
#include "scorefile.h"

#include "top10dialog.h"
#include "scorepanel.h"


#include "wx/mstream.h"
#include "wx/image.h"
#include "wx/utils.h"

#include "../res/toruses.h"
#include "../res/sidebar.h"
#include "../res/pin.h"


	
const int TIMER_ID = 10000; 

//	wx event table
BEGIN_EVENT_TABLE( TorusView, wxPanel )
	EVT_PAINT( 		TorusView::OnPaint )
	EVT_KEY_DOWN( TorusView::OnKeyDown )
	//EVT_NAVIGATION_KEY (TorusView::OnNavigationKey)
	EVT_SIZE( 		TorusView::OnSize )
	EVT_TIMER(TIMER_ID, TorusView::OnTimer )
END_EVENT_TABLE()


//	toruses_xpm ���� ������ �䷯�� ��ġ	x
const int	nFlatTorusPosX	=	TORUS_WIDTH * (MAX_IMAGE_COUNT/2);

//	toruses_xpm ���� �䷯�� ���� ��ġ	y
#define		nFlatTorusPosY( nColor )	(nTorusColor * TORUS_WIDTH + 20)

//	level and trick
const int		TORUS_COUNT_TRICK	=	8;

//	Pipe & Pin trick level adjustment
const int		TORUS_COUNT_LEVEL	=	82;	//	9 * 9

const int		TORUS_MAX_PIPE_BLOCK	=	4;
const int		TORUS_MAX_PIN_BLOCK		=	4;
				



TorusView::TorusView( wxWindow *parent, wxPoint point, wxSize size )
:wxPanel( parent, wxID_ANY, point, size )
{
	SetClientSize(wxSize(TORUS_MAX_WIDTH, TORUS_MAX_HEIGHT));

	//	���� ����� ���� TorusView�� �����͸� �ѱ��.
	m_pTI	=	new TorusInternal( this );

	m_GameState = GAME_OVER;
	setLevel( 3 );
	m_nDifficulty = 3;
	//	
	initGame();
	//updateView();

	//	!! �ѷ��� ũ�⸦ �����ؾ� �Ѵ�.
	m_pTI->setRollerHeight( m_RollerSize.GetHeight() );

	m_pTimer	=	new wxTimer( this, TIMER_ID );



	//
	wxMemoryInputStream istream(toruses_png, sizeof(toruses_png));
	wxImage torus_image(istream, wxBITMAP_TYPE_PNG);
	torus_image.SetMask(true);
	bitmap_toruses = wxBitmap(torus_image);

	wxMemoryInputStream istream_2(sidebar_png, sizeof(sidebar_png));
	bitmap_sidebar = wxBitmap(wxImage(istream_2, wxBITMAP_TYPE_PNG));
	
	wxMemoryInputStream istream_3(pin_png, sizeof(pin_png));
	bitmap_pin = wxBitmap(wxImage(istream_3, wxBITMAP_TYPE_PNG));


	//this->Connect(wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler(TorusView::OnNavigationKey), (wxObject*) NULL, this);
	//new wxLogWindow(this, "log");

	//
	//	test code
	/*
	m_pTI->setPipeBlockSize( 2 );

	m_pTI->push_back( 0, 0 );
	m_pTI->push_back( 1, 1 );
	m_pTI->push_back( 2, 2 );
	*/
}


void TorusView::initGame()
{
	//	������ ������ �غ�
	m_nScore = 0;
	m_nLine = 0;	

	//	���� ���� ���¸� �ʱ�ȭ
	resetStep();

	//	�⺻������ ������ 3 �� �����Ѵ�.
	
	//	����� ���̾�α׸� ���� ����� ������ �����Ѵ�.
	m_pTI->init( getLevel(), getDifficulty() );
	updateView();


	//	�ΰ� ǥ���� ��.. �÷��̾�鵵 ǥ��..
	//readTopPlayer();	
}



TorusView::~TorusView()
{
	if( m_pTimer->IsRunning() == TRUE )
		m_pTimer->Stop();

	zap( m_pTimer );
	zap( m_pTI );


}


void TorusView::OnPaint( wxPaintEvent &)
{
	wxPaintDC pdc( this );

	//	������ ������ �並 �׸���.
//	if( m_GameState != GAME_RUN )
//		drawLogo( pdc );
//	else
		draw( pdc );
}


void TorusView::OnKeyDown( wxKeyEvent &event )
{
	//long keycode = event.GetKeyCode();	
	long keycode = event.GetUnicodeKey();
	
	switch( keycode )
	{
		case WXK_LEFT:
		case 'H':
		case '4':
			m_pTI->movePinLeft();
			break;

		case WXK_RIGHT:
		case 'L':
		case '6':
			m_pTI->movePinRight();
			break;

		case WXK_UP:
		case 'K':
		case '8':
			OnUp();
			break;

		case WXK_DOWN:
		case 'J':
		case '5':
		case '2':
			OnDown();
			break;

		//	Control key strokes
		case WXK_SPACE: 
			switch( m_GameState )
				{
						MyFrame *frame;
						
					case GAME_RUN:
						pauseGame();

	//					frame = (MyFrame*)wxGetApp().GetTopWindow();					
	//					frame->SwitchPanel();
						Refresh( FALSE );
						break;

					case GAME_PAUSE:
						resume();

	//					frame = (MyFrame*)wxGetApp().GetTopWindow();
	//					frame->SwitchPanel();
						Refresh( FALSE );
						break;

					case GAME_OVER:
						//MyFrame *f = (MyFrame *) GetParent();
						startGame();
						break;
				}
		 
		
			break;

	case WXK_ESCAPE:
		//if( m_GameState == GAME_PAUSE )
		//if( m_GameState == GAME_RUN )
		{
			stopGame();			
			MyFrame *frame = (MyFrame*)wxGetApp().GetTopWindow();
			frame->SwitchPanel();
			frame->SetStatusText("");
		}
		/*
		else if ( m_GameState == GAME_RUN )
		{
			pauseGame();			
		}
		*/
		Refresh( FALSE );
		break;


	// Following function keys must be disabled in release build
#ifdef _DEBUG
	//	debug
	case WXK_F2:
		m_pTI->increaseLevel();
		updateView();
		break;
	case WXK_F3:
		m_pTI->decreaseLevel();
		updateView();
		break;

	case WXK_F4:
		step();		
		updateView();
		break;	

	case WXK_F6:
	{
		Top10Dialog	td(this );

		if( td.ShowModal() == wxID_OK )
		{
			wxMessageBox( td.m_wxsName );
		}
		break;
	}

	case WXK_F9:
		OnGameOver();
		Refresh();
		break;
#endif

	default: 
			event.Skip();
			break;
	}


	//	Refresh�� ���� ������ Ű�� ��� ������ ��� ���� �ð��� ���� �����.
	//	Ÿ�̸ӿ� ���� �����ϵ��� ������ �θ� �ȴ�. ������ ���� ������ ���ϴ� 
	//	�ð��̴ϱ�.
	//Refresh( FALSE );
}


void TorusView::OnNavigationKey (wxKeyEvent& event)
{
	this->OnKeyDown(event);
   /*if (wxGetKeyState (WXK_UP)) wxLogMessage ("Up arrow key pressed");
   if (wxGetKeyState (WXK_DOWN)) wxLogMessage ("Down arrow key pressed");
   if (wxGetKeyState (WXK_LEFT)) wxLogMessage ("Left arrow key pressed");
   if (wxGetKeyState (WXK_RIGHT)) wxLogMessage ("Right arrow key pressed");
   if (wxGetKeyState (WXK_TAB)) wxLogMessage ("Tab key pressed");
   */
}



void TorusView::OnUp()
{
	int aPinPos = m_pTI->getPinPos();

	if( m_pTI->isPinEmpty() != true &&
		m_pTI->isPipeFull( aPinPos ) != true )
	{
		//	vc 6.0 �� deque��.. push_front�� �Ʒ��ʿ� ��ġ�Ѵٰ� ���� �ȴ�.
		//	 �׷���.. ���� �����͸� �ø���.. push_front�� �־�� �Ѵ�.
		int nTorusColor =  m_pTI->topPin();

		//	clearPipe�� gainPoint�� ���������� �����ϱ� ������,
		//	pop�� ���� �ϰ� PipeWindow�� �־�� �Ѵ�.
		m_pTI->popPin();

		//	drawPipe();  // this function must be called here, in order to give draw_clear_line effect
		//	but currently it is called in torusinternal.cpp

		m_pTI->push_front( aPinPos, nTorusColor );
		
	}
}

//	Pin�� �� �ڸ��� �ִ� ��쿡��...
void TorusView::OnDown()
{
	int aPinPos = m_pTI->getPinPos();

	if( m_pTI->isPinFull() != true &&
		m_pTI->isPipeEmpty( aPinPos ) != true )
	{
		m_pTI->pushPin( m_pTI->front( aPinPos ) );
		m_pTI->pop_front( aPinPos );
	}
}



void TorusView::drawPin( wxDC &dc )
{	
	wxMemoryDC	mdc;
	wxSize		cs;
	int i;

	cs = GetClientSize();

	//	�������� ũ�⵵ �־�� �� ��..
	int nBaseY	=	m_ScoreSize.GetHeight() + m_RollerSize.GetHeight() + m_PipeSize.GetHeight();
	int nBaseX	=	cs.GetWidth()/2 - m_PinSize.GetWidth()/2;

	//	make torus hidden
	dc.DrawRectangle(	nBaseX, nBaseY, m_PinSize.GetWidth(), m_PinSize.GetHeight() );


	int nVOffset = nBaseY + m_PinSize.GetHeight();	

	int nPinPos = m_pTI->getPinPos() * TORUS_WIDTH;


	//	###### Grid �׸���..
	/*
	nVOffset = nBaseY + m_PinSize.GetHeight();	
	for( i = 0; i < m_PinSize.GetHeight()/10; i ++ )
	{
		
		nVOffset -= TORUS_HEIGHT; 
		dc.DrawRectangle( nPinPos, nVOffset, TORUS_WIDTH, TORUS_HEIGHT );
	}
	*/

	mdc.SelectObject(bitmap_pin);
	
	//	PinSize�� PinPos�� �̿��ؼ� �˸��� ��ġ�� ���� �׸���.
	dc.Blit( nBaseX + ( TORUS_WIDTH/2 - bitmap_pin.GetWidth()/2 ) + nPinPos, nBaseY, TORUS_WIDTH, m_PinSize.GetHeight(),
				&mdc, 0, 0, wxCOPY, TRUE );

	//	���� �׸��� ���� ���� �������� ����
	nVOffset = nBaseY + m_PinSize.GetHeight();		
	
	mdc.SelectObject(bitmap_sidebar);

	//	���� �׸���
	for( i = m_pTI->getPinBlockSize(); i > 0; i-- )
	{
		nVOffset -= TORUS_HEIGHT;
		dc.Blit( nBaseX + nPinPos, nVOffset, TORUS_WIDTH, TORUS_HEIGHT, &mdc, 0, 0 );
	}	


	mdc.SelectObject( bitmap_toruses );


	//	�ɿ� ���� �䷯���� �׸���. �׸� ��.. xy ��ǥ�踦 ����ϱ� ������ 
	//	���� ������ �����ϴ� ���� ������ ��´�. ���� nVOffset�� �̸� �����Ѵ�.
	nVOffset = nBaseY + m_PinSize.GetHeight() - TORUS_HEIGHT * m_pTI->getPinBlockSize();
	//	�ɿ� ���� �䷯���� ������ �����ͼ�
	int nTorusSize = m_pTI->getPinTorusSize();
	for( i = 0; i < nTorusSize; i ++ )
	{
		//	�Ʒ��ʺ��� �䷯���� ������ �����´�
		int nTorusColor = m_pTI->getPinTorusColor( i );		

		//	�䷯���� ���� �׸��� ���� ���� �������� ����
		nVOffset -= TORUS_HEIGHT; 

	
	
		dc.Blit( nBaseX + nPinPos,	//	lefttop
				nVOffset,	//		righttop
				TORUS_WIDTH,
				TORUS_HEIGHT,
				&mdc,
				nFlatTorusPosX,						//	������ �䷯�� ��ġ	x
				nFlatTorusPosY( nTorusColor ),		//	�䷯�� ���� ��ġ    y
				wxCOPY,		//	COPY bit blit operation
				TRUE		//	Transparency
				);
	}


	mdc.SelectObject( wxNullBitmap );
}



void TorusView::drawPipe()
{
	wxClientDC cdc(this);
	//cdc.SetPen( *wxWHITE_PEN );
	drawPipe(cdc);
}


//	���� �������� ũ��� 80�ȼ��̴�.
void TorusView::drawPipe( wxDC &dc )
{
	wxMemoryDC	mdc;
	wxSize		cs;

	cs	= GetClientSize();

	int i;
	int nBaseY = m_RollerSize.GetHeight() + m_ScoreSize.GetHeight() - 1; // 1 �ȼ� ��ŭ ������ݾ�!! �Ʒ��� updateView�� ���ñ�.
	int nBaseX	=	cs.GetWidth()/2 - m_PipeSize.GetWidth()/2;
	
	
	//	�䷯���� �������� �������� ������ ���̴� ���� ����.
	//	�̸� ���ؼ�.. Roller�� ���� ���� �׸���, �� �Ŀ� Score�� �������� �׷��� �Ѵ�.

	/*
	wxPen		pen( wxColour( 255, 0, 255 ), 1, wxDOT );
	dc.SetPen( pen );
	*/

	//  �䷯���� �׷����� ���� ���� 
	dc.DrawRectangle( nBaseX, nBaseY, m_PipeSize.GetWidth(), m_PipeSize.GetHeight() );


	/*
			|  |  |   ^
			|  |  |   |
			|  |  |   | getPipeSize()
			|  |  |   / 
		   ---------
	*/
	//	���� �������� ����
	for( int aCol = 0; aCol < m_pTI->getPipeSize(); aCol++ )
	{
		//TPipe	*aPipePtr = m_pTI->at( aCol );
		//	debug �������� grid �׸���
		/*
		wxPen		pen( wxColour( 0, 0, 255 ), 1, wxDOT );
		dc.SetPen( pen );
		int nVOffset = nBaseY + m_PipeSize.GetHeight();
		for( i = 0; i < m_PipeSize.GetHeight()/10; i ++ )
		{		
			nVOffset -= TORUS_HEIGHT; 
		//	dc.DrawRectangle( TORUS_WIDTH * aCol, nVOffset, TORUS_WIDTH, TORUS_HEIGHT );
		}
		*/

		//	sidebar�� ���� �޸�DC�� �����ϰ�
		mdc.SelectObject(bitmap_sidebar);
		
		//	���� ������ �׸���		
		for( i = 0; i < m_pTI->getPipeBlockSize(); i++ )
		{						
			//	BaseX, BaseY ��ŭ ������ ���� ���ؾ� �Ѵ�.
			dc.Blit(	nBaseX + TORUS_WIDTH * aCol,
						nBaseY + TORUS_HEIGHT * i,
						TORUS_WIDTH , 
						TORUS_HEIGHT,
						&mdc,
						0,
						0,
						wxCOPY,
						FALSE	//	transparent
						);
		}
		
		
		//	draw toruses from bottom to top.		
		//	���� �䷯���� �����ϰ� �����ִ� �䷯���� �׸��� �ϳ��� �������� �ϼ��ȴ�.
		mdc.SelectObject( bitmap_toruses );

		
		//	aCol(umn)�� �������� ������ �ִ� �䷯���� ������ �����´�.
		int nVOffset2 = nBaseY + m_PipeSize.GetHeight() - TORUS_HEIGHT;
		int nTorus = m_pTI->getSize( aCol );
		for(  i = 0; i < nTorus; i++ )
		{
			int nTorusColor = m_pTI->getTorusColor( aCol, i );

			
			dc.Blit(nBaseX + TORUS_WIDTH * aCol,	//	lefttop
					nVOffset2,		//		righttop --- nVOffset2���� BaseY�� ���ԵǾ���
					TORUS_WIDTH,
					TORUS_HEIGHT,
					&mdc,
					nFlatTorusPosX,						//	������ �䷯�� ��ġ	x
					nFlatTorusPosY( nTorusColor ),		//	�䷯�� ���� ��ġ    y
					wxCOPY,		//	COPY bit blit operation
					FALSE		//	Transparency
					);			

			//	�䷯���� ���� �׸��� ���� ���� �������� ����
			nVOffset2 -= TORUS_HEIGHT; 
		}		
	}
}



void TorusView::drawRoller( wxDC &dc )
{
	wxSize		cs;

	cs = GetClientSize();

	int nBaseY	=	m_ScoreSize.GetHeight();
	int nBaseX	=	cs.GetWidth()/2 - m_RollerSize.GetWidth()/2;

	//	erase background with previously set brush.
	// remove below comment if you want to see torus animations
	dc.DrawRectangle( nBaseX, nBaseY, m_RollerSize.GetWidth(), m_RollerSize.GetHeight() );


	//	torus dc
	wxMemoryDC	mdc;

	mdc.SelectObject( bitmap_toruses );

	//wxString wxsTorusPosition = "�������� �䷯���� ��ġ��...";
	int		nPosBuffer[20];	//	 20 ���� ����� ������,... ���ݱ��� 8�� �̻��� �� ���� ����..

	for( int aCol = 0; aCol < m_pTI->getLevel(); aCol++ )
	{
		//	draw toruses from bottom to top.
		int nTorusColor = m_pTI->getTorusColor( aCol );
		int nTorusYPos	= m_pTI->getTorusPosY( aCol );
		int nImageIndex = m_pTI->getTorusImageIndex( aCol );

		dc.Blit(nBaseX + TORUS_WIDTH * aCol,	//	lefttop
				nBaseY + nTorusYPos,			//		righttop
				TORUS_WIDTH,
				TORUS_WIDTH,
				&mdc,
				TORUS_WIDTH * nImageIndex,	//	image index
				nTorusColor * TORUS_WIDTH,
				wxCOPY,
				FALSE	// transparent
				);

	
		nPosBuffer[ aCol ] = nTorusYPos;	

		//wxsTorusPosition += wxString::Format(" [%d] ", nTorusYPos);
	}

	//((wxFrame*)GetParent())->SetStatusText( wxsTorusPosition, 1);
}


void TorusView::drawScore( wxDC &dc )
{
	wxSize		cs;
	wxString	wxsScore;
	int w, h;

	cs = GetClientSize();

	int nBaseY	=	0;
	int nBaseX	=	cs.GetWidth()/2 - m_ScoreSize.GetWidth()/2;

	 dc.DrawRectangle( nBaseX, nBaseY, m_ScoreSize.GetWidth(), m_ScoreSize.GetHeight() );

	dc.SetTextForeground(*wxWHITE);
	wxsScore.Printf("%#08d", m_nScore );
	//	draw a score at center.
	dc.GetTextExtent( wxsScore, &w, &h );	
	dc.DrawText( wxsScore, nBaseX + m_ScoreSize.GetWidth()/2 - w/2, nBaseY );
}


void TorusView::draw()
{
	wxClientDC cdc( this );	
	draw( cdc );
}


void TorusView::draw( wxDC & dc )
{
	wxSize		cs;	//	client size

	cs = GetClientSize();

	//	���� ��ũ�� ���۷� ����.
	wxBitmap	bmPrimaryBuffer( cs.GetWidth(), cs.GetHeight() );

	wxMemoryDC	mdc;

	mdc.SelectObject( bmPrimaryBuffer );
	mdc.Clear();	//	make background whitening

	//	������ ��ü�� ���� ȭ���� �׸���	
	mdc.SetBrush(*wxBLACK_BRUSH);
	mdc.SetPen( *wxBLACK);

	drawRoller( mdc );
	drawScore( mdc );
	drawPipe( mdc );
	drawPin( mdc );

	//	���� ��ũ���� �׷��� �����͸�
	//	 ȭ������ 
	dc.Blit( 0, 0, cs.GetWidth(), cs.GetHeight(), &mdc, 0, 0 );
}


//
//	gainpoint
void TorusView::gainPoint( int row, ClearMethod aClearMethod, bool isAllClear )
{
	const int BASE_POINT = 600;
	const int LEVEL_POINT = 300;
	int ALL_CLEAR_POINT = 0;
	int ROW_POINT = 0;
	int METHOD_POINT = 0;
	int current_point = 0;;
	const int nLevelModifier = m_pTI->getLevel();

	
	METHOD_POINT = BASE_POINT + ( (nLevelModifier - 2) * LEVEL_POINT);

	if( aClearMethod == TORUS_CLEAR_ROW_BY_PIN )
	{
		METHOD_POINT /= 2;		
	}

	 // row modifier
	if( row > 0 )
	{
		ROW_POINT = row * 200 * (nLevelModifier-2);
	}

	if( isAllClear == true )
	{
		//static int acPoints[5] = { 3000, 12000, 27000, 48000, 75000, ... };

		//	series of  all clear is..
		//	An = 3n^2
		ALL_CLEAR_POINT = 3000 * ( nLevelModifier  - 2 ) * ( nLevelModifier  - 2 );
		
	}


	current_point = METHOD_POINT + ROW_POINT + ALL_CLEAR_POINT;

	m_nScore += current_point;	
	m_nLine++;

	
	//	status display
	wxString wxsMessage;
	wxString wxsModifier;
	/*
	wxsMessage.Printf("[%#03d] %s %s = %d", 
							m_nLine, 							
							( aClearMethod == TORUS_CLEAR_ROW_BY_PIN ) ? "Push & Pull" : "Fall",
							(isAllClear == true ) ? "+ ALL CLEAR !" : "",
							current_point		//	score
							);
  */

	wxsMessage.Printf("Cleared Line %#03d : ", m_nLine);
	wxsModifier.Printf("%s(%d)", 
				( aClearMethod == TORUS_CLEAR_ROW_BY_PIN ) ? "Push or Pull" : "Fall", 
				METHOD_POINT );
	wxsMessage += wxsModifier;
		
	if( row > 0 )
	{
		wxsModifier.Printf(" + Row Bonus(%d)", ROW_POINT );
		wxsMessage += wxsModifier;
	}

	if( isAllClear == true )
	{
		wxsModifier.Printf(" + ALL CLEAR!(%d)", ALL_CLEAR_POINT);
		wxsMessage += wxsModifier;
	}

	wxsModifier.Printf(" = %d", current_point );
	wxsMessage += wxsModifier;

	MyFrame *f = (MyFrame*)wxGetApp().GetTopWindow();
	f->SetStatusText( wxsMessage );

	// pause timer for about 99 ms.	
	drawClearLine( row );
	m_pTimer->Stop();

	Sleep(99);	//	sleep for 99 ms to show cleared line	
	m_pTimer->Start();	//	restart with previous timer value 

	// resume timer
	
	//	TRICK�� ������ų��...	
	if( ( m_nLine % TORUS_COUNT_TRICK ) == 0 )
	{
		step();		//	���� �Լ�
	}


}


void TorusView::drawClearLine( int row )
{
	wxClientDC cdc(this);

	wxSize cs = GetClientSize();

//	int i;
	int nBaseY = m_RollerSize.GetHeight() + m_ScoreSize.GetHeight() + m_PipeSize.GetHeight() - 3; // ���̶簳.. see updateView
	int nBaseX	=	cs.GetWidth()/2 - m_PipeSize.GetWidth()/2;
	
	
	wxPen		pen( *wxWHITE, 2, wxLONG_DASH );
	cdc.SetPen( pen );

	cdc.DrawLine(	nBaseX, nBaseY - TORUS_HEIGHT * row - 2, 
					nBaseX + m_PipeSize.GetWidth(), nBaseY - TORUS_HEIGHT * row - 8 );
	
}



/*! \fn void TorusView::OnTimer( wxTimerEvent & event )
    \brief Callback function for timer.
	\return NONE
*/
void TorusView::OnTimer( wxTimerEvent & )
{
//	m_pTI->moveTorus();
	int retcode = m_pTI->moveTorus();
	if( retcode < 0 )
	{
		Refresh( FALSE );
	}
	else
	{
		OnGameOver();		
	}	
}






//	torus state transition diagram
//	      space           space
//	STOP -------->  RUN  -------> PAUSE
//	STOP <--------  RUN  <------- PAUSE
//			   ESC          space or ESC
/*
void	TorusView::autoGameState()
{
	if( m_GameState == STOP )
}
*/

void	TorusView::startGame()
{	
	initGame(); 

	m_pTimer->Start(30); 

	m_GameState	=	GAME_RUN;

	MyFrame *f = (MyFrame*)wxGetApp().GetTopWindow();

	wxString message; 
	message.Printf("Start game!");
	f->SetStatusText( message ); 
}

void	TorusView::stopGame()
{
	m_pTimer->Stop(); 
	m_GameState	=	GAME_OVER;
}

//	implement timer only
void	TorusView::resume()
{
		if( m_GameState == GAME_PAUSE)
		{
				m_pTimer->Start();
				m_GameState	=	GAME_RUN;
		}
}

//	implement timer only
void	TorusView::pauseGame()
{
	//if( m_pTimer->IsRunning() == TRUE )
	if( m_GameState == GAME_RUN )
	{
		m_pTimer->Stop();
		m_GameState	=	GAME_PAUSE;
	}

}



void TorusView::setLevel( int nLevel )
{
	m_nLevel = nLevel;
}

int TorusView::getLevel()
{
	return m_nLevel;
}


void TorusView::OnGameOver()
{
	stopGame();

	//	���� ������ ���ÿ� ������ ������ ����.
	//m_Level = getLevel();

	
	//	top players �� ����� ���, ���� �Է�.	
	m_pScoreFile	= new TScoreFile;

	//	TScoreFile�� sort predicate�� MinScore�� �����ؾ� �Ѵ�.
	if( m_nScore > m_pScoreFile->getMinScore() )
	{
		Top10Dialog	td(this );

		if( td.ShowModal() == wxID_OK )
		{
			//wxMessageBox( td.m_wxsName ); 
			m_pScoreFile->push_back( td.m_wxsName, m_nScore );
		}
		else
		{
			m_pScoreFile->push_back( "Noname", m_nScore );
		}
		m_pScoreFile->write();
	} 
	delete m_pScoreFile;
	

	// 
	MyFrame *f = (MyFrame*)wxGetApp().GetTopWindow();
	f->SwitchPanel();

	wxString message; 
	message.Printf("Thank you for playing wxTorus. Copyright (c) JaeSung Lee");
	f->SetStatusText( message );
}



void TorusView::step()
{
		m_PipeBlockSize++;

		//	���� �ְ� ������ �������� ��.
		if( m_PipeBlockSize % TORUS_MAX_PIPE_BLOCK == 0 )
		{
			m_PinBlockSize++;
			m_PipeBlockSize = 1;

			//	���� �ְ� ������ �������� ��.
			if( m_PinBlockSize % TORUS_MAX_PIN_BLOCK == 0 )
			{
					m_pTI->increaseLevel();

					//	�並 �����ؾ�.. �ϴµ�.. 
					//	 �� �Լ��� step�ȿ� ���� �ִ� ���� ���� ������ ���ϱ�?
					//	�ϴ� �־� �α�� �ϰ�.. ���߿� ������.
					updateView();
					//	OnLevelUp();
					resetStep();

					return ;

			}
		}


		//	BlockSize�� ��ȭ�� ���� ���� ȣ��.
		m_pTI->setPinBlockSize( m_PinBlockSize );
		m_pTI->setPipeBlockSize( m_PipeBlockSize ); 
}

void TorusView::resetStep()
{
	m_PinBlockSize		= 1;
	m_PipeBlockSize		= 1;

	m_pTI->setPinBlockSize( m_PinBlockSize );
	m_pTI->setPipeBlockSize( m_PipeBlockSize ); 

}


void	TorusView::setDifficulty( int nDifficulty )
{
	m_nDifficulty = nDifficulty;
}

int		TorusView::getDifficulty()
{
	return	m_nDifficulty;
}



//	������ ���� �� ���� ȣ���ؼ� ������ ũ�⸦ �����Ѵ�..
void TorusView::updateView()
{
	//	���� ������ ���� �����Ѵ�.
	int nLevel	=	m_pTI->getLevel();

	//	score
	m_ScoreSize.Set(	TORUS_WIDTH * nLevel, 20 );	 // height : 20

	//	width :  , height : nLevel * 10 + 50 = 80
	m_PipeSize.Set(		TORUS_WIDTH * nLevel, 
						nLevel*TORUS_HEIGHT + 50 );	//	3, 7 torus & 1 block guide = 80

	//	���� �⺻ ũ��	
	//	+1�� pipe�� �ɰ��� �Ÿ��� ���ؼ��̴�.
	m_PinSize.Set(		TORUS_WIDTH * nLevel, 
						nLevel*TORUS_HEIGHT + 20 + 1 );	//	���� 5 ���� �䷯�� + ���������� �⺻ �Ÿ� 1 �ȼ� = 51

	m_RollerSize.Set(	TORUS_WIDTH * nLevel,						
						TORUS_MAX_HEIGHT - ( m_PinSize.GetHeight() + m_PipeSize.GetHeight() + m_ScoreSize.GetHeight() ) );

	//	�ѷ��� ũ�⸦ �����ؾ�...
	m_pTI->setRollerHeight( m_RollerSize.GetHeight() );
}


void TorusView::OnSize( wxSizeEvent &)
{
//	wxSize s = GetClientSize();
}


// vim:ts=2:sw=2
