// DLG_Fire.cpp : implementation file
//

#include "stdafx.h"
#include "PrtSys.h"
#include "DLG_Fire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_Fire dialog

//设置滑动条范围的控制数组
GLfloat Fire_Sld[13][2] =	//火的属性滑条控制范围。
{
  {0,	9999} ,				//粒子的数量 unsigned long 1000		0
  {0.0001   , 10.0  } ,		//lifefate	1.0						1
  {0.0001   , 10.0  } ,		//slowdown  2.0						2
  {0.0   ,    500  } ,		//speedMax  50						3
  {0.0   ,    500  } ,		//speedMin  0						4
  {0.0   , 180.0  } ,		//AngleYStar 0						5
  {0.0   , 180.0  } ,		//AngleYEnd  90						6
  {0.0   , 360.0  } ,		//AngleXZStar 0						7
  {0.0   , 360.0  } ,		//AngleXZEnd 360					8	
  {-100.0  , 100.0  } ,		//GetXg 0							9
  {-100.0  , 100.0  } ,		//GetYg 0.1							10
  {-100.0 ,  100.0  },		//GetZg 0							11
  {0.01,	  5.0  }		//贴图宽度							12
};


CDLG_Fire::CDLG_Fire(CWnd* pParent /*=NULL*/)
	: CDialog(CDLG_Fire::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_Fire)
	m_E_xzend = 0.0f;
	m_E_xzstar = 0.0f;
	m_E_yend = 0.0f;
	m_E_ystar = 0.0f;
	m_E_getx = 0.0f;
	m_E_gety = 0.0f;
	m_E_getz = 0.0f;
	m_E_lifefate = 0.0f;
	m_E_maxnum = 0;
	m_E_slowdown = 0.0f;
	m_E_speedmax = 0.0f;
	m_E_speedmin = 0.0f;
	m_E_posx = 0.0f;
	m_E_posy = 0.0f;
	m_E_posz = 0.0f;
	m_E_TW = 0.0f;
	m_RF1 = 0;
	m_RF2 = -1;
	m_RF3 = -1;
	m_RF4 = -1;
	//}}AFX_DATA_INIT
}

CDLG_Fire::~CDLG_Fire()
{

}

