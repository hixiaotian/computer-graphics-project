// MySolarsysView.h : interface of the CMySolarsysView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSDOPENGLVIEW_H__75C5AAEC_37B0_4A8B_9132_9A0C663F6DDC__INCLUDED_)
#define AFX_MYSDOPENGLVIEW_H__75C5AAEC_37B0_4A8B_9132_9A0C663F6DDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Textures.h"

#define MAX_NO_TEXTURES			9
#define PI						3.141592654
#define DEG2RAD(angle)			angle * (PI / 180)	// 将角度转换为弧度
#define ROTATION_SPEED			5
struct Planet
{
	float size, sections, posX, posY, posZ, SolarAngle, OwnAxisAngle;
	int TextureID;
} ;

struct GlobalPosition
{
	float fPosX,
		  fPosY;
	int iDegreesX,
		iDegreesY;
	bool bRotationOn;
} ;

class CMySolarsysView : public CView
{
protected: // create from serialization only
	CMySolarsysView();
	DECLARE_DYNCREATE(CMySolarsysView)

// Attributes
public:
	CMySolarsysDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySolarsysView)
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
	virtual ~CMySolarsysView();
/////////////////////////////////////////////////////////////////
//添加成员函数与成员变量
	BOOL RenderScene();
	BOOL SetupPixelFormat(void);
	void SetLogicalPalette(void);
	BOOL InitializeOpenGL(CDC* pDC);

	HGLRC		m_hRC;			//OpenGL绘制描述表
	HPALETTE	m_hPalette;		//OpenGL调色板
	CDC*	    m_pDC;			//OpenGL设备描述表
/////////////////////////////////////////////////////////////////
	void DrawSun(); 
	void DrawAllOrbits();
	void DrawOrbit(Planet planet);
	void DrawPlanets();
	void DrawPlanet(Planet planet);
	void DrawComet();
	void init(void);
	void InitPlanets();
	void rotate();

	GLuint texture_id[MAX_NO_TEXTURES];
	int iCometAngle, iCometPos ;
	CTextures m_Textures;
	Planet mercury, venus, earth, mars, jupiter, saturn, uranus, neptune, pluto;
	GlobalPosition globalPosition;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMySolarsysView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MySolarsysView.cpp
inline CMySolarsysDoc* CMySolarsysView::GetDocument()
   { return (CMySolarsysDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSDOPENGLVIEW_H__75C5AAEC_37B0_4A8B_9132_9A0C663F6DDC__INCLUDED_)
