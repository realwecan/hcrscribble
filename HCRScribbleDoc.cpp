// HCRScribbleDoc.cpp : implementation of the CHCRScribbleDoc class
//

#include "stdafx.h"
#include "HCRScribble.h"

#include "HCRScribbleDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHCRScribbleDoc

IMPLEMENT_DYNCREATE(CHCRScribbleDoc, CDocument)

BEGIN_MESSAGE_MAP(CHCRScribbleDoc, CDocument)
	//{{AFX_MSG_MAP(CHCRScribbleDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHCRScribbleDoc construction/destruction

CHCRScribbleDoc::CHCRScribbleDoc()
{
	// TODO: add one-time construction code here

}

CHCRScribbleDoc::~CHCRScribbleDoc()
{
}

BOOL CHCRScribbleDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	InitDocument();

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CHCRScribbleDoc serialization

void CHCRScribbleDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
	m_strokeList.Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CHCRScribbleDoc diagnostics

#ifdef _DEBUG
void CHCRScribbleDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHCRScribbleDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHCRScribbleDoc commands

BOOL CHCRScribbleDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	InitDocument();
	return TRUE;
}

void CHCRScribbleDoc::DeleteContents()
{
	while (!m_strokeList.IsEmpty())
	{
		delete m_strokeList.RemoveHead();
	}
	CDocument::DeleteContents();
}

void CHCRScribbleDoc::InitDocument()
{
	m_penCur.CreatePen(PS_SOLID, 2, RGB(0,0,0)); //solid, 2 pixels wide, black color
}

CStroke* CHCRScribbleDoc::NewStroke()
{
	CStroke* pStrokeItem = new CStroke();
	m_strokeList.AddTail(pStrokeItem);
	SetModifiedFlag(); //Mark the document as having been modified
	//for purpose of confirming file Close
	return pStrokeItem;
}

/////////////////////////////////////////////////////////////////////////////
// CStroke

IMPLEMENT_SERIAL(CStroke, CObject, 1) //third argument is wShema, indicating the version of file
CStroke::CStroke()
{
	//This empty constructor is used by serialization
}

void CStroke::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		m_pointArray.Serialize(ar);
	}
	else
	{
		m_pointArray.Serialize(ar);
	}
}

BOOL CStroke::DrawStroke(CDC* pDC)
{
	CPen penStroke;
	if (!penStroke.CreatePen(PS_SOLID,2,RGB(0,0,0)))
		return FALSE;
	CPen* pOldPen = pDC->SelectObject(&penStroke);
	pDC->MoveTo(m_pointArray[0]);
	for (int i=1; i < m_pointArray.GetSize(); i++)
	{
		pDC->LineTo(m_pointArray[i]);
	}
	pDC->SelectObject(pOldPen);
	return FALSE;
}
