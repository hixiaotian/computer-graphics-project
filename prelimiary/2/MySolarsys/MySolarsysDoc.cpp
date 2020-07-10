// MySolarsysDoc.cpp : implementation of the CMySolarsysDoc class
//

#include "stdafx.h"
#include "MySolarsys.h"

#include "MySolarsysDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySolarsysDoc

IMPLEMENT_DYNCREATE(CMySolarsysDoc, CDocument)

BEGIN_MESSAGE_MAP(CMySolarsysDoc, CDocument)
	//{{AFX_MSG_MAP(CMySolarsysDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySolarsysDoc construction/destruction

CMySolarsysDoc::CMySolarsysDoc()
{
	// TODO: add one-time construction code here

}

CMySolarsysDoc::~CMySolarsysDoc()
{
}

BOOL CMySolarsysDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMySolarsysDoc serialization

void CMySolarsysDoc::Serialize(CArchive& ar)
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
// CMySolarsysDoc diagnostics

#ifdef _DEBUG
void CMySolarsysDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMySolarsysDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMySolarsysDoc commands
