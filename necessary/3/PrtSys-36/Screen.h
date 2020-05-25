#if !defined(AFX_SCREEN_H__6CA26872_081A_4DE7_A872_B4759FAEA885__INCLUDED_)
#define AFX_SCREEN_H__6CA26872_081A_4DE7_A872_B4759FAEA885__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen dialog

class CScreen : public CDialog
{
// Construction
public:
	CScreen(CWnd* pParent = NULL);   // standard constructor
	bool IsOK;							// 是否按了OK键
	int GetScreenset(void);				 // 获得显示参数
// Dialog Data
	//{{AFX_DATA(CScreen)
	enum { IDD = IDD_SCREEN };
	int		m_R1024_768;
	int		m_R640_480;
	int		m_R800_600;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScreen)
	afx_msg void OnR17();
	afx_msg void OnR64();
	afx_msg void OnR86();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_H__6CA26872_081A_4DE7_A872_B4759FAEA885__INCLUDED_)
