#include "wx/wx.h"

// A custom modal dialog
class PreferenceDialog : public wxDialog
{
public:
    PreferenceDialog(wxWindow *parent);

    void OnButton(wxCommandEvent& event);

	void OnOK(wxCommandEvent& event);

public:
	int		m_nStartLevel;
	int		m_nDifficulty;
	bool	m_bExtraTorus;

private:
	wxRadioButton *rb1;
	wxRadioButton *rb2;	
	wxRadioButton *rb3;

	wxSlider *m_pStartLevel;

	wxCheckBox *cb1;


    wxButton *m_btnOK;
    wxButton *m_btnCancel;
	wxButton *m_btnApply;

    DECLARE_EVENT_TABLE()
};

