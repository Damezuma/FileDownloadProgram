#pragma once
#include "wx/wx.h"
#include "wx/event.h"
#include <functional>
#include <wx/socket.h>
#include <wx/msgqueue.h>
enum PasswordType
{
	OTP,
	UserPassword
};

class ICommand
{
public:
	virtual bool Execute(wxSocketClient * socket) = 0;
	~ICommand() {}
};
class IFileTransferEvent;
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
			s_instance->m_isEnd = true;
			delete s_instance;
		}
		s_instance = nullptr;
	}
	~ClientFileTransfer();
	void TryLogin(wxString id, wxString password, PasswordType passwordType,  wxEvtHandler* eventHandler , const std::function<void(bool, wxString)> & handler);
	void TryGetTransferLogs(wxEvtHandler* eventHandler,  const std::function<void(bool, wxString msg, std::vector<wxString>)> & handler);
	void TryGetOTP(wxEvtHandler* eventHandler, const std::function<void(bool, wxString)> & handler);

	void AddCommand(ICommand * command);
	bool IsDownloadingFile() { return m_isDownloadingFile; }
	void SetDownloadingFile(bool isDownloadFile) { m_isDownloadingFile = isDownloadFile; }
private:
	bool m_isDownloadingFile = false;
	ClientFileTransfer(ClientFileTransfer& ref) {}
	ClientFileTransfer();
	wxSocketClient* m_socket = nullptr;;
	wxThread* m_threadTransfer = nullptr;
	wxMessageQueue<ICommand*> m_queue;
	bool m_isEnd = false;
	class ThreadTransfer : public wxThread
	{
	public:
		virtual void* Entry() override;
	};
};
