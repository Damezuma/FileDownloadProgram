#include "taskicon.h"
#include "taskicon.h"
#include <wx/icon.h>
#include <wx/bitmap.h>
#include "app.h"
#include "ui.h"
#include "filetransferclient.h"
BEGIN_EVENT_TABLE(TaskIcon, wxTaskBarIcon)
EVT_MENU(wxID_CLOSE, OnMenuClose)
EVT_MENU(wxID_OPEN, OnMenuShow)
EVT_MENU(wxID_NEW, OnMenuGetOTP)
EVT_TASKBAR_LEFT_DCLICK(OnDClickIcon)
END_EVENT_TABLE()

TaskIcon::TaskIcon() :wxTaskBarIcon()
{
	SetIcon(wxICON(WXICON_AAA));
}

wxMenu * TaskIcon::CreatePopupMenu()
{
	wxMenu * menu = new wxMenu();
	menu->Append(wxID_OPEN, wxT("����"));
	menu->Append(wxID_NEW, wxT("OTP����"));
	menu->Append(wxID_HELP, wxT("����"));
	menu->AppendSeparator();
	menu->Append(wxID_CLOSE, wxT("�ݱ�"));
	menu->SetEventHandler(this);
	return menu;
}

void TaskIcon::OnMenuClose(wxCommandEvent & event)
{
	this->Destroy();
	Application::GetInstance()->Exit();
}

void TaskIcon::OnMenuShow(wxCommandEvent & event)
{
	UI & ui = UI::Instance();
	if (ui.mainframe == nullptr)
	{
		ui.mainframe = new MainFrame();
	}
	ui.mainframe->Show();
}

void TaskIcon::OnMenuGetOTP(wxCommandEvent & event)
{
	ClientFileTransfer::Instance().TryGetOTP(this, [this](bool res, wxString msg)->void {
		if (res)
		{
			wxMessageBox(wxString::Format(wxT("OTP�� %s�Դϴ�"),msg), wxT("OTP�߱�"));
		}
		else
		{
			wxMessageBox(msg, wxT("Error!"));
		}
	});
}

void TaskIcon::OnDClickIcon(wxTaskBarIconEvent & event)
{
	UI & ui = UI::Instance();
	if (ui.mainframe == nullptr)
	{
		ui.mainframe = new MainFrame();
	}
	ui.mainframe->Show();
}
