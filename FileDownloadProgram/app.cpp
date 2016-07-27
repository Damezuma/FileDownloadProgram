#include <wx/wx.h>
#include "app.h"
#include <wx/process.h>
#include "logindialog.h"
#include "mainframe.h"
#include <wx/icon.h>
#include <wx/bitmap.h>
#include "ui.h"
bool Application::OnInit()
{
	UI::Initialize();
	m_isLogin = false;
	m_checker = new wxSingleInstanceChecker();
	
	if (m_checker->IsAnotherRunning())
	{
		return false;
	}
	LoginDialog * dialog = new LoginDialog();
	if (dialog->ShowModal() == wxID_OK)
	{
		UI& ui = UI::Instance();
		m_isLogin = true;
		ui.mainframe = new MainFrame();
		ui.mainframe->Show();
		ui.taskIcon = new TaskIcon();
	}
	delete dialog;
	return true;
}
int Application::OnExit()
{
	delete m_checker;
	return 0;
}
DECLARE_APP(Application)
IMPLEMENT_APP(Application)