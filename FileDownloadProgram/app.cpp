#include <wx/wx.h>
#include "app.h"
#include <wx/process.h>
#include "logindialog.h"
#include "mainframe.h"
#include <wx/icon.h>
#include <wx/bitmap.h>
#include "ui.h"
#include <wx/socket.h>
#include "filetransferclient.h"
#include "client_protocol.h"
bool Application::OnInit()
{
	wxSocketBase::Initialize();
	UI::Initialize();
	m_isLogin = false;
	m_checker = new wxSingleInstanceChecker();
	
	//if (m_checker->IsAnotherRunning())
	//{
	//	return false;
	//}
	LoginDialog * dialog = new LoginDialog();
	if (dialog->ShowModal() == wxID_OK)
	{
		wxString id = dialog->GetId();
		wxString password = dialog->GetPassword();

		UI& ui = UI::Instance();
		m_isLogin = true;
		ui.mainframe = new MainFrame();
		ui.mainframe->Show();
		ui.taskIcon = new TaskIcon();
	}
	delete dialog;
	if(m_isLogin == false)
	{
		Exit();
	}
	wxTimer * timer = new wxTimer(this);
	timer->Connect(wxEVT_TIMER, wxTimerEventHandler(Application::OnTimer), NULL, this);
	timer->Start(1000 * 60);
	return true;
}
int Application::OnExit()
{
	delete m_checker;
	UI::Release();
	ClientFileTransfer::Release();
	return 0;
}
void Application::OnTimer(wxTimerEvent & event)
{
	ClientFileTransfer::Instance().AddCommand(new CommandGetRemainedFileList());
}
DECLARE_APP(Application)
IMPLEMENT_APP(Application)