#include "mainframe.h"
#include "ui.h"
MainFrame::MainFrame() :GUIMainFrame(nullptr, wxID_ANY, TEXT("���� �ٿ�ε� ���α׷�"))
{

}

MainFrame::~MainFrame()
{
	UI& ui = UI::Instance();
	ui.mainframe = nullptr;
}
