#pragma  once
#include "wx/wx.h"
#include "wx/artprov.h"
#include "wx/grid.h"
#include "wx/statline.h"
#include "GlobalValue.h"

class MyFrame:public wxFrame
{
public:
	MyFrame(const wxString &title=wxT("AUI"));
	~MyFrame();

	void				InitMainUI();
	void				OnMonTextCtrlEnter(wxCommandEvent& event);

	bool				InsertNodeToList(const pMyGridItem pNewNode);
	int					GetLengthOfList(const pMyGridItem pNodePos);
	void				EraseList(const pMyGridItem pNodePos);

private:
	wxStatusBar*		StatusBar;

	wxGrid*				m_grid;
	wxChoice*			TypeChoice;
	wxTextCtrl*			MonTextCtrl;

	int					InsertPos;
	pMyGridItem			pMyGridItemHead;

	DECLARE_EVENT_TABLE()
};
