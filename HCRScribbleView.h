// HCRScribbleView.h : interface of the CHCRScribbleView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HCRSCRIBBLEVIEW_H__4CE08AF5_8DA2_4BDB_946F_B05095FB2FD0__INCLUDED_)
#define AFX_HCRSCRIBBLEVIEW_H__4CE08AF5_8DA2_4BDB_946F_B05095FB2FD0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CHCRScribbleView : public CView
{
protected: // create from serialization only
	CHCRScribbleView();
	DECLARE_DYNCREATE(CHCRScribbleView)

// Attributes
public:
	CHCRScribbleDoc* GetDocument();
	
protected:
	CStroke* m_pStrokeCur; //the stroke in progress
	CPoint m_ptPrev; //the last mouse pt in the stroke in progress

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHCRScribbleView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHCRScribbleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CHCRScribbleView)
	afx_msg void OnRecoRecognize();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnOptionsIdirch();
	afx_msg void OnOptionsDirch();
	afx_msg void OnOptions8dir();
	afx_msg void OnUpdateOptionsIdirch(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionsDirch(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptions8dir(CCmdUI* pCmdUI);
	afx_msg void On863Train();
	afx_msg void On863Load();
	afx_msg void On863Test();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in HCRScribbleView.cpp
inline CHCRScribbleDoc* CHCRScribbleView::GetDocument()
   { return (CHCRScribbleDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HCRSCRIBBLEVIEW_H__4CE08AF5_8DA2_4BDB_946F_B05095FB2FD0__INCLUDED_)
