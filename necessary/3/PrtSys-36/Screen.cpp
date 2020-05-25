// Screen.cpp : implementation file
//

#include "stdafx.h"
#include "PrtSys.h"
#include "Screen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen dialog


CScreen::CScreen(CWnd* pParent /*=NULL*/)
	: CDialog(CScreen::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScreen)
	m_R1024_768 = -1;
	m_R640_480 = 0;
	m_R800_600 = -1;
	//}}AFX_DATA_INIT
	IsOK=false;
}


void CScreen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen)
	DDX_Radio(pDX, IDC_R_17, m_R1024_768);
	DDX_Radio(pDX, IDC_R_64, m_R640_480);
	DDX_Radio(pDX, IDC_R_86, m_R800_600);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen, CDialog)
	//{{AFX_MSG_MAP(CScreen)
	ON_BN_CLICKED(IDC_R_17, OnR17)
	ON_BN_CLICKED(IDC_R_64, OnR64)
	ON_BN_CLICKED(IDC_R_86, OnR86)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen message handlers

void CScreen::OnR17() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_R1024_768 = 0;
	m_R640_480 = -1;
	m_R800_600 = -1;
	UpdateData(FALSE);
}

void CScreen::OnR64() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_R1024_768 = -1;
	m_R640_480 = 0;
	m_R800_600 = -1;
	UpdateData(FALSE);
}

void CScreen::OnR86() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_R1024_768 = -1;
	m_R640_480 = -1;
	m_R800_600 = 0;
	UpdateData(FALSE);
}

void CScreen::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CScreen::OnOK() 
{
	IsOK=true;
	CDialog::OnOK();
}

int CScreen::GetScreenset(void)
{
	if(IsOK)
	{
		if(	m_R1024_768==0)return 1;
		if(m_R800_600==0)return 2;
		else return 3;
		IsOK=false;
	}
	return 0;
}