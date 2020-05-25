// DLG_Snow.cpp : implementation file
//

#include "stdafx.h"
#include "PrtSys.h"
#include "DLG_Snow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_Snow dialog
//设置滑动条范围的控制数组
GLfloat Snow_Sld[15][2] =	//火的属性滑条控制范围。
{
  {0,	9999} ,				//粒子的数量 unsigned long			0
  {0.0001   , 100.0  } ,		//lifefate	1.0						1
  {0.0001   , 30.0  } ,		//slowdown  2.0						2
  {0.0   ,    100  } ,		//speedMax  50						3
  {0.0   ,    100  } ,		//speedMin  0						4
  {0.0   , 10.0  } ,		//AngleYStar 0						5
  {0.0   , 10.0  } ,		//AngleYEnd  90						6
  {0.0   , 100.0  } ,		//AngleXZStar 0						7
  {0.0   , 100.0  } ,		//AngleXZEnd 360					8	
  {-20.0  , 20.0  } ,		//GetXg 0							9
  {-20.0  , 0  } ,		//GetYg 0.1							10
  {-20.0 ,  20.0  },		//GetZg 0							11
  {0.0001,	  1.0  },		//贴图宽度							12

  //////////////////////////////////////////////////
  //子粒子源滑动条范围控制数组
  {0,	1} ,				//透明度 m_E_alpha 					13
  {0.0   , 0.1  } 			//三叶玫瑰线角度增量 m_E_pram0	0~1	14
};



CDLG_Snow::CDLG_Snow(CWnd* pParent /*=NULL*/)
	: CDialog(CDLG_Snow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_Snow)
	m_E_alpha = 0.0f;
	m_E_xzend = 0.0f;
	m_E_xzstar = 0.0f;
	m_E_yend = 0.0f;
	m_E_ystar = 0.0f;
	m_E_CB = 0.0f;
	m_E_CG = 0.0f;
	m_E_CR = 0.0f;
	m_E_getx = 0.0f;
	m_E_gety = 0.0f;
	m_E_getz = 0.0f;
	m_E_lifefate = 0.0f;
	m_E_posx = 0.0f;
	m_E_posy = 0.0f;
	m_E_posz = 0.0f;
	m_E_maxnum = 0;
	m_E_pram0 = 0.0f;
	m_E_slowdown = 0.0f;
	m_E_speedmax = 0.0f;
	m_E_speedmin = 0.0f;
	m_E_TW = 0.0f;
	m_RS1 = -1;
	m_RS2 = -1;
	m_RS3 = -1;
	m_RS4 = -1;
	m_RS5 = -1;
	m_RS6 = -1;
	//}}AFX_DATA_INIT
}

CDLG_Snow::~CDLG_Snow()
{
}


