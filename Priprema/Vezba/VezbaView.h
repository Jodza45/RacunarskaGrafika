// VezbaView.h : interface of the CVezbaView class
//

#pragma once
#include "DImage.h"
#define MAX_PIECES 9

#define PI 3.1415926536

#define RAD(x) x * PI / 180.0 // stepeni -> radijani
#define DEG(x) x * 180.0 / PI // radijani -> stepeni


class CVezbaView : public CView
{
protected: // create from serialization only
	CVezbaView() noexcept;
	DECLARE_DYNCREATE(CVezbaView)

// Attributes
public:
	CVezbaDoc* GetDocument() const;

	int rotation = 0;
	bool mx = false;
	bool my = false;
	DImage* puzzlePieces[MAX_PIECES];

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void LoadPuzzlePieces();
	void DrawCenter(CDC* pDC);
	void DrawAxes(CDC* pDC);
	void DrawTransparent(CDC* pDC, DImage* img, bool isBlue);
	void makeGray(CBitmap* bitmap);
	void makeBlue(CBitmap* bitmap);
	void DrawGrid(CDC* pDC);
	void DrawBackground(COLORREF backgroundColor, CDC* pDC);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply = false);
	void Mirror(CDC* pDC, bool mx, bool my, bool rightMyltiply = false);
	void Rotate(CDC* pDC, float angle, bool rightMultiply = false);
	void DrawMem(CDC* pDC);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CVezbaView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in VezbaView.cpp
inline CVezbaDoc* CVezbaView::GetDocument() const
   { return reinterpret_cast<CVezbaDoc*>(m_pDocument); }
#endif

