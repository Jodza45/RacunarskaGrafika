
// lab1View.cpp : implementation of the Clab1View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "lab1.h"
#endif

#include "lab1Doc.h"
#include "lab1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#define dim 25
#endif


// Clab1View

IMPLEMENT_DYNCREATE(Clab1View, CView)

BEGIN_MESSAGE_MAP(Clab1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// Clab1View construction/destruction

Clab1View::Clab1View() noexcept
{
	m_bShowGrid = true; //za mrezu(GRID)

}

Clab1View::~Clab1View()
{
}

BOOL Clab1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Clab1View drawing

void Clab1View::OnDraw(CDC* pDC)
{
	Clab1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc); 
	if (!pDoc)
		return;

	CRect ekran(0,0,500,500);
	//CRect ekran;
	//GetClientRect(&ekran);
	CBrush backgroundBrush(RGB(180, 180, 180));

	pDC->FillRect(ekran, &backgroundBrush);



	CPen* greenPen = new CPen(PS_SOLID, 5, RGB(0, 255, 0));
	CPen* oldPen = pDC->SelectObject(greenPen);
	CBrush* orangeBrush = new CBrush(RGB(255, 165, 0));
	CBrush* oldBrush = pDC->SelectObject(orangeBrush);

	CBrush* mainOldBrush = oldBrush;

	//NARANDZASTI POLIGON
	CPoint orangeDots[4];
	orangeDots[0] = CPoint(1 * dim, 16 * dim);
	orangeDots[1] = CPoint(4 * dim, 13 * dim);
	orangeDots[2] = CPoint(7 * dim, 16 * dim);
	orangeDots[3] = CPoint(4 * dim, 19 * dim);

	pDC->Polygon(orangeDots, 4);


	//BELI TROUGAO
	CBrush* whiteBrush = new CBrush(HS_BDIAGONAL, RGB(0, 0, 255));
	oldBrush = pDC->SelectObject(whiteBrush);
	CPoint whiteDots[3];
	whiteDots[0] = CPoint(4 * dim, 13 * dim);
	whiteDots[1] = CPoint(7 * dim, 16 * dim);
	whiteDots[2] = CPoint(7 * dim, 10 * dim);
	//pDC->SetBkMode(TRANSPARENT);

	pDC->Polygon(whiteDots, 3);


	//CRVENI TROUGAO
	CBrush* redBrush = new CBrush(RGB(255, 0, 0));
	oldBrush = pDC->SelectObject(redBrush);
	CPoint redDots[3];
	redDots[0] = CPoint(7 * dim, 16 * dim);
	redDots[1] = CPoint(7 * dim, 10 * dim);
	redDots[2] = CPoint(13 * dim, 10 * dim);

	pDC->Polygon(redDots, 3);


	//LJUBICASTI POLIGON
	CBrush* purpleBrush = new CBrush(RGB(128, 0, 128));
	oldBrush = pDC->SelectObject(purpleBrush);
	CPoint purpleDots[4];
	purpleDots[0] = CPoint(7 * dim, 10 * dim);
	purpleDots[1] = CPoint(13 * dim, 10 * dim);
	purpleDots[2] = CPoint(16 * dim, 7 * dim);
	purpleDots[3] = CPoint(10 * dim, 7 * dim);

	pDC->Polygon(purpleDots, 4);


	//ZUTI TROUGAO
	CBrush* yellowBrush = new CBrush(RGB(255, 255, 0));
	oldBrush = pDC->SelectObject(yellowBrush);
	CPoint yellowDots[3];
	yellowDots[0] = CPoint(10 * dim, 7 * dim);
	yellowDots[1] = CPoint(16 * dim, 7 * dim);
	yellowDots[2] = CPoint(13 * dim, 4 * dim);

	pDC->Polygon(yellowDots, 3);


	//ROZI TROUGAO
	int centarX = (16 * dim + 13 * dim) / 2;
	int centarY = (7 * dim + 4 * dim) / 2;

	CBrush* pinkBrush = new CBrush(RGB(255, 192, 203));
	oldBrush = pDC->SelectObject(pinkBrush);
	CPoint pinkDots[3];
	pinkDots[0] = CPoint(centarX, centarY);
	pinkDots[1] = CPoint(20 * dim, 0);
	pinkDots[2] = CPoint(20 * dim, 11 * dim);

	pDC->Polygon(pinkDots, 3);


	//ZELENI TROUGAO
	CBrush* greenBrush = new CBrush(RGB(0, 200, 0));
	oldBrush = pDC->SelectObject(greenBrush);
	CPoint greenDots[3];
	greenDots[0] = CPoint(centarX, centarY);
	greenDots[1] = CPoint(20 * dim, 0);
	greenDots[2] = CPoint(9 * dim, 0);

	pDC->Polygon(greenDots, 3);


	//GRID
	if (m_bShowGrid)
	{
		CPen* gridPen = new CPen(PS_SOLID, 1, RGB(255, 255, 255));
		CPen* oldPen = pDC->SelectObject(gridPen);

		for (int i = 0; i < 20; i++)
		{
			pDC->MoveTo(i * dim, 0);
			pDC->LineTo(i * dim, 20 * dim);

			pDC->MoveTo(0, i * dim);
			pDC->LineTo(20 * dim, i * dim);
		}

		pDC->SelectObject(oldPen);
		gridPen->DeleteObject();
	}


	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}


// Clab1View printing

BOOL Clab1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Clab1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Clab1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Clab1View diagnostics

#ifdef _DEBUG
void Clab1View::AssertValid() const
{
	CView::AssertValid();
}

void Clab1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Clab1Doc* Clab1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Clab1Doc)));
	return (Clab1Doc*)m_pDocument;
}
#endif //_DEBUG


// Clab1View message handlers


void Clab1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'G' || nChar == 'g')
	{
		m_bShowGrid = !m_bShowGrid;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
