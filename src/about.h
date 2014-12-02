#include "wx/wx.h"

// A custom modal dialog
class AboutDialog : public wxDialog
{
public:
    AboutDialog(wxWindow *parent = NULL);   

	void OnOK(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);

private:
	wxButton *m_btnOK;
    wxButton *m_btnCancel;

    DECLARE_EVENT_TABLE()
};

