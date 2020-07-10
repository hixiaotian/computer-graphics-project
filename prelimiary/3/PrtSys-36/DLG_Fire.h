#if !defined(AFX_DLG_FIRE_H__BE1B001C_923F_4899_B628_E2E498EAB9C4__INCLUDED_)
#define AFX_DLG_FIRE_H__BE1B001C_923F_4899_B628_E2E498EAB9C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLG_Fire.h : header file
//
#include "glo.h"	//引用全局结构定义
#include "coloredit.h"	//引用颜色对话框

/////////////////////////////////////////////////////////////////////////////
// CDLG_Fire dialog

class CDLG_Fire : public CDialog
{
// Construction
public:
	CDLG_Fire(CWnd* pParent = NULL);				// standard constructor
	~CDLG_Fire();
	DlgData_PRT* m_fireDLG;							//建立一个对话框数据结构的指针
	CColorEdit	m_FireColor;						//颜色对话框
	bool		colorEixst;							//是否生成了颜色对话框，如果生成过，则禁止重复成生
	void GetExtFireData(DlgData_PRT &Ex_fireDLG);	//获得外界数据
	void UpdateDLGDate(void);						//更新界面数据，用于读取和初始化！

// Dialog Data
	//{{AFX_DATA(CDLG_Fire)
	enum { IDD = IDD_DLG_Fire };
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
	float	m_E_xzstar;
	float	m_E_yend;
	float	m_E_ystar;
	float	m_E_getx;
	float	m_E_gety;
	float	m_E_getz;
	float	m_E_lifefate;
	int		m_E_maxnum;
	float	m_E_slowdown;
	float	m_E_speedmax;
	float	m_E_speedmin;
	float	m_E_posx;
	float	m_E_posy;
	float	m_E_posz;
	float	m_E_TW;
	int		m_RF1;
	int		m_RF2;
	int		m_RF3;
	int		m_RF4;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLG_Fire)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLG_Fire)
	afx_msg void OnDestroy();
	afx_msg void OnKillfocusEAlxzend();
	afx_msg void OnKillfocusEAlxzstar();
	afx_msg void OnKillfocusEAlyend();
	afx_msg void OnKillfocusEAlystar();
	afx_msg void OnKillfocusEGetx();
	afx_msg void OnKillfocusEGety();
	afx_msg void OnKillfocusEGetz();
	afx_msg void OnKillfocusELifefate();
	afx_msg void OnKillfocusEMaxnum();
	afx_msg void OnKillfocusEPosX();
	afx_msg void OnKillfocusEPosY();
	afx_msg void OnKillfocusEPosZ();
	afx_msg void OnKillfocusESlowdown();
	afx_msg void OnKillfocusESpeedmax();
	afx_msg void OnKillfocusESpeedmin();
	afx_msg void OnReleasedcaptureSldAlxzend(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSldAlxzstar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSldAlyend(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSldAlystar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSldGetx(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSldGety(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSldGetz(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSldLifefate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSldMaxnum(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSldSlowdown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSldSpeedmax(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSldSpeedmin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusETextWidth();
	afx_msg void OnRF1();
	afx_msg void OnRF2();
	afx_msg void OnRF3();
	afx_msg void OnRF4();
	afx_msg void OnReleasedcaptureSTextWitdh(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBColor();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnBfLoad();
	afx_msg void OnBfSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_FIRE_H__BE1B001C_923F_4899_B628_E2E498EAB9C4__INCLUDED_)