void CDLG_Fire::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_Fire)
	DDX_Control(pDX, IDC_S_TextWitdh, m_S_TW);
	DDX_Control(pDX, IDC_SLD_SPEEDMIN, m_S_speedmin);
	DDX_Control(pDX, IDC_SLD_SPEEDMAX, m_S_speedmax);
	DDX_Control(pDX, IDC_SLD_SLOWDOWN, m_S_slowdown);
	DDX_Control(pDX, IDC_SLD_MAXNUM, m_S_maxnum);
	DDX_Control(pDX, IDC_SLD_LIFEFATE, m_S_lifefate);
	DDX_Control(pDX, IDC_SLD_GETZ, m_S_getz);
	DDX_Control(pDX, IDC_SLD_GETY, m_S_gety);
	DDX_Control(pDX, IDC_SLD_GETX, m_S_getx);
	DDX_Control(pDX, IDC_SLD_ALYSTAR, m_S_ystar);
	DDX_Control(pDX, IDC_SLD_ALYEND, m_S_yend);
	DDX_Control(pDX, IDC_SLD_ALXZSTAR, m_S_xzstar);
	DDX_Control(pDX, IDC_SLD_ALXZEND, m_S_xzend);
	DDX_Text(pDX, IDC_E_ALXZEND, m_E_xzend);
	DDV_MinMaxFloat(pDX, m_E_xzend, 0.f, 360.f);
	DDX_Text(pDX, IDC_E_ALXZSTAR, m_E_xzstar);
	DDV_MinMaxFloat(pDX, m_E_xzstar, 0.f, 360.f);
	DDX_Text(pDX, IDC_E_ALYEND, m_E_yend);
	DDV_MinMaxFloat(pDX, m_E_yend, 0.f, 180.f);
	DDX_Text(pDX, IDC_E_ALYSTAR, m_E_ystar);
	DDV_MinMaxFloat(pDX, m_E_ystar, 0.f, 180.f);
	DDX_Text(pDX, IDC_E_GETX, m_E_getx);
	DDV_MinMaxFloat(pDX, m_E_getx, -100.f, 100.f);
	DDX_Text(pDX, IDC_E_GETY, m_E_gety);
	DDV_MinMaxFloat(pDX, m_E_gety, -100.f, 100.f);
	DDX_Text(pDX, IDC_E_GETZ, m_E_getz);
	DDV_MinMaxFloat(pDX, m_E_getz, -100.f, 100.f);
	DDX_Text(pDX, IDC_E_LIFEFATE, m_E_lifefate);
	DDV_MinMaxFloat(pDX, m_E_lifefate, 1.e-004f, 10.f);
	DDX_Text(pDX, IDC_E_MAXNUM, m_E_maxnum);
	DDV_MinMaxInt(pDX, m_E_maxnum, 0, 9999);
	DDX_Text(pDX, IDC_E_SLOWDOWN, m_E_slowdown);
	DDV_MinMaxFloat(pDX, m_E_slowdown, 1.e-004f, 10.f);
	DDX_Text(pDX, IDC_E_SPEEDMAX, m_E_speedmax);
	DDV_MinMaxFloat(pDX, m_E_speedmax, 0.f, 500.f);
	DDX_Text(pDX, IDC_E_SPEEDMIN, m_E_speedmin);
	DDV_MinMaxFloat(pDX, m_E_speedmin, 0.f, 500.f);
	DDX_Text(pDX, IDC_E_POS_X, m_E_posx);
	DDX_Text(pDX, IDC_E_POS_Y, m_E_posy);
	DDV_MinMaxFloat(pDX, m_E_posy, -1000.f, 1000.f);
	DDX_Text(pDX, IDC_E_POS_Z, m_E_posz);
	DDV_MinMaxFloat(pDX, m_E_posz, -1000.f, 1000.f);
	DDX_Text(pDX, IDC_E_TextWidth, m_E_TW);
	DDV_MinMaxFloat(pDX, m_E_TW, 1.e-002f, 5.f);
	DDX_Radio(pDX, IDC_R_F1, m_RF1);
	DDX_Radio(pDX, IDC_R_F2, m_RF2);
	DDX_Radio(pDX, IDC_R_F3, m_RF3);
	DDX_Radio(pDX, IDC_R_F4, m_RF4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_Fire, CDialog)
	//{{AFX_MSG_MAP(CDLG_Fire)
	ON_WM_DESTROY()
	ON_EN_KILLFOCUS(IDC_E_ALXZEND, OnKillfocusEAlxzend)
	ON_EN_KILLFOCUS(IDC_E_ALXZSTAR, OnKillfocusEAlxzstar)
	ON_EN_KILLFOCUS(IDC_E_ALYEND, OnKillfocusEAlyend)
	ON_EN_KILLFOCUS(IDC_E_ALYSTAR, OnKillfocusEAlystar)
	ON_EN_KILLFOCUS(IDC_E_GETX, OnKillfocusEGetx)
	ON_EN_KILLFOCUS(IDC_E_GETY, OnKillfocusEGety)
	ON_EN_KILLFOCUS(IDC_E_GETZ, OnKillfocusEGetz)
	ON_EN_KILLFOCUS(IDC_E_LIFEFATE, OnKillfocusELifefate)
	ON_EN_KILLFOCUS(IDC_E_MAXNUM, OnKillfocusEMaxnum)
	ON_EN_KILLFOCUS(IDC_E_POS_X, OnKillfocusEPosX)
	ON_EN_KILLFOCUS(IDC_E_POS_Y, OnKillfocusEPosY)
	ON_EN_KILLFOCUS(IDC_E_POS_Z, OnKillfocusEPosZ)
	ON_EN_KILLFOCUS(IDC_E_SLOWDOWN, OnKillfocusESlowdown)
	ON_EN_KILLFOCUS(IDC_E_SPEEDMAX, OnKillfocusESpeedmax)
	ON_EN_KILLFOCUS(IDC_E_SPEEDMIN, OnKillfocusESpeedmin)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLD_ALXZEND, OnReleasedcaptureSldAlxzend)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLD_ALXZSTAR, OnReleasedcaptureSldAlxzstar)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLD_ALYEND, OnReleasedcaptureSldAlyend)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLD_ALYSTAR, OnReleasedcaptureSldAlystar)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLD_GETX, OnReleasedcaptureSldGetx)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLD_GETY, OnReleasedcaptureSldGety)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLD_GETZ, OnReleasedcaptureSldGetz)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLD_LIFEFATE, OnReleasedcaptureSldLifefate)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLD_MAXNUM, OnReleasedcaptureSldMaxnum)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLD_SLOWDOWN, OnReleasedcaptureSldSlowdown)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLD_SPEEDMAX, OnReleasedcaptureSldSpeedmax)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLD_SPEEDMIN, OnReleasedcaptureSldSpeedmin)
	ON_EN_KILLFOCUS(IDC_E_TextWidth, OnKillfocusETextWidth)
	ON_BN_CLICKED(IDC_R_F1, OnRF1)
	ON_BN_CLICKED(IDC_R_F2, OnRF2)
	ON_BN_CLICKED(IDC_R_F3, OnRF3)
	ON_BN_CLICKED(IDC_R_F4, OnRF4)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_S_TextWitdh, OnReleasedcaptureSTextWitdh)
	ON_BN_CLICKED(IDC_B_Color, OnBColor)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BF_LOAD, OnBfLoad)
	ON_BN_CLICKED(IDC_BF_SAVE, OnBfSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_Fire message handlers
//更新控件数据
void CDLG_Fire::UpdateDLGDate(void)
{
	m_E_maxnum=m_fireDLG->MAX_PARTICLES;	//粒子的最大数（不可超过10000）

	m_E_posx=m_fireDLG->star_x;				//粒子源的位置
	m_E_posy=m_fireDLG->star_y;
	m_E_posz=m_fireDLG->star_z;
	m_E_lifefate=m_fireDLG->lifefate;		// 削减粒子的生命，数值约大，粒子的生命就越短

	m_E_slowdown=m_fireDLG->slowdown;		// 减慢粒子的速度，数值约大越慢，越小越快
	m_E_speedmax=m_fireDLG->speedMax;		// 速度最大值（大于零！）与方向无关
	m_E_speedmin=m_fireDLG->speedMin;		// 速度最小值（大于零！）

	m_E_ystar=m_fireDLG->AngleYStar;		//速度向量与Y轴的夹角Star<End;
	m_E_yend=m_fireDLG->AngleYEnd;			//速度向量与Y轴的夹角(0~180)

	m_E_xzstar=m_fireDLG->AngleXZStar;		//速度向量在XZ平面与X的夹角（从Y的正向看顺时针）(0~360)
	m_E_xzend=m_fireDLG->AngleXZEnd;		//Star<End;
	
	m_E_getx=m_fireDLG->GetXg;				//粒子系统受到的空间重力场（矢量）
	m_E_gety=m_fireDLG->GetYg;
	m_E_getz=m_fireDLG->GetZg;
	m_E_TW=m_fireDLG->TextWidth;

	//贴图样式选择×××××××××××××××
	switch(m_fireDLG->TextModel)
	{
	case 0:	m_RF1=0;	m_RF2=-1;	m_RF3=-1;	m_RF4=-1;break;
	case 1:	m_RF1=-1;	m_RF2=0;	m_RF3=-1;	m_RF4=-1;break;
	case 2:	m_RF1=-1;	m_RF2=-1;	m_RF3=0;	m_RF4=-1;break;
	case 3:	m_RF1=-1;	m_RF2=-1;	m_RF3=-1;	m_RF4=0;break;
	}
	//***************************************************************
	UpdateData(FALSE);						//将程序里的值及时更新到控件
	//更新颜色！
	if(!colorEixst)
	{
		m_FireColor.Create(IDD_DLG_COLOR,NULL);//创建颜色对话框 ,如果没有生成过则生成
		m_FireColor.m_E_Title="火焰颜色调整";	//改变标题
		m_FireColor.UpdateData(FALSE);
		colorEixst=true;
	}
	m_FireColor.GetExtColorData(m_fireDLG->PrtColor);
	//对滑动条进行初始化
	float pos[13] ;//= (m_E_MaxNum - Fire_Sld[0][0]) * 100.0 / (Fire_Sld[0][1] - Fire_Sld[0][0]);
	pos[0]= (m_E_maxnum - Fire_Sld[0][0]) * 100.0 / (Fire_Sld[0][1] - Fire_Sld[0][0]);
	pos[1]= (m_E_lifefate - Fire_Sld[1][0]) * 100.0 / (Fire_Sld[1][1] - Fire_Sld[1][0]);
	pos[2]= (m_E_slowdown - Fire_Sld[2][0]) * 100.0 / (Fire_Sld[2][1] - Fire_Sld[2][0]);
	pos[3]= (m_E_speedmax - Fire_Sld[3][0]) * 100.0 / (Fire_Sld[3][1] - Fire_Sld[3][0]);
	pos[4]= (m_E_speedmin - Fire_Sld[4][0]) * 100.0 / (Fire_Sld[4][1] - Fire_Sld[4][0]);
	pos[5]= (m_E_ystar - Fire_Sld[5][0]) * 100.0 / (Fire_Sld[5][1] - Fire_Sld[5][0]);
	pos[6]= (m_E_yend - Fire_Sld[6][0]) * 100.0 / (Fire_Sld[6][1] - Fire_Sld[6][0]);
	pos[7]= (m_E_xzstar - Fire_Sld[7][0]) * 100.0 / (Fire_Sld[7][1] - Fire_Sld[7][0]);
	pos[8]= (m_E_xzend - Fire_Sld[8][0]) * 100.0 / (Fire_Sld[8][1] - Fire_Sld[8][0]);
	pos[9]= (m_E_getx - Fire_Sld[9][0]) * 100.0 / (Fire_Sld[9][1] - Fire_Sld[9][0]);
	pos[10]= (m_E_gety - Fire_Sld[10][0]) * 100.0 / (Fire_Sld[10][1] - Fire_Sld[10][0]);
	pos[11]= (m_E_getz - Fire_Sld[11][0]) * 100.0 / (Fire_Sld[11][1] - Fire_Sld[11][0]);
	pos[12]= (m_E_TW - Fire_Sld[12][0]) * 100.0 / (Fire_Sld[12][1] - Fire_Sld[12][0]);

	m_S_maxnum.SetPos(pos[0]);
	m_S_lifefate.SetPos(pos[1]);
	m_S_slowdown.SetPos(pos[2]);
	m_S_speedmax.SetPos(pos[3]);
	m_S_speedmin.SetPos(pos[4]);
	m_S_ystar.SetPos(pos[5]);
	m_S_yend.SetPos(pos[6]);
	m_S_xzstar.SetPos(pos[7]);
	m_S_xzend.SetPos(pos[8]);
	m_S_getx.SetPos(pos[9]);
	m_S_gety.SetPos(pos[10]);
	m_S_getz.SetPos(pos[11]);
	m_S_TW.SetPos(pos[12]);
}
// 接受外界数据
void CDLG_Fire::GetExtFireData(DlgData_PRT &Ex_fireDLG)
{
	m_fireDLG=&Ex_fireDLG;	//取得外界存储数据的结构，因为是传址调用，
							//所以只要在这里改，外面的数据也就改了
	UpdateDLGDate();//更新界面！

}

//关闭时隐藏自己
void CDLG_Fire::OnDestroy() 
{
	CDialog::OnDestroy();
	ShowWindow(SW_HIDE);

	// TODO: Add your message handler code here
}

///////////////////////////////////////////////////////////////////////
//				将对话框上的数据 Edit 更新到程序
///////////////////////////////////////////////////////////////////////

void CDLG_Fire::OnKillfocusEAlxzend() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	if(m_E_xzend<m_E_xzstar)m_E_xzend=m_E_xzstar;
	m_fireDLG->AngleXZEnd=m_E_xzend;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_xzend - Fire_Sld[8][0]) * 100.0 / 
		(Fire_Sld[8][1] - Fire_Sld[8][0]);
    m_S_xzend.SetPos(pos);
	UpdateData(FALSE);
	
}

