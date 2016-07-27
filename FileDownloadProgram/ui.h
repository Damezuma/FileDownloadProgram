#pragma once
#include "taskicon.h"
#include "MainFrame.h"
class UI
{
public:
	static UI& Instance() { if (s_instance == nullptr) { Initialize(); } return *s_instance; }
	static void Initialize();
	TaskIcon * taskIcon = nullptr;
	MainFrame * mainframe = nullptr;
private:
	UI();
	UI(UI & ref) {}
	static UI * s_instance;
};