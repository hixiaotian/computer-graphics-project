#if !defined(AFX_3DSDLG_H__4BF687DE_0B42_427E_8391_14295503761E__INCLUDED_)
#define AFX_3DSDLG_H__4BF687DE_0B42_427E_8391_14295503761E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 3dsDlg.h : header file
//

#include "glo.h"	// 全局结构和函数的定义

/////////////////////////////////////////////////////////////////////////////
// C3dsDlg dialog

class C3dsDlg : public CDialog
{
// Construction
public:
	C3dsDlg(CWnd* pParent = NULL);			// standard constructor
	C3dsDlg::~C3dsDlg(void);
	DlgData_3ds* m_3dsDlgdata;				//定义一个接受对话数据的结构指针
	//bool exist;							//是否被显示
	void GetExtData(DlgData_3ds &m_3dsD);	//获得外界数据

// Dialog Data
	//{{AFX_DATA(C3dsDlg)
	enum { IDD = IDD_3dsCtrlDlg };
	float	m_pos_x;
	float	m_pos_y;
	float	m_pos_z;
	float	m_rot_x;
	float	m_rot_y;
	float	m_rot_z;
	float	m_scale;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C3dsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(C3dsDlg)
	afx_msg void OnDestroy();
	afx_msg void OnKillfocusPosX();
	afx_msg void OnKillfocusPosY();
	afx_msg void OnKillfocusPosZ();
	afx_msg void OnKillfocusRotX();
	afx_msg void OnKillfocusRotY();
	afx_msg void OnKillfocusRotZ();
	afx_msg void OnKillfocusScale();
	afx_msg void OnKillfocusEmmc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DSDLG_H__4BF687DE_0B42_427E_8391_14295503761E__INCLUDED_)
