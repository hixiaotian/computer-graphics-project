// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__53086E03_DAAB_44B9_88B2_D2D6746D76CF__INCLUDED_)
#define AFX_MAINFRM_H__53086E03_DAAB_44B9_88B2_D2D6746D76CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CFrameWnd
{
 private:
	WINDOWPLACEMENT m_OldWndPlacement; // 原窗口位置
	BOOL m_bFullScreen;                // 全屏显示标志
	CRect m_FullScreenRect;            // 全屏显示时的窗口位置


protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ShowWithWindows();
	void FullScreen();
	virtual ~CMainFrame();
	// 原屏幕的分辨率 
	int OLDWidth;//=GetSystemMetrics(SM_CXSCREEN); 
	int OLDHeight;//=GetSystemMetrics(SM_CYSCREEN); 
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnShowFire();
	afx_msg void OnEditAtomicbomb();
	afx_msg void OnUpdateEditAtomicbomb(CCmdUI* pCmdUI);
	afx_msg void On1111();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__53086E03_DAAB_44B9_88B2_D2D6746D76CF__INCLUDED_)
