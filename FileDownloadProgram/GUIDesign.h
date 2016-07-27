///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUIDESIGN_H__
#define __GUIDESIGN_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/dirctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/splitter.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/listctrl.h>
#include <wx/notebook.h>
#include <wx/frame.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class GUIMainFrame
///////////////////////////////////////////////////////////////////////////////
class GUIMainFrame : public wxFrame 
{
	private:
	
	protected:
		wxNotebook* m_notebook1;
		wxPanel* m_panel3;
		wxSplitterWindow* m_splitter1;
		wxPanel* m_panel1;
		wxGenericDirCtrl* ui_dirTreeCtrl;
		wxPanel* m_panel2;
		wxSplitterWindow* m_splitter2;
		wxPanel* m_panel4;
		wxStaticText* m_staticText3;
		wxTextCtrl* ui_reasonSendFileTextCtrl;
		wxPanel* m_panel5;
		wxListBox* ui_listSendFiles;
		wxButton* ui_btnTrySendFile;
		wxPanel* m_panel6;
		wxListCtrl* ui_logList;
	
	public:
		
		GUIMainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("파일 다운로드 프로그램"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~GUIMainFrame();
		
		void m_splitter1OnIdle( wxIdleEvent& )
		{
			m_splitter1->SetSashPosition( 0 );
			m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( GUIMainFrame::m_splitter1OnIdle ), NULL, this );
		}
		
		void m_splitter2OnIdle( wxIdleEvent& )
		{
			m_splitter2->SetSashPosition( 0 );
			m_splitter2->Disconnect( wxEVT_IDLE, wxIdleEventHandler( GUIMainFrame::m_splitter2OnIdle ), NULL, this );
		}
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class GUILoginDialog
///////////////////////////////////////////////////////////////////////////////
class GUILoginDialog : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxTextCtrl* ui_idTextBox;
		wxStaticText* m_staticText2;
		wxTextCtrl* ui_passwordTextBox;
		wxButton* m_button1;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClickTryLogin( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		GUILoginDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("로그인"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~GUILoginDialog();
	
};

#endif //__GUIDESIGN_H__