void CDLG_Fire::OnKillfocusEAlxzstar() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	if(m_E_xzstar>m_E_xzend)m_E_xzstar=m_E_xzend;
	m_fireDLG->AngleXZStar=m_E_xzstar;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_xzstar - Fire_Sld[7][0]) * 100.0 / 
		(Fire_Sld[7][1] - Fire_Sld[7][0]);
    m_S_xzstar.SetPos(pos);
	UpdateData(FALSE);
}

void CDLG_Fire::OnKillfocusEAlyend() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	if(m_E_yend<m_E_ystar)m_E_yend=m_E_ystar;
	m_fireDLG->AngleYEnd=m_E_yend;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_yend - Fire_Sld[6][0]) * 100.0 / 
		(Fire_Sld[6][1] - Fire_Sld[6][0]);
    m_S_yend.SetPos(pos);
	UpdateData(FALSE);
}

void CDLG_Fire::OnKillfocusEAlystar() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	if(m_E_ystar>m_E_yend)m_E_ystar=m_E_yend;
	m_fireDLG->AngleYStar=m_E_ystar;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_ystar - Fire_Sld[5][0]) * 100.0 / 
		(Fire_Sld[5][1] - Fire_Sld[5][0]);
    m_S_ystar.SetPos(pos);
	UpdateData(FALSE);
}

