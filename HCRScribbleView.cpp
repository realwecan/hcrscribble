// HCRScribbleView.cpp : implementation of the CHCRScribbleView class
//

#include "stdafx.h"
#include "HCRScribble.h"

#include "HCRScribbleDoc.h"
#include "HCRScribbleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CHCRRecognizer g_recognizer;

/////////////////////////////////////////////////////////////////////////////
// CHCRScribbleView

IMPLEMENT_DYNCREATE(CHCRScribbleView, CView)

BEGIN_MESSAGE_MAP(CHCRScribbleView, CView)
	//{{AFX_MSG_MAP(CHCRScribbleView)
	ON_COMMAND(ID_RECO_RECOGNIZE, OnRecoRecognize)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_OPTIONS_IDIRCH, OnOptionsIdirch)
	ON_COMMAND(ID_OPTIONS_DIRCH, OnOptionsDirch)
	ON_COMMAND(ID_OPTIONS_8DIR, OnOptions8dir)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_IDIRCH, OnUpdateOptionsIdirch)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_DIRCH, OnUpdateOptionsDirch)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_8DIR, OnUpdateOptions8dir)
	ON_COMMAND(ID_863_TRAIN, On863Train)
	ON_COMMAND(ID_863_LOAD, On863Load)
	ON_COMMAND(ID_863_TEST, On863Test)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHCRScribbleView construction/destruction

CHCRScribbleView::CHCRScribbleView()
{
	// TODO: add construction code here
}

CHCRScribbleView::~CHCRScribbleView()
{
}

BOOL CHCRScribbleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CHCRScribbleView drawing

void CHCRScribbleView::OnDraw(CDC* pDC)
{
	CHCRScribbleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
	// The view delegates the drawing of individual strokes to
	// CStroke::DrawStroke().
	CTypedPtrList<CObList,CStroke*>& strokeList = pDoc->m_strokeList;
	// The "&" above is a reference. It indicates that strokeList is
	// equivalent to pDoc->m_strokeList. No pointer or address here.
	POSITION pos = strokeList.GetHeadPosition();
	while (pos != NULL)
	{
		CStroke* pStroke = strokeList.GetNext(pos);
		pStroke->DrawStroke(pDC);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CHCRScribbleView printing

BOOL CHCRScribbleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CHCRScribbleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CHCRScribbleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CHCRScribbleView diagnostics

#ifdef _DEBUG
void CHCRScribbleView::AssertValid() const
{
	CView::AssertValid();
}

void CHCRScribbleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHCRScribbleDoc* CHCRScribbleView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHCRScribbleDoc)));
	return (CHCRScribbleDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHCRScribbleView message handlers

void CHCRScribbleView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// Pressing the mouse button in the view window starts a new stroke
	
	m_pStrokeCur = GetDocument()->NewStroke();
	// Add first point to the new stroke
	m_pStrokeCur->m_pointArray.Add(point);
	
	SetCapture(); // Captures the mouse until button up.
	m_ptPrev = point; // Serves as the MoveTo() anchor point
	// for the LineTo() the next point,
	// as the user drags the mouse.
	
	CView::OnLButtonDown(nFlags, point);
}

void CHCRScribbleView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// Mouse button up is interesting in the Scribble application
	// only if the user is currently drawing a new stroke by dragging
	// the captured mouse.
	
	if (GetCapture() != this)
		return; // If this window (view) didn't capture the mouse,
				// then the user isn't drawing in this window.
	
	CHCRScribbleDoc* pDoc = GetDocument();
	
	CClientDC dc(this);
	
	CPen* pOldPen = dc.SelectObject(pDoc->GetCurrentPen());
	dc.MoveTo(m_ptPrev);
	dc.LineTo(point);
	dc.SelectObject(pOldPen);
	m_pStrokeCur->m_pointArray.Add(point);
	
	ReleaseCapture(); // Release the mouse capture established at
	// the beginning of the mouse drag.
	
	CView::OnLButtonUp(nFlags, point);
}

void CHCRScribbleView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// Mouse movement is interesting in the Scribble application
	// only if the user is currently drawing a new stroke by dragging
	// the captured mouse.
	
	if ( GetCapture() != this )
		return; // If this window (view) didn't capture the mouse,
	// then the user isn't drawing in this window.
	
	CClientDC dc(this);
	m_pStrokeCur->m_pointArray.Add(point);
	
	// Draw a line from the previous detected point in the mouse
	// drag to the current point.
	CPen* pOldPen = dc.SelectObject(GetDocument()->GetCurrentPen());
	dc.MoveTo(m_ptPrev);
	dc.LineTo(point);
	dc.SelectObject(pOldPen);
	m_ptPrev = point;
	
	CView::OnMouseMove(nFlags, point);
}

WORD GetGBCharFromIndex(int index)
{
	return (BYTE)( index / 94 + 0xb0 ) + (( index % 94 + 0xa1 ) << 8 );
}

void CHCRScribbleView::OnRecoRecognize() 
{
	// TODO: Add your command handler code here
	
	CHCRScribbleDoc* pDoc = GetDocument();
	
	CTypedPtrList<CObList,CStroke*>& strokeList = pDoc->m_strokeList;
	
	g_recognizer.InitWithStrokeList(strokeList);

	g_recognizer.Preprocess();
	
	g_recognizer.Recognize();

	char result_str[30];

	for (int i=0; i<10; i++)
	{
		WORD gbcode = GetGBCharFromIndex( g_recognizer.result[i] );
		memcpy(&(result_str[3*i]),&gbcode,2);
		result_str[3*i+2] = ' ';
	}
	result_str[29] = '\0';

	CString status_bar_msg;
	status_bar_msg.Format("识别结果中的前十个候选字为：%s",result_str);
	CStatusBar* pStatus = (CStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
		pStatus->SetPaneText(0,status_bar_msg);
	
}

void CHCRScribbleView::OnOptionsIdirch() 
{
	// TODO: Add your command handler code here
	g_recognizer.curfea = 0;
}

void CHCRScribbleView::OnOptionsDirch() 
{
	// TODO: Add your command handler code here
	g_recognizer.curfea = 1;

}

void CHCRScribbleView::OnOptions8dir() 
{
	// TODO: Add your command handler code here
	g_recognizer.curfea = 2;
	
}

void CHCRScribbleView::OnUpdateOptionsIdirch(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(g_recognizer.curfea == 0);
}

void CHCRScribbleView::OnUpdateOptionsDirch(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(g_recognizer.curfea == 1);
}

void CHCRScribbleView::OnUpdateOptions8dir(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(g_recognizer.curfea == 2);
}

void CHCRScribbleView::On863Train() 
{
	// TODO: Add your command handler code here
	g_recognizer.TrainClassifier();
}

void CHCRScribbleView::On863Load() 
{
	// TODO: Add your command handler code here
	g_recognizer.LoadClassifier("c:\\classifier4.fea");
}

void CHCRScribbleView::On863Test() 
{
	// TODO: Add your command handler code here
	g_recognizer.TestRecognizer();
}
