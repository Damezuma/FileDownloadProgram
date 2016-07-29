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
	std::vector<wxFileName> m_fileNames;
	void OnActivateLogPage(wxActivateEvent & event);
	virtual void OnClickAddFile(wxCommandEvent& event);
	virtual void OnClickSubmit(wxCommandEvent& event);
	virtual void OnClickOTP(wxCommandEvent& event);
};