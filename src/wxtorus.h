// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif




//	forward declarations
class TorusView;
class ScorePanel;

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources)
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
    #include "mondrian.xpm"
#endif

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
};


DECLARE_APP(MyApp)



// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

	//
	//	wxWindows Event Handlers
    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

	void OnFileOpen(wxCommandEvent& event);
	void OnFilePreference(wxCommandEvent& event);
	void OnButtonPressed(wxCommandEvent& event);

	void OnMouseMove( wxMouseEvent &me );
	void OnMouseLButtonDown( wxMouseEvent &me );
	void OnMouseLButtonUp( wxMouseEvent &me );

	//	OnSize
	void OnSize( wxSizeEvent &event );

	//	MyFrame이 포커스를 받았을때.. 자식에게 포커스를 넘겨준다.
	void OnSetFocus( wxFocusEvent &event );	

	//	end of wxWindows Event Handlers

	//	user functions

	//	show one of the panel, between view and score, exclusively.
	void OnSwitchPanel( wxCommandEvent &);
	void SwitchPanel();

	void start();
	void pause();

private:
    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()

private:
	wxStatusBar *m_pSB;
	wxButton	*m_pBtn;
	bool isCapture;


	TorusView	*torus_view;
	ScorePanel *score_panel;
	
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
	wxTorus_FileOpen = 100,
	wxTorus_FilePreference,
	wxTorus_SwitchPanel,
    // menu items
    wxTorus_Quit,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    wxTorus_About = wxID_ABOUT
};



//	app specific id
//const int wxButtonID = 100001;
