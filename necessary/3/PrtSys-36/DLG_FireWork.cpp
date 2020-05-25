// DLG_FireWork.cpp : implementation file
//

#include "stdafx.h"
#include "PrtSys.h"
#include "DLG_FireWork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_FireWork dialog

//设置滑动条范围的控制数组
GLfloat FireWork_Sld[21][2] =	//火的属性滑条控制范围。
{
  {0,	199} ,				//粒子的数量 unsigned long			0
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
  {0.01,	  5.0  },		//贴图宽度							12

  //////////////////////////////////////////////////
  //子粒子源滑动条范围控制数组
  {0,	1499} ,				//粒子的数量 unsigned long 1000		13
  {0.0001   , 10.0  } ,		//lifefate	1.0						14
  {0.0001   , 10.0  } ,		//slowdown  2.0						15
  {0.0   ,    20  } ,		//speedMax  50						16
  {0.0   ,    20  } ,		//speedMin  0						17
  {-50.0  , 50.0  } ,		//GetXg 0							18
  {-50.0  , 50.0  } ,		//GetYg 0.1							19
  {-50.0 ,  50.0  },		//GetZg 0							20
};


CDLG_FireWork::CDLG_FireWork(CWnd* pParent /*=NULL*/)
	: CDialog(CDLG_FireWork::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_FireWork)
	m_E_xzend = 0.0f;
	m_E_yend = 0.0f;
	m_E_xzstar = 0.0f;
	m_E_ystar = 0.0f;
	m_E_getx = 0.0f;
	m_E_gety = 0.0f;
	m_E_getz = 0.0f;
	m_E_lifefate = 0.0f;
	m_E_maxnum = 0;
	m_E_posx = 0.0f;
	m_E_posy = 0.0f;
	m_E_posz = 0.0f;
	m_E_slowdown = 0.0f;
	m_E_speedmax = 0.0f;
	m_E_speedmin = 0.0f;
	m_E_TW = 0.0f;
	m_ES_getx = 0.0f;
	m_ES_gety = 0.0f;
	m_ES_getz = 0.0f;
	m_ES_lifefate = 0.0f;
	m_ES_maxnum = 0;
	m_ES_slowdown = 0.0f;
	m_ES_speedmax = 0.0f;
	m_ES_speedmin = 0.0f;
	m_RI1 = -1;
	m_RI2 = -1;
	m_RI3 = -1;
	m_RI4 = -1;
	//}}AFX_DATA_INIT
}

CDLG_FireWork::~CDLG_FireWork()
{
//	delete this;
}


