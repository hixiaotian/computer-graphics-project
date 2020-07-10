#if !defined(AFX_DLG_ATOMICBOMB_H__517E1B03_9767_479A_AD03_0E614D411F2F__INCLUDED_)
#define AFX_DLG_ATOMICBOMB_H__517E1B03_9767_479A_AD03_0E614D411F2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLG_AtomicBomb.h : header file
//
#include "glo.h"	//引用全局结构定义
#include "coloredit.h"	//引用颜色对话框

/////////////////////////////////////////////////////////////////////////////
// CDLG_AtomicBomb dialog

class CDLG_AtomicBomb : public CDialog
{
// Construction
public:
	CDLG_AtomicBomb(CWnd* pParent = NULL);   // standard constructor
	~CDLG_AtomicBomb(void);

	DlgData_PRT*	m_DLGData;								//建立一个对话框数据结构的指针
	void			GetExtData(DlgData_PRT &Ex_DLGData);	//获得外界数据

// Dialog Data
	//{{AFX_DATA(CDLG_AtomicBomb)
	enum { IDD = IDD_DLG_AtomicBomb };
	CSliderCtrl	m_S_maxnum;
	int		m_E_maxnum;
	float	m_E_posx;
	float	m_E_posy;
	float	m_E_posz;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLG_AtomicBomb)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLG_AtomicBomb)
	afx_msg void OnKillfocusEaMaxnum();
	afx_msg void OnKillfocusEaPosX();
	afx_msg void OnKillfocusEaPosY();
	afx_msg void OnKillfocusEaPosZ();
	afx_msg void OnReleasedcaptureSaMaxnum(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_ATOMICBOMB_H__517E1B03_9767_479A_AD03_0E614D411F2F__INCLUDED_)
