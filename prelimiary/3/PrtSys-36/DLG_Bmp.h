#if !defined(AFX_DLG_BMP_H__13BA5079_3CBC_4045_8124_4EDC6F5B959D__INCLUDED_)
#define AFX_DLG_BMP_H__13BA5079_3CBC_4045_8124_4EDC6F5B959D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLG_Bmp.h : header file
//
#include "glo.h"	//引用全局结构定义
/////////////////////////////////////////////////////////////////////////////
// CDLG_Bmp dialog

class CDLG_Bmp : public CDialog
{
// Construction
public:
	CDLG_Bmp(CWnd* pParent = NULL);   // standard constructor
	DlgData_PRT*	m_DLGData;								//建立一个对话框数据结构的指针
	void			GetExtData(DlgData_PRT &Ex_DLGData,char (*tFName)[256]);	//获得外界数据
	void			UpdateDLGDate(void);//更新界面数据
	char			(*FileName)[256];	//外界文件名的引用
// Dialog Data
	//{{AFX_DATA(CDLG_Bmp)
	enum { IDD = IDD_DLG_BMP };
	CSliderCtrl	m_S_thick;
	CSliderCtrl	m_S_TW;
	CSliderCtrl	m_S_slowdown;
	CSliderCtrl	m_S_maxnum;
	CSliderCtrl	m_S_lifefate;
	CSliderCtrl	m_S_getz;
	CSliderCtrl	m_S_gety;
	CSliderCtrl	m_S_getx;
	CString	m_E_file;
	float	m_E_getx;
	float	m_E_gety;
	float	m_E_getz;
	float	m_E_lifefate;
	int		m_E_maxnum;
	float	m_E_posx;
	float	m_E_posy;
	float	m_E_posz;
	float	m_E_slowdown;
	float	m_E_TW;
	float	m_E_thick;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLG_Bmp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLG_Bmp)
	afx_msg void OnBbFile();
	afx_msg void OnBbLoad();
	afx_msg void OnBbSave();
	afx_msg void OnKillfocusEbGetx();
	afx_msg void OnKillfocusEbGety();
	afx_msg void OnKillfocusEbGetz();
	afx_msg void OnKillfocusEbLifefate();
	afx_msg void OnKillfocusEbMaxnum();
	afx_msg void OnKillfocusEbPosX();
	afx_msg void OnKillfocusEbPosY();
	afx_msg void OnKillfocusEbPosZ();
	afx_msg void OnKillfocusEbSlowdown();
	afx_msg void OnKillfocusEBTextWidth();
	afx_msg void OnKillfocusEBThick();
	afx_msg void OnReleasedcaptureSbGetx(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSbGety(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSbGetz(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSbLifefate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSbMaxnum(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSbSlowdown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSBTextWitdh(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSBThick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_BMP_H__13BA5079_3CBC_4045_8124_4EDC6F5B959D__INCLUDED_)
