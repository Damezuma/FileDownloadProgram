#pragma once
#include <wx/wx.h>
#include <wx/snglinst.h>
class Application :public wxApp
{
public:
	virtual bool OnInit() override;
	virtual int OnExit() override;
private:
	wxSingleInstanceChecker * m_checker;
	bool m_isLogin;

	
};