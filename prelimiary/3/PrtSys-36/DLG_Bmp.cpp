// DLG_Bmp.cpp : implementation file
//

#include "stdafx.h"
#include "PrtSys.h"
#include "DLG_Bmp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_Bmp dialog

//设置滑动条范围的控制数组
GLfloat BMP_Sld[8][2] =	//BMP的属性滑条控制范围。
{
  {0,	9999} ,				//粒子的数量 unsigned long			0
  {0.0001   , 10.0  } ,		//lifefate	1.0						1
  {0.0001   , 10.0  } ,		//slowdown  2.0						2
  {-20.0  , 20.0  } ,		//GetXg 0							3
  {-20.0  , 20.0  } ,		//GetYg 0.1							4
  {-20.0 ,  20.0  },		//GetZg 0							5
  {0.0001,	  5.0  },		//贴图宽度							6
  {0.0,	  20.0  },			//图象厚度							7
};

CDLG_Bmp::CDLG_Bmp(CWnd* pParent /*=NULL*/)
	: CDialog(CDLG_Bmp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_Bmp)
	m_E_file = _T("");
	m_E_getx = 0.0f;
	m_E_gety = 0.0f;
	m_E_getz = 0.0f;
	m_E_lifefate = 0.0f;
	m_E_maxnum = 0;
	m_E_posx = 0.0f;
	m_E_posy = 0.0f;
	m_E_posz = 0.0f;
	m_E_slowdown = 0.0f;
	m_E_TW = 0.0f;
	m_E_thick = 0.0f;
	//}}AFX_DATA_INIT
}


void CDLG_Bmp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_Bmp)
	DDX_Control(pDX, IDC_SB_Thick, m_S_thick);
	DDX_Control(pDX, IDC_SB_TextWitdh, m_S_TW);
	DDX_Control(pDX, IDC_SB_SLOWDOWN, m_S_slowdown);
	DDX_Control(pDX, IDC_SB_MAXNUM, m_S_maxnum);
	DDX_Control(pDX, IDC_SB_LIFEFATE, m_S_lifefate);
	DDX_Control(pDX, IDC_SB_GETZ, m_S_getz);
	DDX_Control(pDX, IDC_SB_GETY, m_S_gety);
	DDX_Control(pDX, IDC_SB_GETX, m_S_getx);
	DDX_Text(pDX, IDC_EB_FILE, m_E_file);
	DDX_Text(pDX, IDC_EB_GETX, m_E_getx);
	DDX_Text(pDX, IDC_EB_GETY, m_E_gety);
	DDX_Text(pDX, IDC_EB_GETZ, m_E_getz);
	DDX_Text(pDX, IDC_EB_LIFEFATE, m_E_lifefate);
	DDX_Text(pDX, IDC_EB_MAXNUM, m_E_maxnum);
	DDX_Text(pDX, IDC_EB_POS_X, m_E_posx);
	DDX_Text(pDX, IDC_EB_POS_Y, m_E_posy);
	DDX_Text(pDX, IDC_EB_POS_Z, m_E_posz);
	DDX_Text(pDX, IDC_EB_SLOWDOWN, m_E_slowdown);
	DDX_Text(pDX, IDC_EB_TextWidth, m_E_TW);
	DDX_Text(pDX, IDC_EB_Thick, m_E_thick);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_Bmp, CDialog)
	//{{AFX_MSG_MAP(CDLG_Bmp)
	ON_BN_CLICKED(IDC_BB_FILE, OnBbFile)
	ON_BN_CLICKED(IDC_BB_LOAD, OnBbLoad)
	ON_BN_CLICKED(IDC_BB_SAVE, OnBbSave)
	ON_EN_KILLFOCUS(IDC_EB_GETX, OnKillfocusEbGetx)
	ON_EN_KILLFOCUS(IDC_EB_GETY, OnKillfocusEbGety)
	ON_EN_KILLFOCUS(IDC_EB_GETZ, OnKillfocusEbGetz)
	ON_EN_KILLFOCUS(IDC_EB_LIFEFATE, OnKillfocusEbLifefate)
	ON_EN_KILLFOCUS(IDC_EB_MAXNUM, OnKillfocusEbMaxnum)
	ON_EN_KILLFOCUS(IDC_EB_POS_X, OnKillfocusEbPosX)
	ON_EN_KILLFOCUS(IDC_EB_POS_Y, OnKillfocusEbPosY)
	ON_EN_KILLFOCUS(IDC_EB_POS_Z, OnKillfocusEbPosZ)
	ON_EN_KILLFOCUS(IDC_EB_SLOWDOWN, OnKillfocusEbSlowdown)
	ON_EN_KILLFOCUS(IDC_EB_TextWidth, OnKillfocusEBTextWidth)
	ON_EN_KILLFOCUS(IDC_EB_Thick, OnKillfocusEBThick)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SB_GETX, OnReleasedcaptureSbGetx)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SB_GETY, OnReleasedcaptureSbGety)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SB_GETZ, OnReleasedcaptureSbGetz)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SB_LIFEFATE, OnReleasedcaptureSbLifefate)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SB_MAXNUM, OnReleasedcaptureSbMaxnum)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SB_SLOWDOWN, OnReleasedcaptureSbSlowdown)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SB_TextWitdh, OnReleasedcaptureSBTextWitdh)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SB_Thick, OnReleasedcaptureSBThick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_Bmp message handlers
