#if !defined(AFX_COLOREDIT_H__6BC9C571_AA1E_4491_8418_01794FA0516B__INCLUDED_)
#define AFX_COLOREDIT_H__6BC9C571_AA1E_4491_8418_01794FA0516B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorEdit.h : header file
//
#include "glo.h"	//引用全局结构

/////////////////////////////////////////////////////////////////////////////
// CColorEdit dialog

class CColorEdit : public CDialog
{
// Construction
public:
	CColorEdit(CWnd* pParent = NULL);   // standard constructor
	Dlg_COLOR_EDIT* m_color;			// 颜色指针，引用外界颜色
	void GetExtColorData(Dlg_COLOR_EDIT &Ex_color);	//获得外界数据

// Dialog Data
	//{{AFX_DATA(CColorEdit)
	enum { IDD = IDD_DLG_COLOR };
	float	m_B1;
	float	m_B2;
	float	m_B3;
	float	m_B4;
	float	m_B5;
	float	m_G1;
	float	m_G2;
	float	m_G3;
	float	m_G4;
	float	m_G5;
	float	m_R1;
	float	m_R2;
	float	m_R3;
	float	m_R4;
	float	m_R5;
	CString	m_E_Title;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CColorEdit)
	afx_msg void OnKillfocusB1();
	afx_msg void OnKillfocusB2();
	afx_msg void OnKillfocusB3();
	afx_msg void OnKillfocusB4();
	afx_msg void OnKillfocusB5();
	afx_msg void OnKillfocusG1();
	afx_msg void OnKillfocusG2();
	afx_msg void OnKillfocusG3();
	afx_msg void OnKillfocusG4();
	afx_msg void OnKillfocusG5();
	afx_msg void OnKillfocusR1();
	afx_msg void OnKillfocusR2();
	afx_msg void OnKillfocusR3();
	afx_msg void OnKillfocusR4();
	afx_msg void OnKillfocusR5();
	afx_msg void OnBe1();
	afx_msg void OnBe2();
	afx_msg void OnBe3();
	afx_msg void OnBe4();
	afx_msg void OnBe5();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOREDIT_H__6BC9C571_AA1E_4491_8418_01794FA0516B__INCLUDED_)
