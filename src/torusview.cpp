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


//	toruses_xpm 에서 납작한 토러스 위치	x
const int	nFlatTorusPosX	=	TORUS_WIDTH * (MAX_IMAGE_COUNT/2);

//	toruses_xpm 에서 토러스 색상 위치	y
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

	//	점수 계산을 위해 TorusView의 포인터를 넘긴다.
	m_pTI	=	new TorusInternal( this );

	m_GameState = GAME_OVER;
	setLevel( 3 );
	m_nDifficulty = 3;
	//	
	initGame();
	//updateView();

	//	!! 롤러의 크기를 조절해야 한다.
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
	//	게임을 시작할 준비
	m_nScore = 0;
	m_nLine = 0;	

	//	게임 진행 상태를 초기화
	resetStep();

	//	기본적으로 레벨은 3 을 설정한다.
	
	//	사용자 다이얼로그를 통한 사용자 레벨을 적용한다.
	m_pTI->init( getLevel(), getDifficulty() );
	updateView();


	//	로고 표시할 때.. 플레이어들도 표시..
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

	//	각각의 논리적인 뷰를 그린다.
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


	//	Refresh를 하지 않으면 키를 계속 누르는 경우 지연 시간이 많이 생긴다.
	//	타이머에 따라 갱신하도록 내버려 두면 된다. 어차피 눈이 따라가지 못하는 
	//	시간이니까.
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
		//	vc 6.0 의 deque는.. push_front가 아래쪽에 위치한다고 보면 된다.
		//	 그래서.. 핀의 데이터를 올릴때.. push_front로 넣어야 한다.
		int nTorusColor =  m_pTI->topPin();

		//	clearPipe와 gainPoint를 파이프에서 수행하기 때문에,
		//	pop을 먼저 하고 PipeWindow에 넣어야 한다.
		m_pTI->popPin();

		//	drawPipe();  // this function must be called here, in order to give draw_clear_line effect
		//	but currently it is called in torusinternal.cpp

		m_pTI->push_front( aPinPos, nTorusColor );
		
	}
}

