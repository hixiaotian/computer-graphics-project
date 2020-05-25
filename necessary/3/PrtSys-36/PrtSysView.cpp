// PrtSysView.cpp : implementation of the CPrtSysView class
//

#include "stdafx.h"
#include "PrtSys.h"
#include <math.h>
#include "PrtSysDoc.h"
#include "PrtSysView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrtSysView

IMPLEMENT_DYNCREATE(CPrtSysView, CView)

BEGIN_MESSAGE_MAP(CPrtSysView, CView)
	//{{AFX_MSG_MAP(CPrtSysView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(ID_KW_FULLSCREEN, OnFullscreen)
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_KW_FACEORLINE, OnFaceorline)
	ON_COMMAND(ID_KW_HIDEORSHOW, OnHideorshow)
	ON_COMMAND(ID_KW_SHOW3DSDLG, OnShow3dsdlg)
	ON_COMMAND(ID_KW_PLAY, OnPlay)
	ON_COMMAND(ID_SHOW_FIRE, OnShowFire)
	ON_UPDATE_COMMAND_UI(ID_SHOW_FIRE, OnUpdateShowFire)
	ON_COMMAND(ID_KW_SETFULLSCREEN, OnSetfullscreen)
	ON_COMMAND(ID_SHOW_WATERFALL, OnShowWaterfall)
	ON_UPDATE_COMMAND_UI(ID_SHOW_WATERFALL, OnUpdateShowWaterfall)
	ON_COMMAND(ID_SHOW_FIREWORK, OnShowFirework)
	ON_UPDATE_COMMAND_UI(ID_SHOW_FIREWORK, OnUpdateShowFirework)
	ON_COMMAND(ID_EDIT_FIRE, OnEditFire)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIRE, OnUpdateEditFire)
	ON_COMMAND(ID_EDIT_FIREWORK, OnEditFirework)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIREWORK, OnUpdateEditFirework)
	ON_COMMAND(ID_EDIT_WATERFALL, OnEditWaterfall)
	ON_UPDATE_COMMAND_UI(ID_EDIT_WATERFALL, OnUpdateEditWaterfall)
	ON_COMMAND(ID_SHOW_ATOMICBOMB, OnShowAtomicbomb)
	ON_UPDATE_COMMAND_UI(ID_SHOW_ATOMICBOMB, OnUpdateShowAtomicbomb)
	ON_COMMAND(ID_EDIT_ATOMICBOMB, OnEditAtomicbomb)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ATOMICBOMB, OnUpdateEditAtomicbomb)
	ON_COMMAND(ID_SHOW_SNOW, OnShowSnow)
	ON_UPDATE_COMMAND_UI(ID_SHOW_SNOW, OnUpdateShowSnow)
	ON_COMMAND(ID_EDIT_SNOW, OnEditSnow)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SNOW, OnUpdateEditSnow)
	ON_COMMAND(ID_KW_CROSS, OnKwCross)
	ON_COMMAND(ID_KW_DEMO, OnKwDemo)
	ON_COMMAND(ID_KW_FILEOPEN, OnKwFileopen)
	ON_COMMAND(ID_EDIT_BMP, OnEditBmp)
	ON_COMMAND(ID_SHOW_BMP, OnShowBmp)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BMP, OnUpdateEditBmp)
	ON_UPDATE_COMMAND_UI(ID_SHOW_BMP, OnUpdateShowBmp)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrtSysView construction/destruction

CPrtSysView::CPrtSysView()
{
	// TODO: add construction code here
	InitTotalDlgData();//初始化所有对话框数据
}