void CDLG_Bmp::UpdateDLGDate(void)//更新界面数据
{
	m_E_maxnum=m_DLGData->MAX_PARTICLES;	//粒子的最大数（不可超过10000）

	m_E_posx=m_DLGData->star_x;				//粒子源的位置
	m_E_posy=m_DLGData->star_y;
	m_E_posz=m_DLGData->star_z;
	m_E_lifefate=m_DLGData->lifefate;		// 削减粒子的生命，数值约大，粒子的生命就越短
	m_E_slowdown=m_DLGData->slowdown;		// 减慢粒子的速度，数值约大越慢，越小越快

	m_E_getx=m_DLGData->GetXg;				//粒子系统受到的空间重力场（矢量）
	m_E_gety=m_DLGData->GetYg;
	m_E_getz=m_DLGData->GetZg;
	m_E_TW=m_DLGData->TextWidth;			//贴图宽度
	m_E_thick=m_DLGData->pram[1];			//厚度

	LPTSTR p = m_E_file.GetBuffer(260);	//开辟缓冲区
	strcpy(p,*FileName);				//将文件名装入String对象中
	m_E_file.ReleaseBuffer();			//释放缓冲


	//***************************************************************
	UpdateData(FALSE);						//将程序里的值及时更新到控件

	//对滑动条进行初始化
	float pos[8] ;
	pos[0]= (m_E_maxnum - BMP_Sld[0][0]) * 100.0 / (BMP_Sld[0][1] - BMP_Sld[0][0]);
	pos[1]= (m_E_lifefate - BMP_Sld[1][0]) * 100.0 / (BMP_Sld[1][1] - BMP_Sld[1][0]);
	pos[2]= (m_E_slowdown - BMP_Sld[2][0]) * 100.0 / (BMP_Sld[2][1] - BMP_Sld[2][0]);
	pos[3]= (m_E_getx - BMP_Sld[3][0]) * 100.0 / (BMP_Sld[3][1] - BMP_Sld[3][0]);
	pos[4]= (m_E_gety - BMP_Sld[4][0]) * 100.0 / (BMP_Sld[4][1] - BMP_Sld[4][0]);
	pos[5]= (m_E_getz - BMP_Sld[5][0]) * 100.0 / (BMP_Sld[5][1] - BMP_Sld[5][0]);
	pos[6]= (m_E_TW - BMP_Sld[6][0]) * 100.0 / (BMP_Sld[6][1] - BMP_Sld[6][0]);
	pos[7]= (m_E_thick - BMP_Sld[7][0]) * 100.0 / (BMP_Sld[7][1] - BMP_Sld[7][0]);

	m_S_maxnum.SetPos(pos[0]);
	m_S_lifefate.SetPos(pos[1]);
	m_S_slowdown.SetPos(pos[2]);
	m_S_getx.SetPos(pos[3]);
	m_S_gety.SetPos(pos[4]);
	m_S_getz.SetPos(pos[5]);
	m_S_TW.SetPos(pos[6]);
	m_S_thick.SetPos(pos[7]);
}




