#pragma once
#include "GUIDesign.h"

class LoginDialog :public GUILoginDialog
{
public:
	LoginDialog();
protected:
	virtual void OnClickTryLogin(wxCommandEvent & event) override;
};