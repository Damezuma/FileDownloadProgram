#pragma once
#include "wx/wx.h"
#include "wx/socket.h"
#include "filetransferclient.h"
#include <wx/filename.h>
class CommandLogin : public ICommand {
public:
	CommandLogin(const wxString & userId, const wxString & password, PasswordType passwordType, wxEvtHandler* eventHandler, const std::function<void(bool, wxString)> & handler);
	virtual bool Execute(wxSocketClient * socket)override;
private:
	wxString m_id;
	wxByte * m_shaBytes = nullptr;
	PasswordType m_passwordType;
	wxEvtHandler* m_eventHandler;
	std::function<void(bool, wxString)> m_handler;
};
class CommandGetFileLog: public ICommand {
public:
	CommandGetFileLog(wxEvtHandler* eventHandler, const std::function<void(bool, wxString msg, std::vector<wxString>)> & handler);
	virtual bool Execute(wxSocketClient * socket)override;
private:
	wxEvtHandler* m_eventHandler;
	std::function<void(bool, wxString msg, std::vector<wxString>)>  m_handler;
};
class CommandGetRemainedFileList: public ICommand {
public:
	virtual bool Execute(wxSocketClient * socket)override;
};
class CommandGetFile : public ICommand {
public:
	CommandGetFile(const wxString & m_directory, int uid);
	virtual bool Execute(wxSocketClient * socket) override;
private:
	int m_uid;
	wxString m_directory;
};
class CommandCompleteDownloadFile : public ICommand {
public:
	CommandCompleteDownloadFile(int uid);
	virtual bool Execute(wxSocketClient * socket) override;
private:
	int m_uid;
};
class CommandGetOTP : public ICommand {
public:
	CommandGetOTP(wxEvtHandler* eventHandler, const std::function<void(bool, wxString)> & handler);
	virtual bool Execute(wxSocketClient * socket) override;
private:
	wxEvtHandler* m_eventHandler;
	std::function<void(bool, wxString)> m_handler;
};



class IFileTransferEvent {
public:
	virtual void OnFaild(const wxString & reason) = 0;
	virtual void OnProgrssTransferFile(int index,int totoalSize, int readSize) = 0;
	virtual void OnComplete() = 0;
};
class CommandSendFile : public ICommand {
public:
	CommandSendFile(const wxString& comments, std::vector<wxFileName> & files,  wxEvtHandler* eventHandler, IFileTransferEvent * handler);
	virtual bool Execute(wxSocketClient * socket) override;
private:
	std::vector<wxFileName> m_files;
	wxEvtHandler* m_eventHandler;
	IFileTransferEvent* m_handler;
	wxString m_comments;
};