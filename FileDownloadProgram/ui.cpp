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