
// lab2_noviView.h : interface of the Clab2noviView class
//

#pragma once


class Clab2noviView : public CView
{
protected: // create from serialization only
	Clab2noviView() noexcept;
	DECLARE_DYNCREATE(Clab2noviView)

// Attributes
public:
	Clab2noviDoc* GetDocument() const;
	bool showGrid;
	float rotate1;
	float rotate2;
	HENHMETAFILE cactus;
	HENHMETAFILE lightCactus;

// Operations
public:
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void makeGrid(CDC* pDC);
	void drawCircle(CDC* pDC, int radius);
	void DrawFigure(CDC* pDC);
	void DrawBranch(CDC* pDC, float scaleX, float scaleY, bool isLight, bool hasJoint = true);
	void drawPotBase(CDC* pDC);
	void drawPotRim(CDC* pDC);
	void DrawLabel(CDC* pDC, CString text);
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~Clab2noviView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in lab2_noviView.cpp
inline Clab2noviDoc* Clab2noviView::GetDocument() const
   { return reinterpret_cast<Clab2noviDoc*>(m_pDocument); }
#endif

