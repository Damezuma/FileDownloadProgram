#include "logindialog.h"
#include "logindialog.h"

LoginDialog::LoginDialog() :GUILoginDialog(nullptr)
{
	
}
void LoginDialog::OnClickTryLogin(wxCommandEvent & event)
{
	this->EndDialog(wxID_OK);
}