void CDLG_Snow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_Snow)
	DDX_Control(pDX, IDC_SS_MAXNUM, m_S_maxnum);
	DDX_Control(pDX, IDC_SS_PRAM0, m_S_pram0);
	DDX_Control(pDX, IDC_SS_SLOWDOWN, m_S_slowdown);
	DDX_Control(pDX, IDC_SS_SPEEDMAX, m_S_speedmax);
	DDX_Control(pDX, IDC_SS_SPEEDMIN, m_S_speedmin);
	DDX_Control(pDX, IDC_SS_TextWidth, m_S_TW);
	DDX_Control(pDX, IDC_SS_LIFEFATE, m_S_lifefate);
	DDX_Control(pDX, IDC_SS_GETZ, m_S_getz);
	DDX_Control(pDX, IDC_SS_GETY, m_S_gety);
	DDX_Control(pDX, IDC_SS_GETX, m_S_getx);
	DDX_Control(pDX, IDC_SS_ALYSTAR, m_S_ystar);
	DDX_Control(pDX, IDC_SS_ALYEND, m_S_yend);
	DDX_Control(pDX, IDC_SS_ALXZSTAR, m_S_xzstar);
	DDX_Control(pDX, IDC_SS_ALXZEND, m_S_xzend);
	DDX_Control(pDX, IDC_SS_ALPHA, m_S_alpha);
	DDX_Text(pDX, IDC_ES_ALPHA, m_E_alpha);
	DDV_MinMaxFloat(pDX, m_E_alpha, 0.f, 1.f);
	DDX_Text(pDX, IDC_ES_ALXZEND, m_E_xzend);
	DDV_MinMaxFloat(pDX, m_E_xzend, 0.f, 100.f);
	DDX_Text(pDX, IDC_ES_ALXZSTAR, m_E_xzstar);
	DDV_MinMaxFloat(pDX, m_E_xzstar, 0.f, 100.f);
	DDX_Text(pDX, IDC_ES_ALYEND, m_E_yend);
	DDV_MinMaxFloat(pDX, m_E_yend, 0.f, 10.f);
	DDX_Text(pDX, IDC_ES_ALYSTAR, m_E_ystar);
	DDV_MinMaxFloat(pDX, m_E_ystar, 0.f, 10.f);
	DDX_Text(pDX, IDC_ES_C_B, m_E_CB);
	DDV_MinMaxFloat(pDX, m_E_CB, 0.f, 1.f);
	DDX_Text(pDX, IDC_ES_C_G, m_E_CG);
	DDV_MinMaxFloat(pDX, m_E_CG, 0.f, 1.f);
	DDX_Text(pDX, IDC_ES_C_R, m_E_CR);
	DDV_MinMaxFloat(pDX, m_E_CR, 0.f, 1.f);
	DDX_Text(pDX, IDC_ES_GETX, m_E_getx);
	DDV_MinMaxFloat(pDX, m_E_getx, -20.f, 20.f);
	DDX_Text(pDX, IDC_ES_GETY, m_E_gety);
	DDV_MinMaxFloat(pDX, m_E_gety, -20.f, 0.f);
	DDX_Text(pDX, IDC_ES_GETZ, m_E_getz);
	DDV_MinMaxFloat(pDX, m_E_getz, -20.f, 20.f);
	DDX_Text(pDX, IDC_ES_LIFEFATE, m_E_lifefate);
	DDV_MinMaxFloat(pDX, m_E_lifefate, 1.e-004f, 100.f);
	DDX_Text(pDX, IDC_ES_POS_X, m_E_posx);
	DDV_MinMaxFloat(pDX, m_E_posx, -1000.f, 1000.f);
	DDX_Text(pDX, IDC_ES_POS_Y, m_E_posy);
	DDV_MinMaxFloat(pDX, m_E_posy, -1000.f, 1000.f);
	DDX_Text(pDX, IDC_ES_POS_Z, m_E_posz);
	DDV_MinMaxFloat(pDX, m_E_posz, -1000.f, 1000.f);
	DDX_Text(pDX, IDC_ES_MAXNUM, m_E_maxnum);
	DDV_MinMaxInt(pDX, m_E_maxnum, 0, 9999);
	DDX_Text(pDX, IDC_ES_PRAM0, m_E_pram0);
	DDV_MinMaxFloat(pDX, m_E_pram0, 0.f, 0.1f);
	DDX_Text(pDX, IDC_ES_SLOWDOWN, m_E_slowdown);
	DDV_MinMaxFloat(pDX, m_E_slowdown, 1.e-004f, 30.f);
	DDX_Text(pDX, IDC_ES_SPEEDMAX, m_E_speedmax);
	DDV_MinMaxFloat(pDX, m_E_speedmax, 0.f, 100.f);
	DDX_Text(pDX, IDC_ES_SPEEDMIN, m_E_speedmin);
	DDV_MinMaxFloat(pDX, m_E_speedmin, 0.f, 100.f);
	DDX_Text(pDX, IDC_ES_TextWidth, m_E_TW);
	DDV_MinMaxFloat(pDX, m_E_TW, 1.e-004f, 5.f);
	DDX_Radio(pDX, IDC_RS1, m_RS1);
	DDX_Radio(pDX, IDC_RS2, m_RS2);
	DDX_Radio(pDX, IDC_RS3, m_RS3);
	DDX_Radio(pDX, IDC_RS4, m_RS4);
	DDX_Radio(pDX, IDC_RS5, m_RS5);
	DDX_Radio(pDX, IDC_RS6, m_RS6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_Snow, CDialog)
	//{{AFX_MSG_MAP(CDLG_Snow)
	ON_BN_CLICKED(IDC_BS_COLOR, OnBsColor)
	ON_EN_KILLFOCUS(IDC_ES_ALPHA, OnKillfocusEsAlpha)
	ON_EN_KILLFOCUS(IDC_ES_ALXZEND, OnKillfocusEsAlxzend)
	ON_EN_KILLFOCUS(IDC_ES_ALXZSTAR, OnKillfocusEsAlxzstar)
	ON_EN_KILLFOCUS(IDC_ES_ALYEND, OnKillfocusEsAlyend)
	ON_EN_KILLFOCUS(IDC_ES_ALYSTAR, OnKillfocusEsAlystar)
	ON_EN_KILLFOCUS(IDC_ES_C_B, OnKillfocusEsCB)
	ON_EN_KILLFOCUS(IDC_ES_C_G, OnKillfocusEsCG)
	ON_EN_KILLFOCUS(IDC_ES_C_R, OnKillfocusEsCR)
	ON_EN_KILLFOCUS(IDC_ES_GETX, OnKillfocusEsGetx)
	ON_EN_KILLFOCUS(IDC_ES_GETY, OnKillfocusEsGety)
	ON_EN_KILLFOCUS(IDC_ES_GETZ, OnKillfocusEsGetz)
	ON_EN_KILLFOCUS(IDC_ES_LIFEFATE, OnKillfocusEsLifefate)
	ON_EN_KILLFOCUS(IDC_ES_MAXNUM, OnKillfocusEsMaxnum)
	ON_EN_KILLFOCUS(IDC_ES_POS_X, OnKillfocusEsPosX)
	ON_EN_KILLFOCUS(IDC_ES_POS_Y, OnKillfocusEsPosY)
	ON_EN_KILLFOCUS(IDC_ES_POS_Z, OnKillfocusEsPosZ)
	ON_EN_KILLFOCUS(IDC_ES_PRAM0, OnKillfocusEsPram0)
	ON_EN_KILLFOCUS(IDC_ES_SLOWDOWN, OnKillfocusEsSlowdown)
	ON_EN_KILLFOCUS(IDC_ES_SPEEDMAX, OnKillfocusEsSpeedmax)
	ON_EN_KILLFOCUS(IDC_ES_SPEEDMIN, OnKillfocusEsSpeedmin)
	ON_EN_KILLFOCUS(IDC_ES_TextWidth, OnKillfocusESTextWidth)
	ON_BN_CLICKED(IDC_RS1, OnRs1)
	ON_BN_CLICKED(IDC_RS2, OnRs2)
	ON_BN_CLICKED(IDC_RS3, OnRs3)
	ON_BN_CLICKED(IDC_RS4, OnRs4)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SS_ALPHA, OnReleasedcaptureSsAlpha)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SS_ALXZEND, OnReleasedcaptureSsAlxzend)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SS_ALXZSTAR, OnReleasedcaptureSsAlxzstar)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SS_ALYEND, OnReleasedcaptureSsAlyend)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SS_ALYSTAR, OnReleasedcaptureSsAlystar)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SS_GETX, OnReleasedcaptureSsGetx)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SS_GETY, OnReleasedcaptureSsGety)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SS_GETZ, OnReleasedcaptureSsGetz)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SS_LIFEFATE, OnReleasedcaptureSsLifefate)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SS_MAXNUM, OnReleasedcaptureSsMaxnum)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SS_PRAM0, OnReleasedcaptureSsPram0)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SS_SLOWDOWN, OnReleasedcaptureSsSlowdown)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SS_SPEEDMAX, OnReleasedcaptureSsSpeedmax)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SS_SPEEDMIN, OnReleasedcaptureSsSpeedmin)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SS_TextWidth, OnReleasedcaptureSSTextWidth)
	ON_BN_CLICKED(IDC_BS_LOAD, OnBsLoad)
	ON_BN_CLICKED(IDC_BS_SAVE, OnBsSave)
	ON_BN_CLICKED(IDC_BS_EQUAL, OnBsEqual)
	ON_BN_CLICKED(IDC_RS5, OnRs5)
	ON_BN_CLICKED(IDC_RS6, OnRs6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_Snow message handlers
void	CDLG_Snow::UpdateDLGDate()//更新界面数据
{
	m_E_maxnum=m_DLGData->MAX_PARTICLES;	//粒子的最大数（不可超过10000）

	m_E_posx=m_DLGData->star_x;				//粒子源的位置
	m_E_posy=m_DLGData->star_y;
	m_E_posz=m_DLGData->star_z;
	m_E_lifefate=m_DLGData->lifefate;		// 削减粒子的生命，数值约大，粒子的生命就越短

	m_E_slowdown=m_DLGData->slowdown;		// 减慢粒子的速度，数值约大越慢，越小越快
	m_E_speedmax=m_DLGData->speedMax;		// 速度最大值（大于零！）与方向无关
	m_E_speedmin=m_DLGData->speedMin;		// 速度最小值（大于零！）

	m_E_ystar=m_DLGData->AngleYStar;		//速度向量与Y轴的夹角Star<End;
	m_E_yend=m_DLGData->AngleYEnd;			//速度向量与Y轴的夹角(0~180)

	m_E_xzstar=m_DLGData->AngleXZStar;		//速度向量在XZ平面与X的夹角（从Y的正向看顺时针）(0~360)
	m_E_xzend=m_DLGData->AngleXZEnd;		//Star<End;
	
	m_E_getx=m_DLGData->GetXg;				//粒子系统受到的空间重力场（矢量）
	m_E_gety=m_DLGData->GetYg;
	m_E_getz=m_DLGData->GetZg;
	m_E_TW=m_DLGData->TextWidth;			//贴图宽度

	m_E_alpha=m_DLGData->PrtColor.Red[1];	//透明度
	m_E_pram0=m_DLGData->pram[0];			// 三叶玫瑰线角度增量	0~1

	m_E_CR=m_DLGData->PrtColor.Red[0];		//颜色
	m_E_CG=m_DLGData->PrtColor.Green[0];
	m_E_CB=m_DLGData->PrtColor.Blue[0];

	//贴图样式选择×××××××××××××××
	switch(m_DLGData->TextModel)
	{
	case 0:	m_RS1=0;	m_RS2=-1;	m_RS3=-1;	m_RS4=-1;	m_RS5=-1;	m_RS6=-1;break;
	case 1:	m_RS1=-1;	m_RS2=0;	m_RS3=-1;	m_RS4=-1;	m_RS5=-1;	m_RS6=-1;break;
	case 2:	m_RS1=-1;	m_RS2=-1;	m_RS3=0;	m_RS4=-1;	m_RS5=-1;	m_RS6=-1;break;
	case 3:	m_RS1=-1;	m_RS2=-1;	m_RS3=-1;	m_RS4=0;	m_RS5=-1;	m_RS6=-1;break;
	case 4:	m_RS1=-1;	m_RS2=-1;	m_RS3=-1;	m_RS4=-1;	m_RS5=0;	m_RS6=-1;break;
	case 5:	m_RS1=-1;	m_RS2=-1;	m_RS3=-1;	m_RS4=-1;	m_RS5=-1;	m_RS6=0;break;

	}

	//***************************************************************
	UpdateData(FALSE);						//将程序里的值及时更新到控件

	//对滑动条进行初始化
	float pos[15] ;//= (m_E_MaxNum - Snow_Sld[0][0]) * 100.0 / (Snow_Sld[0][1] - Snow_Sld[0][0]);
	pos[0]= (m_E_maxnum - Snow_Sld[0][0]) * 100.0 / (Snow_Sld[0][1] - Snow_Sld[0][0]);
	pos[1]= (m_E_lifefate - Snow_Sld[1][0]) * 100.0 / (Snow_Sld[1][1] - Snow_Sld[1][0]);
	pos[2]= (m_E_slowdown - Snow_Sld[2][0]) * 100.0 / (Snow_Sld[2][1] - Snow_Sld[2][0]);
	pos[3]= (m_E_speedmax - Snow_Sld[3][0]) * 100.0 / (Snow_Sld[3][1] - Snow_Sld[3][0]);
	pos[4]= (m_E_speedmin - Snow_Sld[4][0]) * 100.0 / (Snow_Sld[4][1] - Snow_Sld[4][0]);
	pos[5]= (m_E_ystar - Snow_Sld[5][0]) * 100.0 / (Snow_Sld[5][1] - Snow_Sld[5][0]);
	pos[6]= (m_E_yend - Snow_Sld[6][0]) * 100.0 / (Snow_Sld[6][1] - Snow_Sld[6][0]);
	pos[7]= (m_E_xzstar - Snow_Sld[7][0]) * 100.0 / (Snow_Sld[7][1] - Snow_Sld[7][0]);
	pos[8]= (m_E_xzend - Snow_Sld[8][0]) * 100.0 / (Snow_Sld[8][1] - Snow_Sld[8][0]);
	pos[9]= (m_E_getx - Snow_Sld[9][0]) * 100.0 / (Snow_Sld[9][1] - Snow_Sld[9][0]);
	pos[10]= (m_E_gety - Snow_Sld[10][0]) * 100.0 / (Snow_Sld[10][1] - Snow_Sld[10][0]);
	pos[11]= (m_E_getz - Snow_Sld[11][0]) * 100.0 / (Snow_Sld[11][1] - Snow_Sld[11][0]);
	pos[12]= (m_E_TW - Snow_Sld[12][0]) * 100.0 / (Snow_Sld[12][1] - Snow_Sld[12][0]);
	pos[13]= (m_E_alpha - Snow_Sld[13][0]) * 100.0 / (Snow_Sld[13][1] - Snow_Sld[13][0]);
	pos[14]= (m_E_pram0 - Snow_Sld[14][0]) * 100.0 / (Snow_Sld[14][1] - Snow_Sld[14][0]);

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
	m_S_alpha.SetPos(pos[13]);
	m_S_pram0.SetPos(pos[14]);
}

void	CDLG_Snow::GetExtData(DlgData_PRT &Ex_DLGData)	//获得外界数据
{
	m_DLGData=&Ex_DLGData;	//取得外界存储数据的结构，因为是传址调用，
							//所以只要在这里改，外面的数据也就改了
	UpdateDLGDate();//更新界面数据
}

//颜色编辑对话框
void CDLG_Snow::OnBsColor() 
{
	// TODO: Add your control notification handler code here
	CColorDialog cdlg;		//定义对话框
	COLORREF cr;
	if(cdlg.DoModal()==IDOK)	//显示对话框
	{
		cr=cdlg.GetColor();	//获得颜色
	}
	m_E_CR=(float)GetRValue(cr)/255.0;
	m_E_CG=(float)GetGValue(cr)/255.0;
	m_E_CB=(float)GetBValue(cr)/255.0;

	m_DLGData->PrtColor.Red[0]=m_E_CR;		//颜色
	m_DLGData->PrtColor.Green[0]=m_E_CG;
	m_DLGData->PrtColor.Blue[0]=m_E_CB;

	UpdateData(FALSE);		//更新数据
}

///////////////////////////////////////////////////////////
//					文本对话框的编辑
//////////////////////////////////////////////////////////

void CDLG_Snow::OnKillfocusEsAlpha() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->PrtColor.Red[1]=m_E_alpha;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_alpha - Snow_Sld[13][0]) * 100.0 / 
		(Snow_Sld[13][1] - Snow_Sld[13][0]);
    m_S_alpha.SetPos(pos);
}