//初始所有对话框数据
void CPrtSysView::InitTotalDlgData()
{
	//初始化3ds场景对话框数据
	m_3dsDD.hide=false;
	m_3dsDD.IsLine=true;
	m_3dsDD.scale=0.02;
	m_3dsDD.r_data.Angle_x=0.0;
	m_3dsDD.r_data.Angle_y=0.0;
	m_3dsDD.r_data.Angle_z=0.0;
	m_3dsDD.t_data.x=0.0;
	m_3dsDD.t_data.y=0.0;
	m_3dsDD.t_data.z=0.0;

	//**************************
	//控制信息初始化
	m_ptrCtrl.p_fire=false;
	m_ptrCtrl.p_waterfall=false;
	m_ptrCtrl.p_firework=false;
	m_ptrCtrl.p_atomicbomb=false;
	m_ptrCtrl.p_snow=false;
	m_ptrCtrl.p_rain=false;
	m_ptrCtrl.p_demo=false;	//初始演示关闭
	m_ptrCtrl.p_bmp=false;
	m_ptrCtrl.IsPlay=true;


	//*******************************
	//全屏显示信息
	ScreenData.Width=640;
	ScreenData.Height=480;
	ScreenData.IsFullScreen=false;	//全屏关闭
	cross=true;			//显示坐标轴。
	demoPlay=false;		//演示关闭
	DemoAutoFinish=false;
	//**************************
	//生成各个对话框
	m_screen.Create(IDD_SCREEN,NULL); 

	m_firedlg.Create(IDD_DLG_Fire,NULL); 
	m_firedlg.colorEixst=false;//没有生成过颜色对话框

	m_waterfalldlg.Create(IDD_DLG_WaterFall,NULL);
	m_waterfalldlg.colorEixst=false;//没有生成过颜色对话框

	m_fireworkdlg.Create(IDD_DLG_FireWork,NULL);
	m_fireworkdlg.colorEixst=false;//没有生成过颜色对话框

	m_atomicbombdlg.Create(IDD_DLG_AtomicBomb,NULL);

	m_snowdlg.Create(IDD_DLG_Snow,NULL);

	m_bmpdlg.Create(IDD_DLG_BMP,NULL);

	DSdlgExist=false;	//3ds 对话框还不存在
}

CPrtSysView::~CPrtSysView()
{
}

BOOL CPrtSysView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	////////////////////////////////////////////////////////////////
	//设置窗口类型
	cs.style |=WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	////////////////////////////////////////////////////////////////
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CPrtSysView drawing

void CPrtSysView::OnDraw(CDC* pDC)
{
	CPrtSysDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	//////////////////////////////////////////////////////////////////
	RenderScene();	//渲染场景
	//////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////
// CPrtSysView printing

BOOL CPrtSysView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPrtSysView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPrtSysView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CPrtSysView diagnostics

#ifdef _DEBUG
void CPrtSysView::AssertValid() const
{
	CView::AssertValid();
}

void CPrtSysView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPrtSysDoc* CPrtSysView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPrtSysDoc)));
	return (CPrtSysDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPrtSysView message handlers

int CPrtSysView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	//////////////////////////////////////////////////////////////////
	//初始化OpenGL和设置定时器
	m_pDC = new CClientDC(this);
	SetTimer(1, 20, NULL);
	InitializeOpenGL(m_pDC);
	//////////////////////////////////////////////////////////////////
	Init();	
	return 0;
}

void CPrtSysView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	/////////////////////////////////////////////////////////////////
	//删除调色板和渲染上下文、定时器
	::wglMakeCurrent(0,0);
	::wglDeleteContext( m_hRC);
	if (m_hPalette)
	    DeleteObject(m_hPalette);
	if ( m_pDC )
	{
		delete m_pDC;
	}
	KillTimer(1);		
	/////////////////////////////////////////////////////////////////
}


void CPrtSysView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	/////////////////////////////////////////////////////////////////
	//添加窗口缩放时的图形变换函数
	glViewport(0,0,cx,cy);
	GLdouble aspect_ratio;
	aspect_ratio = (GLdouble)cx/(GLdouble)cy;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0F, aspect_ratio, 1.0F, 10000.0F);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/////////////////////////////////////////////////////////////////
}


void CPrtSysView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	/////////////////////////////////////////////////////////////////
	//添加定时器响应函数和场景更新函数
	Invalidate(FALSE);	
	/////////////////////////////////////////////////////////////////
	CView::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////
