#include "mainframe.h"
#include "ui.h"
MainFrame::MainFrame() :GUIMainFrame(nullptr, wxID_ANY, TEXT("파일 다운로드 프로그램"))
{
	SetIcon(wxICON(WXICON_AAA));
		//this->m_panel6->Connect(wxEVT_ACTIVATE, wxActivateEventHandler(MainFrame::OnActivateLogPage),nullptr,this);
}

MainFrame::~MainFrame()
{
	UI& ui = UI::Instance();
	ui.mainframe = nullptr;
}

void MainFrame::OnActivateLogPage(wxActivateEvent & event)
{
	
}
