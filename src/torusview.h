#ifndef __TorusView__Included__
#define __TorusView__Included__


#include "wx/wx.h"
#include "global.h"

//#define __THREAD_VERSION 0

class	TorusInternal;
class	TorusThread;
class	TScoreFile;

class ScorePanel;


//	토러스의 게임 진행 상황을 알려 준다.
//	게임의 상황에 따라 적절히 바꿔 주면 된다.
enum GameState { GAME_RUN, GAME_PAUSE, GAME_OVER };		



class TorusView: public wxPanel
{
	DECLARE_EVENT_TABLE()

public:
	TorusView( wxWindow *parent, wxPoint point, wxSize size );
	~TorusView();

private:
	bool bSwitch;

public:	
	
	void resetStep();
	void step();

	//	점수 계산용... torusinternal에서 호출된다.
	void gainPoint( int nLevel, ClearMethod aClearMethod, bool isAllClear );
	
	//	레벨의 증감에 따른 윈도우 크기 변경
	void updateView();

	//	게임 조절  함수
	void	startGame();
	void	stopGame();
	void	pauseGame();
	void	resume();

	void initGame();

	//	pref. dlg. 에 설정된 레벨을 전달
	void	setLevel( int nLevel );
	int		getLevel();

	void	setDifficulty( int nDiff );
	int		getDifficulty();

	void	OnLevelUp();		
	void OnGameOver();


	void	onThreadTimer( );

	void drawPipe();	//	torusinternal에서 호출(clear line 효과)

	void draw();
	void draw( wxDC & dc);

private:
	void drawPin( wxDC &dc );
	
	void drawPipe( wxDC &dc );
	void drawRoller( wxDC &dc );
	void drawScore( wxDC &dc );

	void drawClearLine( int row );	

public:

	// new in wxwidgets 3.0 (why?)
	void OnNavigationKey (wxKeyEvent& event);

	//	U/D Key Process
	void OnDown();
	void OnUp();

	//	key event
	void OnPaint( wxPaintEvent &event );
	void OnKeyDown( wxKeyEvent &event );
	void OnSize( wxSizeEvent &event );

	//	timer event
	void OnTimer( wxTimerEvent & event );

private:
	//ScorePanel *score_panel;
	/// bitmap from xml resources
	wxBitmap	bitmap_toruses, bitmap_pin, bitmap_sidebar, bitmap_logo;

	//	플레이어 표시
	wxString m_wxsPlayerEntry[10];

	//	토러스의 내적 표현
	TorusInternal	*m_pTI;
	
	//	시간.. 똑똑똑똑..	//	혹은 쓰레드를 쓰던지...
	wxTimer			*m_pTimer;


	//	game status
	GameState		m_GameState;
	int			m_nLevel;		//	설정된 레벨
	int			m_nDifficulty;
	//	File object
	//	점수로 파일로 기록하기 위한 클래스
	TScoreFile		*m_pScoreFile;

	
	//	game control variables
	int m_PipeBlockSize;
	int m_PinBlockSize;


	int			m_nScore;
	int			m_nLine;

	wxSize		m_PipeSize;
	wxSize		m_PinSize;
	wxSize		m_RollerSize;
	wxSize		m_ScoreSize;
};


#endif
