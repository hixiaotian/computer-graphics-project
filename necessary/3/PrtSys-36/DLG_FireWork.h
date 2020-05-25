#if !defined(AFX_DLG_FIREWORK_H__D698F265_C40E_4F39_A38E_C72F7D61B91A__INCLUDED_)
#define AFX_DLG_FIREWORK_H__D698F265_C40E_4F39_A38E_C72F7D61B91A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLG_FireWork.h : header file
//
#include "glo.h"	//引用全局结构定义
#include "coloredit.h"	//引用颜色对话框

/////////////////////////////////////////////////////////////////////////////
// CDLG_FireWork dialog

class CDLG_FireWork : public CDialog
{
// Construction
public:
	CDLG_FireWork(CWnd* pParent = NULL);   // standard constructor
	~CDLG_FireWork(void);
	DlgData_PRT*	m_DLGData;								//建立一个对话框数据结构的指针
	CColorEdit		m_ColorDLG;								//颜色对话框
	bool			colorEixst;								//是否生成了颜色对话框，如果生成过，则禁止重复成生
	void			GetExtData(DlgData_PRT &Ex_DLGData);	//获得外界数据
	void			UpdateDLGDate(void);//更新界面数据
// Dialog Data
	//{{AFX_DATA(CDLG_FireWork)
	enum { IDD = IDD_DLG_FireWork };
	CSliderCtrl	m_SS_speedmin;
	CSliderCtrl	m_SS_speedmax;
	CSliderCtrl	m_SS_slowdown;
	CSliderCtrl	m_SS_maxnum;
	CSliderCtrl	m_SS_lifefate;
	CSliderCtrl	m_SS_getz;
	CSliderCtrl	m_SS_gety;
	CSliderCtrl	m_SS_getx;
	CSliderCtrl	m_S_TW;
	CSliderCtrl	m_S_speedmin;
	CSliderCtrl	m_S_speedmax;
	CSliderCtrl	m_S_slowdown;
	CSliderCtrl	m_S_maxnum;
	CSliderCtrl	m_S_lifefate;
	CSliderCtrl	m_S_getz;
	CSliderCtrl	m_S_gety;
	CSliderCtrl	m_S_getx;
	CSliderCtrl	m_S_ystar;
	CSliderCtrl	m_S_yend;
	CSliderCtrl	m_S_xzstar;
	CSliderCtrl	m_S_xzend;
	float	m_E_xzend;
	float	m_E_yend;
	float	m_E_xzstar;
	float	m_E_ystar;
	float	m_E_getx;
	float	m_E_gety;
	float	m_E_getz;
	float	m_E_lifefate;
	int		m_E_maxnum;
	float	m_E_posx;
	float	m_E_posy;
	float	m_E_posz;
	float	m_E_slowdown;
	float	m_E_speedmax;
	float	m_E_speedmin;
	float	m_E_TW;
	float	m_ES_getx;
	float	m_ES_gety;
	float	m_ES_getz;
	float	m_ES_lifefate;
	int		m_ES_maxnum;
	float	m_ES_slowdown;
	float	m_ES_speedmax;
	float	m_ES_speedmin;
	int		m_RI1;
	int		m_RI2;
	int		m_RI3;
	int		m_RI4;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLG_FireWork)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLG_FireWork)
	afx_msg void OnBiColor();
	afx_msg void OnKillfocusEiAlxzend();
	afx_msg void OnKillfocusEiAlxzstar();
	afx_msg void OnKillfocusEiAlyend();
	afx_msg void OnKillfocusEiAlystar();
	afx_msg void OnKillfocusEiGetx();
	afx_msg void OnKillfocusEiGety();
	afx_msg void OnKillfocusEiGetz();
	afx_msg void OnKillfocusEiLifefate();
	afx_msg void OnKillfocusEiMaxnum();
	afx_msg void OnKillfocusEiPosX();
	afx_msg void OnKillfocusEiPosY();
	afx_msg void OnKillfocusEiPosZ();
	afx_msg void OnKillfocusEiSlowdown();
	afx_msg void OnKillfocusEiSpeedmax();
	afx_msg void OnKillfocusEiSpeedmin();
	afx_msg void OnKillfocusEITextWidth();
	afx_msg void OnKillfocusEisGetx();
	afx_msg void OnKillfocusEisGety();
	afx_msg void OnKillfocusEisGetz();
	afx_msg void OnKillfocusEisLifefate();
	afx_msg void OnKillfocusEisMaxnum();
	afx_msg void OnKillfocusEisSlowdown();
	afx_msg void OnKillfocusEisSpeedmax();
	afx_msg void OnKillfocusEisSpeedmin();
	afx_msg void OnRi1();
	afx_msg void OnRi2();
	afx_msg void OnRi3();
	afx_msg void OnRi4();
	afx_msg void OnReleasedcaptureSiAlxzend(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSiAlxzstar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSiAlyend(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSiAlystar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSiGetx(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSiGety(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSiGetz(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSiLifefate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSiMaxnum(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSiSlowdown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSiSpeedMin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSiSpeedmax(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSITextWidth(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSisGetx(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSisGety(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSisGetz(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSisLifefate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSisMaxnum(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSisSlowdown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSisSpeedmax(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSisSpeedmin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	afx_msg void OnBfwLoad();
	afx_msg void OnBfwSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_FIREWORK_H__D698F265_C40E_4F39_A38E_C72F7D61B91A__INCLUDED_)
