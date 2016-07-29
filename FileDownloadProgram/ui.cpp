#include "ui.h"
UI* UI::s_instance = nullptr;
UI::UI()
{

}
void UI::Initialize()
{
	if (s_instance == nullptr)
	{
		s_instance = new UI();
	}
}

void UI::Release()
{
	if (s_instance->mainframe != nullptr)
	{
		delete s_instance->mainframe;
		s_instance->mainframe = nullptr;
	}
	if (s_instance->taskIcon != nullptr)
	{
		delete s_instance->taskIcon;
		s_instance->taskIcon = nullptr;
	}
	
	delete s_instance;
	s_instance = nullptr;
}
