#pragma once
#include "wx/wx.h"
#include "wx/taskbar.h"
class TaskIcon : public wxTaskBarIcon
{
public:
	TaskIcon();
	~TaskIcon();
protected:
	wxTimer * m_timer;
	virtual wxMenu * CreatePopupMenu();
	void OnMenuClose(wxCommandEvent & event);
	void OnMenuShow(wxCommandEvent & event);
	void OnMenuGetOTP(wxCommandEvent & event);
	void OnDClickIcon(wxTaskBarIconEvent & event);
	void OnTimer(wxTimerEvent & event);
DECLARE_EVENT_TABLE();
};