void CDLG_Snow::OnKillfocusEsAlxzend() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->AngleXZEnd=m_E_xzend;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_xzend - Snow_Sld[8][0]) * 100.0 / 
		(Snow_Sld[8][1] - Snow_Sld[8][0]);
    m_S_xzend.SetPos(pos);
}

void CDLG_Snow::OnKillfocusEsAlxzstar() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->AngleXZStar=m_E_xzstar;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_xzstar - Snow_Sld[7][0]) * 100.0 / 
		(Snow_Sld[7][1] - Snow_Sld[7][0]);
    m_S_xzstar.SetPos(pos);
}

void CDLG_Snow::OnKillfocusEsAlyend() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	if(m_E_yend>m_E_ystar)m_E_yend=m_E_ystar;
	m_DLGData->AngleYEnd=m_E_yend;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_yend - Snow_Sld[6][0]) * 100.0 / 
		(Snow_Sld[6][1] - Snow_Sld[6][0]);
    m_S_yend.SetPos(pos);
	UpdateData(FALSE); 
}

void CDLG_Snow::OnKillfocusEsAlystar() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	if(m_E_ystar<m_E_yend)m_E_ystar=m_E_yend;
	m_DLGData->AngleYStar=m_E_ystar;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_ystar - Snow_Sld[5][0]) * 100.0 / 
		(Snow_Sld[5][1] - Snow_Sld[5][0]);
    m_S_ystar.SetPos(pos);
	UpdateData(FALSE); 
}

