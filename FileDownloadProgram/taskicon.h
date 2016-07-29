#pragma once
#include "wx/wx.h"
#include "wx/taskbar.h"
class TaskIcon : public wxTaskBarIcon
{
public:
	TaskIcon();
protected:
	virtual wxMenu * CreatePopupMenu();
	void OnMenuClose(wxCommandEvent & event);
	void OnMenuShow(wxCommandEvent & event);
	void OnMenuGetOTP(wxCommandEvent & event);
	void OnDClickIcon(wxTaskBarIconEvent & event);
	void OnTimer(wxTimerEvent & event);
DECLARE_EVENT_TABLE();
};