void CDLG_Fire::OnKillfocusEGetx() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_fireDLG->GetXg=m_E_getx;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_getx - Fire_Sld[9][0]) * 100.0 / 
		(Fire_Sld[9][1] - Fire_Sld[9][0]);
    m_S_getx.SetPos(pos);
}

void CDLG_Fire::OnKillfocusEGety() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_fireDLG->GetYg=m_E_gety;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_gety - Fire_Sld[10][0]) * 100.0 / 
		(Fire_Sld[10][1] - Fire_Sld[10][0]);
    m_S_gety.SetPos(pos);
}

void CDLG_Fire::OnKillfocusEGetz() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_fireDLG->GetZg=m_E_getz;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_getz - Fire_Sld[11][0]) * 100.0 / 
		(Fire_Sld[11][1] - Fire_Sld[11][0]);
    m_S_getz.SetPos(pos);
}

void CDLG_Fire::OnKillfocusELifefate() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_fireDLG->lifefate=m_E_lifefate;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_lifefate - Fire_Sld[1][0]) * 100.0 / 
		(Fire_Sld[1][1] - Fire_Sld[1][0]);
    m_S_lifefate.SetPos(pos);
}

void CDLG_Fire::OnKillfocusEMaxnum() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_fireDLG->MAX_PARTICLES=m_E_maxnum;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_maxnum - Fire_Sld[0][0]) * 100.0 / 
		(Fire_Sld[0][1] - Fire_Sld[0][0]);
    m_S_maxnum.SetPos(pos);
}