void CDLG_FireWork::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_FireWork)
	DDX_Control(pDX, IDC_SIS_SPEEDMIN, m_SS_speedmin);
	DDX_Control(pDX, IDC_SIS_SPEEDMAX, m_SS_speedmax);
	DDX_Control(pDX, IDC_SIS_SLOWDOWN, m_SS_slowdown);
	DDX_Control(pDX, IDC_SIS_MAXNUM, m_SS_maxnum);
	DDX_Control(pDX, IDC_SIS_LIFEFATE, m_SS_lifefate);
	DDX_Control(pDX, IDC_SIS_GETZ, m_SS_getz);
	DDX_Control(pDX, IDC_SIS_GETY, m_SS_gety);
	DDX_Control(pDX, IDC_SIS_GETX, m_SS_getx);
	DDX_Control(pDX, IDC_SI_TextWidth, m_S_TW);
	DDX_Control(pDX, IDC_SI_SPEED_MIN, m_S_speedmin);
	DDX_Control(pDX, IDC_SI_SPEEDMAX, m_S_speedmax);
	DDX_Control(pDX, IDC_SI_SLOWDOWN, m_S_slowdown);
	DDX_Control(pDX, IDC_SI_MAXNUM, m_S_maxnum);
	DDX_Control(pDX, IDC_SI_LIFEFATE, m_S_lifefate);
	DDX_Control(pDX, IDC_SI_GETZ, m_S_getz);
	DDX_Control(pDX, IDC_SI_GETY, m_S_gety);
	DDX_Control(pDX, IDC_SI_GETX, m_S_getx);
	DDX_Control(pDX, IDC_SI_ALYSTAR, m_S_ystar);
	DDX_Control(pDX, IDC_SI_ALYEND, m_S_yend);
	DDX_Control(pDX, IDC_SI_ALXZSTAR, m_S_xzstar);
	DDX_Control(pDX, IDC_SI_ALXZEND, m_S_xzend);
	DDX_Text(pDX, IDC_EI_ALXZEND, m_E_xzend);
	DDV_MinMaxFloat(pDX, m_E_xzend, 0.f, 360.f);
	DDX_Text(pDX, IDC_EI_ALYEND, m_E_yend);
	DDV_MinMaxFloat(pDX, m_E_yend, 0.f, 180.f);
	DDX_Text(pDX, IDC_EI_ALXZSTAR, m_E_xzstar);
	DDV_MinMaxFloat(pDX, m_E_xzstar, 0.f, 360.f);
	DDX_Text(pDX, IDC_EI_ALYSTAR, m_E_ystar);
	DDV_MinMaxFloat(pDX, m_E_ystar, 0.f, 180.f);
	DDX_Text(pDX, IDC_EI_GETX, m_E_getx);
	DDV_MinMaxFloat(pDX, m_E_getx, -100.f, 100.f);
	DDX_Text(pDX, IDC_EI_GETY, m_E_gety);
	DDX_Text(pDX, IDC_EI_GETZ, m_E_getz);
	DDV_MinMaxFloat(pDX, m_E_getz, -100.f, 100.f);
	DDX_Text(pDX, IDC_EI_LIFEFATE, m_E_lifefate);
	DDV_MinMaxFloat(pDX, m_E_lifefate, 1.e-004f, 10.f);
	DDX_Text(pDX, IDC_EI_MAXNUM, m_E_maxnum);
	DDV_MinMaxInt(pDX, m_E_maxnum, 0, 199);
	DDX_Text(pDX, IDC_EI_POS_X, m_E_posx);
	DDV_MinMaxFloat(pDX, m_E_posx, -1000.f, 1000.f);
	DDX_Text(pDX, IDC_EI_POS_Y, m_E_posy);
	DDV_MinMaxFloat(pDX, m_E_posy, -1000.f, 1000.f);
	DDX_Text(pDX, IDC_EI_POS_Z, m_E_posz);
	DDV_MinMaxFloat(pDX, m_E_posz, -1000.f, 1000.f);
	DDX_Text(pDX, IDC_EI_SLOWDOWN, m_E_slowdown);
	DDV_MinMaxFloat(pDX, m_E_slowdown, 1.e-004f, 10.f);
	DDX_Text(pDX, IDC_EI_SPEEDMAX, m_E_speedmax);
	DDV_MinMaxFloat(pDX, m_E_speedmax, 0.f, 500.f);
	DDX_Text(pDX, IDC_EI_SPEEDMIN, m_E_speedmin);
	DDV_MinMaxFloat(pDX, m_E_speedmin, 0.f, 500.f);
	DDX_Text(pDX, IDC_EI_TextWidth, m_E_TW);
	DDV_MinMaxFloat(pDX, m_E_TW, 0.f, 5.f);
	DDX_Text(pDX, IDC_EIS_GETX, m_ES_getx);
	DDV_MinMaxFloat(pDX, m_ES_getx, -50.f, 50.f);
	DDX_Text(pDX, IDC_EIS_GETY, m_ES_gety);
	DDV_MinMaxFloat(pDX, m_ES_gety, -50.f, 50.f);
	DDX_Text(pDX, IDC_EIS_GETZ, m_ES_getz);
	DDV_MinMaxFloat(pDX, m_ES_getz, -50.f, 50.f);
	DDX_Text(pDX, IDC_EIS_LIFEFATE, m_ES_lifefate);
	DDV_MinMaxFloat(pDX, m_ES_lifefate, 1.e-004f, 10.f);
	DDX_Text(pDX, IDC_EIS_MAXNUM, m_ES_maxnum);
	DDV_MinMaxInt(pDX, m_ES_maxnum, 0, 1499);
	DDX_Text(pDX, IDC_EIS_SLOWDOWN, m_ES_slowdown);
	DDV_MinMaxFloat(pDX, m_ES_slowdown, 1.e-004f, 10.f);
	DDX_Text(pDX, IDC_EIS_SPEEDMAX, m_ES_speedmax);
	DDV_MinMaxFloat(pDX, m_ES_speedmax, 0.f, 20.f);
	DDX_Text(pDX, IDC_EIS_SPEEDMIN, m_ES_speedmin);
	DDV_MinMaxFloat(pDX, m_ES_speedmin, 0.f, 20.f);
	DDX_Radio(pDX, IDC_RI_1, m_RI1);
	DDX_Radio(pDX, IDC_RI_2, m_RI2);
	DDX_Radio(pDX, IDC_RI_3, m_RI3);
	DDX_Radio(pDX, IDC_RI_4, m_RI4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_FireWork, CDialog)
	//{{AFX_MSG_MAP(CDLG_FireWork)
	ON_BN_CLICKED(IDC_BI_COLOR, OnBiColor)
	ON_EN_KILLFOCUS(IDC_EI_ALXZEND, OnKillfocusEiAlxzend)
	ON_EN_KILLFOCUS(IDC_EI_ALXZSTAR, OnKillfocusEiAlxzstar)
	ON_EN_KILLFOCUS(IDC_EI_ALYEND, OnKillfocusEiAlyend)
	ON_EN_KILLFOCUS(IDC_EI_ALYSTAR, OnKillfocusEiAlystar)
	ON_EN_KILLFOCUS(IDC_EI_GETX, OnKillfocusEiGetx)
	ON_EN_KILLFOCUS(IDC_EI_GETY, OnKillfocusEiGety)
	ON_EN_KILLFOCUS(IDC_EI_GETZ, OnKillfocusEiGetz)
	ON_EN_KILLFOCUS(IDC_EI_LIFEFATE, OnKillfocusEiLifefate)
	ON_EN_KILLFOCUS(IDC_EI_MAXNUM, OnKillfocusEiMaxnum)
	ON_EN_KILLFOCUS(IDC_EI_POS_X, OnKillfocusEiPosX)
	ON_EN_KILLFOCUS(IDC_EI_POS_Y, OnKillfocusEiPosY)
	ON_EN_KILLFOCUS(IDC_EI_POS_Z, OnKillfocusEiPosZ)
	ON_EN_KILLFOCUS(IDC_EI_SLOWDOWN, OnKillfocusEiSlowdown)
	ON_EN_KILLFOCUS(IDC_EI_SPEEDMAX, OnKillfocusEiSpeedmax)
	ON_EN_KILLFOCUS(IDC_EI_SPEEDMIN, OnKillfocusEiSpeedmin)
	ON_EN_KILLFOCUS(IDC_EI_TextWidth, OnKillfocusEITextWidth)
	ON_EN_KILLFOCUS(IDC_EIS_GETX, OnKillfocusEisGetx)
	ON_EN_KILLFOCUS(IDC_EIS_GETY, OnKillfocusEisGety)
	ON_EN_KILLFOCUS(IDC_EIS_GETZ, OnKillfocusEisGetz)
	ON_EN_KILLFOCUS(IDC_EIS_LIFEFATE, OnKillfocusEisLifefate)
	ON_EN_KILLFOCUS(IDC_EIS_MAXNUM, OnKillfocusEisMaxnum)
	ON_EN_KILLFOCUS(IDC_EIS_SLOWDOWN, OnKillfocusEisSlowdown)
	ON_EN_KILLFOCUS(IDC_EIS_SPEEDMAX, OnKillfocusEisSpeedmax)
	ON_EN_KILLFOCUS(IDC_EIS_SPEEDMIN, OnKillfocusEisSpeedmin)
	ON_BN_CLICKED(IDC_RI_1, OnRi1)
	ON_BN_CLICKED(IDC_RI_2, OnRi2)
	ON_BN_CLICKED(IDC_RI_3, OnRi3)
	ON_BN_CLICKED(IDC_RI_4, OnRi4)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SI_ALXZEND, OnReleasedcaptureSiAlxzend)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SI_ALXZSTAR, OnReleasedcaptureSiAlxzstar)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SI_ALYEND, OnReleasedcaptureSiAlyend)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SI_ALYSTAR, OnReleasedcaptureSiAlystar)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SI_GETX, OnReleasedcaptureSiGetx)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SI_GETY, OnReleasedcaptureSiGety)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SI_GETZ, OnReleasedcaptureSiGetz)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SI_LIFEFATE, OnReleasedcaptureSiLifefate)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SI_MAXNUM, OnReleasedcaptureSiMaxnum)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SI_SLOWDOWN, OnReleasedcaptureSiSlowdown)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SI_SPEED_MIN, OnReleasedcaptureSiSpeedMin)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SI_SPEEDMAX, OnReleasedcaptureSiSpeedmax)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SI_TextWidth, OnReleasedcaptureSITextWidth)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SIS_GETX, OnReleasedcaptureSisGetx)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SIS_GETY, OnReleasedcaptureSisGety)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SIS_GETZ, OnReleasedcaptureSisGetz)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SIS_LIFEFATE, OnReleasedcaptureSisLifefate)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SIS_MAXNUM, OnReleasedcaptureSisMaxnum)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SIS_SLOWDOWN, OnReleasedcaptureSisSlowdown)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SIS_SPEEDMAX, OnReleasedcaptureSisSpeedmax)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SIS_SPEEDMIN, OnReleasedcaptureSisSpeedmin)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BFW_LOAD, OnBfwLoad)
	ON_BN_CLICKED(IDC_BFW_SAVE, OnBfwSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_FireWork message handlers
//获得外部数据，刷新显示界面。
void CDLG_FireWork::UpdateDLGDate(void)//更新界面数据
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


	//贴图样式选择×××××××××××××××
	//贴图样式选择×××××××××××××××
	switch(m_DLGData->TextModel)
	{
	case 0:	m_RI1=0;	m_RI2=-1;	m_RI3=-1;	m_RI4=-1;break;
	case 1:	m_RI1=-1;	m_RI2=0;	m_RI3=-1;	m_RI4=-1;break;
	case 2:	m_RI1=-1;	m_RI2=-1;	m_RI3=0;	m_RI4=-1;break;
	case 3:	m_RI1=-1;	m_RI2=-1;	m_RI3=-1;	m_RI4=0;break;
	}


	if(!colorEixst)
	{
		m_ColorDLG.Create(IDD_DLG_COLOR,NULL);//创建颜色对话框 ,如果没有生成过则生成
		m_ColorDLG.m_E_Title="礼花颜色调整";	//改变标题
		m_ColorDLG.UpdateData(FALSE);
		colorEixst=true;
	}
	m_ColorDLG.GetExtColorData(m_DLGData->PrtColor);
	//***************************************************************
	UpdateData(FALSE);						//将程序里的值及时更新到控件

	//对滑动条进行初始化
	float pos[21] ;//= (m_E_MaxNum - FireWork_Sld[0][0]) * 100.0 / (FireWork_Sld[0][1] - FireWork_Sld[0][0]);
	pos[0]= (m_E_maxnum - FireWork_Sld[0][0]) * 100.0 / (FireWork_Sld[0][1] - FireWork_Sld[0][0]);
	pos[1]= (m_E_lifefate - FireWork_Sld[1][0]) * 100.0 / (FireWork_Sld[1][1] - FireWork_Sld[1][0]);
	pos[2]= (m_E_slowdown - FireWork_Sld[2][0]) * 100.0 / (FireWork_Sld[2][1] - FireWork_Sld[2][0]);
	pos[3]= (m_E_speedmax - FireWork_Sld[3][0]) * 100.0 / (FireWork_Sld[3][1] - FireWork_Sld[3][0]);
	pos[4]= (m_E_speedmin - FireWork_Sld[4][0]) * 100.0 / (FireWork_Sld[4][1] - FireWork_Sld[4][0]);
	pos[5]= (m_E_ystar - FireWork_Sld[5][0]) * 100.0 / (FireWork_Sld[5][1] - FireWork_Sld[5][0]);
	pos[6]= (m_E_yend - FireWork_Sld[6][0]) * 100.0 / (FireWork_Sld[6][1] - FireWork_Sld[6][0]);
	pos[7]= (m_E_xzstar - FireWork_Sld[7][0]) * 100.0 / (FireWork_Sld[7][1] - FireWork_Sld[7][0]);
	pos[8]= (m_E_xzend - FireWork_Sld[8][0]) * 100.0 / (FireWork_Sld[8][1] - FireWork_Sld[8][0]);
	pos[9]= (m_E_getx - FireWork_Sld[9][0]) * 100.0 / (FireWork_Sld[9][1] - FireWork_Sld[9][0]);
	pos[10]= (m_E_gety - FireWork_Sld[10][0]) * 100.0 / (FireWork_Sld[10][1] - FireWork_Sld[10][0]);
	pos[11]= (m_E_getz - FireWork_Sld[11][0]) * 100.0 / (FireWork_Sld[11][1] - FireWork_Sld[11][0]);
	pos[12]= (m_E_TW - FireWork_Sld[12][0]) * 100.0 / (FireWork_Sld[12][1] - FireWork_Sld[12][0]);

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


	//////////////////////////////////////////
	//子粒子源数据接收

	m_ES_maxnum=m_DLGData->SonPDD.MAX_PARTICLES;	//粒子的最大数（不可超过10000）
	m_ES_lifefate=m_DLGData->SonPDD.lifefate;		// 削减粒子的生命，数值约大，粒子的生命就越短
	m_ES_slowdown=m_DLGData->SonPDD.slowdown;		// 减慢粒子的速度，数值约大越慢，越小越快
	m_ES_speedmax=m_DLGData->SonPDD.speedMax;		// 速度最大值（大于零！）与方向无关
	m_ES_speedmin=m_DLGData->SonPDD.speedMin;		// 速度最小值（大于零！）
	m_ES_getx=m_DLGData->SonPDD.GetXg;				//粒子系统受到的空间重力场（矢量）
	m_ES_gety=m_DLGData->SonPDD.GetYg;
	m_ES_getz=m_DLGData->SonPDD.GetZg;

	pos[13]= (m_ES_maxnum - FireWork_Sld[13][0]) * 100.0 / (FireWork_Sld[13][1] - FireWork_Sld[13][0]);
	pos[14]= (m_ES_lifefate - FireWork_Sld[14][0]) * 100.0 / (FireWork_Sld[14][1] - FireWork_Sld[14][0]);
	pos[15]= (m_ES_slowdown- FireWork_Sld[15][0]) * 100.0 / (FireWork_Sld[15][1] - FireWork_Sld[15][0]);
	pos[16]= (m_ES_speedmax - FireWork_Sld[16][0]) * 100.0 / (FireWork_Sld[16][1] - FireWork_Sld[16][0]);
	pos[17]= (m_ES_speedmin - FireWork_Sld[17][0]) * 100.0 / (FireWork_Sld[17][1] - FireWork_Sld[17][0]);
	pos[18]= (m_ES_getx - FireWork_Sld[18][0]) * 100.0 / (FireWork_Sld[18][1] - FireWork_Sld[18][0]);
	pos[19]= (m_ES_gety - FireWork_Sld[19][0]) * 100.0 / (FireWork_Sld[19][1] - FireWork_Sld[19][0]);
	pos[20]= (m_ES_getz - FireWork_Sld[20][0]) * 100.0 / (FireWork_Sld[20][1] - FireWork_Sld[20][0]);

	m_SS_maxnum.SetPos(pos[13]);
	m_SS_lifefate.SetPos(pos[14]);
	m_SS_slowdown.SetPos(pos[15]);
	m_SS_speedmax.SetPos(pos[16]);
	m_SS_speedmin.SetPos(pos[17]);
	m_SS_getx.SetPos(pos[18]);
	m_SS_gety.SetPos(pos[19]);
	m_SS_getz.SetPos(pos[20]);

	UpdateData(FALSE);						//将程序里的值及时更新到控件
}