//	                  设置逻辑调色板
//////////////////////////////////////////////////////////////////////
void CPrtSysView::SetLogicalPalette(void)
{
    struct
    {
        WORD Version;
        WORD NumberOfEntries;
        PALETTEENTRY aEntries[256];
    } logicalPalette = { 0x300, 256 };

	BYTE reds[] = {0, 36, 72, 109, 145, 182, 218, 255};
	BYTE greens[] = {0, 36, 72, 109, 145, 182, 218, 255};
	BYTE blues[] = {0, 85, 170, 255};

    for (int colorNum=0; colorNum<256; ++colorNum)
    {
        logicalPalette.aEntries[colorNum].peRed =
            reds[colorNum & 0x07];
        logicalPalette.aEntries[colorNum].peGreen =
            greens[(colorNum >> 0x03) & 0x07];
        logicalPalette.aEntries[colorNum].peBlue =
            blues[(colorNum >> 0x06) & 0x03];
        logicalPalette.aEntries[colorNum].peFlags = 0;
    }

    m_hPalette = CreatePalette ((LOGPALETTE*)&logicalPalette);
}

//////////////////////////////////////////////////////////
//						初始化openGL场景
//////////////////////////////////////////////////////////
BOOL CPrtSysView::InitializeOpenGL(CDC* pDC)
{
	m_pDC = pDC;
	SetupPixelFormat();
	//生成绘制描述表
	m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc());
	//置当前绘制描述表
	::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	//*************************************************************************
	glClearDepth(1.0f);									// 设置深度缓存
	glDisable(GL_ALPHA_TEST);							// 禁用Alpha测试
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);	// 真正精细的透视修正
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);				// Really Nice Point Smoothing

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// 黑色背景
	glShadeModel(GL_SMOOTH);							// 启用阴影平滑


	GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f};
	GLfloat diffuseLight[] = { 0.3f, 0.3f, 0.3f, 1.0f};
	GLfloat specularLight[] = {1.0,1.0,1.0,1.0};
	GLfloat lightPos[]     = {6000.0f,6000.0f,6000.0f, 1.0f};
	GLfloat lightPos1[]     = {-300.0f,300.0f,300.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	//**********************************************************************
	return TRUE;
}

//////////////////////////////////////////////////////////
//						设置像素格式
//////////////////////////////////////////////////////////
BOOL CPrtSysView::SetupPixelFormat()
{
	PIXELFORMATDESCRIPTOR pfd = { 
	    sizeof(PIXELFORMATDESCRIPTOR),    // pfd结构的大小 
	    1,                                // 版本号 
	    PFD_DRAW_TO_WINDOW |              // 支持在窗口中绘图 
	    PFD_SUPPORT_OPENGL |              // 支持 OpenGL 
	    PFD_DOUBLEBUFFER,                 // 双缓存模式 
	    PFD_TYPE_RGBA,                    // RGBA 颜色模式 
	    24,                               // 24 位颜色深度 
	    0, 0, 0, 0, 0, 0,                 // 忽略颜色位 
	    0,                                // 没有非透明度缓存 
	    0,                                // 忽略移位位 
	    0,                                // 无累加缓存 
	    0, 0, 0, 0,                       // 忽略累加位 
	    32,                               // 32 位深度缓存     
	    0,                                // 无模板缓存 
	    0,                                // 无辅助缓存 
	    PFD_MAIN_PLANE,                   // 主层 
	    0,                                // 保留 
	    0, 0, 0                           // 忽略层,可见性和损毁掩模 
	}; 	
	int pixelformat;
	pixelformat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);//选择像素格式
	::SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd);	//设置像素格式
	if(pfd.dwFlags & PFD_NEED_PALETTE)
		SetLogicalPalette();	//设置逻辑调色板
	return TRUE;
}

//想显示的东西再此调用
//////////////////////////////////////////////////////////
//						场景绘制与渲染
//////////////////////////////////////////////////////////
BOOL CPrtSysView::RenderScene() 
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//眼睛的视点！！！！
	gluLookAt(m_camera.eyePos[0],m_camera.eyePos[1],m_camera.eyePos[2], 
				m_camera.target[0],m_camera.target[1],m_camera.target[2], 
				m_camera.eyeUp[0], m_camera.eyeUp[1], m_camera.eyeUp[2]);

	if(cross)DrawAxis();//画坐标轴

	Draw3ds(); //画3D物体
	SwapBuffers(m_pDC->GetSafeHdc());		//交互缓冲区
	return TRUE;
}