void CDLG_Fire::OnKillfocusEPosX() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_fireDLG->star_x=m_E_posx;		//将程序的值更新到真正所控制的对象
}

void CDLG_Fire::OnKillfocusEPosY() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_fireDLG->star_y=m_E_posy;		//将程序的值更新到真正所控制的对象
}

void CDLG_Fire::OnKillfocusEPosZ() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);				//将界面的值更新到程序
	m_fireDLG->star_z=m_E_posz;		//将程序的值更新到真正所控制的对象
}

void CDLG_Fire::OnKillfocusESlowdown() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_fireDLG->slowdown=m_E_slowdown;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_slowdown - Fire_Sld[2][0]) * 100.0 / 
		(Fire_Sld[2][1] - Fire_Sld[2][0]);
    m_S_slowdown.SetPos(pos);
}

void CDLG_Fire::OnKillfocusESpeedmax() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	//对最大最小进行检查
	if(m_E_speedmax<m_E_speedmin)
			m_E_speedmax=m_E_speedmin;

	m_fireDLG->speedMax=m_E_speedmax;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_speedmax - Fire_Sld[3][0]) * 100.0 / 
		(Fire_Sld[3][1] - Fire_Sld[3][0]);
    m_S_speedmax.SetPos(pos);
	UpdateData(FALSE);
}

