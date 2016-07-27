#pragma once
#include "GUIDesign.h"

class LoginDialog :public GUILoginDialog
{
public:
	LoginDialog();
	wxString GetId();
	wxString GetPassword();
protected:
	virtual void OnClickTryLogin(wxCommandEvent & event) override;
};