//////////////////////////////////////////////////////////
//							DrawAxis()
//////////////////////////////////////////////////////////
void CPrtSysView::DrawAxis()
{
	float len=10.0;
	glBegin( GL_LINES);
			// x轴		(红色)
			glColor3f(1.0F, 0.0F, 0.0F);
			glVertex3f(-len, 0.0f, 0.0f);
			glVertex3f( len, 0.0f, 0.0f);
			glVertex3f( len-0.5, 0.5f, 0.0f);
			glVertex3f( len, 0.0f, 0.0f);
			glVertex3f( len-0.5,-0.5f,-0.0f);
			glVertex3f( len, 0.0f, 0.0f);

			glColor3f(1.0F, 1.0F, 0.0F);//画“X”
			glVertex3f( len,		0.0f,		0.0f);
			glVertex3f( len+1.0f,	1.0f,		0.0f);
			glVertex3f( len+1.0f,	0.0f,		0.0f);
			glVertex3f( len,		1.0f,		0.0f);

			
			// y轴		（绿色）
			glColor3f(0.0F, 1.0F, 0.0F);
			glVertex3f( 0.0f, -len, 0.0f);
			glVertex3f( 0.0f,  len, 0.0f);
			glVertex3f(-0.5f,  len-0.5f, 0.0f);
			glVertex3f( 0.0f,  len, 0.0f);
			glVertex3f( 0.5f,  len-0.5f, 0.0f);
			glVertex3f( 0.0f,  len, 0.0f);

			glColor3f(1.0F, 1.0F, 0.0F);//画“Y”
			glVertex3f( 0.0f,	len,		0.0f);
			glVertex3f( 0.0f,	len+0.5f,	0.0f);
			glVertex3f( 0.0f,	len+0.5f,	0.0f);
			glVertex3f( 0.5f,	len+1.0f,	0.0f);
			glVertex3f( 0.0f,	len+0.5f,	0.0f);
			glVertex3f( -0.5f,	len+1.0f,	0.0f);

			// z轴		（蓝色）
			glColor3f(0.0F, 0.0F, 1.0F);
			glVertex3f( 0.0f, 0.0f, -len);
			glVertex3f( 0.0f, 0.0f,  len);
			glVertex3f(-0.5f, 0.0f,  len-0.5f);
			glVertex3f( 0.0f, 0.0f,  len);
			glVertex3f( 0.5f, 0.0f,  len-0.5f);
			glVertex3f( 0.0f, 0.0f,  len);

			glColor3f(1.0F, 1.0F, 0.0F);//画“Z”
			glVertex3f( 0.0f,	0.0f,		len);
			glVertex3f( 1.0f,	0.0f,		len);
			glVertex3f( 0.0f,	0.0f,		len);
			glVertex3f( 1.0f,	1.0f,		len);
			glVertex3f( 1.0f,	1.0f,		len);
			glVertex3f( 0.0f,	1.0f,		len);

			///////////////////////////////////////
			//画网格
			glColor3f(1.0F, 0.0F, 1.0F);
			for(int i=0;i<=10;i++)
			{
				glVertex3f( -len,	0.0f,	len/5.0*i-len);
				glVertex3f( len,	0.0f,	len/5.0*i-len);

				glVertex3f( len/5.0*i-len,	0.0f,	-len);
				glVertex3f( len/5.0*i-len,	0.0f,	len);
			}


	glEnd();
}

//////////////////////////////////////////////////////////
//	显示函数，显示所有的东西
//////////////////////////////////////////////////////////
void CPrtSysView::Draw3ds()
{
	if(!demoPlay)
	{
		if (m_3dsLoaded) //3DS模型的显示,当不演示又读进来时才显示
		{

			m_3dsobj.Draw(m_3dsDD.IsLine
				,m_3dsDD.scale
				,m_3dsDD.hide
				,m_3dsDD.t_data
				,m_3dsDD.r_data);
		}
		m_particleControl.draw(m_camera,m_ptrCtrl);	//显示粒子，引用当前摄像机
	}
	else
	{
		bool t=true;
		m_Demo_Bit->draw(m_camera,t,DemoAutoFinish);
	}
	if((demoPlay)&&(DemoAutoFinish))//如果演示自动结束
	{
		delete m_Demo_Bit;//删除演示类
		demoPlay=false;		
		ShowCursor(TRUE);		//打开鼠标指针
		cross=true;				//打开坐标轴
		glClearColor(0,0,0,0.5);		//清除屏幕
		ExitFullScreen();//退出全屏
		ScreenData.IsFullScreen=false;
	}


}