void CDLG_FireWork::GetExtData(DlgData_PRT &Ex_DLGData)
{
	m_DLGData=&Ex_DLGData;	//取得外界存储数据的结构，因为是传址调用，
							//所以只要在这里改，外面的数据也就改了
	UpdateDLGDate();//更新界面
}

///////////////////////////////////////////////////////////////////////
//				将对话框上的数据 Edit 更新到程序
///////////////////////////////////////////////////////////////////////
void CDLG_FireWork::OnKillfocusEiAlxzend() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	if(m_E_xzend<m_E_xzstar)m_E_xzend=m_E_xzstar;
	m_DLGData->AngleXZEnd=m_E_xzend;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_xzend - FireWork_Sld[8][0]) * 100.0 / 
		(FireWork_Sld[8][1] - FireWork_Sld[8][0]);
    m_S_xzend.SetPos(pos);
	UpdateData(FALSE);
}

void CDLG_FireWork::OnKillfocusEiAlxzstar() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	if(m_E_xzstar>m_E_xzend)m_E_xzstar=m_E_xzend;
	m_DLGData->AngleXZStar=m_E_xzstar;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_xzstar - FireWork_Sld[7][0]) * 100.0 / 
		(FireWork_Sld[7][1] - FireWork_Sld[7][0]);
    m_S_xzstar.SetPos(pos);
	UpdateData(FALSE);
}

