#include "wx/wx.h"
#include "wx/dialog.h"

// A custom modal dialog
class Top10Dialog : public wxDialog
{
public:
    Top10Dialog(wxWindow *parent);   

	void OnOK(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);

public:
	wxString	m_wxsName;

private:
	wxTextCtrl	*m_pTextCtrl;

    wxButton *m_btnOK;
    wxButton *m_btnCancel;

    DECLARE_EVENT_TABLE()
};