void CDLG_Fire::OnKillfocusESpeedmin() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	//对最大最小进行检查
	if(m_E_speedmin>m_E_speedmax)
			m_E_speedmin=m_E_speedmax;

	m_fireDLG->speedMin=m_E_speedmin;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_speedmin - Fire_Sld[4][0]) * 100.0 / 
		(Fire_Sld[4][1] - Fire_Sld[4][0]);
    m_S_speedmin.SetPos(pos);
	UpdateData(FALSE);
}

////////////////////////////////////////////////////////////////////////////////
//				将滑条数据更新到程序和物体
////////////////////////////////////////////////////////////////////////////////
void CDLG_Fire::OnReleasedcaptureSldAlxzend(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_xzend=(m_S_xzend.GetPos()/100.0)
		*(Fire_Sld[8][1] - Fire_Sld[8][0]) 
		+ Fire_Sld[8][0];

	//对最大最小进行检查
	if(m_E_xzend<m_E_xzstar)
	{
		m_E_xzend=m_E_xzstar;
		float pos = (m_E_xzend - Fire_Sld[8][0]) * 100.0 / 
		(Fire_Sld[8][1] - Fire_Sld[8][0]);
		m_S_xzend.SetPos(pos);
	}

	m_fireDLG->AngleXZEnd=m_E_xzend;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Fire::OnReleasedcaptureSldAlxzstar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_xzstar=(m_S_xzstar.GetPos()/100.0)
		*(Fire_Sld[7][1] - Fire_Sld[7][0]) 
		+ Fire_Sld[7][0];

	//对最大最小进行检查
	if(m_E_xzstar>m_E_xzend)
	{
		m_E_xzstar=m_E_xzend;
		float pos = (m_E_xzstar - Fire_Sld[7][0]) * 100.0 / 
		(Fire_Sld[7][1] - Fire_Sld[7][0]);
		m_S_xzstar.SetPos(pos);
	}

	m_fireDLG->AngleXZStar=m_E_xzstar;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Fire::OnReleasedcaptureSldAlyend(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_yend=(m_S_yend.GetPos()/100.0)
		*(Fire_Sld[6][1] - Fire_Sld[6][0]) 
		+ Fire_Sld[6][0];


	if(m_E_yend<m_E_ystar)
	{
		m_E_yend=m_E_ystar;
		float pos = (m_E_yend - Fire_Sld[6][0]) * 100.0 / 
		(Fire_Sld[6][1] - Fire_Sld[6][0]);
		m_S_yend.SetPos(pos);
	}
	m_fireDLG->AngleYEnd=m_E_yend;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Fire::OnReleasedcaptureSldAlystar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_ystar=(m_S_ystar.GetPos()/100.0)
		*(Fire_Sld[5][1] - Fire_Sld[5][0]) 
		+ Fire_Sld[5][0];

	//对最大最小进行检查
	if(m_E_ystar>m_E_yend)
	{
		m_E_ystar=m_E_yend;
		float pos = (m_E_ystar - Fire_Sld[5][0]) * 100.0 / 
		(Fire_Sld[5][1] - Fire_Sld[5][0]);
		m_S_ystar.SetPos(pos);
	}

	m_fireDLG->AngleYStar=m_E_ystar;
	UpdateData(FALSE); 	
	*pResult = 0;
}

void CDLG_Fire::OnReleasedcaptureSldGetx(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_getx=(m_S_getx.GetPos()/100.0)
		*(Fire_Sld[9][1] - Fire_Sld[9][0]) 
		+ Fire_Sld[9][0];
	m_fireDLG->GetXg=m_E_getx;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Fire::OnReleasedcaptureSldGety(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_gety=(m_S_gety.GetPos()/100.0)
		*(Fire_Sld[10][1] - Fire_Sld[10][0]) 
		+ Fire_Sld[10][0];
	m_fireDLG->GetYg=m_E_gety;
	UpdateData(FALSE); 	
	*pResult = 0;
}

void CDLG_Fire::OnReleasedcaptureSldGetz(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_getz=(m_S_getz.GetPos()/100.0)
		*(Fire_Sld[11][1] - Fire_Sld[11][0]) 
		+ Fire_Sld[11][0];
	m_fireDLG->GetZg=m_E_getz;
	UpdateData(FALSE); 	
	*pResult = 0;
}

void CDLG_Fire::OnReleasedcaptureSldLifefate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_lifefate=(m_S_lifefate.GetPos()/100.0)
		*(Fire_Sld[1][1] - Fire_Sld[1][0]) 
		+ Fire_Sld[1][0];
	m_fireDLG->lifefate=m_E_lifefate;
	UpdateData(FALSE); 	
	*pResult = 0;
}

void CDLG_Fire::OnReleasedcaptureSldMaxnum(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_maxnum=(m_S_maxnum.GetPos()/100.0)
		*(Fire_Sld[0][1] - Fire_Sld[0][0]) 
		+ Fire_Sld[0][0];
	m_fireDLG->MAX_PARTICLES=m_E_maxnum;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Fire::OnReleasedcaptureSldSlowdown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_slowdown=(m_S_slowdown.GetPos()/100.0)
		*(Fire_Sld[2][1] - Fire_Sld[2][0]) 
		+ Fire_Sld[2][0];
	m_fireDLG->slowdown=m_E_slowdown;
	UpdateData(FALSE); 	
	*pResult = 0;
}

void CDLG_Fire::OnReleasedcaptureSldSpeedmax(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_speedmax=(m_S_speedmax.GetPos()/100.0)
		*(Fire_Sld[3][1] - Fire_Sld[3][0]) 
		+ Fire_Sld[3][0];

	//对最大最小进行检查
	if(m_E_speedmax<m_E_speedmin)
	{
		m_E_speedmax=m_E_speedmin;
		float pos = (m_E_speedmax - Fire_Sld[3][0]) * 100.0 / 
		(Fire_Sld[3][1] - Fire_Sld[3][0]);
		m_S_speedmax.SetPos(pos);
	}

	m_fireDLG->speedMax=m_E_speedmax;
	UpdateData(FALSE); 	
	*pResult = 0;
}

void CDLG_Fire::OnReleasedcaptureSldSpeedmin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_speedmin=(m_S_speedmin.GetPos()/100.0)
		*(Fire_Sld[4][1] - Fire_Sld[4][0]) 
		+ Fire_Sld[4][0];

	//对最大最小进行检查
	if(m_E_speedmin>m_E_speedmax)
	{
		m_E_speedmin=m_E_speedmax;
		float pos = (m_E_speedmin - Fire_Sld[4][0]) * 100.0 / 
		(Fire_Sld[4][1] - Fire_Sld[4][0]);
		m_S_speedmin.SetPos(pos);
	}

	m_fireDLG->speedMin=m_E_speedmin;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Fire::OnKillfocusETextWidth() 
{

	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_fireDLG->TextWidth=m_E_TW;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_TW - Fire_Sld[12][0]) * 100.0 / 
		(Fire_Sld[12][1] - Fire_Sld[12][0]);
    m_S_TW.SetPos(pos);

}