void CDLG_FireWork::OnKillfocusEiAlyend() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	if(m_E_yend<m_E_ystar)m_E_yend=m_E_ystar;
	m_DLGData->AngleYEnd=m_E_yend;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_yend - FireWork_Sld[6][0]) * 100.0 / 
		(FireWork_Sld[6][1] - FireWork_Sld[6][0]);
    m_S_yend.SetPos(pos);
	UpdateData(FALSE); 
}

void CDLG_FireWork::OnKillfocusEiAlystar() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	if(m_E_ystar>m_E_yend)m_E_ystar=m_E_yend;
	m_DLGData->AngleYStar=m_E_ystar;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_ystar - FireWork_Sld[5][0]) * 100.0 / 
		(FireWork_Sld[5][1] - FireWork_Sld[5][0]);
    m_S_ystar.SetPos(pos);
	UpdateData(FALSE); 
}

void CDLG_FireWork::OnKillfocusEiGetx() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->GetXg=m_E_getx;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_getx - FireWork_Sld[9][0]) * 100.0 / 
		(FireWork_Sld[9][1] - FireWork_Sld[9][0]);
    m_S_getx.SetPos(pos);
}

void CDLG_FireWork::OnKillfocusEiGety() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->GetYg=m_E_gety;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_gety - FireWork_Sld[10][0]) * 100.0 / 
		(FireWork_Sld[10][1] - FireWork_Sld[10][0]);
    m_S_gety.SetPos(pos);
}

