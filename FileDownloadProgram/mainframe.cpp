#include "mainframe.h"
#include "ui.h"
MainFrame::MainFrame() :GUIMainFrame(nullptr, wxID_ANY, TEXT("파일 다운로드 프로그램"))
{

}

MainFrame::~MainFrame()
{
	UI& ui = UI::Instance();
	ui.mainframe = nullptr;
}
