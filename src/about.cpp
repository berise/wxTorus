#include "about.h"
#include "wx/statline.h"	//	wxStaticLine
#include <wx/mstream.h>
#include <wx/image.h>


//#include "../res/author.h"
#include "../res/author2.h"
//#include "../res/temple.h"

const int  ID_BUTTON_OK = 2000;
const int  ID_BUTTON_CANCEL = 2001;

const int  ID_TE_NAME		= 2005;


BEGIN_EVENT_TABLE(AboutDialog, wxDialog)    
	EVT_BUTTON(wxID_OK, AboutDialog::OnOK)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// AboutDialog
// ----------------------------------------------------------------------------
AboutDialog::AboutDialog(wxWindow *parent)
             : wxDialog(parent, -1, wxString(_T("토러스에 대하여(About wxTorus)")))
{
	wxBoxSizer *sizer_v = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer *sizer_sb = new wxStaticBoxSizer( 
    		new wxStaticBox(this, -1, _T("About torus"))
		, wxHORIZONTAL);

	
	wxMemoryInputStream istream(author2_png, sizeof(author2_png));
	wxImage logo_image(istream, wxBITMAP_TYPE_PNG);
	wxBitmap bitmap_author(logo_image);


	//wxBitmap bitmap_author = wxXmlResource::Get()->LoadBitmap("WXTORUS_AUTHOR");
	wxStaticBitmap *picture_frame = 
			new wxStaticBitmap(this, 
			wxID_ANY, 
			bitmap_author,
			wxDefaultPosition);

	sizer_sb->Add(picture_frame, 1, wxALL, 10 );

	wxStaticText *authors_word = new wxStaticText(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, 300) );

	wxString string_words("Program : JaeSung Lee(berise@gmail.com)\n"
                          "Graphic : Park Jong Soon(my friend)\n\n"
						  "----------------------------\n"						  
                          "1999 ktorus (Qt V.0.98)\n"
                          "2001 torus_win32 (vwcl)\n"
						  "2005 torus_win32 (wxWidgets)\n"
						  "please see http://sourceforge.net/projects/ktorus");
	authors_word->SetLabel(string_words);

	sizer_sb->Add(authors_word, 1, wxALL, 10 );
 
	// OK, CANCEL, Apply 버튼들...
	wxBoxSizer	*ButtonSizer	=	new wxBoxSizer( wxHORIZONTAL );

	m_btnOK		= new wxButton(this, wxID_OK, _T("&OK"));    
	
    ButtonSizer->Add(m_btnOK, 1, wxCENTER , 10);    
	

	//	sizer top contains all sizers
	sizer_v->Add( sizer_sb, 1, wxALL ,10 );
	

	//	라인을 하나 긋는 것은 wxEXPAND와 사용하면 안된다.
	//	비율을 맞춰서 라인을 긋은 GROW를 사용해야 알맞은 효과를 얻는다.
	sizer_v->Add( new wxStaticLine( this, -1 ), 0, wxEXPAND | wxALL, 5 );

	//	오른쪽에 붙이기
	sizer_v->Add( ButtonSizer, 0, wxCENTER | wxALL, 10);


    SetAutoLayout(TRUE);
    SetSizer(sizer_v);

    //sizer_v->SetSizeHints(this);
	sizer_v->Fit(this);

    m_btnOK->SetDefault();
}




void AboutDialog::OnOK(wxCommandEvent& event)
{
	Show(false);
	//wxDialog::OnOK( event );
}

void AboutDialog::OnCancel(wxCommandEvent& event)
{
//	m_wxsName	=	"Anonymous Player";

//	wxDialog::OnOK( event );
}