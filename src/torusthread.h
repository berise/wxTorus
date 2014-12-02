#include "wx/thread.h"
#include "wx/string.h"

//	FD
class TorusView;



//	got this code from wxMSW samples in official distribution package
class TorusThread : public wxThread
{
public:
    TorusThread(TorusView *pTV);

    // thread execution starts here
    virtual void *Entry();

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    virtual void OnExit();

    // write something to the text control
    void WriteText(const wxString& text);

public:
    size_t   m_count;
    TorusView	*m_pTV;
};

