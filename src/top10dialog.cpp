#include "Top10Dialog.h"
#include "wx/statline.h"	//	wxStaticLine


const int  ID_BUTTON_OK = 2000;
const int  ID_BUTTON_CANCEL = 2001;

const int  ID_TE_NAME		= 2005;


BEGIN_EVENT_TABLE(Top10Dialog, wxDialog)    
	EVT_BUTTON(wxID_OK, Top10Dialog::OnOK)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// Top10Dialog
// ----------------------------------------------------------------------------


Top10Dialog::Top10Dialog(wxWindow *parent)
             : wxDialog(parent, -1, wxString(_T("토러스 고수 10인(Torus Top 10 Players)")))
{
	wxBoxSizer *vertical_sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText *thanks_words = new wxStaticText(this, wxID_ANY, "");
	thanks_words->SetLabel("축하합니다! 토러스 수뇌권에 입성한 것을 축하합니다.\n"
						"(Congratulation!. You've made a top ten list)");

	vertical_sizer->Add( thanks_words ,
				1,		//	 vertically stretchable
				wxALL,
				10 ); // set border width to 50	

    wxStaticBoxSizer *sb_sizer = new wxStaticBoxSizer( 
									new wxStaticBox(this, 
													-1, 
													"당신의 이름은? (What is your name?"),
													wxHORIZONTAL);

	m_pTextCtrl = new wxTextCtrl( this, ID_TE_NAME, "" , wxDefaultPosition, wxSize(200, 20) );
    sb_sizer->Add( m_pTextCtrl, 1, wxALL, 5 );
	sb_sizer->AddSpacer(10);
	
	vertical_sizer->Add( sb_sizer, 1, wxEXPAND | wxALL, 10 );
	
 

	// OK, CANCEL, Apply 버튼들...
	wxBoxSizer	*horizontal_sizer	=	new wxBoxSizer( wxHORIZONTAL );

	m_btnOK		= new wxButton(this, wxID_OK, _T("&OK"));
    m_btnCancel = new wxButton(this, wxID_CANCEL, _T("&Cancel"));
	
    horizontal_sizer->Add(m_btnOK, 1, wxALIGN_RIGHT | wxALL, 5);
    horizontal_sizer->Add(m_btnCancel, 1, wxALIGN_RIGHT | wxALL, 5);
	

	
	
	//	라인을 하나 긋는 것은 wxEXPAND와 사용하면 안된다.
	//	비율을 맞춰서 라인을 긋은 GROW를 사용해야 알맞은 효과를 얻는다.
	vertical_sizer->Add( new wxStaticLine( this, -1 ), 0, wxGROW | wxALL, 5 );

	//	오른쪽에 붙이기
	vertical_sizer->Add( horizontal_sizer, 0, wxALIGN_RIGHT );


    SetAutoLayout(TRUE);
    SetSizer(vertical_sizer);

    //vertical_sizer->SetSizeHints(this);
	vertical_sizer->Fit(this);

    m_pTextCtrl->SetFocus();
    m_btnOK->SetDefault();
}




void Top10Dialog::OnOK(wxCommandEvent& event)
{
	m_wxsName	=	m_pTextCtrl->GetLineText( 0 );

	//wxDialog::OnOK( event );
}

void Top10Dialog::OnCancel(wxCommandEvent& event)
{
//	m_wxsName	=	"Anonymous Player";

//	wxDialog::OnOK( event );
}