void CDLG_FireWork::OnKillfocusEiGetz() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->GetZg=m_E_getz;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_getz - FireWork_Sld[11][0]) * 100.0 / 
		(FireWork_Sld[11][1] - FireWork_Sld[11][0]);
    m_S_getz.SetPos(pos);
}

void CDLG_FireWork::OnKillfocusEiLifefate() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->lifefate=m_E_lifefate;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_lifefate - FireWork_Sld[1][0]) * 100.0 / 
		(FireWork_Sld[1][1] - FireWork_Sld[1][0]);
    m_S_lifefate.SetPos(pos);
}

void CDLG_FireWork::OnKillfocusEiMaxnum() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->MAX_PARTICLES=m_E_maxnum;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_maxnum - FireWork_Sld[0][0]) * 100.0 / 
		(FireWork_Sld[0][1] - FireWork_Sld[0][0]);
    m_S_maxnum.SetPos(pos);
}

void CDLG_FireWork::OnKillfocusEiPosX() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->star_x=m_E_posx;		//将程序的值更新到真正所控制的对象
}

void CDLG_FireWork::OnKillfocusEiPosY() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->star_y=m_E_posy;		//将程序的值更新到真正所控制的对象
}

void CDLG_FireWork::OnKillfocusEiPosZ() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->star_z=m_E_posz;		//将程序的值更新到真正所控制的对象
}

void CDLG_FireWork::OnKillfocusEiSlowdown() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->slowdown=m_E_slowdown;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_slowdown - FireWork_Sld[2][0]) * 100.0 / 
		(FireWork_Sld[2][1] - FireWork_Sld[2][0]);
    m_S_slowdown.SetPos(pos);
}

void CDLG_FireWork::OnKillfocusEiSpeedmax() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	if(m_E_speedmax<m_E_speedmin)m_E_speedmax=m_E_speedmin;
	m_DLGData->speedMax=m_E_speedmax;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_speedmax - FireWork_Sld[3][0]) * 100.0 / 
		(FireWork_Sld[3][1] - FireWork_Sld[3][0]);
    m_S_speedmax.SetPos(pos);
	UpdateData(FALSE); 
}

void CDLG_FireWork::OnKillfocusEiSpeedmin() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	if(m_E_speedmin>m_E_speedmax)m_E_speedmin=m_E_speedmax;
	m_DLGData->speedMin=m_E_speedmin;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_speedmin - FireWork_Sld[4][0]) * 100.0 / 
		(FireWork_Sld[4][1] - FireWork_Sld[4][0]);
    m_S_speedmin.SetPos(pos);
	UpdateData(FALSE); 
}

void CDLG_FireWork::OnKillfocusEITextWidth() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);					//将界面的值更新到程序
	m_DLGData->TextWidth=m_E_TW;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_TW - FireWork_Sld[12][0]) * 100.0 / 
		(FireWork_Sld[12][1] - FireWork_Sld[12][0]);
    m_S_TW.SetPos(pos);
}