void CDLG_Fire::OnRF1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_RF1=0;
	m_RF2=-1;
	m_RF3=-1;
	m_RF4=-1;
	m_fireDLG->TextModel=0;
	UpdateData(FALSE); 
}

void CDLG_Fire::OnRF2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_RF1=-1;
	m_RF2=0;
	m_RF3=-1;
	m_RF4=-1;
	m_fireDLG->TextModel=1;
	UpdateData(FALSE); 
}

void CDLG_Fire::OnRF3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_RF1=-1;
	m_RF2=-1;
	m_RF3=0;
	m_RF4=-1;
	m_fireDLG->TextModel=2;
	UpdateData(FALSE); 
}

void CDLG_Fire::OnRF4() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_RF1=-1;
	m_RF2=-1;
	m_RF3=-1;
	m_RF4=0;
	m_fireDLG->TextModel=3;
	UpdateData(FALSE); 
}

void CDLG_Fire::OnReleasedcaptureSTextWitdh(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_TW=(m_S_TW.GetPos()/100.0)
		*(Fire_Sld[12][1] - Fire_Sld[12][0]) 
		+ Fire_Sld[12][0];
	m_fireDLG->TextWidth=m_E_TW;
	UpdateData(FALSE); 
	*pResult = 0;
}

//打开颜色编辑对话框
void CDLG_Fire::OnBColor() 
{
	// TODO: Add your control notification handler code here
	//颜色子对话框的数据调入
	if(!colorEixst)
	{
		m_FireColor.Create(IDD_DLG_COLOR,NULL);//创建颜色对话框 ,如果没有生成过则生成
		m_FireColor.m_E_Title="火焰颜色调整";	//改变标题
		m_FireColor.UpdateData(FALSE);
		colorEixst=true;
	}
	m_FireColor.GetExtColorData(m_fireDLG->PrtColor);
	m_FireColor.ShowWindow(SW_SHOW);
}