void CDLG_Snow::OnKillfocusEsCB() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->PrtColor.Blue[0]=m_E_CB;
}

void CDLG_Snow::OnKillfocusEsCG() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->PrtColor.Green[0]=m_E_CG;

}

void CDLG_Snow::OnKillfocusEsCR() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->PrtColor.Red[0]=m_E_CR;		//颜色
}

void CDLG_Snow::OnKillfocusEsGetx() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->GetXg=m_E_getx;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_getx - Snow_Sld[9][0]) * 100.0 / 
		(Snow_Sld[9][1] - Snow_Sld[9][0]);
    m_S_getx.SetPos(pos);
}

void CDLG_Snow::OnKillfocusEsGety() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->GetYg=m_E_gety;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_gety - Snow_Sld[10][0]) * 100.0 / 
		(Snow_Sld[10][1] - Snow_Sld[10][0]);
    m_S_gety.SetPos(pos);
}

void CDLG_Snow::OnKillfocusEsGetz() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->GetZg=m_E_getz;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_getz - Snow_Sld[11][0]) * 100.0 / 
		(Snow_Sld[11][1] - Snow_Sld[11][0]);
    m_S_getz.SetPos(pos);
}

void CDLG_Snow::OnKillfocusEsLifefate() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->lifefate=m_E_lifefate;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_lifefate - Snow_Sld[1][0]) * 100.0 / 
		(Snow_Sld[1][1] - Snow_Sld[1][0]);
    m_S_lifefate.SetPos(pos);
}

