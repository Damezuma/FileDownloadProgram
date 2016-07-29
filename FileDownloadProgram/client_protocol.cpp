#include "client_protocol.h"
#include "KISA_SHA256.h"
#include <wx/stdpaths.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/dir.h>
#include "ui.h"
#include <thread>
CommandGetFileLog::CommandGetFileLog(wxEvtHandler * eventHandler, const std::function<void(bool, wxString msg, std::vector<wxString>)>& handler)
{
	m_eventHandler = eventHandler;
	m_handler = handler;
}
bool CommandGetFileLog::Execute(wxSocketClient * socket)
{
	auto handler = this->m_handler;
	if (socket == nullptr)
	{
		
		m_eventHandler->CallAfter([handler] {
			handler(false, wxT("CAN NOT CONNECT TO SERVER"),std::vector<wxString>());
		});
	}
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

	
	m_eventHandler->CallAfter([this, handler, res, msg, resultList] {
		handler(res, msg, *resultList);
		delete resultList;
	});
	return true;
}

bool CommandGetRemainedFileList::Execute(wxSocketClient * socket)
{
	if (socket == nullptr)
	{
		return true;
	}
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
	if (socket == nullptr)
	{
		return true;
	}
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
		return true;
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

			if (socket->IsDisconnected() == false)
			{
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
					else if(socket->IsDisconnected())
					{
						break;
					}
				} while (true);
			BREAK_DO_WHILE:
				if (!socket->IsDisconnected())
				{
					wxString msg = wxString::FromUTF8(buf, buf.GetDataLen());
					if (msg == wxT("SUCCESS USER LOGIN"))
					{
						res = true;
					}
				}
				else
				{
					msg = wxT("SERVER IS DISCONNECTED!");
				}
				
			}
			else
			{
				msg = wxT("SERVER IS DISCONNECTED!");
			}
		}
	}
	m_eventHandler->CallAfter([this, handler, res, msg] {
		handler(res, msg);
	});
	delete[] m_shaBytes;
	return true;
}

CommandCompleteDownloadFile::CommandCompleteDownloadFile(int uid)
{
	m_uid = uid;
}

bool CommandCompleteDownloadFile::Execute(wxSocketClient * socket)
{
	if (socket == nullptr)
	{
		return true;
	}
	wxMemoryBuffer memory;
	auto CMD_LIST = "COMPLETE GET FILE\n";
	wxString uid = wxString::Format(wxT("%d\n"), m_uid);
	auto data = uid.ToUTF8();
	int sizeLastRead;
	bool isEnd = false;
	int step = 0;
	wxString msg;
	std::vector<wxString>* resultList = new std::vector<wxString>();

	socket->Write(CMD_LIST, strlen(CMD_LIST));
	socket->Write(data, data.length());

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
				memory.AppendData(temp + lastIndex, i + 1 - lastIndex);
				lastIndex = i;
				switch (step)
				{
				case 0: {
					wxString ressultCode = wxString::FromUTF8(memory, memory.GetDataLen());
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
					wxString listString = wxString::FromUTF8(memory, memory.GetDataLen());
					listString.Remove(0, listString.Find(wxT("\n")) + 1);
					int splitIndex = -1;
					while ((splitIndex = listString.Find(wxT(';'))) != -1)
					{
						wxString item = listString.substr(0, splitIndex);
						resultList->push_back(std::move(item));
						listString.Remove(0, splitIndex + 1);
					}
					isEnd = true;
					//res = true;
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
			memory.AppendData(temp, sizeLastRead);
		}
	} while (isEnd == false);

	if (UI::Instance().mainframe != nullptr)
	{
		wxMutexGuiEnter();
		if (UI::Instance().mainframe != nullptr)
		{
			UI::Instance().mainframe->UpdateLogList(std::move(*resultList));
		}
		wxMutexGuiLeave();
		delete resultList;
	}

	return true;
}

CommandGetOTP::CommandGetOTP(wxEvtHandler * eventHandler, const std::function<void(bool, wxString)>& handler)
{
	m_eventHandler = eventHandler;
	m_handler = handler;
}

bool CommandGetOTP::Execute(wxSocketClient * socket)
{
	auto handler = m_handler;
	auto CMD_LIST = "GET OTP\n";
	int step = 1;
	wxMemoryBuffer memory;
	wxString reason = wxT("UNKNOWN");
	wxString OTPCode;
	bool res = false;
	if (socket == nullptr)
	{

		m_eventHandler->CallAfter([handler]() {
			handler(false, wxT("CAN NOT CONNECT TO SERVER!"));
		});
		return false;
	}

	socket->Write(CMD_LIST, strlen(CMD_LIST));
	while (true)
	{
		wxByte abyte = -1;
		if (socket->Read(&abyte, 1).GetLastIOReadSize() == 1)
		{
			if (abyte == '\n')
			{
				if (step == 1)
				{
					wxString msg = wxString::FromUTF8(memory, memory.GetDataLen());
					if (msg == wxT("OK"))
					{
						memory.Clear();
						step = 2;
					}
					else
					{
						reason = msg;
						break;
					}
				}
				else if(step == 2)
				{
					res = true;
					OTPCode = wxString::FromUTF8(memory, memory.GetDataLen());
					break;
				}
			}
			else
			{
				memory.AppendByte(abyte);
			}
		}
		else if (socket->IsDisconnected())
		{
			break;
		}
	}
	if (res)
	{
		m_eventHandler->CallAfter([handler, res, OTPCode]() {
			handler(res, OTPCode);
		});
	}
	else
	{
		m_eventHandler->CallAfter([handler, res, reason]() {
			handler(res, reason);
		});
	}
	
	return true;
}

