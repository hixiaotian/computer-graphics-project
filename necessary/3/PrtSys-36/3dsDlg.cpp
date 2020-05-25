// 3dsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PrtSys.h"
#include "3dsDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3dsDlg dialog


C3dsDlg::C3dsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C3dsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(C3dsDlg)
	m_pos_x = 0.0f;
	m_pos_y = 0.0f;
	m_pos_z = 0.0f;
	m_rot_x = 0.0f;
	m_rot_y = 0.0f;
	m_rot_z = 0.0f;
	m_scale = 0.0f;
	//}}AFX_DATA_INIT
}

C3dsDlg::~C3dsDlg(void)
{
//	delete this;
}

void C3dsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3dsDlg)
	DDX_Text(pDX, IDC_POS_X, m_pos_x);
	DDX_Text(pDX, IDC_POS_Y, m_pos_y);
	DDX_Text(pDX, IDC_POS_Z, m_pos_z);
	DDX_Text(pDX, IDC_ROT_X, m_rot_x);
	DDX_Text(pDX, IDC_ROT_Y, m_rot_y);
	DDX_Text(pDX, IDC_ROT_Z, m_rot_z);
	DDX_Text(pDX, IDC_SCALE, m_scale);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C3dsDlg, CDialog)
	//{{AFX_MSG_MAP(C3dsDlg)
	ON_WM_DESTROY()
	ON_EN_KILLFOCUS(IDC_POS_X, OnKillfocusPosX)
	ON_EN_KILLFOCUS(IDC_POS_Y, OnKillfocusPosY)
	ON_EN_KILLFOCUS(IDC_POS_Z, OnKillfocusPosZ)
	ON_EN_KILLFOCUS(IDC_ROT_X, OnKillfocusRotX)
	ON_EN_KILLFOCUS(IDC_ROT_Y, OnKillfocusRotY)
	ON_EN_KILLFOCUS(IDC_ROT_Z, OnKillfocusRotZ)
	ON_EN_KILLFOCUS(IDC_SCALE, OnKillfocusScale)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3dsDlg message handlers
//获得外界数据
void C3dsDlg::GetExtData(DlgData_3ds &m_3dsD)
{
	//取得外界存储数据的结构，因为是传址调用，所以只要在这里改，外面的数据也就改了
	m_3dsDlgdata=&m_3dsD;	
	m_pos_x=m_3dsDlgdata->t_data.x;
	m_pos_y=m_3dsDlgdata->t_data.y;
	m_pos_z=m_3dsDlgdata->t_data.z;

	m_rot_x=m_3dsDlgdata->r_data.Angle_x;
	m_rot_y=m_3dsDlgdata->r_data.Angle_y;
	m_rot_z=m_3dsDlgdata->r_data.Angle_z;

	m_scale=m_3dsDlgdata->scale;

	UpdateData(FALSE); //将程序里的值及时更新到控件
}


//关闭对话框时销毁自己
void C3dsDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
}

///////////////////////////////////////////////////////////
//		更新数据
///////////////////////////////////////////////////////////
void C3dsDlg::OnKillfocusPosX() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);//将控键的值更新到程序中。
	m_3dsDlgdata->t_data.x=m_pos_x;
}

void C3dsDlg::OnKillfocusPosY() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);//将控键的值更新到程序中。
	m_3dsDlgdata->t_data.y=m_pos_y;
}

void C3dsDlg::OnKillfocusPosZ() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);//将控键的值更新到程序中。
	m_3dsDlgdata->t_data.z=m_pos_z;
}

void C3dsDlg::OnKillfocusRotX() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);//将控键的值更新到程序中。
	m_3dsDlgdata->r_data.Angle_x=m_rot_x;
}

void C3dsDlg::OnKillfocusRotY() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);//将控键的值更新到程序中。
	m_3dsDlgdata->r_data.Angle_y=m_rot_y;
}

void C3dsDlg::OnKillfocusRotZ() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);//将控键的值更新到程序中。
	m_3dsDlgdata->r_data.Angle_z=m_rot_z;
}

void C3dsDlg::OnKillfocusScale() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);//将控键的值更新到程序中。
	m_3dsDlgdata->scale=m_scale;
}

