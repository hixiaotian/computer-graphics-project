#if !defined(AFX_DLG_WATERFALL_H__376D4B87_F57F_4EF3_8B5C_9E15E425DC83__INCLUDED_)
#define AFX_DLG_WATERFALL_H__376D4B87_F57F_4EF3_8B5C_9E15E425DC83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLG_WaterFall.h : header file
//
#include "glo.h"	//引用全局结构定义
#include "coloredit.h"	//引用颜色对话框

/////////////////////////////////////////////////////////////////////////////
// CDLG_WaterFall dialog

class CDLG_WaterFall : public CDialog
{
// Construction
public:
	CDLG_WaterFall(CWnd* pParent = NULL);   // standard constructor
	~CDLG_WaterFall();
	//从瀑布对话框开始，内部的变量和函数采用统一命名，这样可以加快编程速度
	DlgData_PRT*	m_DLGData;								//建立一个对话框数据结构的指针
	CColorEdit		m_ColorDLG;								//颜色对话框
	bool			colorEixst;								//是否生成了颜色对话框，如果生成过，则禁止重复成生
	void			GetExtData(DlgData_PRT &Ex_DLGData);	//获得外界数据
	void			UpdateDLGDate(void);					//更新界面数据
// Dialog Data
	//{{AFX_DATA(CDLG_WaterFall)
	enum { IDD = IDD_DLG_WaterFall };
	CSliderCtrl	m_S_TW;
	CSliderCtrl	m_S_speedmin;
	CSliderCtrl	m_S_speedmax;
	CSliderCtrl	m_S_slowdown;
	CSliderCtrl	m_S_maxnum;
	CSliderCtrl	m_S_lifefate;
	CSliderCtrl	m_S_getz;
	CSliderCtrl	m_S_gety;
	CSliderCtrl	m_S_getx;
	CSliderCtrl	m_S_yend;
	CSliderCtrl	m_S_xzstar;
	CSliderCtrl	m_S_xzend;
	float	m_E_xzend;
	float	m_E_xzstar;
	float	m_E_yend;
	float	m_E_getx;
	float	m_E_gety;
	float	m_E_getz;
	float	m_E_lifefate;
	int		m_E_maxnum;
	float	m_E_posy;
	float	m_E_posz;
	float	m_E_slowdown;
	float	m_E_speedmax;
	float	m_E_speedmin;
	float	m_E_posx;
	float	m_E_TW;
	int		m_RW1;
	int		m_RW2;
	int		m_RW3;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLG_WaterFall)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLG_WaterFall)
	afx_msg void OnKillfocusEwAlxzend();
	afx_msg void OnKillfocusEwAlxzstar();
	afx_msg void OnKillfocusEwAlyend();
	afx_msg void OnKillfocusEwGetx();
	afx_msg void OnKillfocusEwGety();
	afx_msg void OnKillfocusEwGetz();
	afx_msg void OnKillfocusEwLifefate();
	afx_msg void OnKillfocusEwMaxnum();
	afx_msg void OnKillfocusEwPosX();
	afx_msg void OnKillfocusEwPosY();
	afx_msg void OnKillfocusEwPosZ();
	afx_msg void OnKillfocusEwSlowdown();
	afx_msg void OnKillfocusEwSpeedmax();
	afx_msg void OnKillfocusEwSpeedmin();
	afx_msg void OnReleasedcaptureSwAlxzend(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSwAlxzstar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSwAlyend(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSwGetx(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSwGety(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSwGetz(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSwLifefate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSwMaxnum(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSwSlowdown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSwSpeedmax(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSwSpeedmin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	afx_msg void OnKillfocusEwPpx();
	afx_msg void OnBwColor();
	afx_msg void OnKillfocusEWTextWidth();
	afx_msg void OnReleasedcaptureSWTextWidth(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRw1();
	afx_msg void OnRw2();
	afx_msg void OnRw3();
	afx_msg void OnBwLoad();
	afx_msg void OnBwSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_WATERFALL_H__376D4B87_F57F_4EF3_8B5C_9E15E425DC83__INCLUDED_)