void CDLG_Snow::OnKillfocusEsMaxnum() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->MAX_PARTICLES=m_E_maxnum;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_maxnum - Snow_Sld[0][0]) * 100.0 / 
		(Snow_Sld[0][1] - Snow_Sld[0][0]);
    m_S_maxnum.SetPos(pos);
}

void CDLG_Snow::OnKillfocusEsPosX() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->star_x=m_E_posx;		//将程序的值更新到真正所控制的对象
}

void CDLG_Snow::OnKillfocusEsPosY() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->star_y=m_E_posy;		//将程序的值更新到真正所控制的对象
}

void CDLG_Snow::OnKillfocusEsPosZ() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->star_z=m_E_posz;		//将程序的值更新到真正所控制的对象
}

void CDLG_Snow::OnKillfocusEsPram0() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->pram[0]=m_E_pram0;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_pram0 - Snow_Sld[14][0]) * 100.0 / 
		(Snow_Sld[14][1] - Snow_Sld[14][0]);
    m_S_pram0.SetPos(pos);
}

void CDLG_Snow::OnKillfocusEsSlowdown() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->slowdown=m_E_slowdown;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_slowdown - Snow_Sld[2][0]) * 100.0 / 
		(Snow_Sld[2][1] - Snow_Sld[2][0]);
    m_S_slowdown.SetPos(pos);
}

