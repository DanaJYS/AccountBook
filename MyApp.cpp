#include "MyApp.h"
#include "MyFrame.h"

DECLARE_APP(MyApp)

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	wxFrame* frame = new MyFrame(wxT("AccountBook"));
	frame->Show(true);
	return true;
}