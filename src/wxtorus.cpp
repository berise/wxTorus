//
//

//#include "wx/memory.h"	//	wxDebugContext
#include "wx/wx.h"
#include "wx/image.h"


#include "wxtorus.h"
#include "about.h"

#include "torusview.h"
#include "scorepanel.h"
#include "preferencedialog.h"


// simply global -_-
wxBoxSizer *v_sizer;
wxPanel *place_holder;

// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(wxTorus_FileOpen,  MyFrame::OnFileOpen)
	EVT_MENU(wxTorus_FilePreference,  MyFrame::OnFilePreference)
	EVT_MENU(wxTorus_SwitchPanel,  MyFrame::OnSwitchPanel)
    EVT_MENU(wxTorus_Quit,  MyFrame::OnQuit)
    EVT_MENU(wxTorus_About, MyFrame::OnAbout)

	EVT_SIZE(	MyFrame::OnSize )
	EVT_SET_FOCUS(	MyFrame::OnSetFocus ) 

END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // create the main application window
    MyFrame *frame = new MyFrame(_T("wxTorus App"),
                                 wxDefaultPosition,
								 //wxDefaultSize);
								 wxSize(TORUS_MAX_WIDTH, TORUS_MAX_HEIGHT));
    frame->Show(TRUE);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned FALSE here, the
    // application would exit immediately.
    return TRUE;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------
//, wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE),
// frame constructor
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
       : wxFrame(NULL, -1, title, pos, size, wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCAPTION |wxCLOSE_BOX ),
	   torus_view(NULL)
{
    // set the frame icon
    SetIcon(wxICON(mondrian));


	//	
	wxImage::AddHandler(new wxPNGHandler);


#if wxUSE_MENUS
    // create a menu bar
    wxMenu *menuFile = new wxMenu;
//	menuFile->Append(wxTorus_FileOpen, _T("&Open...\tCtrl-O"), _T("Open FileDialog"));
	menuFile->Append(wxTorus_FilePreference, _T("&Preference...\tCtrl-T"));
//	menuFile->Append(wxTorus_SwitchPanel, _T("&Switch Panel...\tCtrl-W"));

	menuFile->AppendSeparator();

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxTorus_About, _T("&About...\tF1"), _T("Show about dialog"));

    menuFile->Append(wxTorus_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, _T("파일(&F)"));
    menuBar->Append(helpMenu, _T("도움말(&H)"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    m_pSB = CreateStatusBar();
    m_pSB->SetStatusText(_T("Welcome to wxTorus!"));
	//SetStatusText(_T("through member variable in 2nd pane"), 1);
#endif // wxUSE_STATUSBAR

	SetClientSize(wxSize(TORUS_MAX_WIDTH, TORUS_MAX_HEIGHT));

	wxSize client_size = GetClientSize();	

	/*
	torus_view	=	new TorusView( this,
					wxDefaultPosition,
					//client_size);
					wxSize(TORUS_MAX_WIDTH, TORUS_MAX_HEIGHT) );
	//torus_view->Hide();	// torus_view->Show(false);


	score_panel = new ScorePanel( this,
						wxDefaultPosition,
						//client_size);
					wxSize(TORUS_MAX_WIDTH, TORUS_MAX_HEIGHT) );

	v_sizer = new wxBoxSizer(wxVERTICAL); 

	v_sizer->Add( score_panel, 1, wxGROW |wxCENTER, 10 );
	v_sizer->Add( torus_view, 1, wxGROW | wxCENTER, 10 );
	

	SetSizer(v_sizer);
	//v_sizer->SetSizeHints(this);
	SetAutoLayout(true);

	score_panel->SetFocus();
	score_panel->updateScore();
	*/
	place_holder = new wxPanel(this, 
							wxID_ANY,
						   	wxDefaultPosition,
						   	//wxDefaultSize );
						   	wxSize(TORUS_MAX_WIDTH, TORUS_MAX_HEIGHT) );

	v_sizer = new wxBoxSizer(wxVERTICAL); 
	score_panel = new ScorePanel( place_holder,
						wxDefaultPosition,
						//wxDefaultSize);
						wxSize(TORUS_MAX_WIDTH, TORUS_MAX_HEIGHT) );
	torus_view	=	new TorusView( place_holder,
					wxDefaultPosition,
						//wxDefaultSize);
					wxSize(TORUS_MAX_WIDTH, TORUS_MAX_HEIGHT) );
	
	v_sizer->Add( score_panel, 1, wxGROW |wxCENTER, 10 );
	v_sizer->Add( torus_view, 1, wxGROW | wxCENTER, 10 );

	place_holder->SetSizer( v_sizer );
	v_sizer->SetSizeHints(place_holder);

//	place_holder->SetAutoLayout(true);

	//	show score_panel first and give a focus
	v_sizer->Show( torus_view, false );
	v_sizer->Show( score_panel, true );

	score_panel->SetFocus();
	score_panel->updateScore();


	this->Connect(wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler(TorusView::OnKeyDown), (wxObject*) NULL, this);

	/*
	wxDebugContext::SetCheckpoint();	
	  // On MSW, Dump() crashes if using wxLogGui,
  // so use wxLogStderr instead.
  wxLog* oldLog = wxLog::SetActiveTarget(new wxLogStderr);

  wxDebugContext::PrintClasses();
  wxDebugContext::Dump();
  wxDebugContext::PrintStatistics();  

  // Set back to wxLogGui
  delete wxLog::SetActiveTarget(oldLog);
  */
}


//
// event handlers
void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
	torus_view->stopGame(); 
	delete torus_view;

    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	AboutDialog about;

	if( about.ShowModal() == wxID_OK )
		about.Destroy();
}



