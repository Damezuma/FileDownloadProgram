#include "client_protocol.h"
#include "KISA_SHA256.h"
#include <wx/stdpaths.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/dir.h>
CommandGetFileLog::CommandGetFileLog(wxEvtHandler * eventHandler, const std::function<void(bool, wxString msg, std::vector<wxString>)>& handler)
{
	m_eventHandler = eventHandler;
	m_handler = handler;
}
bool CommandGetFileLog::Execute(wxSocketClient * socket)
{
	auto CMD_LIST = "LIST\n";
	socket->Write(CMD_LIST, strlen(CMD_LIST));
	wxMemoryBuffer buffer;
	bool isEnd = false;
	bool res = false;
	wxString msg;
	std::vector<wxString>* resultList = new std::vector<wxString>();
	int step = 0;
	socket->SetTimeout(5);

	do
	{
		int sizeLastRead = 0;
		BYTE temp[2048];
		if (socket->IsDisconnected())
		{
			msg = wxT("SERVER IS DISCONNECTED!");
			break;
		}
		socket->WaitForRead(5);
		sizeLastRead = socket->Read(temp, 2048).GetLastIOReadSize();
		int lastIndex = 0;
		for (int i = 0; i < sizeLastRead; i++)
		{
			if (temp[i] == '\n')
			{
				buffer.AppendData(temp + lastIndex, i + 1 - lastIndex);
				lastIndex = i;
				switch (step)
				{
				case 0: {
					wxString ressultCode = wxString::FromUTF8(buffer, buffer.GetDataLen());
					if (ressultCode == wxT("OK\n"))
					{
						step = 1;
					}
					else
					{
						isEnd = true;
						msg = wxT("SERVER DENIED!");
					}
				}
						break;
				case 1: {
					wxString listString = wxString::FromUTF8(buffer, buffer.GetDataLen());
					listString.Remove(0, listString.Find(wxT("\n")) + 1);
					int splitIndex = -1;
					while ((splitIndex = listString.Find(wxT(';'))) != -1)
					{
						wxString item = listString.substr(0, splitIndex);
						resultList->push_back(std::move(item));
						listString.Remove(0, splitIndex + 1);
					}
					isEnd = true;
					res = true;
				}
						break;
				}
				if (isEnd)
				{
					break;
				}
			}
		}
		if (lastIndex == 0)
		{
			buffer.AppendData(temp, sizeLastRead);
		}
	} while (isEnd == false);

	auto handler = this->m_handler;
	m_eventHandler->CallAfter([this, handler, res, msg, resultList] {
		handler(res, msg, *resultList);
		delete resultList;
	});
	return true;
}

bool CommandGetRemainedFileList::Execute(wxSocketClient * socket)
{
	auto CMD_LIST = "REMAINED FILE LIST\n";
	socket->Write(CMD_LIST, strlen(CMD_LIST));
	
	bool isEnd = false;
	int step = 0;
	wxMemoryBuffer memory;
	auto stantardPath = wxStandardPaths::Get();
	wxDateTime time = wxDateTime::Now();
	wxFileName directory_path = stantardPath.GetDocumentsDir();
	directory_path.AppendDir(time.Format(wxT("%Y%m%d%H%M")));
	

	wxByte abyte = 0;
	while (isEnd == false)
	{
		switch (step)
		{
		case 0:
			if (socket->Read(&abyte, 1).GetLastIOReadSize() == 1)
			{
				if (abyte == '\n')
				{
					if (wxString::FromUTF8(memory, memory.GetDataLen()) == wxT("OK"))
					{
						memory.Clear();
						step = 1;
					}
				}
				else
				{
					memory.AppendByte(abyte);
				}
			}
			else
			{
				isEnd = true;
			}
			break;
		case 1:
			if (socket->Read(&abyte, 1).GetLastIOReadSize() == 1)
			{
				if (abyte == '\n')
				{
					wxString fileInfo = wxString::FromUTF8(memory, memory.GetDataLen());
					if (fileInfo.Length() == 0)
					{
						isEnd = true;
						break;
					}
					if (wxDir::Exists(directory_path.GetPath(true)) == false)
					{
						wxDir::Make(directory_path.GetPath(true));
					}
					while (fileInfo.Find(wxT(";")) != -1)
					{
						wxString fileName = fileInfo.substr(0, fileInfo.Find(wxT('|')));
						fileInfo.Remove(0, fileInfo.Find(wxT('|')) + 1);
						wxString fileUid = fileInfo.substr(0, fileInfo.Find(wxT(';')));
						fileInfo.Remove(0, fileInfo.Find(wxT(';')) + 1);
						long uid = -1;
						fileUid.ToLong(&uid);
						ClientFileTransfer::Instance().AddCommand(new CommandGetFile(directory_path.GetFullPath(), uid));
					}

					isEnd = true;
				}
				else
				{
					memory.AppendByte(abyte);
				}
			}
			break;
		}
	}
	return false;
}

