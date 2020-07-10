// DLG_AtomicBomb.cpp : implementation file
//

#include "stdafx.h"
#include "PrtSys.h"
#include "DLG_AtomicBomb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_AtomicBomb dialog
//设置滑动条范围的控制数组
GLfloat AB_Sld[2][2] =	//原子弹的属性滑条控制范围。
{
  {0,	9999} ,				//粒子的数量 unsigned long 1000		0
  {0.01,	  5.0  }		//贴图宽度							12
};

CDLG_AtomicBomb::CDLG_AtomicBomb(CWnd* pParent /*=NULL*/)
	: CDialog(CDLG_AtomicBomb::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_AtomicBomb)
	m_E_maxnum = 0;
	m_E_posx = 0.0f;
	m_E_posy = 0.0f;
	m_E_posz = 0.0f;
	//}}AFX_DATA_INIT
}

CDLG_AtomicBomb::~CDLG_AtomicBomb(void)
{

}
	

void CDLG_AtomicBomb::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_AtomicBomb)
	DDX_Control(pDX, IDC_SA_MAXNUM, m_S_maxnum);
	DDX_Text(pDX, IDC_EA_MAXNUM, m_E_maxnum);
	DDV_MinMaxInt(pDX, m_E_maxnum, 0, 9999);
	DDX_Text(pDX, IDC_EA_POS_X, m_E_posx);
	DDV_MinMaxFloat(pDX, m_E_posx, -1000.f, 1000.f);
	DDX_Text(pDX, IDC_EA_POS_Y, m_E_posy);
	DDV_MinMaxFloat(pDX, m_E_posy, -1000.f, 1000.f);
	DDX_Text(pDX, IDC_EA_POS_Z, m_E_posz);
	DDV_MinMaxFloat(pDX, m_E_posz, -1000.f, 1000.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_AtomicBomb, CDialog)
	//{{AFX_MSG_MAP(CDLG_AtomicBomb)
	ON_EN_KILLFOCUS(IDC_EA_MAXNUM, OnKillfocusEaMaxnum)
	ON_EN_KILLFOCUS(IDC_EA_POS_X, OnKillfocusEaPosX)
	ON_EN_KILLFOCUS(IDC_EA_POS_Y, OnKillfocusEaPosY)
	ON_EN_KILLFOCUS(IDC_EA_POS_Z, OnKillfocusEaPosZ)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SA_MAXNUM, OnReleasedcaptureSaMaxnum)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_AtomicBomb message handlers
// 获得外界数据
void CDLG_AtomicBomb::GetExtData(DlgData_PRT &Ex_DLGData)
{
	m_DLGData=&Ex_DLGData;	//取得外界存储数据的结构，因为是传址调用，
							//所以只要在这里改，外面的数据也就改了
	m_E_maxnum=m_DLGData->MAX_PARTICLES;	//粒子的最大数（不可超过10000）

	m_E_posx=m_DLGData->star_x;				//粒子源的位置
	m_E_posy=m_DLGData->star_y;
	m_E_posz=m_DLGData->star_z;

	//***************************************************************
	UpdateData(FALSE);						//将程序里的值及时更新到控件

	//对滑动条进行初始化
	float pos[2] ;//= (m_E_MaxNum - AB_Sld[0][0]) * 100.0 / (AB_Sld[0][1] - AB_Sld[0][0]);
	pos[0]= (m_E_maxnum - AB_Sld[0][0]) * 100.0 / (AB_Sld[0][1] - AB_Sld[0][0]);
	m_S_maxnum.SetPos(pos[0]);
}

void CDLG_AtomicBomb::OnKillfocusEaMaxnum() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->MAX_PARTICLES=m_E_maxnum;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_maxnum - AB_Sld[0][0]) * 100.0 / 
		(AB_Sld[0][1] - AB_Sld[0][0]);
    m_S_maxnum.SetPos(pos);
}

void CDLG_AtomicBomb::OnKillfocusEaPosX() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->star_x=m_E_posx;		//将程序的值更新到真正所控制的对象
}

void CDLG_AtomicBomb::OnKillfocusEaPosY() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->star_y=m_E_posy;		//将程序的值更新到真正所控制的对象
}

void CDLG_AtomicBomb::OnKillfocusEaPosZ() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->star_z=m_E_posz;		//将程序的值更新到真正所控制的对象
}

void CDLG_AtomicBomb::OnReleasedcaptureSaMaxnum(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_maxnum=(m_S_maxnum.GetPos()/100.0)
		*(AB_Sld[0][1] - AB_Sld[0][0]) 
		+ AB_Sld[0][0];
	m_DLGData->MAX_PARTICLES=m_E_maxnum;
	UpdateData(FALSE); 
	*pResult = 0;
}
