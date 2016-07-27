#pragma once
#include "wx/wx.h"
#include "wx/event.h"
#include <functional>
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
	void TryLogin(wxString id, wxString password, PasswordType passwordType,  wxEvtHandler* eventHandler ,const std::function<void(bool)> & handler);
private:
	ClientFileTransfer(ClientFileTransfer& ref) {}
	ClientFileTransfer() {}

	class LoginThread : public wxThread
	{
	public:
		LoginThread(const wxString & userId, const wxString & password, PasswordType passwordType, wxEvtHandler* eventHandler, const std::function<void(bool)> & handler);
		~LoginThread() {

		}
		virtual void* Entry() override;
	private:
		wxString m_id;
		wxByte * m_shaBytes = nullptr;
		PasswordType m_passwordType;
		wxEvtHandler* m_eventHandler;
		std::function<void(bool)> m_handler;
	};
};