void CDLG_FireWork::OnKillfocusEisGetx() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->SonPDD.GetXg=m_ES_getx;		//将程序的值更新到真正所控制的对象
	float pos = (m_ES_getx - FireWork_Sld[18][0]) * 100.0 / 
		(FireWork_Sld[18][1] - FireWork_Sld[18][0]);
    m_SS_getx.SetPos(pos);
}

void CDLG_FireWork::OnKillfocusEisGety() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->SonPDD.GetYg=m_ES_gety;		//将程序的值更新到真正所控制的对象
	float pos = (m_ES_gety - FireWork_Sld[19][0]) * 100.0 / 
		(FireWork_Sld[19][1] - FireWork_Sld[19][0]);
    m_SS_gety.SetPos(pos);
}

void CDLG_FireWork::OnKillfocusEisGetz() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->SonPDD.GetZg=m_ES_getz;		//将程序的值更新到真正所控制的对象
	float pos = (m_ES_getz - FireWork_Sld[20][0]) * 100.0 / 
		(FireWork_Sld[20][1] - FireWork_Sld[20][0]);
    m_SS_getz.SetPos(pos);
}

void CDLG_FireWork::OnKillfocusEisLifefate() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->SonPDD.lifefate=m_ES_lifefate;		//将程序的值更新到真正所控制的对象
	float pos = (m_ES_lifefate - FireWork_Sld[14][0]) * 100.0 / 
		(FireWork_Sld[14][1] - FireWork_Sld[14][0]);
    m_SS_lifefate.SetPos(pos);
}

void CDLG_FireWork::OnKillfocusEisMaxnum() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->SonPDD.MAX_PARTICLES=m_ES_maxnum;		//将程序的值更新到真正所控制的对象
	float pos = (m_ES_maxnum - FireWork_Sld[13][0]) * 100.0 / 
		(FireWork_Sld[13][1] - FireWork_Sld[13][0]);
    m_SS_maxnum.SetPos(pos);
}

void CDLG_FireWork::OnKillfocusEisSlowdown() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->SonPDD.slowdown=m_ES_slowdown;		//将程序的值更新到真正所控制的对象
	float pos = (m_ES_slowdown - FireWork_Sld[15][0]) * 100.0 / 
		(FireWork_Sld[15][1] - FireWork_Sld[15][0]);
    m_SS_slowdown.SetPos(pos);
}

void CDLG_FireWork::OnKillfocusEisSpeedmax() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	if(m_ES_speedmax<m_ES_speedmin)m_ES_speedmax=m_ES_speedmin;
	m_DLGData->SonPDD.speedMax=m_ES_speedmax;		//将程序的值更新到真正所控制的对象
	float pos = (m_ES_speedmax - FireWork_Sld[16][0]) * 100.0 / 
		(FireWork_Sld[16][1] - FireWork_Sld[16][0]);
    m_SS_speedmax.SetPos(pos);
	UpdateData(FALSE);
}

void CDLG_FireWork::OnKillfocusEisSpeedmin() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	if(m_ES_speedmin>m_ES_speedmax)m_ES_speedmin=m_ES_speedmax;
	m_DLGData->SonPDD.speedMin=m_ES_speedmin;		//将程序的值更新到真正所控制的对象
	float pos = (m_ES_speedmin - FireWork_Sld[17][0]) * 100.0 / 
		(FireWork_Sld[17][1] - FireWork_Sld[17][0]);
    m_SS_speedmin.SetPos(pos);
	UpdateData(FALSE);
}