/////////////////////////////////////////////////////////
//							初始化显示设置
/////////////////////////////////////////////////////////
void CPrtSysView::Init(GLvoid)
{

	m_3dsLoaded  = FALSE;
	m_camera.InitCamera();//初始化摄像机

	mouseprevpoint.x = 0;
	mouseprevpoint.y = 0;
	mouserightdown = FALSE;
	mouseleftdown = FALSE;
	m_particleControl.GetInitInfo();	//例子系统初始化开始
	
}

/////////////////////////////////////////////////////////////
//全屏显示
////////////////////////////////////////////////////////////
void CPrtSysView::OnFullscreen() 
{
	// TODO: Add your command handler code here
	DEVMODE dmSettings;								

	memset(&dmSettings,0,sizeof(dmSettings));		

	if(!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dmSettings))
	{
		MessageBox(/*NULL, */"Could Not Enum Display Settings", "Error", MB_OK);
		return;
	}
			CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;

			dmSettings.dmPelsWidth	= ScreenData.Width;//640;//SCREEN_WIDTH;		
			dmSettings.dmPelsHeight	= ScreenData.Height;//480;//SCREEN_HEIGHT;	
				//获取当前屏幕的分辨率 
				pFrame->OLDWidth=GetSystemMetrics(SM_CXSCREEN); 
				pFrame->OLDHeight=GetSystemMetrics(SM_CYSCREEN); 

			int result = ChangeDisplaySettings(&dmSettings,CDS_FULLSCREEN);	

			if(result != DISP_CHANGE_SUCCESSFUL)
			{

				MessageBox(/*NULL, */ "Display Mode Not Compatible", "Error", MB_OK);
				PostQuitMessage(0);
			}
				// 全屏显示
		pFrame->FullScreen();
	/////////////////////////////////////////////////////////////////
	//添加窗口缩放时的图形变换函数
	glViewport(0,0,dmSettings.dmPelsWidth,dmSettings.dmPelsHeight);
	/////////////////////////////////////////////////////////////////
	GLdouble aspect_ratio;
	aspect_ratio = (GLdouble)dmSettings.dmPelsWidth/(GLdouble)dmSettings.dmPelsHeight;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0F, aspect_ratio, 1.0F, 10000.0F);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	ScreenData.IsFullScreen=true;
}

/////////////////////////////////////////////////////////////////////////
//			退出全屏
/////////////////////////////////////////////////////////////////////////
void CPrtSysView::ExitFullScreen()
{
		DEVMODE dmSettings;								

	memset(&dmSettings,0,sizeof(dmSettings));		

	if(!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dmSettings))
	{
		MessageBox(/*NULL, */"Could Not Enum Display Settings", "Error", MB_OK);
		return;
	}
	CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;

	dmSettings.dmPelsWidth	= pFrame->OLDWidth;//SCREEN_WIDTH;		
	dmSettings.dmPelsHeight	= pFrame->OLDHeight;//SCREEN_HEIGHT;
	
	int result = ChangeDisplaySettings(&dmSettings,CDS_FULLSCREEN);	

	if(result != DISP_CHANGE_SUCCESSFUL)
	{
		MessageBox(/*NULL, */ "Display Mode Not Compatible", "Error", MB_OK);
		PostQuitMessage(0);
	}

	// 恢复窗口显示
	pFrame->ShowWithWindows();
}



/////////////////////////////////////////////////////////////////////////
//			按ESC退出全屏
/////////////////////////////////////////////////////////////////////////
void CPrtSysView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_ESCAPE)
	{
		if(ScreenData.IsFullScreen)
		{ExitFullScreen();//退出全屏
		ScreenData.IsFullScreen=false;
		}
		if(demoPlay)
		{
		delete m_Demo_Bit;//删除演示类
		demoPlay=false;
		ShowCursor(TRUE);			//打开鼠标指针
		cross=true;					//打开坐标轴
		DemoAutoFinish=false;
		m_camera.InitCamera();
		glClearColor(0,0,0,0.5);		//清除屏幕
		}
	}

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}



