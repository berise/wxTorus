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
	//	������ ���Ϸ� ����ϱ� ���� Ŭ����
	TScoreFile		*m_pScoreFile;	
	wxListView *score_listview;
};


#endif