/////////////////////////////////////////////////////////////////////
//				滑条与外界进行互动
//////////////////////////////////////////////////////////////////////
void CDLG_FireWork::OnReleasedcaptureSiAlxzend(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_xzend=(m_S_xzend.GetPos()/100.0)
		*(FireWork_Sld[8][1] - FireWork_Sld[8][0]) 
		+ FireWork_Sld[8][0];

	if(m_E_xzend<m_E_xzstar)
	{
		m_E_xzend=m_E_xzstar;
		float pos = (m_E_xzend - FireWork_Sld[8][0]) * 100.0 / 
		(FireWork_Sld[8][1] - FireWork_Sld[8][0]);
		m_S_xzend.SetPos(pos);
	}

	m_DLGData->AngleXZEnd=m_E_xzend;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_FireWork::OnReleasedcaptureSiAlxzstar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_xzstar=(m_S_xzstar.GetPos()/100.0)
		*(FireWork_Sld[7][1] - FireWork_Sld[7][0]) 
		+ FireWork_Sld[7][0];

	if(m_E_xzstar>m_E_xzend)
	{
		m_E_xzstar=m_E_xzend;
		float pos = (m_E_xzstar - FireWork_Sld[7][0]) * 100.0 / 
		(FireWork_Sld[7][1] - FireWork_Sld[7][0]);
		m_S_xzstar.SetPos(pos);
	}

	m_DLGData->AngleXZStar=m_E_xzstar;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_FireWork::OnReleasedcaptureSiAlyend(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_yend=(m_S_yend.GetPos()/100.0)
		*(FireWork_Sld[6][1] - FireWork_Sld[6][0]) 
		+ FireWork_Sld[6][0];

	if(m_E_yend<m_E_ystar)
	{
		m_E_yend=m_E_ystar;
		float pos = (m_E_yend - FireWork_Sld[6][0]) * 100.0 / 
		(FireWork_Sld[6][1] - FireWork_Sld[6][0]);
		m_S_yend.SetPos(pos);
	}

	m_DLGData->AngleYEnd=m_E_yend;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_FireWork::OnReleasedcaptureSiAlystar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_ystar=(m_S_ystar.GetPos()/100.0)
		*(FireWork_Sld[5][1] - FireWork_Sld[5][0]) 
		+ FireWork_Sld[5][0];

	if(m_E_ystar>m_E_yend)
	{
		m_E_ystar=m_E_yend;
		float pos = (m_E_ystar - FireWork_Sld[5][0]) * 100.0 / 
		(FireWork_Sld[5][1] - FireWork_Sld[5][0]);
		m_S_ystar.SetPos(pos);
	}

	m_DLGData->AngleYStar=m_E_ystar;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_FireWork::OnReleasedcaptureSiGetx(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_getx=(m_S_getx.GetPos()/100.0)
		*(FireWork_Sld[9][1] - FireWork_Sld[9][0]) 
		+ FireWork_Sld[9][0];
	m_DLGData->GetXg=m_E_getx;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_FireWork::OnReleasedcaptureSiGety(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_gety=(m_S_gety.GetPos()/100.0)
		*(FireWork_Sld[10][1] - FireWork_Sld[10][0]) 
		+ FireWork_Sld[10][0];
	m_DLGData->GetYg=m_E_gety;
	UpdateData(FALSE); 	
	*pResult = 0;
}

void CDLG_FireWork::OnReleasedcaptureSiGetz(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_getz=(m_S_getz.GetPos()/100.0)
		*(FireWork_Sld[11][1] - FireWork_Sld[11][0]) 
		+ FireWork_Sld[11][0];
	m_DLGData->GetZg=m_E_getz;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_FireWork::OnReleasedcaptureSiLifefate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_lifefate=(m_S_lifefate.GetPos()/100.0)
		*(FireWork_Sld[1][1] - FireWork_Sld[1][0]) 
		+ FireWork_Sld[1][0];
	m_DLGData->lifefate=m_E_lifefate;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_FireWork::OnReleasedcaptureSiMaxnum(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_maxnum=(m_S_maxnum.GetPos()/100.0)
		*(FireWork_Sld[0][1] - FireWork_Sld[0][0]) 
		+ FireWork_Sld[0][0];
	m_DLGData->MAX_PARTICLES=m_E_maxnum;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_FireWork::OnReleasedcaptureSiSlowdown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_slowdown=(m_S_slowdown.GetPos()/100.0)
		*(FireWork_Sld[2][1] - FireWork_Sld[2][0]) 
		+ FireWork_Sld[2][0];
	m_DLGData->slowdown=m_E_slowdown;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_FireWork::OnReleasedcaptureSiSpeedMin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_speedmin=(m_S_speedmin.GetPos()/100.0)
		*(FireWork_Sld[4][1] - FireWork_Sld[4][0]) 
		+ FireWork_Sld[4][0];

	if(m_E_speedmin>m_E_speedmax)
	{
		m_E_speedmin=m_E_speedmax;
		float pos = (m_E_speedmin - FireWork_Sld[4][0]) * 100.0 / 
		(FireWork_Sld[4][1] - FireWork_Sld[4][0]);
		m_S_speedmin.SetPos(pos);
	}

	m_DLGData->speedMin=m_E_speedmin;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_FireWork::OnReleasedcaptureSiSpeedmax(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_speedmax=(m_S_speedmax.GetPos()/100.0)
		*(FireWork_Sld[3][1] - FireWork_Sld[3][0]) 
		+ FireWork_Sld[3][0];

	if(m_E_speedmax<m_E_speedmin)
	{
		m_E_speedmax=m_E_speedmin;
		float pos = (m_E_speedmax - FireWork_Sld[3][0]) * 100.0 / 
		(FireWork_Sld[3][1] - FireWork_Sld[3][0]);
		m_S_speedmax.SetPos(pos);
	}

	m_DLGData->speedMax=m_E_speedmax;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_FireWork::OnReleasedcaptureSITextWidth(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_TW=(m_S_TW.GetPos()/100.0)
		*(FireWork_Sld[12][1] - FireWork_Sld[12][0]) 
		+ FireWork_Sld[12][0];
	m_DLGData->TextWidth=m_E_TW;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_FireWork::OnReleasedcaptureSisGetx(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_ES_getx=(m_SS_getx.GetPos()/100.0)
		*(FireWork_Sld[18][1] - FireWork_Sld[18][0]) 
		+ FireWork_Sld[18][0];
	m_DLGData->SonPDD.GetXg=m_ES_getx;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_FireWork::OnReleasedcaptureSisGety(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_ES_gety=(m_SS_gety.GetPos()/100.0)
		*(FireWork_Sld[19][1] - FireWork_Sld[19][0]) 
		+ FireWork_Sld[19][0];
	m_DLGData->SonPDD.GetYg=m_ES_gety;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_FireWork::OnReleasedcaptureSisGetz(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_ES_getz=(m_SS_getz.GetPos()/100.0)
		*(FireWork_Sld[20][1] - FireWork_Sld[20][0]) 
		+ FireWork_Sld[20][0];
	m_DLGData->SonPDD.GetZg=m_ES_getz;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_FireWork::OnReleasedcaptureSisLifefate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_ES_lifefate=(m_SS_lifefate.GetPos()/100.0)
		*(FireWork_Sld[14][1] - FireWork_Sld[14][0]) 
		+ FireWork_Sld[14][0];
	m_DLGData->SonPDD.lifefate=m_ES_lifefate;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_FireWork::OnReleasedcaptureSisMaxnum(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_ES_maxnum=(m_SS_maxnum.GetPos()/100.0)
		*(FireWork_Sld[13][1] - FireWork_Sld[13][0]) 
		+ FireWork_Sld[13][0];
	m_DLGData->SonPDD.MAX_PARTICLES=m_ES_maxnum;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_FireWork::OnReleasedcaptureSisSlowdown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_ES_slowdown=(m_SS_slowdown.GetPos()/100.0)
		*(FireWork_Sld[15][1] - FireWork_Sld[15][0]) 
		+ FireWork_Sld[15][0];
	m_DLGData->SonPDD.slowdown=m_ES_slowdown;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_FireWork::OnReleasedcaptureSisSpeedmax(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_ES_speedmax=(m_SS_speedmax.GetPos()/100.0)
		*(FireWork_Sld[16][1] - FireWork_Sld[16][0]) 
		+ FireWork_Sld[16][0];

	if(m_ES_speedmax<m_ES_speedmin)
	{
		m_ES_speedmax=m_ES_speedmin;
		float pos = (m_ES_speedmax - FireWork_Sld[16][0]) * 100.0 / 
		(FireWork_Sld[16][1] - FireWork_Sld[16][0]);
		m_SS_speedmax.SetPos(pos);
	}

	m_DLGData->SonPDD.speedMax=m_ES_speedmax;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_FireWork::OnReleasedcaptureSisSpeedmin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_ES_speedmin=(m_SS_speedmin.GetPos()/100.0)
		*(FireWork_Sld[17][1] - FireWork_Sld[17][0]) 
		+ FireWork_Sld[17][0];

	if(m_ES_speedmin>m_ES_speedmax)
	{
		m_ES_speedmin=m_ES_speedmax;
		float pos = (m_ES_speedmin - FireWork_Sld[17][0]) * 100.0 / 
		(FireWork_Sld[17][1] - FireWork_Sld[17][0]);
		m_SS_speedmin.SetPos(pos);
	}

	m_DLGData->SonPDD.speedMin=m_ES_speedmin;
	UpdateData(FALSE); 
	*pResult = 0;
}



/////////////////////////////////////////////////
//颜色按钮，显示颜色对话框
void CDLG_FireWork::OnBiColor() 
{
	// TODO: Add your control notification handler code here
	//颜色子对话框的数据调入
	if(!colorEixst)
	{
		m_ColorDLG.Create(IDD_DLG_COLOR,NULL);//创建颜色对话框 ,如果没有生成过则生成
		m_ColorDLG.m_E_Title="礼花颜色调整";	//改变标题
		m_ColorDLG.UpdateData(FALSE);
		colorEixst=true;
	}
	m_ColorDLG.GetExtColorData(m_DLGData->PrtColor);
	m_ColorDLG.ShowWindow(SW_SHOW);
}
///////////////////////////////////////////////////
//贴图选择单选按钮
void CDLG_FireWork::OnRi1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_RI1=0;
	m_RI2=-1;
	m_RI3=-1;
	m_RI4=-1;
	m_DLGData->TextModel=0;
	UpdateData(FALSE); 
}

void CDLG_FireWork::OnRi2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_RI1=-1;
	m_RI2=0;
	m_RI3=-1;
	m_RI4=-1;
	m_DLGData->TextModel=1;
	UpdateData(FALSE); 
}

void CDLG_FireWork::OnRi3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_RI1=-1;
	m_RI2=-1;
	m_RI3=0;
	m_RI4=-1;
	m_DLGData->TextModel=2;
	UpdateData(FALSE); 
}

void CDLG_FireWork::OnRi4() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_RI1=-1;
	m_RI2=-1;
	m_RI3=-1;
	m_RI4=0;
	m_DLGData->TextModel=3;
	UpdateData(FALSE); 
}

void CDLG_FireWork::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(colorEixst)//如果创建过颜色对话框则让他也隐藏
	{
		m_ColorDLG.ShowWindow(SW_HIDE);
	}
	CDialog::OnClose();
}

//加载效果
void CDLG_FireWork::OnBfwLoad() 
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
				if(ch!=3)MessageBox("不是礼花效果文件！");
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
void CDLG_FireWork::OnBfwSave() 
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
		int ch=3;
		fputc(ch,fp);	//写入粒子效果的表示，3就是礼花！
		fwrite(m_DLGData,sizeof(*m_DLGData),1,fp);//写入我们的效果数据
		fclose(fp);//关闭文件指针
		}
	}
	SetCurrentDirectory(szCurDir);//恢复当前路径！
}
