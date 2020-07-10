#if !defined(AFX_DLG_SNOW_H__19F8F7A4_54BC_46B9_883C_899B2F537155__INCLUDED_)
#define AFX_DLG_SNOW_H__19F8F7A4_54BC_46B9_883C_899B2F537155__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLG_Snow.h : header file
//
#include "glo.h"	//引用全局结构定义
#include "coloredit.h"	//引用颜色对话框
/////////////////////////////////////////////////////////////////////////////
// CDLG_Snow dialog

class CDLG_Snow : public CDialog
{
// Construction
public:
	CDLG_Snow(CWnd* pParent = NULL);   // standard constructor
	~CDLG_Snow(void);
	DlgData_PRT*	m_DLGData;								//建立一个对话框数据结构的指针
	CColorEdit		m_ColorDLG;								//颜色对话框
	bool			colorEixst;								//是否生成了颜色对话框，如果生成过，则禁止重复成生
	void			GetExtData(DlgData_PRT &Ex_DLGData);	//获得外界数据
	void			UpdateDLGDate(void);//更新界面数据
// Dialog Data
	//{{AFX_DATA(CDLG_Snow)
	enum { IDD = IDD_DLG_Snow };
	CSliderCtrl	m_S_maxnum;
	CSliderCtrl	m_S_pram0;
	CSliderCtrl	m_S_slowdown;
	CSliderCtrl	m_S_speedmax;
	CSliderCtrl	m_S_speedmin;
	CSliderCtrl	m_S_TW;
	CSliderCtrl	m_S_lifefate;
	CSliderCtrl	m_S_getz;
	CSliderCtrl	m_S_gety;
	CSliderCtrl	m_S_getx;
	CSliderCtrl	m_S_ystar;
	CSliderCtrl	m_S_yend;
	CSliderCtrl	m_S_xzstar;
	CSliderCtrl	m_S_xzend;
	CSliderCtrl	m_S_alpha;
	float	m_E_alpha;
	float	m_E_xzend;
	float	m_E_xzstar;
	float	m_E_yend;
	float	m_E_ystar;
	float	m_E_CB;
	float	m_E_CG;
	float	m_E_CR;
	float	m_E_getx;
	float	m_E_gety;
	float	m_E_getz;
	float	m_E_lifefate;
	float	m_E_posx;
	float	m_E_posy;
	float	m_E_posz;
	int		m_E_maxnum;
	float	m_E_pram0;
	float	m_E_slowdown;
	float	m_E_speedmax;
	float	m_E_speedmin;
	float	m_E_TW;
	int		m_RS1;
	int		m_RS2;
	int		m_RS3;
	int		m_RS4;
	int		m_RS5;
	int		m_RS6;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLG_Snow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLG_Snow)
	afx_msg void OnBsColor();
	afx_msg void OnKillfocusEsAlpha();
	afx_msg void OnKillfocusEsAlxzend();
	afx_msg void OnKillfocusEsAlxzstar();
	afx_msg void OnKillfocusEsAlyend();
	afx_msg void OnKillfocusEsAlystar();
	afx_msg void OnKillfocusEsCB();
	afx_msg void OnKillfocusEsCG();
	afx_msg void OnKillfocusEsCR();
	afx_msg void OnKillfocusEsGetx();
	afx_msg void OnKillfocusEsGety();
	afx_msg void OnKillfocusEsGetz();
	afx_msg void OnKillfocusEsLifefate();
	afx_msg void OnKillfocusEsMaxnum();
	afx_msg void OnKillfocusEsPosX();
	afx_msg void OnKillfocusEsPosY();
	afx_msg void OnKillfocusEsPosZ();
	afx_msg void OnKillfocusEsPram0();
	afx_msg void OnKillfocusEsSlowdown();
	afx_msg void OnKillfocusEsSpeedmax();
	afx_msg void OnKillfocusEsSpeedmin();
	afx_msg void OnKillfocusESTextWidth();
	afx_msg void OnRs1();
	afx_msg void OnRs2();
	afx_msg void OnRs3();
	afx_msg void OnRs4();
	afx_msg void OnReleasedcaptureSsAlpha(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSsAlxzend(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSsAlxzstar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSsAlyend(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSsAlystar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSsGetx(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSsGety(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSsGetz(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSsLifefate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSsMaxnum(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSsPram0(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSsSlowdown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSsSpeedmax(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSsSpeedmin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSSTextWidth(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBsLoad();
	afx_msg void OnBsSave();
	afx_msg void OnBsEqual();
	afx_msg void OnRs5();
	afx_msg void OnRs6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_SNOW_H__19F8F7A4_54BC_46B9_883C_899B2F537155__INCLUDED_)
