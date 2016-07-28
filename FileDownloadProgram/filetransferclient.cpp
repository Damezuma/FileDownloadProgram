#include "filetransferclient.h"

#include <wx/socket.h>
#include <wx/sckaddr.h>

#include "client_protocol.h"
ClientFileTransfer * ClientFileTransfer::s_instance = nullptr;
const wxString SERVER_IP = wxT("piti.co.kr");
ClientFileTransfer::ClientFileTransfer()
{
	m_threadTransfer = new ThreadTransfer();
	m_threadTransfer->Run();
}
ClientFileTransfer::~ClientFileTransfer()
{
	if (m_threadTransfer != nullptr)
	{
		if (m_threadTransfer->IsDetached() == false)
		{
			m_threadTransfer->Wait();
		}
	}
}
void ClientFileTransfer::TryLogin(wxString id, wxString password, PasswordType passwordType, wxEvtHandler* eventHandler, const std::function<void(bool, wxString)> & handler)
{
	AddCommand(new CommandLogin(id, password, passwordType, eventHandler, handler));
}

void ClientFileTransfer::TryGetTransferLogs(wxEvtHandler * eventHandler, const std::function<void(bool,  wxString msg, std::vector<wxString>)>& handler)
{
	AddCommand(new CommandGetFileLog(eventHandler, handler));
}

void ClientFileTransfer::TryGetOTP(wxEvtHandler * eventHandler, const std::function<void(bool, wxString)>& handler)
{
	AddCommand(new CommandGetOTP(eventHandler, handler));
}

void ClientFileTransfer::AddCommand(ICommand * command)
{	
	m_queue.Post(command);
}
void * ClientFileTransfer::ThreadTransfer::Entry()
{
	wxSocketClient* socket = new wxSocketClient();
	wxIPV4address address;
	address.Hostname(SERVER_IP);
	address.Service(1455);
	if (!socket->Connect(address))
	{
		delete socket;
		socket = nullptr;
	}
	while (ClientFileTransfer::Instance().m_isEnd == false)
	{
		auto & instance = ClientFileTransfer::Instance();
		ICommand * command = nullptr;
		if (instance.m_queue.ReceiveTimeout(1000, command) == wxMSGQUEUE_NO_ERROR)
		{
			if (socket != nullptr)
			{
				if (socket->IsDisconnected())
				{
					socket = nullptr;
				}
			}
			command->Execute(socket);
			delete command;
		}
	}
	if (socket != nullptr)
	{
		socket->Close();
		delete socket;
	}
	return nullptr;
}