void MyFrame::OnFileOpen(wxCommandEvent& WXUNUSED(event))
{
	wxArrayString	wxsFilenames;
	wxString		wxsFilename;
	wxFileDialog *pDlg = new wxFileDialog( this, "Open a text file",
											"", "", "All files(*.*)|*.*|TextFiles(*.txt)|*.txt",
											wxFD_OPEN | wxFD_MULTIPLE, wxDefaultPosition);

	if( pDlg->ShowModal() == wxID_OK )
	{
		pDlg->GetFilenames( wxsFilenames );
		for(unsigned i = 0; i < wxsFilenames.GetCount(); i++ )
		{
			wxsFilename += wxsFilenames[i];
			wxsFilename += "\n";
		}
		wxMessageBox( wxsFilename, "File Open Dialog [TEST]", wxOK | wxICON_EXCLAMATION, this );
	}

	pDlg->Destroy();
}

void MyFrame::OnFilePreference(wxCommandEvent& WXUNUSED(event))
{
	PreferenceDialog *pref_dialog = new PreferenceDialog( this );

	if( pref_dialog->ShowModal() == wxID_OK )
	{
 		torus_view->setLevel( pref_dialog->m_nStartLevel );		
		torus_view->setDifficulty( pref_dialog->m_nDifficulty );
	}

	if( pref_dialog != NULL )
		delete pref_dialog;
	pref_dialog = NULL;

}


void MyFrame::OnSize( wxSizeEvent & event )
{
	if( place_holder!=NULL)
	{
		wxSize s = GetClientSize();
		place_holder->SetClientSize(s);
	}
}

//	윈도우 프로시져를 가진 자식이 코커스를 가지게 한다.
void MyFrame::OnSetFocus( wxFocusEvent &event )
{
	wxWindow *pWindow = NULL;
	wxString debug_info;

	if( torus_view->IsShown() == true )
	{
		pWindow = torus_view;
		debug_info = "view get a focus!";
	}
	else
	{
		pWindow = score_panel;
		debug_info = "panel get a focus!";
	}

	pWindow->SetFocus();

	m_pSB->SetStatusText( debug_info );
}


void MyFrame::OnSwitchPanel(wxCommandEvent &)
{
	SwitchPanel();
}

void MyFrame::SwitchPanel()
{
	if( torus_view->IsShown() == true )
	{
		v_sizer->Show( score_panel, true );
		v_sizer->Show( torus_view, false );
		v_sizer->Layout();
		score_panel->SetFocus();
	}
	else
	{		
		v_sizer->Show( score_panel, false );
		v_sizer->Show( torus_view, true );
		v_sizer->Layout();
		torus_view->SetFocus();
	}
}

void MyFrame::start()
{
	torus_view->startGame();
}

void MyFrame::pause()
{
	torus_view->pauseGame();
}

// vim:sts=2:ts=2
