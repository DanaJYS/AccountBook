#include "MyFrame.h"

enum
{
	ID_MonTextCtrlEnter,
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)

	EVT_TEXT_ENTER(ID_MonTextCtrlEnter, MyFrame::OnMonTextCtrlEnter)

END_EVENT_TABLE()

MyFrame::MyFrame(const wxString &title)
:wxFrame(NULL,wxID_ANY,title,wxPoint(300,200),wxSize(500,350))
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	typeTotalNum.fType0TotalNum = 0;
	typeTotalNum.fType1TotalNum = 0;
	typeTotalNum.fTotalNum = 0;

	InitMainUI();

	InsertPos = 0;

	pMyGridItemHead = new MyGridItem;
	pMyGridItemHead->cItemType = 0;
	pMyGridItemHead->fItemNum = 0;
	pMyGridItemHead->stItemTime.Year = "0";
	pMyGridItemHead->stItemTime.Month = "0";
	pMyGridItemHead->stItemTime.Day = "0";
	pMyGridItemHead->stItemTime.hmsTime = "0";
	pMyGridItemHead->pNextItem = NULL;
	pMyGridItemHead->pPreItem = NULL;

	this->Centre( wxBOTH );
}

MyFrame::~MyFrame()
{
	EraseList(pMyGridItemHead);
}

void MyFrame::InitMainUI()
{
	wxBoxSizer* topSizer;
	topSizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* gridSizer;
	gridSizer = new wxBoxSizer( wxVERTICAL );

	m_grid = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxSize(400, 200), 0 );

	// Grid
	m_grid->CreateGrid( 1, 3 );
	m_grid->EnableEditing( true );
	m_grid->EnableGridLines( true );
	m_grid->EnableDragGridSize( false );
	m_grid->SetMargins( 0, 0 );

	// Columns
	m_grid->EnableDragColMove( false );
	m_grid->EnableDragColSize( true );
	m_grid->SetColLabelSize( 30 );
	for (int i=0; i<3; i++)
	{
		m_grid->SetColSize(i, 150);
	}
	m_grid->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	m_grid->EnableDragRowSize( true );
	m_grid->SetRowLabelSize( 80 );
	m_grid->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Label Appearance

	// Cell Defaults
	m_grid->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	m_grid->SetColLabelValue( 0, wxT("项目") );
	m_grid->SetColLabelValue( 1, wxT("时间(月/日/年 时:分:秒)") );
	m_grid->SetColLabelValue( 2, wxT("金额") );

	wxString wxStrType0Num = wxT("收入=")+wxString::Format(wxT("%.3f"), typeTotalNum.fType0TotalNum);
	wxString wxStrType1Num = wxT("支出=")+wxString::Format(wxT("%.3f"), typeTotalNum.fType1TotalNum);
	wxString wxStrTotalNum = wxT("结余=")+wxString::Format(wxT("%.3f"), typeTotalNum.fTotalNum);
	m_grid->SetCellValue(0, 0, wxStrType0Num);
	m_grid->SetCellValue(0, 1, wxStrType1Num);
	m_grid->SetCellValue(0, 2, wxStrTotalNum);
	wxColor color = RGB(0,0,200);
	m_grid->SetCellBackgroundColour(color, 0, 0);
	m_grid->SetCellBackgroundColour(color, 0, 1);
	m_grid->SetCellBackgroundColour(color, 0, 2);
	//m_grid->AutoSizeColumn(1);
	//m_grid->AutoSizeColLabelSize(1);
	
	gridSizer->Add( m_grid, 1, wxALL|wxEXPAND, 5 );


	topSizer->Add( gridSizer, 2, wxEXPAND, 5 );

	wxStaticLine* m_statLine1;
	m_statLine1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );

	topSizer->Add( m_statLine1, 0, wxEXPAND, 5 );

	wxBoxSizer* bottomSizer;
	bottomSizer = new wxBoxSizer( wxHORIZONTAL );

	wxString InOutChoiceChoices[] = { wxT("收入"), wxT("支出") };
	int InOutChoiceNChoices = sizeof( InOutChoiceChoices ) / sizeof( wxString );
	TypeChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, InOutChoiceNChoices, InOutChoiceChoices, 0 );
	TypeChoice->SetSelection( 0 );
	bottomSizer->Add( TypeChoice, 0, wxALL, 5 );

	MonTextCtrl = new wxTextCtrl( this, ID_MonTextCtrlEnter, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bottomSizer->Add( MonTextCtrl, 0, wxALL, 5 );


	topSizer->Add( bottomSizer, 1, wxEXPAND, 5 );


	this->SetSizer( topSizer );
	this->Layout();

	StatusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
}

