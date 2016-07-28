#include "logindialog.h"
#include "logindialog.h"
#include "filetransferclient.h"
#include <wx/activityindicator.h>
LoginDialog::LoginDialog() :GUILoginDialog(nullptr)
{
	
}
wxString LoginDialog::GetId()
{
	return this->ui_idTextBox->GetValue();
}
wxString LoginDialog::GetPassword()
{
	return this->ui_passwordTextBox->GetValue();;
}
void LoginDialog::OnClickTryLogin(wxCommandEvent & event)
{
	auto & instance = ClientFileTransfer::Instance();
	wxActivityIndicator * indicator = new wxActivityIndicator(this, wxID_ANY);
	indicator->FitInside();
	indicator->Fit();
	indicator->CentreOnParent();
	indicator->Start();
	Enable(false);
	instance.TryLogin(GetId(), GetPassword(), PasswordType::UserPassword, this, [this, indicator](bool res,wxString msg)->void {
		this->Enable(true);
		indicator->Stop();
		indicator->Close(true);
		indicator->Destroy();
		
		if (res)
		{
			this->EndDialog(wxID_OK);
		}
		else
		{
			wxMessageBox(msg, wxT("Error!"));
		}
	});
}