void CDLG_Snow::OnKillfocusEsSpeedmax() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	if(m_E_speedmax<m_E_speedmin)m_E_speedmax=m_E_speedmin;
	m_DLGData->speedMax=m_E_speedmax;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_speedmax - Snow_Sld[3][0]) * 100.0 / 
		(Snow_Sld[3][1] - Snow_Sld[3][0]);
    m_S_speedmax.SetPos(pos);
	UpdateData(FALSE); 
}

void CDLG_Snow::OnKillfocusEsSpeedmin() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	if(m_E_speedmin>m_E_speedmax)m_E_speedmin=m_E_speedmax;
	m_DLGData->speedMin=m_E_speedmin;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_speedmin - Snow_Sld[4][0]) * 100.0 / 
		(Snow_Sld[4][1] - Snow_Sld[4][0]);
    m_S_speedmin.SetPos(pos);
	UpdateData(FALSE); 
}

void CDLG_Snow::OnKillfocusESTextWidth() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);					//将界面的值更新到程序
	m_DLGData->TextWidth=m_E_TW;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_TW - Snow_Sld[12][0]) * 100.0 / 
		(Snow_Sld[12][1] - Snow_Sld[12][0]);
    m_S_TW.SetPos(pos);
}

void CDLG_Snow::OnReleasedcaptureSsAlpha(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_alpha=(m_S_alpha.GetPos()/100.0)
		*(Snow_Sld[13][1] - Snow_Sld[13][0]) 
		+ Snow_Sld[13][0];
	m_DLGData->PrtColor.Red[1]=m_E_alpha;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Snow::OnReleasedcaptureSsAlxzend(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_xzend=(m_S_xzend.GetPos()/100.0)
		*(Snow_Sld[8][1] - Snow_Sld[8][0]) 
		+ Snow_Sld[8][0];
	m_DLGData->AngleXZEnd=m_E_xzend;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Snow::OnReleasedcaptureSsAlxzstar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_xzstar=(m_S_xzstar.GetPos()/100.0)
		*(Snow_Sld[7][1] - Snow_Sld[7][0]) 
		+ Snow_Sld[7][0];
	m_DLGData->AngleXZStar=m_E_xzstar;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Snow::OnReleasedcaptureSsAlyend(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_yend=(m_S_yend.GetPos()/100.0)
		*(Snow_Sld[6][1] - Snow_Sld[6][0]) 
		+ Snow_Sld[6][0];

	if(m_E_yend>m_E_ystar)
	{
		m_E_yend=m_E_ystar;
		float pos = (m_E_yend - Snow_Sld[6][0]) * 100.0 / 
		(Snow_Sld[6][1] - Snow_Sld[6][0]);
		m_S_yend.SetPos(pos);
	}

	m_DLGData->AngleYEnd=m_E_yend;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Snow::OnReleasedcaptureSsAlystar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_ystar=(m_S_ystar.GetPos()/100.0)
		*(Snow_Sld[5][1] - Snow_Sld[5][0]) 
		+ Snow_Sld[5][0];

	if(m_E_ystar<m_E_yend)
	{
		m_E_ystar=m_E_yend;
		float pos = (m_E_ystar - Snow_Sld[5][0]) * 100.0 / 
		(Snow_Sld[5][1] - Snow_Sld[5][0]);
		m_S_ystar.SetPos(pos);
	}

	m_DLGData->AngleYStar=m_E_ystar;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Snow::OnReleasedcaptureSsGetx(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_getx=(m_S_getx.GetPos()/100.0)
		*(Snow_Sld[9][1] - Snow_Sld[9][0]) 
		+ Snow_Sld[9][0];

	m_DLGData->GetXg=m_E_getx;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Snow::OnReleasedcaptureSsGety(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_gety=(m_S_gety.GetPos()/100.0)
		*(Snow_Sld[10][1] - Snow_Sld[10][0]) 
		+ Snow_Sld[10][0];
	m_DLGData->GetYg=m_E_gety;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Snow::OnReleasedcaptureSsGetz(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_getz=(m_S_getz.GetPos()/100.0)
		*(Snow_Sld[11][1] - Snow_Sld[11][0]) 
		+ Snow_Sld[11][0];
	m_DLGData->GetZg=m_E_getz;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Snow::OnReleasedcaptureSsLifefate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_lifefate=(m_S_lifefate.GetPos()/100.0)
		*(Snow_Sld[1][1] - Snow_Sld[1][0]) 
		+ Snow_Sld[1][0];
	m_DLGData->lifefate=m_E_lifefate;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Snow::OnReleasedcaptureSsMaxnum(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_maxnum=(m_S_maxnum.GetPos()/100.0)
		*(Snow_Sld[0][1] - Snow_Sld[0][0]) 
		+ Snow_Sld[0][0];
	m_DLGData->MAX_PARTICLES=m_E_maxnum;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Snow::OnReleasedcaptureSsPram0(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_pram0=(m_S_pram0.GetPos()/100.0)
		*(Snow_Sld[14][1] - Snow_Sld[14][0]) 
		+ Snow_Sld[14][0];
	m_DLGData->pram[0]=m_E_pram0;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Snow::OnReleasedcaptureSsSlowdown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_slowdown=(m_S_slowdown.GetPos()/100.0)
		*(Snow_Sld[2][1] - Snow_Sld[2][0]) 
		+ Snow_Sld[2][0];
	m_DLGData->slowdown=m_E_slowdown;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Snow::OnReleasedcaptureSsSpeedmax(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_speedmax=(m_S_speedmax.GetPos()/100.0)
		*(Snow_Sld[3][1] - Snow_Sld[3][0]) 
		+ Snow_Sld[3][0];

	if(m_E_speedmax<m_E_speedmin)
	{
		m_E_speedmax=m_E_speedmin;
		float pos = (m_E_speedmax - Snow_Sld[3][0]) * 100.0 / 
		(Snow_Sld[3][1] - Snow_Sld[3][0]);
		m_S_speedmax.SetPos(pos);
	}

	m_DLGData->speedMax=m_E_speedmax;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Snow::OnReleasedcaptureSsSpeedmin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_speedmin=(m_S_speedmin.GetPos()/100.0)
		*(Snow_Sld[4][1] - Snow_Sld[4][0]) 
		+ Snow_Sld[4][0];

	if(m_E_speedmin>m_E_speedmax)
	{
		m_E_speedmin=m_E_speedmax;
		float pos = (m_E_speedmin - Snow_Sld[4][0]) * 100.0 / 
		(Snow_Sld[4][1] - Snow_Sld[4][0]);
		m_S_speedmin.SetPos(pos);
	}

	m_DLGData->speedMin=m_E_speedmin;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Snow::OnReleasedcaptureSSTextWidth(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_TW=(m_S_TW.GetPos()/100.0)
		*(Snow_Sld[12][1] - Snow_Sld[12][0]) 
		+ Snow_Sld[12][0];
	m_DLGData->TextWidth=m_E_TW;
	UpdateData(FALSE); 
	*pResult = 0;
}



void CDLG_Snow::OnRs1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_RS1=0;
	m_RS2=-1;
	m_RS3=-1;
	m_RS4=-1;
	m_RS5=-1;
	m_RS6=-1;
	m_DLGData->TextModel=0;
	UpdateData(FALSE); 
}

void CDLG_Snow::OnRs2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_RS1=-1;
	m_RS2=0;
	m_RS3=-1;
	m_RS4=-1;
	m_RS5=-1;
	m_RS6=-1;
	m_DLGData->TextModel=1;
	UpdateData(FALSE); 
}

void CDLG_Snow::OnRs3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_RS1=-1;
	m_RS2=-1;
	m_RS3=0;
	m_RS4=-1;
	m_RS5=-1;
	m_RS6=-1;
	m_DLGData->TextModel=2;
	UpdateData(FALSE); 
}

void CDLG_Snow::OnRs4() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_RS1=-1;
	m_RS2=-1;
	m_RS3=-1;
	m_RS4=0;
	m_RS5=-1;
	m_RS6=-1;
	m_DLGData->TextModel=3;
	UpdateData(FALSE); 
}

void CDLG_Snow::OnRs5() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_RS1=-1;
	m_RS2=-1;
	m_RS3=-1;
	m_RS4=-1;
	m_RS5=0;
	m_RS6=-1;
	m_DLGData->TextModel=4;
	UpdateData(FALSE); 	
}

void CDLG_Snow::OnRs6() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_RS1=-1;
	m_RS2=-1;
	m_RS3=-1;
	m_RS4=-1;
	m_RS5=-1;
	m_RS6=0;
	m_DLGData->TextModel=5;
	UpdateData(FALSE); 
}


//装载效果
void CDLG_Snow::OnBsLoad() 
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
				if(ch!=5)MessageBox("不是雪雨场效果文件！");
				else
				{
					fseek(fp,5L,0);
					fread(m_DLGData,sizeof(*m_DLGData),1,fp);//读入我们的效果数据
				}
			}
			fclose(fp);//关闭文件指针
		}
		UpdateDLGDate();//更新界面数据
	}
	SetCurrentDirectory(szCurDir);//恢复当前路径！
}

//保存效果
void CDLG_Snow::OnBsSave() 
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
		int ch=5;
		fputc(ch,fp);	//写入粒子效果的表示，5就是雪雨场！
		fwrite(m_DLGData,sizeof(*m_DLGData),1,fp);//写入我们的效果数据
		fclose(fp);//关闭文件指针
		}
	}
	SetCurrentDirectory(szCurDir);//恢复当前路径！
}

void CDLG_Snow::OnBsEqual() 
{
	// TODO: Add your control notification handler code here
	m_DLGData->dead=true;
}