void CDLG_Bmp::OnBbFile() 
{
	// TODO: Add your control notification handler code here
	char szCurDir[256];//保存当前目录
	GetCurrentDirectory(sizeof(szCurDir),szCurDir);

	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,"真彩24b位图文件(*.bmp)|*.bmp||",NULL);

	if(dlg.DoModal()==IDOK)						//显示文件对话框
	{
		m_E_file = dlg.GetPathName();			//取得文件名
		strcpy(*FileName,m_E_file);				//将文件名更新到外界的对象
		m_DLGData->dead=true;					//设置文件更新标致
		UpdateDLGDate();						//更新界面数据
	}
	SetCurrentDirectory(szCurDir);				//恢复当前路径！
}

void CDLG_Bmp::OnBbLoad() 
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
				if(ch!=7)MessageBox("不是位图效果文件！");
				else
				{
					fseek(fp,5L,0);
					fread(m_DLGData,sizeof(*m_DLGData),1,fp);//读入我们的效果数据
					fseek(fp,5L+sizeof(*m_DLGData),0);
					fread(FileName,260,1,fp);//读入位图文件名
					m_DLGData->dead=true;	//设置文件更新标致
				}
			}
			fclose(fp);//关闭文件指针
		}
		UpdateDLGDate();//更新界面数据
	}
	SetCurrentDirectory(szCurDir);//恢复当前路径！
}

void CDLG_Bmp::OnBbSave() 
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
		int ch=7;
		fputc(ch,fp);	//写入粒子效果的表示，7就是位图！
		fwrite(m_DLGData,sizeof(*m_DLGData),1,fp);//写入我们的效果数据
		fwrite(FileName,260,1,fp);//写入位图文件名
		fclose(fp);//关闭文件指针
		}
	}
	SetCurrentDirectory(szCurDir);//恢复当前路径！
}

void CDLG_Bmp::OnKillfocusEbGetx() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->GetXg=m_E_getx;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_getx - BMP_Sld[3][0]) * 100.0 / 
		(BMP_Sld[3][1] - BMP_Sld[3][0]);
    m_S_getx.SetPos(pos);
}

void CDLG_Bmp::OnKillfocusEbGety() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->GetYg=m_E_gety;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_gety - BMP_Sld[4][0]) * 100.0 / 
		(BMP_Sld[4][1] - BMP_Sld[4][0]);
    m_S_gety.SetPos(pos);
}

void CDLG_Bmp::OnKillfocusEbGetz() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->GetZg=m_E_getz;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_getz - BMP_Sld[5][0]) * 100.0 / 
		(BMP_Sld[5][1] - BMP_Sld[5][0]);
    m_S_getz.SetPos(pos);
}

void CDLG_Bmp::OnKillfocusEbLifefate() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->lifefate=m_E_lifefate;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_lifefate - BMP_Sld[1][0]) * 100.0 / 
		(BMP_Sld[1][1] - BMP_Sld[1][0]);
    m_S_lifefate.SetPos(pos);
}

void CDLG_Bmp::OnKillfocusEbMaxnum() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->MAX_PARTICLES=m_E_maxnum;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_maxnum - BMP_Sld[0][0]) * 100.0 / 
		(BMP_Sld[0][1] - BMP_Sld[0][0]);
    m_S_maxnum.SetPos(pos);
}

void CDLG_Bmp::OnKillfocusEbPosX() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->star_x=m_E_posx;		//将程序的值更新到真正所控制的对象
}

void CDLG_Bmp::OnKillfocusEbPosY() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->star_y=m_E_posy;		//将程序的值更新到真正所控制的对象
}

void CDLG_Bmp::OnKillfocusEbPosZ() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->star_z=m_E_posz;		//将程序的值更新到真正所控制的对象
}

