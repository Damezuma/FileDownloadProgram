#include "filetransferclient.h"
#include "KISA_SHA256.h"
#include <wx/socket.h>
#include <wx/sckaddr.h>
ClientFileTransfer * ClientFileTransfer::s_instance = nullptr;
const wxString SERVER_IP = wxT("127.0.0.1");
ClientFileTransfer::~ClientFileTransfer()
{
	if (m_socket != nullptr)
	{
		m_socket->Close();
		delete m_socket;
	}
}
void ClientFileTransfer::TryLogin(wxString id, wxString password, PasswordType passwordType, wxEvtHandler* eventHandler, const std::function<void(bool, wxString)> & handler)
{
	LoginThread * thread = new LoginThread(id, password, passwordType, eventHandler, handler);
	thread->Run();
}

ClientFileTransfer::LoginThread::LoginThread(const wxString & userId, const wxString & password, PasswordType passwordType, wxEvtHandler* eventHandler, const std::function<void(bool, wxString)> & handler)
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
	wxString msg;
	auto handler = this->m_handler;

	wxSocketClient* socket = new wxSocketClient();
	wxIPV4address address;
	address.Hostname(SERVER_IP);
	address.Service(1455);
	
	if (socket->Connect(address))
	{
		wxByte buffer[9];
		int countReadByte = 0;
		do{
			if (socket->IsDisconnected())break;
			countReadByte += socket->Read(buffer + countReadByte, 9 - countReadByte).GetLastIOReadSize();
		} while (countReadByte != 9);
		if (socket->IsConnected())
		{
			wxString s = wxString::FromUTF8((char*)buffer, countReadByte);
//현재 접속한 서버 포트가 올바른 서버인지 체크한다.
			if (s.StartsWith(wxT("FTS")) == false)
			{
				msg = wxT("SERVER IS NOT OPENED!");
			}
			else
			{
				auto a = "FTS00.01\n";
				int sending_count = 0;
				do {
					if (socket->IsDisconnected())break;
					sending_count += socket->Write(a + sending_count, 9 - sending_count).LastWriteCount();
				} while (sending_count != 9);
				wxString hexSha;
				for (int i = 0; i < 32; i++)
				{
					hexSha.Append(wxString::Format(wxT("%02X"), m_shaBytes[i]));
				}
				wxString loginCommand = wxString::Format(wxT("%s;%s;%d\n"), m_id, hexSha, (m_passwordType == PasswordType::UserPassword) ? 0 : 1);
				auto c = loginCommand.ToUTF8();
				socket->Write(c, c.length());
				if (socket->IsDisconnected())
				{
					msg = wxT("SERVER IS DISCONNECTED!");
				}
				wxMemoryBuffer buf;

				do
				{
					wxByte temp[4] = { 0 };
					int lastReadCount = socket->Read(temp, 4).GetLastIOReadSize();
					for (int i = 0; i < lastReadCount; i++)
					{
						if (temp[i] == '\n')
						{
							lastReadCount = i;
							buf.AppendData(temp, lastReadCount);
							goto BREAK_DO_WHILE;
							break;
						}
					}
					if (lastReadCount != 0)
					{
						buf.AppendData(temp, lastReadCount);
					}
				} while (true);
BREAK_DO_WHILE:
				
				wxString msg = wxString::FromUTF8(buf, buf.GetDataLen());
				if (msg == wxT("SUCCESS USER LOGIN"))
				{
					ClientFileTransfer::Instance().m_socket = socket;
					res = true;
				}
			}
		}
	}
	else
	{
		msg = wxT("SERVER IS NOT OPENED!");
	}
	if (res == false)
	{
		socket->Close();
		delete socket;
	}
	
	m_eventHandler->CallAfter([handler,res,msg]{
		handler(res, msg);
	});
	delete[] m_shaBytes;
	return nullptr;
}