void MyFrame::OnMonTextCtrlEnter(wxCommandEvent& event)
{
	//获取cItemType
	int itemType = TypeChoice->GetSelection();

	//获取dItemNum
	wxString strValue = MonTextCtrl->GetValue();
	double fValue = 0;
	if (!strValue.ToDouble(&fValue))
	{
		SetStatusText(wxT("输入错误！"));
		MonTextCtrl->Clear();
		return;
	}

	//获取strItemTime
	MyTime curTime = GetCurTime();

	//创建新项目的节点
	pMyGridItem MyGridItemNode = new MyGridItem;
	MyGridItemNode->cItemType = itemType;
	MyGridItemNode->fItemNum = fValue;
	MyGridItemNode->stItemTime = curTime;
	MyGridItemNode->pPreItem = NULL;
	MyGridItemNode->pNextItem = NULL;

	//将新项目节点插入到双向链表中
	wxColor backgroundColor;
	if ( InsertNodeToList(MyGridItemNode) )
	{
		m_grid->InsertRows(InsertPos, 1);
		int GridRowNum = m_grid->GetNumberRows()-1;
		if (MyGridItemNode->cItemType == 0)
		{
			m_grid->SetCellValue(InsertPos, 0, wxT("收入"));
			backgroundColor = RGB(200, 0, 0);

			typeTotalNum.fType0TotalNum += MyGridItemNode->fItemNum;
			typeTotalNum.fTotalNum += MyGridItemNode->fItemNum;
			wxString wxStrType0Num = wxT("收入=")+wxString::Format(wxT("%.3f"), typeTotalNum.fType0TotalNum);
			wxString wxStrTotalNum = wxT("结余=")+wxString::Format(wxT("%.3f"), typeTotalNum.fTotalNum);
			m_grid->SetCellValue(wxStrType0Num, GridRowNum, 0);
			m_grid->SetCellValue(wxStrTotalNum, GridRowNum, 2);
		}
		else if (MyGridItemNode->cItemType == 1)
		{
			m_grid->SetCellValue(InsertPos, 0, wxT("支出"));
			backgroundColor = RGB(200, 200, 0);

			typeTotalNum.fType1TotalNum += MyGridItemNode->fItemNum;
			typeTotalNum.fTotalNum -= MyGridItemNode->fItemNum;
			wxString wxStrType1Num = wxT("支出=")+wxString::Format(wxT("%.3f"), typeTotalNum.fType1TotalNum);
			wxString wxStrTotalNum = wxT("结余=")+wxString::Format(wxT("%.3f"), typeTotalNum.fTotalNum);
			m_grid->SetCellValue(wxStrType1Num, GridRowNum, 1);
			m_grid->SetCellValue(wxStrTotalNum, GridRowNum, 2);
		}
		
		string strDispTime = MyGridItemNode->stItemTime.Month+"/"+MyGridItemNode->stItemTime.Day+"/"+
							MyGridItemNode->stItemTime.Year+" "+MyGridItemNode->stItemTime.hmsTime;
		wxString wxStrDispTime(strDispTime);
		m_grid->SetCellValue(InsertPos, 1, wxStrDispTime);

		wxString wxStrDispNum = wxString::Format(wxT("%.3f"), MyGridItemNode->fItemNum);
		m_grid->SetCellValue(InsertPos, 2, wxStrDispNum);

		m_grid->SetCellBackgroundColour(backgroundColor, InsertPos, 0);
		m_grid->SetCellBackgroundColour(backgroundColor, InsertPos, 1);
		m_grid->SetCellBackgroundColour(backgroundColor, InsertPos, 2);
	}
	else
	{
		SetStatusText(wxT("新项目插入失败！"));
		return;
	}
}

bool MyFrame::InsertNodeToList(const pMyGridItem pNewNode)
{
	if (pNewNode == NULL || pMyGridItemHead == NULL)
		return false;

	InsertPos = 0;
	pMyGridItem p0 = pMyGridItemHead;
	pMyGridItem p1 = pMyGridItemHead;

	while (p1->pNextItem!=NULL && CompareGridItem(p1, pNewNode)==-1)
	{
		p0 = p1;
		p1 = p1->pNextItem;
		InsertPos++;
	}

	
	if (CompareGridItem(p1, pNewNode) >= 0)
	{
		p0->pNextItem = pNewNode;
		pNewNode->pNextItem = p1;
		p1->pPreItem = pNewNode;
		pNewNode->pPreItem = p0;
		InsertPos--;
	}
	else if (p1->pNextItem == NULL )
	{
		p1->pNextItem = pNewNode;
		pNewNode->pPreItem = p1;
	}
	else if (CompareGridItem(p1, pNewNode) == -2)
		return false;

	return true;
}

int MyFrame::GetLengthOfList(const pMyGridItem pNodePos)
{
	if (pNodePos == NULL)
		return -1;

	int res = 0;
	pMyGridItem p0 = pNodePos;

	if (p0->stItemTime.Year == "0")
		p0 = p0->pNextItem;

	while (pNodePos != NULL)
	{
		res++;
		p0 = p0->pNextItem;
	}

	return res;
}

void MyFrame::EraseList(const pMyGridItem pNodePos)
{
	if (pNodePos == NULL)
		return;

	pMyGridItem p0 = pNodePos;
	pMyGridItem p1 = pNodePos->pNextItem;

	delete p0;
	while (p1 != NULL)
	{
		p0 = p1;
		p1 = p1->pNextItem;
		delete p0;
	}
}