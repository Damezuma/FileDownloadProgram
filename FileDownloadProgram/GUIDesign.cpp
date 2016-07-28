///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "GUIDesign.h"

///////////////////////////////////////////////////////////////////////////

GUIMainFrame::GUIMainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel3 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_splitter2 = new wxSplitterWindow( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter2->Connect( wxEVT_IDLE, wxIdleEventHandler( GUIMainFrame::m_splitter2OnIdle ), NULL, this );
	
	m_panel4 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText3 = new wxStaticText( m_panel4, wxID_ANY, wxT("전송사유"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer6->Add( m_staticText3, 0, wxALL, 5 );
	
	ui_reasonSendFileTextCtrl = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	bSizer6->Add( ui_reasonSendFileTextCtrl, 1, wxEXPAND, 5 );
	
	
	m_panel4->SetSizer( bSizer6 );
	m_panel4->Layout();
	bSizer6->Fit( m_panel4 );
	m_panel5 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	ui_listSendFiles = new wxListBox( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer7->Add( ui_listSendFiles, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	ui_btnAddFiles = new wxButton( m_panel5, wxID_ANY, wxT("파일추가"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer10->Add( ui_btnAddFiles, 0, wxALL, 5 );
	
	ui_btnTrySendFile = new wxButton( m_panel5, wxID_ANY, wxT("전송"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer10->Add( ui_btnTrySendFile, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizer7->Add( bSizer10, 0, wxEXPAND, 5 );
	
	
	m_panel5->SetSizer( bSizer7 );
	m_panel5->Layout();
	bSizer7->Fit( m_panel5 );
	m_splitter2->SplitHorizontally( m_panel4, m_panel5, 0 );
	bSizer4->Add( m_splitter2, 1, wxEXPAND, 5 );
	
	
	m_panel3->SetSizer( bSizer4 );
	m_panel3->Layout();
	bSizer4->Fit( m_panel3 );
	m_notebook1->AddPage( m_panel3, wxT("전송"), false );
	m_panel6 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	ui_logList = new wxListCtrl( m_panel6, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	bSizer8->Add( ui_logList, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel6->SetSizer( bSizer8 );
	m_panel6->Layout();
	bSizer8->Fit( m_panel6 );
	m_notebook1->AddPage( m_panel6, wxT("전송기록"), false );
	
	bSizer3->Add( m_notebook1, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer3 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	ui_btnAddFiles->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIMainFrame::OnClickAddFile ), NULL, this );
	ui_btnTrySendFile->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIMainFrame::OnClickSubmit ), NULL, this );
}

GUIMainFrame::~GUIMainFrame()
{
	// Disconnect Events
	ui_btnAddFiles->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIMainFrame::OnClickAddFile ), NULL, this );
	ui_btnTrySendFile->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIMainFrame::OnClickSubmit ), NULL, this );
	
}

GUILoginDialog::GUILoginDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("ID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer2->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	ui_idTextBox = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( ui_idTextBox, 0, wxALL, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Password"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer2->Add( m_staticText2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	ui_passwordTextBox = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	fgSizer2->Add( ui_passwordTextBox, 0, wxALL, 5 );
	
	
	bSizer2->Add( fgSizer2, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	ui_radioPassword = new wxRadioButton( this, wxID_ANY, wxT("비밀번호"), wxDefaultPosition, wxDefaultSize, 0 );
	ui_radioPassword->SetValue( true ); 
	bSizer9->Add( ui_radioPassword, 0, wxALL, 5 );
	
	ui_radioOTP = new wxRadioButton( this, wxID_ANY, wxT("OTP"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( ui_radioOTP, 0, wxALL, 5 );
	
	
	bSizer2->Add( bSizer9, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_button1 = new wxButton( this, wxID_ANY, wxT("로그인"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_button1, 0, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer2 );
	this->Layout();
	bSizer2->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUILoginDialog::OnClickTryLogin ), NULL, this );
}

GUILoginDialog::~GUILoginDialog()
{
	// Disconnect Events
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUILoginDialog::OnClickTryLogin ), NULL, this );
	
}

GUIUploadProgressDialog::GUIUploadProgressDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("전체"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer2->Add( m_staticText4, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	ui_progress1 = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	ui_progress1->SetValue( 0 ); 
	fgSizer2->Add( ui_progress1, 0, wxALL, 5 );
	
	ui_fileName = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	ui_fileName->Wrap( -1 );
	fgSizer2->Add( ui_fileName, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	ui_progress2 = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	ui_progress2->SetValue( 0 ); 
	fgSizer2->Add( ui_progress2, 0, wxALL, 5 );
	
	
	this->SetSizer( fgSizer2 );
	this->Layout();
	fgSizer2->Fit( this );
	
	this->Centre( wxBOTH );
}

GUIUploadProgressDialog::~GUIUploadProgressDialog()
{
}