//////////////////////////////////////////////////////////////////////////////
//				鼠标控制系列
//////////////////////////////////////////////////////////////////////////////
void CPrtSysView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetCapture( );
	SetCursor(AfxGetApp()->LoadCursor(IDC_ROT));
	mouseleftdown = TRUE;
	mouseprevpoint.x = point.x;
	mouseprevpoint.y = point.y;	
	CView::OnLButtonDown(nFlags, point);
	CView::OnLButtonDown(nFlags, point);
}

void CPrtSysView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture( );
	mouseleftdown = FALSE;
	CView::OnLButtonUp(nFlags, point);
}

//双击视角回原
void CPrtSysView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_camera.InitCamera();//初始化摄像机
	RenderScene();
	CView::OnLButtonDblClk(nFlags, point);
}

void CPrtSysView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetCapture( );
	SetCursor(AfxGetApp()->LoadCursor(IDC_MAGNIFI));
	mouserightdown = TRUE;
	mouseprevpoint.x = point.x;
	mouseprevpoint.y = point.y;
	CView::OnRButtonDown(nFlags, point);
}

void CPrtSysView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture( );
	mouserightdown = FALSE;
	CView::OnRButtonUp(nFlags, point);
}

//鼠标滚轮控制拉伸镜头（其实和右键一样）
BOOL CPrtSysView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	SetCursor(AfxGetApp()->LoadCursor(IDC_WHEEL));
	m_camera.SetEyeRadius(zDelta/5.0);
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


//鼠标移动控制函数
void CPrtSysView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//如果左键按下了，应该进行镜头旋转操作
	if(mouseleftdown)
	{

		m_camera.SetEyePos(0, (point.x - mouseprevpoint.x));
		m_camera.SetEyePos(1, (point.y - mouseprevpoint.y)); 
	}
	else
	//如果右键按下了，应该进行镜头的拉伸操作（对准target伸缩）
		if(mouserightdown)		
		{	
		//拉远拉近
		m_camera.SetEyeRadius(point.y - mouseprevpoint.y);
		}
	CView::OnMouseMove(nFlags, point);

	mouseprevpoint.x = point.x;
	mouseprevpoint.y = point.y;	
}


///////////////////////////////////////////////////////////////////////////
//					键盘控制函数
///////////////////////////////////////////////////////////////////////////
void CPrtSysView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	m_camera.SliderCamera(nChar);//移动摄像机。

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

//切换场景的面、线显示。
void CPrtSysView::OnFaceorline() 
{
	// TODO: Add your command handler code here
	m_3dsDD.IsLine=!m_3dsDD.IsLine;
	
}
//隐藏或者显示3ds场景
void CPrtSysView::OnHideorshow() 
{
	// TODO: Add your command handler code here
	m_3dsDD.hide=!m_3dsDD.hide;
	if(DSdlgExist)
		m_3dsCtrlDlg.ShowWindow(SW_HIDE); 
}

//打开3ds场景属性修改器
void CPrtSysView::OnShow3dsdlg() 
{
	// TODO: Add your command handler code here
	if((m_3dsLoaded)&&(!m_3dsDD.hide))
	{
		if(DSdlgExist==false)//如果没有生成过则生成
		{
			m_3dsCtrlDlg.Create(IDD_3dsCtrlDlg,NULL);
			DSdlgExist=true;
		}
	m_3dsCtrlDlg.ShowWindow(SW_SHOW); 
	m_3dsCtrlDlg.GetExtData(m_3dsDD);	//获得外界数据
	
	}
	else AfxMessageBox("注意：现在没有3DS物体!");
}

//播放或者停止粒子动画。
void CPrtSysView::OnPlay() 
{
	// TODO: Add your command handler code here
	m_ptrCtrl.IsPlay=!m_ptrCtrl.IsPlay;
}


//打开全屏设置显示对话框，修改显示参数。
void CPrtSysView::OnSetfullscreen() 
{
	// TODO: Add your command handler code here
		m_screen.ShowWindow(SW_SHOW);	//显示
		switch(m_screen.GetScreenset())
		{
		case 3:
			ScreenData.Width=640;
			ScreenData.Height=480;
			break;
		case 2:
			ScreenData.Width=800;
			ScreenData.Height=600;
			break;
		case 1:
			ScreenData.Width=1024;
			ScreenData.Height=768;
		default:break;
		}
}

