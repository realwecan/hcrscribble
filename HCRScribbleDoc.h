// HCRScribbleDoc.h : interface of the CHCRScribbleDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HCRSCRIBBLEDOC_H__02E02F21_4103_4C33_B560_C4A74F751659__INCLUDED_)
#define AFX_HCRSCRIBBLEDOC_H__02E02F21_4103_4C33_B560_C4A74F751659__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxtempl.h> //added by wecan, for template use


/////////////////////////////////////////////////////////////////////////////
// class CStroke
//
// A stroke is a series of connected points in the scribble drawing.
// A scribble document may have multiple strokes.

class CStroke : public CObject
{
public:
	CStroke(); //constructor
protected:
	DECLARE_SERIAL(CStroke)
		
		// Attributes
public:
	CArray<CPoint,CPoint> m_pointArray; //series of connected points
	
	// Operations
public:
	BOOL DrawStroke(CDC* pDC);
	
public:
	virtual void Serialize(CArchive& ar);
};


class CHCRScribbleDoc : public CDocument
{
protected: // create from serialization only
	CHCRScribbleDoc();
	DECLARE_DYNCREATE(CHCRScribbleDoc)

// Attributes
protected:
	CPen m_penCur; //pen created at every time InitDocument();
				   //could be used later upon request
public:
	CTypedPtrList<CObList,CStroke*> m_strokeList;
	CPen* GetCurrentPen() { return &m_penCur; }

// Operations
public:
	CStroke* NewStroke();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHCRScribbleDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHCRScribbleDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void InitDocument();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CHCRScribbleDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HCRSCRIBBLEDOC_H__02E02F21_4103_4C33_B560_C4A74F751659__INCLUDED_)
