#include <wx/activityindicator.h>
#include "mainframe.h"
#include "ui.h"
#include "filetransferclient.h"
#include "client_protocol.h"
#include "wx/filedlg.h "
MainFrame::MainFrame() :GUIMainFrame(nullptr, wxID_ANY, TEXT("파일 다운로드 프로그램"))
{
	SetIcon(wxICON(WXICON_AAA));
	wxActivityIndicator * indicator = new wxActivityIndicator(this, wxID_ANY);
	indicator->FitInside();
	indicator->Fit();
	indicator->CentreOnParent();
	indicator->Start();
	Enable(false);
	ClientFileTransfer::Instance().TryGetTransferLogs(this, [this, indicator](bool res, wxString msg, std::vector<wxString> list) {
		indicator->Stop();
		indicator->Destroy();
		Enable(true);
		if (res == false)
		{
			wxMessageBox(msg);
			return;
		}
		else
		{
			this->UpdateLogList(std::move(list));
		}
	});
}

MainFrame::~MainFrame()
{
	UI& ui = UI::Instance();
	ui.mainframe = nullptr;
}

void MainFrame::UpdateLogList(std::vector<wxString>&& data)
{
	
	ui_logList->ClearAll();
	ui_logList->AppendColumn(wxT("파일 이름"));
	ui_logList->AppendColumn(wxT("날짜"));
	ui_logList->AppendColumn(wxT("사유"));
	ui_logList->AppendColumn(wxT("전송상태"));
	int i = 0;
	for (wxString  row : data)
	{
		int t = row.Find(wxT('|'));
		wxString fileName = row.substr(0, t);
		row.Remove(0, t + 1);
		t = row.Find(wxT('|'));
		wxString date = row.substr(0, t);
		row.Remove(0, t + 1);
		t = row.Find(wxT('|'));
		wxString reasonTransfer = row.substr(0, t);
		row.Remove(0, t + 1);
		wxString status = row;

		wxListItem item;
		item.SetText(fileName);
		item.SetId(i);
		ui_logList->InsertItem(item);
		ui_logList->SetItem(i, 1, date);
		ui_logList->SetItem(i, 2, reasonTransfer);
		ui_logList->SetItem(i, 3, status);
		i++;
	}
}

void MainFrame::OnActivateLogPage(wxActivateEvent & event)
{
	
}

void MainFrame::OnClickAddFile(wxCommandEvent & event)
{
	wxFileDialog dialog(this, wxFileSelectorPromptStr, wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_OPEN | wxFD_MULTIPLE | wxFD_PREVIEW | wxFD_FILE_MUST_EXIST);
	if (dialog.ShowModal() == wxID_OK)
	{
		wxArrayString paths;
		dialog.GetPaths(paths);
		for (wxString & path : paths)
		{
			wxFileName name(path);
			ui_listSendFiles->Append(name.GetName() + wxT(".") + name.GetExt());
			m_fileNames.push_back(name);
		}
	}
	dialog.Destroy();
}
class Listener : public IFileTransferEvent
{
public:
	virtual void OnFaild(const wxString & reason)
	{

	}
	virtual void OnProgrssTransferFile(int index, int percent)
	{

	}
	virtual void OnComplete()
	{
	}
};
void MainFrame::OnClickSubmit(wxCommandEvent & event)
{
	UI::Instance().uploadPrograssDialog = new GUIUploadProgressDialog(this);
	UI::Instance().uploadPrograssDialog->ui_progress1->SetRange(m_fileNames.size());
	UI::Instance().uploadPrograssDialog->ui_progress1->SetValue(0);
	UI::Instance().uploadPrograssDialog->ui_progress2->SetValue(0);
	this->Enable(false);
	
	class Temp : public IFileTransferEvent {
	public:
		virtual void OnFaild(const wxString & reason)
		{
			UI::Instance().mainframe->Enable(true);
			UI::Instance().uploadPrograssDialog->EndModal(0);
			delete UI::Instance().uploadPrograssDialog;
			UI::Instance().uploadPrograssDialog = nullptr;

			wxMessageBox(reason);
		}
		virtual void OnProgrssTransferFile(int index, int totoalSize, int readSize)
		{
			auto & instance = UI::Instance();
			if (instance.uploadPrograssDialog->ui_progress1->GetValue() != index)
			{
				instance.uploadPrograssDialog->ui_progress1->SetValue(index);
			}
			if (instance.uploadPrograssDialog->ui_progress2->GetRange() != totoalSize)
			{
				instance.uploadPrograssDialog->ui_progress2->SetValue(0);
				instance.uploadPrograssDialog->ui_progress2->SetRange(totoalSize);
			}
			if (instance.uploadPrograssDialog->ui_progress2->GetValue() != readSize)
			{
				instance.uploadPrograssDialog->ui_progress2->SetValue(readSize);
			}
			wxString item = instance.mainframe->ui_listSendFiles->GetString(index);
			if (item != instance.uploadPrograssDialog->ui_fileName->GetLabelText())
			{
				instance.uploadPrograssDialog->ui_fileName->SetLabelText(item);
				instance.uploadPrograssDialog->Fit();
			}
			
		}
		virtual void OnComplete()
		{
			UI::Instance().uploadPrograssDialog->EndModal(0);
			delete UI::Instance().uploadPrograssDialog;
			UI::Instance().uploadPrograssDialog = nullptr;
			UI::Instance().mainframe->Enable(true);
			wxMessageBox(wxT("성공"));
		}
	private:
		
	};
	Temp a;
	ClientFileTransfer::Instance().AddCommand(new CommandSendFile(ui_reasonSendFileTextCtrl->GetValue(), m_fileNames, this, &a));
	UI::Instance().uploadPrograssDialog->ShowModal();
}

void MainFrame::OnClickOTP(wxCommandEvent & event)
{
	ClientFileTransfer::Instance().TryGetOTP(this, [this](bool res, wxString msg)->void {
		if (res)
		{
			wxMessageBox(wxString::Format(wxT("OTP는 %s입니다"), msg), wxT("OTP발급"));
		}
		else
		{
			wxMessageBox(msg, wxT("Error!"));
		}
	});
}
