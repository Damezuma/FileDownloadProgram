#include "filetransferclient.h"
#include "KISA_SHA256.h"

ClientFileTransfer * ClientFileTransfer::s_instance = nullptr;

ClientFileTransfer::~ClientFileTransfer()
{
}
void ClientFileTransfer::TryLogin(wxString id, wxString password, PasswordType passwordType, wxEvtHandler* eventHandler, const std::function<void(bool)> & handler)
{
	LoginThread * thread = new LoginThread(id, password, passwordType, eventHandler, handler);
	thread->Run();
}

ClientFileTransfer::LoginThread::LoginThread(const wxString & userId, const wxString & password, PasswordType passwordType, wxEvtHandler* eventHandler, const std::function<void(bool)> & handler)
{
	m_id = userId;
	m_shaBytes = new wxByte[64]{ 0 };
	m_id = userId;
	m_passwordType = passwordType;
	auto buf = password.ToUTF8();
	SHA256_Encrpyt((unsigned char *)buf.data(), buf.length(), m_shaBytes);

	m_passwordType = passwordType;
	m_eventHandler = eventHandler;
	m_handler = handler;
}

void * ClientFileTransfer::LoginThread::Entry()
{
	bool res = false;
	auto handler = this->m_handler;

	Sleep(5000);

	m_eventHandler->CallAfter([handler,res]{
		handler(res);
	});
	delete[] m_shaBytes;
	return nullptr;
}
