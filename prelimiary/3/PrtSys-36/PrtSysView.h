// PrtSysView.h : interface of the CPrtSysView class
// 粒子系统控制总类
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRTSYSVIEW_H__9D19F39A_F765_4FAD_9FB7_73F33254A238__INCLUDED_)
#define AFX_PRTSYSVIEW_H__9D19F39A_F765_4FAD_9FB7_73F33254A238__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mainfrm.h"			// 用来控制全屏
#include "3dsObject.h"			// 3ds物体说明类
#include "cameye.h"				// 摄像机类文件
#include "3dsDlg.h"				// 3ds对话框类
#include "glo.h"				// 全局结构和函数的定义
#include "screen.h"				// 全屏显示控制对话框
#include "Particle.h"			// CParticle说明文件,引入粒子总控对象。
#include "DLG_Fire.h"			// 火焰对话框类
#include "DLG_WaterFall.h"		// 瀑布对话框类
#include "DLG_FireWork.h"		// 礼花对话框类
#include "DLG_AtomicBomb.h"		// 原子弹对话框类
#include "DLG_Snow.h"			// 雪花对话框类
#include "DLG_Bmp.h"			// 位图对话框类


class CPrtSysView : public CView
{
protected: // create from serialization only
	CPrtSysView();
	DECLARE_DYNCREATE(CPrtSysView)

// Attributes
public:
	CPrtSysDoc* GetDocument();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrtSysView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPrtSysView();
	//////////////////////////////////////////////////////////////////
	//添加成员函数与成员变量
	BOOL RenderScene();
	BOOL SetupPixelFormat(void);
	void SetLogicalPalette(void);
	BOOL InitializeOpenGL(CDC* pDC);

	HGLRC		m_hRC;			//OpenGL绘制描述表
	HPALETTE	m_hPalette;		//OpenGL调色板
	CDC*	    m_pDC;			//OpenGL设备描述表
	void Init(GLvoid);
	void Draw3ds();
	void DrawAxis();
	void ExitFullScreen();		//退出全屏

	//******************************************************
	//3ds物体控制
	C3dsObject	m_3dsobj;		//3DS物体对象
	BOOL		m_3dsLoaded;	//确认3DS物体读入。

	//******************************************************
	CCamEye				m_camera;			//摄像机对象
	CParticleControl	m_particleControl;	//粒子控制对象
	CPrt_Demo_BIT		*m_Demo_Bit;		//演示类指针

	BOOL		mouserightdown;			// 鼠标右键按下标志
	BOOL		mouseleftdown;			// 鼠标左键按下标志
	CPoint		mouseprevpoint; 
	//*****************************************************
	//显示控制变量,为真时显示、为假时不显示
	Dlg_PRT_Control	m_ptrCtrl;			//显示控制结构。


	//////////////////////////////////////////////////////////
	//对话框数据说明区域
	///////////////////////////////////////////////////////////
	void InitTotalDlgData(void);		// 初始化所有对话框数据
	C3dsDlg		m_3dsCtrlDlg;			// 3ds场景控制对话框
	bool		DSdlgExist;				// 3ds场景存在标志 
	DlgData_3ds	m_3dsDD;				// 3ds场景控制对话框数据结构
	//////////////////////
	//用来控制屏幕相关属性的对话框
	ScreenDataStruct	ScreenData;
	CScreen				m_screen;	//屏幕属性对话框
	bool				cross;		//坐标轴显示标志，true显示
	bool				demoPlay;	//演示播放的标志
	bool				DemoAutoFinish;//演示自动结束
	/////////////////////////
	CDLG_Fire			m_firedlg;				// 火焰对话框
	CDLG_WaterFall		m_waterfalldlg;			// 瀑布对话框
	CDLG_FireWork		m_fireworkdlg;			// 礼花对话框
	CDLG_AtomicBomb		m_atomicbombdlg;		// 原子弹对话框
	CDLG_Snow			m_snowdlg;				// 雪的对话框
	CDLG_Bmp			m_bmpdlg;				// 位图对话框



//////////////////////////////////////////////////////////////////
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPrtSysView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnFullscreen();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnFaceorline();
	afx_msg void OnHideorshow();
	afx_msg void OnShow3dsdlg();
	afx_msg void OnPlay();
	afx_msg void OnShowFire();
	afx_msg void OnUpdateShowFire(CCmdUI* pCmdUI);
	afx_msg void OnSetfullscreen();
	afx_msg void OnShowWaterfall();
	afx_msg void OnUpdateShowWaterfall(CCmdUI* pCmdUI);
	afx_msg void OnShowFirework();
	afx_msg void OnUpdateShowFirework(CCmdUI* pCmdUI);
	afx_msg void OnEditFire();
	afx_msg void OnUpdateEditFire(CCmdUI* pCmdUI);
	afx_msg void OnEditFirework();
	afx_msg void OnUpdateEditFirework(CCmdUI* pCmdUI);
	afx_msg void OnEditWaterfall();
	afx_msg void OnUpdateEditWaterfall(CCmdUI* pCmdUI);
	afx_msg void OnShowAtomicbomb();
	afx_msg void OnUpdateShowAtomicbomb(CCmdUI* pCmdUI);
	afx_msg void OnEditAtomicbomb();
	afx_msg void OnUpdateEditAtomicbomb(CCmdUI* pCmdUI);
	afx_msg void OnShowSnow();
	afx_msg void OnUpdateShowSnow(CCmdUI* pCmdUI);
	afx_msg void OnEditSnow();
	afx_msg void OnUpdateEditSnow(CCmdUI* pCmdUI);
	afx_msg void OnEditRain();
	afx_msg void OnUpdateEditRain(CCmdUI* pCmdUI);
	afx_msg void OnShowRain();
	afx_msg void OnUpdateShowRain(CCmdUI* pCmdUI);
	afx_msg void OnKwCross();
	afx_msg void OnKwDemo();
	afx_msg void OnKwFileopen();
	afx_msg void OnEditBmp();
	afx_msg void OnShowBmp();
	afx_msg void OnUpdateEditBmp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowBmp(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PrtSysView.cpp
inline CPrtSysDoc* CPrtSysView::GetDocument()
   { return (CPrtSysDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRTSYSVIEW_H__9D19F39A_F765_4FAD_9FB7_73F33254A238__INCLUDED_)
