#include <wx/activityindicator.h>
#include "mainframe.h"
#include "ui.h"
#include "filetransferclient.h"
MainFrame::MainFrame() :GUIMainFrame(nullptr, wxID_ANY, TEXT("���� �ٿ�ε� ���α׷�"))
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
	ui_logList->AppendColumn(wxT("���� �̸�"));
	ui_logList->AppendColumn(wxT("��¥"));
	ui_logList->AppendColumn(wxT("����"));
	ui_logList->AppendColumn(wxT("���ۻ���"));
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