CommandSendFile::CommandSendFile(const wxString& comments, std::vector<wxFileName> & files, wxEvtHandler* eventHandler, IFileTransferEvent* handler)
{
	m_handler = handler;
	m_eventHandler = eventHandler;
	m_files = files;
	m_comments = comments;
}

bool CommandSendFile::Execute(wxSocketClient * socket)
{
	auto handler = m_handler;
	if (socket == nullptr)
	{
		m_eventHandler->CallAfter([handler]() {
			handler->OnFaild(wxT("CAN NOT CONNECT TO SERVER!"));
		});
		return false;
	}
	auto CMD_LIST = "POST FILE\n";
	bool ableContinue = false;
	wxMemoryBuffer memory;
	socket->Write(CMD_LIST, strlen(CMD_LIST));
	
	{
		m_comments.Replace('\n', '\r');
		m_comments.Append(wxT('\n'));
		auto a = m_comments.ToUTF8();
		socket->Write(a, a.length());
	}
	wxByte byte;
	ableContinue = false;
	while (true)
	{
		if (socket->Read(&byte, 1).GetLastIOReadSize() != 0)
		{
			if (byte == '\n')
			{
				wxString msg = wxString::FromUTF8(memory, memory.GetDataLen());
				if (msg == wxT("OK"))
				{
					ableContinue = true;
				}
				else
				{
					m_eventHandler->CallAfter([handler]() {
						handler->OnFaild(wxT("SERVER DENEID ACCESS!"));
					});
					return true;
				}
			}
			memory.AppendByte(byte);
		}
		if (ableContinue == true)
		{
			break;
		}
	}
	
	int i = 0;
	while (i < m_files.size())
	{
		wxFile file;
		if (file.Open(m_files[i].GetFullPath()) == false)
		{
			wxString fullPath = m_files[i].GetFullPath();
			m_eventHandler->CallAfter([fullPath,handler]() {
				handler->OnFaild(wxT("CAN NOT ACCESS '") + fullPath + wxT("'"));
			});
			return false;
		}
		wxString fileName = m_files[i].GetName() + "." + m_files[i].GetExt();
		int fileSize = file.Length();
		{
			auto stringByteBuffer = fileName.ToUTF8();
			socket->Write(stringByteBuffer, stringByteBuffer.length());
			wxString fileSizeString = wxString::Format(wxT("|%d\n"), fileSize);
			stringByteBuffer = fileSizeString.ToUTF8();
			int writtenSize = 0;
			while (stringByteBuffer.length() != writtenSize)
			{
				writtenSize += socket->Write(stringByteBuffer.data() + writtenSize, stringByteBuffer.length() - writtenSize).GetLastIOWriteSize();
			}
			
		}
			
		wxByte* outputbuffer = new wxByte[1024 * 1024]; //1메가 단위로 버퍼를 읽는다.
		auto* receiver = m_handler;
		auto* eventProcesser = m_eventHandler;
		std::thread threadGetProgress([i,socket,fileSize, receiver, eventProcesser]() {
			long sizeServerGet = 0;
			wxMemoryBuffer memory;
			wxByte byte;
			memory.Clear();
			while (true)
			{
				if (socket->Read(&byte, 1).GetLastIOReadSize() != 0)
				{
					if (byte == '\n')
					{
						wxString msg = wxString::FromUTF8(memory, memory.GetDataLen());

						long sizeNowServerGet = 0;
						if (msg.ToLong(&sizeNowServerGet, 0))
						{
							sizeServerGet += sizeNowServerGet;
							receiver->OnProgrssTransferFile(i, fileSize, sizeServerGet);
							if (sizeServerGet == fileSize)
							{
								receiver->OnProgrssTransferFile(i, fileSize, sizeServerGet);
								break;
							}
						}
						else
						{
							eventProcesser->CallAfter([receiver]() {
								receiver->OnFaild(wxT("SERVER DENEID ACCESS!"));
							});
							break;
						}
						memory.Clear();
					}
					memory.AppendByte(byte);
				}
				else if (socket->IsDisconnected())
				{
					eventProcesser->CallAfter([receiver]() {
						receiver->OnFaild(wxT("DISCONNECTED!"));
					});
					break;
				}
			}
		});
		do
		{

			ssize_t readSize = file.Read(outputbuffer, 1024 * 1024);
			if (readSize > 0)
			{
				int writeSize = socket->Write(outputbuffer, readSize).GetLastIOWriteSize();
				while (writeSize < readSize)
				{
					writeSize += socket->Write(outputbuffer + writeSize, readSize - writeSize).GetLastIOWriteSize();
				}
			}
		} while (file.Eof() == false);
		file.Close();
		if (threadGetProgress.joinable())
		{
			threadGetProgress.join();
		}
		else
		{
			threadGetProgress.detach();
		}
		
		i++;
		delete[]outputbuffer;
	}
	char END_SIGN[] = "END|0\n";
	socket->Write(END_SIGN, strlen(END_SIGN));
	m_handler->OnComplete();
	return true;
}
