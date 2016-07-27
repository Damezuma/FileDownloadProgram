#pragma once
#include "wx/wx.h"
#include "wx/event.h"
#include <functional>
#include <wx/socket.h>
enum PasswordType
{
	OTP,
	UserPassword
};
class ClientFileTransfer
{
private:

	static ClientFileTransfer * s_instance;
public:
	static ClientFileTransfer& Instance() {
		if (s_instance == nullptr) { s_instance = new ClientFileTransfer(); }
		return *s_instance;
	};
	static void Release() {
		if (s_instance != nullptr)
		{
			delete s_instance;
		}
		
		s_instance = nullptr;
	}
	~ClientFileTransfer();
	void TryLogin(wxString id, wxString password, PasswordType passwordType,  wxEvtHandler* eventHandler , const std::function<void(bool, wxString)> & handler);
private:
	ClientFileTransfer(ClientFileTransfer& ref) {}
	ClientFileTransfer() {}

	wxSocketClient* m_socket = nullptr;;

	class LoginThread : public wxThread
	{
	public:
		LoginThread(const wxString & userId, const wxString & password, PasswordType passwordType, wxEvtHandler* eventHandler, const std::function<void(bool, wxString)> & handler);
		~LoginThread() {

		}
		virtual void* Entry() override;
	private:
		wxString m_id;
		wxByte * m_shaBytes = nullptr;
		PasswordType m_passwordType;
		wxEvtHandler* m_eventHandler;
		std::function<void(bool, wxString)> m_handler;
	};
};
