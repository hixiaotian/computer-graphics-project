// PrtSysDoc.cpp : implementation of the CPrtSysDoc class
//

#include "stdafx.h"
#include "PrtSys.h"

#include "PrtSysDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrtSysDoc

IMPLEMENT_DYNCREATE(CPrtSysDoc, CDocument)

BEGIN_MESSAGE_MAP(CPrtSysDoc, CDocument)
	//{{AFX_MSG_MAP(CPrtSysDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrtSysDoc construction/destruction

CPrtSysDoc::CPrtSysDoc()
{
	// TODO: add one-time construction code here

}

CPrtSysDoc::~CPrtSysDoc()
{
}

BOOL CPrtSysDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CPrtSysDoc serialization

void CPrtSysDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPrtSysDoc diagnostics

#ifdef _DEBUG
void CPrtSysDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPrtSysDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPrtSysDoc commands
