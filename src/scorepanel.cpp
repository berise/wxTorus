#include "wx/wx.h"
#include "wx/listctrl.h"


#include "scorefile.h"
#include "scorepanel.h"
#include "wxtorus.h"

#include "wx/mstream.h"
#include "wx/image.h"

#include "../res/logo.h"





//	wx event table
BEGIN_EVENT_TABLE( ScorePanel, wxPanel )
	EVT_KEY_DOWN( ScorePanel::OnKeyDown )
	EVT_SET_FOCUS( ScorePanel::OnSetFocus ) 
END_EVENT_TABLE()


ScorePanel::ScorePanel( wxWindow *parent, const wxPoint &point, const wxSize &size )
:wxPanel( parent, wxID_ANY, point, size )
{
//	wxPanel *p = new wxPanel(this, -1 );
	//SetBackgroundColour(wxColour(122, 128, 128));

	 wxBoxSizer *horizontal_sizer = new wxBoxSizer( wxHORIZONTAL );

	 wxBoxSizer *vertical_sizer = new wxBoxSizer( wxVERTICAL );


  wxMemoryInputStream istream(logo_png, sizeof(logo_png));
  wxImage logo_image(istream);
  logo_image.SetMaskColour(logo_image.GetRed(0,0),logo_image.GetGreen(0,0),logo_image.GetBlue(0,0)); 
  logo_image.SetMask(true); //Not sure if you need that after calling SetMaskColour()...

  wxBitmap logo_bitmap = wxBitmap(logo_image);



  vertical_sizer->AddStretchSpacer(1);
/*
	 wxBitmap logo_bitmap(wxtorus);
	 */
	 wxStaticBitmap *logo_static = new wxStaticBitmap(this, -1, logo_bitmap, wxDefaultPosition);

	 vertical_sizer->Add( logo_static, 0, wxCENTER, 10  );
	 vertical_sizer->AddSpacer(10);
	 
	 
	 wxBoxSizer *statsizer = new wxStaticBoxSizer(
		new wxStaticBox(this,  wxID_ANY, _T("토러스 고수(Torus Top Players)")), wxVERTICAL);


	 score_listview = new wxListView( this, 
											wxID_ANY, 
											wxDefaultPosition, 
											wxSize(300, 180), 
											wxLC_REPORT | wxSIMPLE_BORDER );
	 //|wxLC_HRULES   );
		score_listview->Enable(false);
	 score_listview->InsertColumn(0, "Rank");
	 score_listview->InsertColumn(1, "Name");
	 score_listview->InsertColumn(2, "Score");

	score_listview->SetColumnWidth( 0, wxLIST_AUTOSIZE_USEHEADER);
    score_listview->SetColumnWidth( 1, wxLIST_AUTOSIZE );
	score_listview->SetColumnWidth( 1, wxLIST_AUTOSIZE );

	statsizer->Add( score_listview, 1, wxEXPAND | wxCENTER, 10);
	vertical_sizer->Add( statsizer, 0, wxCENTER, 20);


	vertical_sizer->AddStretchSpacer(1);
	
	// place my words in center both horizontally and vertically.
	// remove exEXPAND.
	wxStaticText *disclaimer =
				new wxStaticText( this,
								wxID_ANY,
								_T("place holder"),
								wxDefaultPosition,
								wxSize(300, 100),
								wxALIGN_LEFT );

	wxString text = "This software is provided \"as is\" software. "
		"You can distribute it as freely as you like. If you enjoyed "
		"this program, then please send an e-mail to me.\n\n";
	disclaimer->SetLabel( text );

	vertical_sizer->Add( disclaimer, 0, wxCENTER, 50 );
	vertical_sizer->AddStretchSpacer(1);

	wxStaticText *press_any_key = new wxStaticText( this, wxID_ANY, "Press Space To Start", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
	
	wxFont f(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	press_any_key->SetFont(f);
	vertical_sizer->Add( press_any_key,
		1,
		wxEXPAND | wxCENTER,
		10); 


	vertical_sizer->AddStretchSpacer(1);


	//	copyright 
	wxStaticText *copyright=
				new wxStaticText( this,
								wxID_ANY,
								_T("place holder"),
								wxDefaultPosition,
								wxDefaultSize,
								wxALIGN_CENTER );
	wxString copyright_text = "Copyright(c) all rights reserved. JaeSung Lee (berise^at^gmail^dot^com)";
	copyright->SetLabel( copyright_text );
	vertical_sizer->Add( copyright, 0, wxCENTER, 50 );
	

	horizontal_sizer->Add( vertical_sizer, 1, wxEXPAND | wxCENTER, 10 ); 
	SetSizer( horizontal_sizer );
  
  // don't allow frame to get smaller than what the sizers tell it and also set
  // the initial size as calculated by the sizers
	horizontal_sizer->SetSizeHints( this );
	
}

ScorePanel::~ScorePanel()
{

}


void ScorePanel::OnKeyDown( wxKeyEvent &event )
{
	long keycode = event.GetKeyCode();

	switch( keycode )
	{		
	case WXK_SPACE:
		{
		//MyFrame *f = (MyFrame*)GetParent()->GetParent();	//	panel <- frame

		MyFrame *f  = (MyFrame*)wxGetApp().GetTopWindow();
		f->SwitchPanel();
		f->start();
		break;
		}

	case WXK_ESCAPE:
		{
		
		  wxWindow *top_window = wxGetApp().GetTopWindow();
		  top_window->Close();
		}

		break;


	default:
		event.Skip();
		break;
	}

}


void ScorePanel::updateScore()
{
	TScoreFile	*pSF = new TScoreFile;
	wxString players[10];
	wxString scores[10];
	wxString rank;

	score_listview->DeleteAllItems();
	for( int i = 0; i < 10; i++ )
	{		
		int			nScore;
		char		szPlayer[256];
		
		nScore = pSF->getScore( i );
		pSF->getPlayer( i, szPlayer );

		rank.Printf("%#02d", i+1);
		scores[i].Printf("%d", nScore);
		players[i] = szPlayer;
		//m_wxsPlayerEntry[i].Printf("%#02d.     %#07d     %#15s", i+1, nScore, szPlayer );


		score_listview->InsertItem( i, rank, 0);
		score_listview->SetItem( i, 1, players[i]);
		score_listview->SetItem( i, 2, scores[i]);

	//	wxMessageBox( players[i]);
	}
	wxSize cs = score_listview->GetClientSize();

	score_listview->SetColumnWidth( 0, cs.GetWidth()/3);
	score_listview->SetColumnWidth( 1, cs.GetWidth()/3);
	score_listview->SetColumnWidth( 1, cs.GetWidth()/3);

/*
	score_listview->SetColumnWidth( 0, wxLIST_AUTOSIZE_USEHEADER);
	score_listview->SetColumnWidth( 1, wxLIST_AUTOSIZE );
	score_listview->SetColumnWidth( 1, wxLIST_AUTOSIZE );
	*/

	delete pSF;
}

void ScorePanel::OnSetFocus( wxFocusEvent & )
{ 
	updateScore();
}
