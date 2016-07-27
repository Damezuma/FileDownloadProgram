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
	delete s_instance;
	s_instance = nullptr;
}