CommandGetFile::CommandGetFile(const wxString & directory, int uid)
{
	m_directory = directory;
	m_uid = uid;
}

bool CommandGetFile::Execute(wxSocketClient * socket)
{
	wxFile * file = nullptr;
	bool isEnd = false;
	int step = 0;
	long remainFileSize = 0;
	wxMemoryBuffer memory;
	wxByte abyte = 0;
	BYTE temp[2048] = { 0 };
	char  CMD_LIST[] ="GET FILE\n";
	socket->Write(CMD_LIST, strlen(CMD_LIST));
	wxString uid = wxString::Format(wxT("%d\n"),m_uid);
	auto data = uid.ToUTF8();
	socket->Write(data.data(), data.length());
	
	while (isEnd == false)
	{
		switch (step)
		{
		case 0:
			if (socket->Read(&abyte, 1).GetLastIOReadSize() == 1)
			{
				if (abyte == '\n')
				{
					if (wxString::FromUTF8(memory, memory.GetDataLen()) == wxT("OK"))
					{
						memory.Clear();
						step = 1;
					}
				}
				else
				{
					memory.AppendByte(abyte);
				}
			}
			else
			{
				isEnd = true;
			}
			break;
		case 1:
			if (socket->Read(&abyte, 1).GetLastIOReadSize() == 1)
			{
				if (abyte == '\n')
				{
					wxString fileInfo = wxString::FromUTF8(memory, memory.GetDataLen());
					if (fileInfo.Length() == 0)
					{
						isEnd = true;
						break;
					}
					
					wxString fileName = fileInfo.substr(0, fileInfo.Find(wxT('|')));
					fileInfo.Remove(0, fileInfo.Find(wxT('|')) + 1);
					wxString fileSize = fileInfo;
					fileSize.ToLong(&remainFileSize);
					wxFileName filePath = m_directory;
					filePath.SetName(fileName);
					wxFile file;
					file.Create(filePath.GetFullPath());
					do
					{
						int bufferSize = 2048;
						if (remainFileSize < 2048)
						{
							bufferSize = remainFileSize;
						}
						int readSize = socket->Read(temp, bufferSize).GetLastIOReadSize();
						if (readSize != 0)
						{
							remainFileSize -= readSize;
							file.Write(temp, readSize);
						}
					} while (remainFileSize > 0);
					file.Close();
					isEnd = true;
					ClientFileTransfer::Instance().AddCommand(new CommandCompleteDownloadFile(m_uid));
				}
				else
				{
					memory.AppendByte(abyte);
				}
			}
			break;
		}
	}
	return true;
}

CommandLogin::CommandLogin(const wxString & userId, const wxString & password, PasswordType passwordType, wxEvtHandler * eventHandler, const std::function<void(bool, wxString)>& handler)
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

bool CommandLogin::Execute(wxSocketClient * socket)
{
	
	bool res = false;
	wxString msg;
	auto handler = this->m_handler;
	wxByte buffer[9];
	int countReadByte = 0;
	if (socket == nullptr)
	{
		m_eventHandler->CallAfter([handler] {
			handler(false, wxT("CAN NOT CONNECT TO SERVER"));
		});
		delete[] m_shaBytes;
	}
	do {
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
			do
			{
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
				res = true;
			}
		}
	}
	if (res == false)
	{
		socket->Close();
		delete socket;
	}

	m_eventHandler->CallAfter([this, handler, res, msg] {
		handler(res, msg);
	});
	delete[] m_shaBytes;
	if (res)
	{
		ClientFileTransfer::Instance().AddCommand(new CommandGetRemainedFileList());
	}
	return true;
}

CommandCompleteDownloadFile::CommandCompleteDownloadFile(int uid)
{
	m_uid = uid;
}

bool CommandCompleteDownloadFile::Execute(wxSocketClient * socket)
{
	wxMemoryBuffer memory;
	auto CMD_LIST = "COMPLETE GET FILE\n";
	wxString uid = wxString::Format(wxT("%d\n"), m_uid);
	auto data = uid.ToUTF8();
	
	socket->Write(CMD_LIST, strlen(CMD_LIST));
	socket->Write(data, data.length());
	while (true)
	{
		wxByte abyte = -1;
		if (socket->Read(&abyte, 1).GetLastIOReadSize() == 1)
		{
			if (abyte == '\n')
			{
				if (wxString::FromUTF8(memory, memory.GetDataLen()) == wxT("OK"))
				{
					memory.Clear();
					break;
				}
			}
			else
			{
				memory.AppendByte(abyte);
			}
		}
		else if(socket->IsDisconnected())
		{
			break;
		}
	}
	return true;
}