int CDLG_Fire::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CDLG_Fire::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//AfxMessageBox("BBB");
	if(colorEixst)//如果创建过颜色对话框则让他也隐藏
	{
		m_FireColor.ShowWindow(SW_HIDE);
	}
	CDialog::OnClose();
}

//打开效果
void CDLG_Fire::OnBfLoad() 
{
	// TODO: Add your control notification handler code here
	char szCurDir[256];//保存当前目录
	GetCurrentDirectory(sizeof(szCurDir),szCurDir);

	CString filename;
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,"效果文件(*.pse)|*.pse||",NULL);

	if(dlg.DoModal()==IDOK)//显示文件对话框
	{
		filename = dlg.GetPathName();//取得文件名

	FILE *fp;
	fp=fopen(filename,"rb+");//打开保存的文件
	if(!fp)MessageBox("打开文件失败！");
	else
	{
		fseek(fp,0L,0);//移到文件开头
		char s[10];
		fgets(s,5,fp);//读入开头文件表示
		if(strcmp(s,"PSES")!=0)MessageBox("不是粒子效果文件！");
		else
		{
			fseek(fp,4L,0);//读取效果
			int ch;
			ch=fgetc(fp);
			if(ch!=1)MessageBox("不是火焰效果文件！");
			else
			{
				fseek(fp,5L,0);
				fread(m_fireDLG,sizeof(*m_fireDLG),1,fp);//读入我们的效果数据
			}
		}
		fclose(fp);//关闭文件指针
	}
	UpdateDLGDate();//更新界面数据
	}
	SetCurrentDirectory(szCurDir);//恢复当前路径！

}

//保存效果
void CDLG_Fire::OnBfSave() 
{
	// TODO: Add your control notification handler code here
	char szCurDir[256];//保存当前目录
	GetCurrentDirectory(sizeof(szCurDir),szCurDir);

	CString filename;
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"效果文件(*.pse)|*.pse||",NULL);

	if(dlg.DoModal()==IDOK)//显示文件对话框
	{
		filename = dlg.GetPathName();//取得文件名

	FILE *fp;
	fp=fopen(filename,"wb+");//建立保存的文件
	if(!fp)MessageBox("写文件失败！");
	else{
	fseek(fp,0L,0);//移到文件开头
	fputs("PSES",fp);//写入开头文件表示
	int ch=1;
	fputc(ch,fp);	//写入粒子效果的表示，1就是火焰！
	fwrite(m_fireDLG,sizeof(*m_fireDLG),1,fp);//写入我们的效果数据
	fclose(fp);//关闭文件指针
	}
	}
	SetCurrentDirectory(szCurDir);//恢复当前路径！
}
