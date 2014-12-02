#ifndef __ScorePanel_Included
#define __ScorePanel_Included


#include "wx/wx.h"


class wxPanel;
class wxListView;

class TScoreFile;


class ScorePanel: public wxPanel
{
public:
	ScorePanel( wxWindow *parent, const wxPoint &point, const wxSize &size );
	~ScorePanel();

	
	void updateScore();

	//	do not write function below EVENT_TABLE;
	DECLARE_EVENT_TABLE()
protected:
	void OnKeyDown(wxKeyEvent &);
	void OnSetFocus(wxFocusEvent &);



	//	File object
	//	점수로 파일로 기록하기 위한 클래스
	TScoreFile		*m_pScoreFile;	
	wxListView *score_listview;
};


#endif