////////////////////////////////////////////////////////////////////////////
//显示火焰粒子效果
void CPrtSysView::OnShowFire() 
{
	// TODO: Add your command handler code here
	m_ptrCtrl.p_fire=!m_ptrCtrl.p_fire;

	if(!m_ptrCtrl.p_fire)
	{
		m_firedlg.ShowWindow(SW_HIDE);	//火焰的编辑对话框隐藏则随之的颜色对话框也隐藏。
		if(m_firedlg.colorEixst)m_firedlg.m_FireColor.ShowWindow(SW_HIDE);
	}
}

//更新显示火焰菜单前的小对勾
void CPrtSysView::OnUpdateShowFire(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_ptrCtrl.p_fire)
	{
      pCmdUI->SetCheck();
	}else
	{
      pCmdUI->SetCheck(0);
	}
}

//允许打开编辑对话框
void CPrtSysView::OnEditFire() 
{
	// TODO: Add your command handler code here
	if(m_ptrCtrl.p_fire)
	{
		m_firedlg.GetExtFireData(m_particleControl.m_prtFire.PDD);	//获得外界数据
		m_firedlg.ShowWindow(SW_SHOW);	//显示
	}
}
//禁止打开时将对话框变灰
void CPrtSysView::OnUpdateEditFire(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_ptrCtrl.p_fire);
}

/////////////////////////////////////////////////////////////////////
//瀑布效果的菜单编辑
void CPrtSysView::OnShowWaterfall() 
{
	// TODO: Add your command handler code here
	m_ptrCtrl.p_waterfall=!m_ptrCtrl.p_waterfall;

	if(!m_ptrCtrl.p_waterfall)
	{
		m_waterfalldlg.ShowWindow(SW_HIDE);	//编辑对话框隐藏则随之的颜色对话框也隐藏。
		if(m_waterfalldlg.colorEixst)m_waterfalldlg.m_ColorDLG.ShowWindow(SW_HIDE);
	}
}

void CPrtSysView::OnUpdateShowWaterfall(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_ptrCtrl.p_waterfall)
	{
      pCmdUI->SetCheck();
	}else
	{
      pCmdUI->SetCheck(0);
	}
}

void CPrtSysView::OnEditWaterfall() 
{
	// TODO: Add your command handler code here
	if(m_ptrCtrl.p_waterfall)
	{
		m_waterfalldlg.GetExtData(m_particleControl.m_prtWaterFall.PDD);	//获得外界数据
		m_waterfalldlg.ShowWindow(SW_SHOW);	//显示
	}
}

void CPrtSysView::OnUpdateEditWaterfall(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_ptrCtrl.p_waterfall);
}

/////////////////////////////////////////////////////
//礼花效果的菜单
void CPrtSysView::OnShowFirework() 
{
	// TODO: Add your command handler code here
		m_ptrCtrl.p_firework=!m_ptrCtrl.p_firework;

	if(!m_ptrCtrl.p_firework)
	{
		m_fireworkdlg.ShowWindow(SW_HIDE);	//编辑对话框隐藏则随之的颜色对话框也隐藏。
		if(m_fireworkdlg.colorEixst)m_fireworkdlg.m_ColorDLG.ShowWindow(SW_HIDE);
	}
	
}

void CPrtSysView::OnUpdateShowFirework(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_ptrCtrl.p_firework)
	{
      pCmdUI->SetCheck();
	}else
	{
      pCmdUI->SetCheck(0);
	}
}


void CPrtSysView::OnEditFirework() 
{
	// TODO: Add your command handler code here
	if(m_ptrCtrl.p_firework)
	{
		m_fireworkdlg.GetExtData(m_particleControl.m_prtFireWork.PDD);	//获得外界数据
		m_fireworkdlg.ShowWindow(SW_SHOW);	//显示
	}
}

void CPrtSysView::OnUpdateEditFirework(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_ptrCtrl.p_firework);
}


///////////////////////////////////////////////////////////
//原子弹的菜单
void CPrtSysView::OnShowAtomicbomb() 
{
	
	// TODO: Add your command handler code here
	m_ptrCtrl.p_atomicbomb=!m_ptrCtrl.p_atomicbomb;
	
	if(!m_ptrCtrl.p_atomicbomb)
	{
		m_atomicbombdlg.ShowWindow(SW_HIDE);	//编辑对话框隐藏则随之的颜色对话框也隐藏。
	}
	
}

