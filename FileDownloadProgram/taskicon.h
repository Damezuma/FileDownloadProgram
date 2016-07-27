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
DECLARE_EVENT_TABLE();
};
