#include "taskicon.h"
#include "taskicon.h"
#include <wx/icon.h>
#include <wx/bitmap.h>
#include "app.h"
#include "ui.h"
BEGIN_EVENT_TABLE(TaskIcon, wxTaskBarIcon)
EVT_MENU(wxID_CLOSE, OnMenuClose)
EVT_MENU(wxID_OPEN, OnMenuShow)
END_EVENT_TABLE()

TaskIcon::TaskIcon() :wxTaskBarIcon()
{
	SetIcon(wxICON(WXICON_AAA));
}

wxMenu * TaskIcon::CreatePopupMenu()
{
	wxMenu * menu = new wxMenu();
	menu->Append(wxID_OPEN, wxT("열기"));
	menu->Append(wxID_HELP, wxT("정보"));
	menu->AppendSeparator();
	menu->Append(wxID_CLOSE, wxT("닫기"));
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
