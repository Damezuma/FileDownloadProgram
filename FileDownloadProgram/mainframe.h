#pragma once
#include<wx/wx.h>
#include "GUIDesign.h"
class MainFrame : public GUIMainFrame
{
public:
	MainFrame();
	~MainFrame();
	void UpdateLogList(std::vector<wxString>&& data);
protected:
	void OnActivateLogPage(wxActivateEvent & event);
};