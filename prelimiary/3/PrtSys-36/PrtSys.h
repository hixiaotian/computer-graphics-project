// PrtSys.h : main header file for the PRTSYS application
//

#if !defined(AFX_PRTSYS_H__E909DF64_CEC4_4787_AE76_001BA5EF5D72__INCLUDED_)
#define AFX_PRTSYS_H__E909DF64_CEC4_4787_AE76_001BA5EF5D72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPrtSysApp:
// See PrtSys.cpp for the implementation of this class
//

class CPrtSysApp : public CWinApp
{
public:
	CPrtSysApp();
//	BOOL OpenFile(LPCTSTR lpszPathName);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrtSysApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CPrtSysApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRTSYS_H__E909DF64_CEC4_4787_AE76_001BA5EF5D72__INCLUDED_)
