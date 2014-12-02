#include "preferencedialog.h"
#include "wx/statline.h"	//	wxStaticLine
#include "wx/dialog.h"


const int  ID_BUTTON_OK = 2000;
const int  ID_BUTTON_CANCEL = 2001;
const int  ID_BUTTON_APPLY = 2002;

const int  ID_CHECKBOX_1 = 200;
const int  ID_RADIOBUTTON_1 = 201;
const int  ID_RADIOBUTTON_2 = 202;
const int  ID_RADIOBUTTON_3 = 202;

BEGIN_EVENT_TABLE(PreferenceDialog, wxDialog)    
	EVT_BUTTON(wxID_OK, PreferenceDialog::OnOK)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// PreferenceDialog
// ----------------------------------------------------------------------------




PreferenceDialog::PreferenceDialog(wxWindow *parent)
             : wxDialog(parent, -1, wxString(_T("Modal dialog")))
{
	wxBoxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);	

    wxStaticBoxSizer *rbSizer = 
			new wxStaticBoxSizer(
						new wxStaticBox (this, -1, _T("난이도(Difficulty)")),
						wxVERTICAL );
    
	rb1 = new wxRadioButton( this, ID_RADIOBUTTON_1, _T("쉬움(&Beginner)"), wxPoint(210,170), wxDefaultSize, wxRB_GROUP );
	//rb1->Disable();		//	not yet implemented
    rbSizer->Add( rb1, 1, wxALL, 5 );

	rb2 = new wxRadioButton( this, ID_RADIOBUTTON_2, _T("중간(&Intermediate)"), wxPoint(340,170), wxDefaultSize );
	//rb2->Disable();		//	not yet implemented
    rbSizer->Add( rb2, 1, wxALL, 5 );

	rb3 = new wxRadioButton( this, ID_RADIOBUTTON_3, _T("어려움(&Expert)"), wxPoint(340,170), wxDefaultSize );
	rbSizer->Add( rb3, 1, wxALL, 5 );
	rb3->SetValue( TRUE );

	//
	//	시작 레벨을 정하는 곳
	wxStaticBoxSizer *sdSizer =			//	 slider
			new wxStaticBoxSizer(
						new wxStaticBox (this, -1, _T("시작 레벨(Starting Level)")),
						wxVERTICAL );
    
	//	Slider
	m_pStartLevel	=	new wxSlider( this, -1, 5, 3, 5,
								wxPoint(18,90), wxSize(155,-1),
                             wxSL_AUTOTICKS | wxSL_LABELS  );
	
	//m_pStartLevel->SetTickFreq( 1, 0 );

	sdSizer->Add( m_pStartLevel, 1, wxGROW );


	wxStaticBoxSizer *cbSizer =			//	 slider
			new wxStaticBoxSizer(
						new wxStaticBox (this, -1, _T("추가 토러스(Extra Torus) :: 체크해도 안됨 -> 구현되지 않음")),
						wxVERTICAL );

	//	check box for extra torus usage
	cb1 = new wxCheckBox( this, ID_CHECKBOX_1, _T("(노란색 토러스를 사용(Use &Yellow Torus (Harder Level))"), wxPoint(210,170), wxDefaultSize );
	cb1->SetValue( FALSE );
	//cb1->Disable();	
    cbSizer->Add( cb1, 1, wxALL, 5 );
	
 
	// OK, CANCEL, Apply 버튼들...
	wxBoxSizer	*ButtonSizer	=	new wxBoxSizer( wxHORIZONTAL );

	m_btnOK		= new wxButton(this, wxID_OK, _T("&OK"));
    m_btnCancel = new wxButton(this, wxID_CANCEL, _T("&Cancel"));
    m_btnApply	= new wxButton(this, wxID_APPLY, _T("&Apply"));
	
    ButtonSizer->Add(m_btnOK, 1, wxALIGN_RIGHT | wxALL, 5);
    ButtonSizer->Add(m_btnCancel, 1, wxALIGN_RIGHT | wxALL, 5);
    ButtonSizer->Add(m_btnApply, 1, wxALIGN_RIGHT | wxALL, 5);

	

	sizerTop->Add( rbSizer, 1, wxEXPAND | wxALL, 5 );


	sizerTop->Add( sdSizer, 0, wxGROW | wxALL, 5 );

	sizerTop->Add( cbSizer, 0, wxGROW | wxALL, 5 );

	

	//	라인을 하나 긋는 것은 wxEXPAND와 사용하면 안된다.
	//	비율을 맞춰서 라인을 긋은 GROW를 사용해야 알맞은 효과를 얻는다.
	sizerTop->Add( new wxStaticLine( this, -1 ), 0, wxGROW | wxALL, 5 );

	//	오른쪽에 붙이기
	sizerTop->Add( ButtonSizer, 0, wxALIGN_RIGHT );

    SetAutoLayout(TRUE);
    SetSizer(sizerTop);

    sizerTop->SetSizeHints(this);
    //sizerTop->Fit(this);

    m_btnOK->SetFocus();
    m_btnOK->SetDefault();
}


void PreferenceDialog::OnButton(wxCommandEvent& event)
{
    if ( event.GetEventObject() == m_btnCancel )
    {
        //delete m_btnOK;        m_btnOK = NULL;        m_btnCancel->Disable();
    }
    else if ( event.GetEventObject() == m_btnOK )
    {		
        wxGetTextFromUser(_T("Dummy prompt"),
                          _T("Modal dialog called from dialog"),
                          _T(""), this);
    }
    else
    {
        event.Skip();
    }
}


void PreferenceDialog::OnOK(wxCommandEvent& event)
{
	m_nStartLevel	= m_pStartLevel->GetValue();

	if( rb3->GetValue() == TRUE)
	{
		m_nDifficulty = 3;	//	 1,2,3
	} 
	else if ( rb2->GetValue() == TRUE )
	{
		m_nDifficulty = 2;	//	 1,2,3
	}
	else
	{
		m_nDifficulty = 1;	//	 1,2,3
	}


	
	if( cb1->GetValue() == TRUE )
	{
		m_bExtraTorus	=	TRUE;
	}
	else
	{
		m_bExtraTorus	=	FALSE;
	}


	//wxDialog::OnOK( event );
	Show(false);
}