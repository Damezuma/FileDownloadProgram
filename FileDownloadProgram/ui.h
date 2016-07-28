#pragma once
#include "taskicon.h"
#include "MainFrame.h"
#include "GUIDesign.h"
class UI
{
public:
	static UI& Instance() { if (s_instance == nullptr) { Initialize(); } return *s_instance; }
	static void Initialize();
	static void Release();
	TaskIcon * taskIcon = nullptr;
	MainFrame * mainframe = nullptr;
	GUIUploadProgressDialog * uploadPrograssDialog = nullptr;
private:
	UI();
	UI(UI & ref) {}
	static UI * s_instance;
};