#include "torusthread.h"
#include "torusview.h"


TorusThread::TorusThread(TorusView *pTV)
        : wxThread()
{
    m_count = 0;
    m_pTV	=	pTV;
}


void TorusThread::WriteText(const wxString& text)
{
    wxString msg;

    // before doing any GUI calls we must ensure that this thread is the only
    // one doing it!

    wxMutexGuiEnter();

    msg << text;
    //m_frame->WriteText(msg);

    wxMutexGuiLeave();
}

void TorusThread::OnExit()
{
}


//	���Ӿ��� ������ ��... ���� �ð��� ������ �Լ��� �θ���.(��~~)
void *TorusThread::Entry()
{
	while( 1 )
    {
        // check if we were asked to exit
        if ( TestDestroy() )
            break;


		m_pTV->onThreadTimer();

        // wxSleep() can't be called from non-GUI thread!
        wxThread::Sleep(30);
    }


    return NULL;
}