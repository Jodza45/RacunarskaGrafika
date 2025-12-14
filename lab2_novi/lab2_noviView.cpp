
// lab2_noviView.cpp : implementation of the Clab2noviView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "lab2_novi.h"
#endif

#include "lab2_noviDoc.h"
#include "lab2_noviView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define dim 25
#include <cmath>
#define KRUG_DIM 12
#define TRANS 37
#define UGAO 45
#define KAKTUS_X 10
#define KAKTUS_Y 20




// Clab2noviView

IMPLEMENT_DYNCREATE(Clab2noviView, CView)

BEGIN_MESSAGE_MAP(Clab2noviView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// Clab2noviView construction/destruction

Clab2noviView::Clab2noviView() noexcept
{
	showGrid = true;
	rotate1 = 0.0f;
	rotate2 = 0.0f;

	cactus = GetEnhMetaFile(_T("res/cactus_part.emf"));
	lightCactus = GetEnhMetaFile(_T("res/cactus_part_light.emf"));
}

Clab2noviView::~Clab2noviView()
{
	if (cactus)
		DeleteEnhMetaFile(cactus);

	if (lightCactus)
		DeleteEnhMetaFile(lightCactus);
}

BOOL Clab2noviView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Clab2noviView drawing
void Clab2noviView::drawCircle(CDC* pDC, int radius)
{
	pDC->Ellipse(-radius, -radius, radius, radius);
}

void Clab2noviView::DrawBranch(CDC* pDC, float scaleX, float scaleY, bool isLight, bool hasJoint)
{
	HENHMETAFILE mf = isLight ? lightCactus : cactus;

	Translate(pDC, 0, -TRANS, false);

	Scale(pDC, scaleX, scaleY, false);
	pDC->PlayMetaFile(mf, CRect(-KAKTUS_X, -KAKTUS_Y, KAKTUS_X, KAKTUS_Y));
	Scale(pDC, 1.0 / scaleX, 1.0 / scaleY, false);

	if (hasJoint)
	{
		Translate(pDC, 0, -TRANS, false);
		drawCircle(pDC, KRUG_DIM);
	}
}

void Clab2noviView::drawPotBase(CDC* pDC)
{
	CPen blackPen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush brownBrush(RGB(137, 81, 41));
	CPen* pOldPen = pDC->SelectObject(&blackPen);
	CBrush* pOldBrush = pDC->SelectObject(&brownBrush);

	CPoint bigBrown[4];
	bigBrown[0] = CPoint(8 * dim, 18 * dim);
	bigBrown[1] = CPoint(12 * dim, 18 * dim);
	bigBrown[2] = CPoint(11 * dim, 20 * dim);
	bigBrown[3] = CPoint(9 * dim, 20 * dim);
	pDC->Polygon(bigBrown, 4);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

void Clab2noviView::drawPotRim(CDC* pDC)
{
	CPen blackPen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush brownBrush(RGB(137, 81, 41));
	CPen* pOldPen = pDC->SelectObject(&blackPen);
	CBrush* pOldBrush = pDC->SelectObject(&brownBrush);

	pDC->Rectangle(7.5 * dim, 17 * dim, 12.5 * dim, 18 * dim);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

void Clab2noviView::DrawLabel(CDC* pDC, CString text)
{
	CFont font;
	font.CreateFont(
		(int)(1.1 * dim),   // Visina fonta
		15,                 // Širina
		-900,               // nEscapement: Rotacija za -90 stepeni. OVO JE KLJUČNA PROMENA.
		-900,               // nOrientation: Rotacija karaktera da se poklopi
		FW_NORMAL,          // Debljina
		FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

	// Selektovanje fonta i postavljanje DC-a
	CFont* pOldFont = pDC->SelectObject(&font);
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF oldColor = pDC->GetTextColor();

	// Pozicije X i Y (gornji desni ugao)
	int posX = 19 * dim;
	// Pošto tekst sada raste "nadole", moramo da počnemo od više Y koordinate.
	int posY = 2 * dim;

	// Crtanje "senke" (tamniji tekst, malo pomeren)
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->TextOut(posX, posY, text);

	// Crtanje glavnog, svetlijeg teksta preko senke
	pDC->SetTextColor(RGB(255, 255, 0)); // Žuta boja
	pDC->TextOut(posX - 2, posY - 2, text);

	// Vraćanje starih GDI objekata
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(oldBkMode);
	pDC->SetTextColor(oldColor);
}

void Clab2noviView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xform;
	xform.eDx = dX;
	xform.eDy = dY;
	xform.eM11 = 1;
	xform.eM12 = 0;
	xform.eM21 = 0;
	xform.eM22 = 1;

	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void Clab2noviView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xform;
	xform.eDx = 0;
	xform.eDy = 0;
	xform.eM11 = sX;
	xform.eM12 = 0;
	xform.eM21 = 0;
	xform.eM22 = sY;

	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void Clab2noviView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float angleRad = angle * 3.14159 / 180.0;

	XFORM xform;
	xform.eDy = 0;
	xform.eDx = 0;
	xform.eM11 = cos(angleRad);
	xform.eM12 = sin(angleRad);
	xform.eM21 = -sin(angleRad);
	xform.eM22 = cos(angleRad);

	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void Clab2noviView::DrawFigure(CDC* pDC)
{
	Rotate(pDC, rotate1, false);      //rotiranje prvog svetlog kaktusa
	DrawBranch(pDC, 3.0, 1.75, true);

	XFORM joint2;
	pDC->GetWorldTransform(&joint2);
	
	DrawBranch(pDC, 1.0, 1.75, false, false); 

	pDC->SetWorldTransform(&joint2);
	Rotate(pDC, -UGAO, false);
	DrawBranch(pDC, 1.0, 1.75, false);

	XFORM jointLeft;
	pDC->GetWorldTransform(&jointLeft); 

	pDC->SetWorldTransform(&joint2);
	Rotate(pDC, UGAO, false);
	DrawBranch(pDC, 1.0, 1.75, false);

	XFORM jointRight;
	pDC->GetWorldTransform(&jointRight);

	//------------------------------------------------------

	pDC->SetWorldTransform(&jointLeft);
	
	Rotate(pDC, -UGAO, false);
	DrawBranch(pDC, 1.75, 1.75, false, false); 

	pDC->SetWorldTransform(&jointLeft);

	Rotate(pDC, UGAO, false);
	DrawBranch(pDC, 1.75, 1.75, false);

	DrawBranch(pDC, 1.75, 1.75, false, false);

	pDC->SetWorldTransform(&jointRight); 

	Rotate(pDC, rotate2, false); 
	Rotate(pDC, -UGAO, false); 
	DrawBranch(pDC, 1.75, 1.75, true, false); 

	pDC->SetWorldTransform(&jointRight);

	Rotate(pDC, UGAO, false);
	DrawBranch(pDC, 1.75, 1.75, false);

	XFORM farRightJoint;
	pDC->GetWorldTransform(&farRightJoint);

	//pDC->SetWorldTransform(&farRightJoint);

	Rotate(pDC, -UGAO, false);
	DrawBranch(pDC, 1.75, 1.75, false, false);

	pDC->SetWorldTransform(&farRightJoint);

	Rotate(pDC, UGAO, false);
	DrawBranch(pDC, 1.75, 1.75, false, false);
}

void Clab2noviView::makeGrid(CDC* pDC)
{
	CPen gridPen(PS_SOLID, 1, RGB(255, 255, 255));
	CPen* oldPen = pDC->SelectObject(&gridPen);

	if (showGrid)
	{
		for (int i = 0; i < 21; i++)
		{
			pDC->MoveTo(i * dim, 0);
			pDC->LineTo(i * dim, 20 * dim);

			pDC->MoveTo(0, i * dim);
			pDC->LineTo(20 * dim, i * dim);
		}
	}
	

	pDC->SelectObject(oldPen);
}


void Clab2noviView::OnDraw(CDC* pDC)
{
	Clab2noviDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect ekran(0, 0, 500, 500);
	CBrush backBrush(RGB(135, 206, 250));
	pDC->FillRect(ekran, &backBrush);

	int oldMode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldXForm;
	pDC->GetWorldTransform(&oldXForm);

	drawPotBase(pDC);

	CBrush greenBrush(RGB(0, 150, 0));
	pDC->SelectObject(&greenBrush);
	Translate(pDC, 10 * dim, 17 * dim, false);
	drawCircle(pDC, KRUG_DIM);

	XFORM prviKrug;
	pDC->GetWorldTransform(&prviKrug);

	pDC->SetWorldTransform(&oldXForm);

	drawPotRim(pDC);

	pDC->SetWorldTransform(&prviKrug);
	pDC->SelectObject(&greenBrush);

	DrawFigure(pDC);

	pDC->SetWorldTransform(&oldXForm);
	pDC->SetGraphicsMode(oldMode);

	DrawLabel(pDC, CString("Kaktusss"));

	makeGrid(pDC);
}


// Clab2noviView printing

BOOL Clab2noviView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Clab2noviView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Clab2noviView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Clab2noviView diagnostics

#ifdef _DEBUG
void Clab2noviView::AssertValid() const
{
	CView::AssertValid();
}

void Clab2noviView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Clab2noviDoc* Clab2noviView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Clab2noviDoc)));
	return (Clab2noviDoc*)m_pDocument;
}
#endif //_DEBUG


// Clab2noviView message handlers

void Clab2noviView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case 'G':
		{
			showGrid = !showGrid;
			break;
		}
		case 'N':
		{
			rotate1 -= 0.5f;
			break;
		}
		case 'M':
		{
			rotate1 += 0.5f;
			break;
		}
		case 'J':
		{
			rotate2 -= 0.5f;
			break;
		}
		case 'K':
		{
			rotate2 += 0.5f;
			break;
		}
	}

	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
