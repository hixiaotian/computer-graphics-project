// PrtSysDoc.h : interface of the CPrtSysDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRTSYSDOC_H__0217B7E7_DB46_48AD_ACDD_919AE123F651__INCLUDED_)
#define AFX_PRTSYSDOC_H__0217B7E7_DB46_48AD_ACDD_919AE123F651__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPrtSysDoc : public CDocument
{
protected: // create from serialization only
	CPrtSysDoc();
	DECLARE_DYNCREATE(CPrtSysDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrtSysDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPrtSysDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPrtSysDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRTSYSDOC_H__0217B7E7_DB46_48AD_ACDD_919AE123F651__INCLUDED_)