void CDLG_Bmp::OnKillfocusEbSlowdown() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->slowdown=m_E_slowdown;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_slowdown - BMP_Sld[2][0]) * 100.0 / 
		(BMP_Sld[2][1] - BMP_Sld[2][0]);
    m_S_slowdown.SetPos(pos);
}

void CDLG_Bmp::OnKillfocusEBTextWidth() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->TextWidth=m_E_TW;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_TW - BMP_Sld[6][0]) * 100.0 / 
		(BMP_Sld[6][1] - BMP_Sld[6][0]);
    m_S_TW.SetPos(pos);
}

void CDLG_Bmp::OnKillfocusEBThick() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);						//将界面的值更新到程序
	m_DLGData->pram[1]=m_E_thick;		//将程序的值更新到真正所控制的对象
	float pos = (m_E_thick - BMP_Sld[7][0]) * 100.0 / 
		(BMP_Sld[7][1] - BMP_Sld[7][0]);
    m_S_thick.SetPos(pos);
}

void CDLG_Bmp::OnReleasedcaptureSbGetx(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_getx=(m_S_getx.GetPos()/100.0)
		*(BMP_Sld[3][1] - BMP_Sld[3][0]) 
		+ BMP_Sld[3][0];
	m_DLGData->GetXg=m_E_getx;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Bmp::OnReleasedcaptureSbGety(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_gety=(m_S_gety.GetPos()/100.0)
		*(BMP_Sld[4][1] - BMP_Sld[4][0]) 
		+ BMP_Sld[4][0];
	m_DLGData->GetYg=m_E_gety;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Bmp::OnReleasedcaptureSbGetz(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_E_getz=(m_S_getz.GetPos()/100.0)
		*(BMP_Sld[5][1] - BMP_Sld[5][0]) 
		+ BMP_Sld[5][0];
	m_DLGData->GetZg=m_E_getz;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Bmp::OnReleasedcaptureSbLifefate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_E_lifefate=(m_S_lifefate.GetPos()/100.0)
		*(BMP_Sld[1][1] - BMP_Sld[1][0]) 
		+ BMP_Sld[1][0];
	m_DLGData->lifefate=m_E_lifefate;

	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Bmp::OnReleasedcaptureSbMaxnum(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_E_maxnum=(m_S_maxnum.GetPos()/100.0)
		*(BMP_Sld[0][1] - BMP_Sld[0][0]) 
		+ BMP_Sld[0][0];
	m_DLGData->MAX_PARTICLES=m_E_maxnum;

	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Bmp::OnReleasedcaptureSbSlowdown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_E_slowdown=(m_S_slowdown.GetPos()/100.0)
		*(BMP_Sld[2][1] - BMP_Sld[2][0]) 
		+ BMP_Sld[2][0];
	m_DLGData->slowdown=m_E_slowdown;

	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Bmp::OnReleasedcaptureSBTextWitdh(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_E_TW=(m_S_TW.GetPos()/100.0)
		*(BMP_Sld[6][1] - BMP_Sld[6][0]) 
		+ BMP_Sld[6][0];
	m_DLGData->TextWidth=m_E_TW;

	UpdateData(FALSE); 
	*pResult = 0;
}

void CDLG_Bmp::OnReleasedcaptureSBThick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_E_thick=(m_S_thick.GetPos()/100.0)
		*(BMP_Sld[7][1] - BMP_Sld[7][0]) 
		+ BMP_Sld[7][0];
	m_DLGData->pram[1]=m_E_thick;

	UpdateData(FALSE); 
	*pResult = 0;
}
////////////////////////////////////////
void CDLG_Bmp::GetExtData(DlgData_PRT &Ex_DLGData,char (*tFName)[256])	//获得外界数据
{
	m_DLGData=&Ex_DLGData;	//取得外界存储数据的结构，因为是传址调用，
							//所以只要在这里改，外面的数据也就改了
	FileName=tFName;		//获得文件名
	UpdateDLGDate();		//更新界面数据
}