//	Pin에 빈 자리가 있는 경우에만...
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

	//	점수판의 크기도 넣어야 할 것..
	int nBaseY	=	m_ScoreSize.GetHeight() + m_RollerSize.GetHeight() + m_PipeSize.GetHeight();
	int nBaseX	=	cs.GetWidth()/2 - m_PinSize.GetWidth()/2;

	//	make torus hidden
	dc.DrawRectangle(	nBaseX, nBaseY, m_PinSize.GetWidth(), m_PinSize.GetHeight() );


	int nVOffset = nBaseY + m_PinSize.GetHeight();	

	int nPinPos = m_pTI->getPinPos() * TORUS_WIDTH;


	//	###### Grid 그리기..
	/*
	nVOffset = nBaseY + m_PinSize.GetHeight();	
	for( i = 0; i < m_PinSize.GetHeight()/10; i ++ )
	{
		
		nVOffset -= TORUS_HEIGHT; 
		dc.DrawRectangle( nPinPos, nVOffset, TORUS_WIDTH, TORUS_HEIGHT );
	}
	*/

	mdc.SelectObject(bitmap_pin);
	
	//	PinSize와 PinPos를 이용해서 알맞은 위치에 핀을 그린다.
	dc.Blit( nBaseX + ( TORUS_WIDTH/2 - bitmap_pin.GetWidth()/2 ) + nPinPos, nBaseY, TORUS_WIDTH, m_PinSize.GetHeight(),
				&mdc, 0, 0, wxCOPY, TRUE );

	//	핀을 그리기 위해 세로 오프셋을 조절
	nVOffset = nBaseY + m_PinSize.GetHeight();		
	
	mdc.SelectObject(bitmap_sidebar);

	//	블럭을 그린다
	for( i = m_pTI->getPinBlockSize(); i > 0; i-- )
	{
		nVOffset -= TORUS_HEIGHT;
		dc.Blit( nBaseX + nPinPos, nVOffset, TORUS_WIDTH, TORUS_HEIGHT, &mdc, 0, 0 );
	}	


	mdc.SelectObject( bitmap_toruses );


	//	핀에 쌓인 토러스를 그린다. 그릴 때.. xy 좌표계를 사용하기 때문에 
	//	왼쪽 위에서 시작하는 것을 기준으 삼는다. 따라서 nVOffset을 미리 설정한다.
	nVOffset = nBaseY + m_PinSize.GetHeight() - TORUS_HEIGHT * m_pTI->getPinBlockSize();
	//	핀에 쌓인 토러스의 개수를 가져와서
	int nTorusSize = m_pTI->getPinTorusSize();
	for( i = 0; i < nTorusSize; i ++ )
	{
		//	아래쪽부터 토러스의 색상을 가져온다
		int nTorusColor = m_pTI->getPinTorusColor( i );		

		//	토러스를 겹쳐 그리기 위해 세로 오프셋을 조절
		nVOffset -= TORUS_HEIGHT; 

	
	
		dc.Blit( nBaseX + nPinPos,	//	lefttop
				nVOffset,	//		righttop
				TORUS_WIDTH,
				TORUS_HEIGHT,
				&mdc,
				nFlatTorusPosX,						//	납작한 토러스 위치	x
				nFlatTorusPosY( nTorusColor ),		//	토러스 색상 위치    y
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


//	최초 파이프의 크기는 80픽셀이다.
void TorusView::drawPipe( wxDC &dc )
{
	wxMemoryDC	mdc;
	wxSize		cs;

	cs	= GetClientSize();

	int i;
	int nBaseY = m_RollerSize.GetHeight() + m_ScoreSize.GetHeight() - 1; // 1 픽셀 만큼 띄웠었잖아!! 아래의 updateView를 보시길.
	int nBaseX	=	cs.GetWidth()/2 - m_PipeSize.GetWidth()/2;
	
	
	//	토러스가 파이프로 떨어질때 밖으로 보이는 것을 방지.
	//	이를 위해서.. Roller를 가장 먼저 그리고, 그 후에 Score와 파이프를 그려야 한다.

	/*
	wxPen		pen( wxColour( 255, 0, 255 ), 1, wxDOT );
	dc.SetPen( pen );
	*/

	//  토러스가 그려지는 것을 방지 
	dc.DrawRectangle( nBaseX, nBaseY, m_PipeSize.GetWidth(), m_PipeSize.GetHeight() );


	/*
			|  |  |   ^
			|  |  |   |
			|  |  |   | getPipeSize()
			|  |  |   / 
		   ---------
	*/
	//	현재 파이프의 개수
	for( int aCol = 0; aCol < m_pTI->getPipeSize(); aCol++ )
	{
		//TPipe	*aPipePtr = m_pTI->at( aCol );
		//	debug 파이프에 grid 그리기
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

		//	sidebar를 먼저 메모리DC로 선택하고
		mdc.SelectObject(bitmap_sidebar);
		
		//	블럭된 개수를 그린다		
		for( i = 0; i < m_pTI->getPipeBlockSize(); i++ )
		{						
			//	BaseX, BaseY 만큼 떨어진 것을 더해야 한다.
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
		//	이제 토러스를 선택하고 누워있는 토러스를 그리면 하나의 파이프가 완성된다.
		mdc.SelectObject( bitmap_toruses );

		
		//	aCol(umn)의 파이프가 가지고 있는 토러스의 개수를 가져온다.
		int nVOffset2 = nBaseY + m_PipeSize.GetHeight() - TORUS_HEIGHT;
		int nTorus = m_pTI->getSize( aCol );
		for(  i = 0; i < nTorus; i++ )
		{
			int nTorusColor = m_pTI->getTorusColor( aCol, i );

			
			dc.Blit(nBaseX + TORUS_WIDTH * aCol,	//	lefttop
					nVOffset2,		//		righttop --- nVOffset2에는 BaseY가 포함되었음
					TORUS_WIDTH,
					TORUS_HEIGHT,
					&mdc,
					nFlatTorusPosX,						//	납작한 토러스 위치	x
					nFlatTorusPosY( nTorusColor ),		//	토러스 색상 위치    y
					wxCOPY,		//	COPY bit blit operation
					FALSE		//	Transparency
					);			

			//	토러스를 겹쳐 그리기 위해 세로 오프셋을 조절
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

	//wxString wxsTorusPosition = "떨어지는 토러스의 위치는...";
	int		nPosBuffer[20];	//	 20 개면 충분한 개수지,... 지금까지 8개 이상을 본 적이 없다..

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

	//	오프 스크린 버퍼로 쓸것.
	wxBitmap	bmPrimaryBuffer( cs.GetWidth(), cs.GetHeight() );

	wxMemoryDC	mdc;

	mdc.SelectObject( bmPrimaryBuffer );
	mdc.Clear();	//	make background whitening

	//	각각의 객체에 대한 화면을 그린다	
	mdc.SetBrush(*wxBLACK_BRUSH);
	mdc.SetPen( *wxBLACK);

	drawRoller( mdc );
	drawScore( mdc );
	drawPipe( mdc );
	drawPin( mdc );

	//	오프 스크린에 그려진 데이터를
	//	 화면으로 
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
	
	//	TRICK을 증가시킬때...	
	if( ( m_nLine % TORUS_COUNT_TRICK ) == 0 )
	{
		step();		//	스텝 함수
	}


}


void TorusView::drawClearLine( int row )
{
	wxClientDC cdc(this);

	wxSize cs = GetClientSize();

//	int i;
	int nBaseY = m_RollerSize.GetHeight() + m_ScoreSize.GetHeight() + m_PipeSize.GetHeight() - 3; // 사이띄개.. see updateView
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

	//	게임 오버와 동시에 설정의 레벨로 변경.
	//m_Level = getLevel();

	
	//	top players 을 기록한 경우, 점수 입력.	
	m_pScoreFile	= new TScoreFile;

	//	TScoreFile의 sort predicate인 MinScore과 상응해야 한다.
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

		//	블럭이 최고 개수에 도달했을 때.
		if( m_PipeBlockSize % TORUS_MAX_PIPE_BLOCK == 0 )
		{
			m_PinBlockSize++;
			m_PipeBlockSize = 1;

			//	핀이 최고 개수에 도달했을 때.
			if( m_PinBlockSize % TORUS_MAX_PIN_BLOCK == 0 )
			{
					m_pTI->increaseLevel();

					//	뷰를 갱신해야.. 하는데.. 
					//	 이 함수를 step안에 집어 넣는 것이 과연 괜찮은 일일까?
					//	일단 넣어 두기로 하고.. 나중에 살피자.
					updateView();
					//	OnLevelUp();
					resetStep();

					return ;

			}
		}


		//	BlockSize에 변화가 있을 때만 호출.
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



//	레벨이 오를 때 마다 호출해서 적당한 크기를 조절한다..
void TorusView::updateView()
{
	//	현재 레벨에 따라 조절한다.
	int nLevel	=	m_pTI->getLevel();

	//	score
	m_ScoreSize.Set(	TORUS_WIDTH * nLevel, 20 );	 // height : 20

	//	width :  , height : nLevel * 10 + 50 = 80
	m_PipeSize.Set(		TORUS_WIDTH * nLevel, 
						nLevel*TORUS_HEIGHT + 50 );	//	3, 7 torus & 1 block guide = 80

	//	핀의 기본 크기	
	//	+1은 pipe와 핀과의 거리를 위해서이다.
	m_PinSize.Set(		TORUS_WIDTH * nLevel, 
						nLevel*TORUS_HEIGHT + 20 + 1 );	//	최초 5 개의 토러스 + 파이프와의 기본 거리 1 픽셀 = 51

	m_RollerSize.Set(	TORUS_WIDTH * nLevel,						
						TORUS_MAX_HEIGHT - ( m_PinSize.GetHeight() + m_PipeSize.GetHeight() + m_ScoreSize.GetHeight() ) );

	//	롤러의 크기를 조절해야...
	m_pTI->setRollerHeight( m_RollerSize.GetHeight() );
}


void TorusView::OnSize( wxSizeEvent &)
{
//	wxSize s = GetClientSize();
}


// vim:ts=2:sw=2
