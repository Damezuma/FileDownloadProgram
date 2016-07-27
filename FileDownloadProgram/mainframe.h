#pragma once
#include<wx/wx.h>
#include "GUIDesign.h"
class MainFrame : public GUIMainFrame
{
public:
	MainFrame();
	~MainFrame();
protected:
	void OnActivateLogPage(wxActivateEvent & event);
};