void CPrtSysView::OnUpdateShowAtomicbomb(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_ptrCtrl.p_atomicbomb)
	{
      pCmdUI->SetCheck();
	}else
	{
      pCmdUI->SetCheck(0);
	}
}

void CPrtSysView::OnEditAtomicbomb() 
{
	// TODO: Add your command handler code here
	
	if(m_ptrCtrl.p_atomicbomb)
	{
		m_atomicbombdlg.GetExtData(m_particleControl.m_prtAtomicBomb.PDD);	//获得外界数据
		m_atomicbombdlg.ShowWindow(SW_SHOW);	//显示
	}

}

void CPrtSysView::OnUpdateEditAtomicbomb(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_ptrCtrl.p_atomicbomb);
}


//////////////////////////////////////////////////////////
//雪花控制菜单
void CPrtSysView::OnShowSnow() 
{
	// TODO: Add your command handler code here
	m_ptrCtrl.p_snow=!m_ptrCtrl.p_snow;

	if(!m_ptrCtrl.p_snow)
	{
		m_snowdlg.ShowWindow(SW_HIDE);	//编辑对话框隐藏则随之的颜色对话框也隐藏。
	}

}

void CPrtSysView::OnUpdateShowSnow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_ptrCtrl.p_snow);
}

void CPrtSysView::OnEditSnow() 
{
	// TODO: Add your command handler code here

	if(m_ptrCtrl.p_snow)
	{
		m_snowdlg.GetExtData(m_particleControl.m_prtSnow.PDD);	//获得外界数据
		m_snowdlg.ShowWindow(SW_SHOW);	//显示
	}

}

void CPrtSysView::OnUpdateEditSnow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_ptrCtrl.p_snow);
}

//////////////////////////////////////////////////////////////
//控制坐标轴的显示
void CPrtSysView::OnKwCross() 
{
	// TODO: Add your command handler code here
	cross=!cross;
}

//演示动画
void CPrtSysView::OnKwDemo() 
{
	// TODO: Add your command handler code here
	DemoAutoFinish=false;
	CPrt_Demo_BIT *db=new CPrt_Demo_BIT;//生成演示类
	m_Demo_Bit=db;
	m_Demo_Bit->GetInitInfo();			//对演示进行初始化。
	ShowCursor(FALSE);					//隐藏鼠标指针
	cross=false;
	demoPlay=true;
	OnFullscreen();					//全屏显示
	ScreenData.IsFullScreen=true;
}

//我的打开文件对话框，拒绝系统自带
void CPrtSysView::OnKwFileopen() 
{

	// TODO: Add your command handler code here
	char szCurDir[256];//保存当前目录
	GetCurrentDirectory(sizeof(szCurDir),szCurDir);

	CString filename;
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,"3DS(*.3ds)|*.3ds||",NULL);

	if(dlg.DoModal()==IDOK)//显示文件对话框
	{
		filename = dlg.GetPathName();//取得文件名
		m_3dsLoaded=m_3dsobj.LoadFile(filename);
	}
	SetCurrentDirectory(szCurDir);//恢复当前路径！

}

void CPrtSysView::OnEditBmp() 
{
	// TODO: Add your command handler code here
	if(m_ptrCtrl.p_bmp)
	{
		m_bmpdlg.GetExtData(m_particleControl.m_prtBMP.PDD,&m_particleControl.m_prtBMP.BMP_FileName);	//获得外界数据
		m_bmpdlg.ShowWindow(SW_SHOW);	//显示
	}
}

void CPrtSysView::OnShowBmp() 
{
	// TODO: Add your command handler code here
	m_ptrCtrl.p_bmp=!m_ptrCtrl.p_bmp;

	if(!m_ptrCtrl.p_bmp)
	{
		m_bmpdlg.ShowWindow(SW_HIDE);	//火焰的编辑对话框隐藏则随之的颜色对话框也隐藏。
	}

}

void CPrtSysView::OnUpdateEditBmp(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_ptrCtrl.p_bmp);
}

void CPrtSysView::OnUpdateShowBmp(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_ptrCtrl.p_bmp)
	{
      pCmdUI->SetCheck();
	}else
	{
      pCmdUI->SetCheck(0);
	}
}
