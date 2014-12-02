#ifndef __TorusView__Included__
#define __TorusView__Included__


#include "wx/wx.h"
#include "global.h"

//#define __THREAD_VERSION 0

class	TorusInternal;
class	TorusThread;
class	TScoreFile;

class ScorePanel;


//	�䷯���� ���� ���� ��Ȳ�� �˷� �ش�.
//	������ ��Ȳ�� ���� ������ �ٲ� �ָ� �ȴ�.
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

	//	���� ����... torusinternal���� ȣ��ȴ�.
	void gainPoint( int nLevel, ClearMethod aClearMethod, bool isAllClear );
	
	//	������ ������ ���� ������ ũ�� ����
	void updateView();

	//	���� ����  �Լ�
	void	startGame();
	void	stopGame();
	void	pauseGame();
	void	resume();

	void initGame();

	//	pref. dlg. �� ������ ������ ����
	void	setLevel( int nLevel );
	int		getLevel();

	void	setDifficulty( int nDiff );
	int		getDifficulty();

	void	OnLevelUp();		
	void OnGameOver();


	void	onThreadTimer( );

	void drawPipe();	//	torusinternal���� ȣ��(clear line ȿ��)

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

	//	�÷��̾� ǥ��
	wxString m_wxsPlayerEntry[10];

	//	�䷯���� ���� ǥ��
	TorusInternal	*m_pTI;
	
	//	�ð�.. �ȶȶȶ�..	//	Ȥ�� �����带 ������...
	wxTimer			*m_pTimer;


	//	game status
	GameState		m_GameState;
	int			m_nLevel;		//	������ ����
	int			m_nDifficulty;
	//	File object
	//	������ ���Ϸ� ����ϱ� ���� Ŭ����
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
