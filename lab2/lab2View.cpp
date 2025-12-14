
// lab2View.cpp : implementation of the Clab2View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "lab2.h"
#endif

#include "lab2Doc.h"
#include "lab2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define dim 25
#include <cmath>


// Clab2View

IMPLEMENT_DYNCREATE(Clab2View, CView)

BEGIN_MESSAGE_MAP(Clab2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// Clab2View construction/destruction

Clab2View::Clab2View() noexcept : m_hCactusPart(NULL), m_hCactusPartLight(NULL)
{
	m_showGid = true;
	m_lightPartAngle1 = 0.0f;
	m_lightPartAngle2 = 0.0f;


	m_hCactusPart = GetEnhMetaFile(_T("res/cactus_part.emf"));
	m_hCactusPartLight = GetEnhMetaFile(_T("res/cactus_part_light.emf"));
}

Clab2View::~Clab2View()
{
	if (m_hCactusPart)
		DeleteEnhMetaFile(m_hCactusPart);

	if (m_hCactusPartLight)
		DeleteEnhMetaFile(m_hCactusPartLight);
}

BOOL Clab2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}



void Clab2View::drawGrid(CDC* pDC)
{
	CPen gridPen(PS_SOLID, 1, RGB(255, 255, 255));
	CPen* oldPen = pDC->SelectObject(&gridPen);

	if (m_showGid)
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

void Clab2View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xform;
	xform.eM11 = 1.0;
	xform.eM12 = 0.0;
	xform.eM21 = 0.0;
	xform.eM22 = 1.0;
	xform.eDx = dX;
	xform.eDy = dY;

	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void Clab2View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xform;
	xform.eM11 = sX;
	xform.eM12 = 0;
	xform.eM21 = 0;
	xform.eM22 = sY;
	xform.eDx = 0;
	xform.eDy = 0;

	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void Clab2View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float angleRad = angle * 3.14159 / 180.0;

	XFORM xform;
	xform.eM11 = cos(angleRad);
	xform.eM12 = sin(angleRad);
	xform.eM21 = -sin(angleRad);
	xform.eM22 = cos(angleRad);
	xform.eDx = 0;
	xform.eDy = 0;
	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}


// Clab2View drawing

void Clab2View::OnDraw(CDC* pDC)
{
	Clab2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect ekran;
	GetClientRect(&ekran);

	pDC->SetMapMode(MM_ISOTROPIC);
	pDC->SetWindowExt(500, 500);
	pDC->SetViewportExt(ekran.Width(), ekran.Height());
	pDC->SetWindowOrg(0, 0);


	CRect logicalRect(0, 0, 500, 500);
	CBrush backBrush(RGB(135, 206, 250));
	pDC->FillRect(logicalRect, &backBrush);

	//-------------------------------------------------

	int oldMode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldXForm;
	pDC->GetWorldTransform(&oldXForm);

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

	CBrush greenBrush(RGB(0, 150, 0));
	pDC->SelectObject(&greenBrush);
	pDC->SelectObject(&blackPen);

	Translate(pDC, 10 * dim, 17 * dim, false); //translate za prvi krug

	pDC->Ellipse(-12, -12, 12, 12);//zeleni kruzic

	XFORM prviKRug;
	pDC->GetWorldTransform(&prviKRug);

	

	pDC->SetWorldTransform(&oldXForm);
	
	pOldBrush = pDC->SelectObject(&brownBrush);
	pDC->Rectangle(7.5 * dim, 17 * dim, 12.5 * dim, 18 * dim);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);


	//prvi svetli kaktus

	Translate(pDC, 10 * dim, 17 * dim, false);
	Rotate(pDC, m_lightPartAngle1, false);
	Translate(pDC, 0, -37, false);
	Scale(pDC, 3, 1.75, false);

	

	pDC->PlayMetaFile(m_hCactusPartLight, CRect(-10, -20, 10, 20));
	Scale(pDC, 1.0 / 3, 1.0 / 1.75, false);


	//---------------------------------------------------------------------- DRUGI DEO

	Translate(pDC, 0, -37, false);
	pDC->SelectObject(&greenBrush);
	pDC->SelectObject(&blackPen);
	pDC->Ellipse(-12, -12, 12, 12); //kruzic

	XFORM drugiKruzic;
	pDC->GetWorldTransform(&drugiKruzic);

	Translate(pDC, 0, -37, false); //srednji tanji
	Scale(pDC, 1, 1.75, false);
	pDC->PlayMetaFile(m_hCactusPart, CRect(-10, -20, 10, 20));
	Scale(pDC, 1 / 1, 1 / 1.75, false);

	pDC->SetWorldTransform(&drugiKruzic);

	Rotate(pDC, -45, false); //drugi tanji levo
	Translate(pDC, 0, -37, false);
	XFORM leviKruzic;
	pDC->GetWorldTransform(&leviKruzic);
	Scale(pDC, 1, 1.75, false);
	pDC->PlayMetaFile(m_hCactusPart, CRect(-10, -20, 10, 20));
	Scale(pDC, 1 / 1, 1 / 1.75, false);

	pDC->SetWorldTransform(&drugiKruzic); //treci tanji desno
	Rotate(pDC, 45, false);
	Translate(pDC, 0, -37, false);
	XFORM desniKruzic;
	pDC->GetWorldTransform(&desniKruzic);
	Scale(pDC, 1, 1.75, false);
	pDC->PlayMetaFile(m_hCactusPart, CRect(-10, -20, 10, 20));
	Scale(pDC, 1 / 1, 1 / 1.75, false);
	

	//-------------------------------------------------------------------TRECI DEO -- LEVA STRANA

	pDC->SetWorldTransform(&leviKruzic);
	Translate(pDC, 0, -37, false);
	Rotate(pDC, 45, false);
	pDC->Ellipse(-12, -12, 12, 12); //kruzic
	XFORM lKruzic;
	pDC->GetWorldTransform(&lKruzic);
	Rotate(pDC, -90, false);
	Translate(pDC, 0, -37, false);
	Scale(pDC, 1.75, 1.75, false);
	pDC->PlayMetaFile(m_hCactusPart, CRect(-10, -20, 10, 20));
	Scale(pDC, 1 / 1.75, 1 / 1.75, false);

	pDC->SetWorldTransform(&lKruzic);

	Translate(pDC, 0, -37, false);
	Scale(pDC, 1.75, 1.75, false);
	pDC->PlayMetaFile(m_hCactusPart, CRect(-10, -20, 10, 20));

	Scale(pDC, 1 / 1.75, 1 / 1.75, false);
	Translate(pDC, 0, -37, false);
	pDC->Ellipse(-12, -12, 12, 12);
	//Scale(pDC, 1 / 1.75, 1 / 1.75, false);

	Translate(pDC, 0, -37, false);
	Scale(pDC, 3, 1.75, false);
	pDC->PlayMetaFile(m_hCactusPart, CRect(-10, -20, 10, 20));
	

	//------------------------------------------------------------------CETVRTI DEO - DESNA STRANA

	pDC->SetWorldTransform(&desniKruzic);
	Translate(pDC, 0, -37, false);
	pDC->Ellipse(-12, -12, 12, 12);

	XFORM rKruzic;
	pDC->GetWorldTransform(&rKruzic);
	Rotate(pDC, m_lightPartAngle2, false);

	Rotate(pDC, -45, false);
	Translate(pDC, 0, -37, false);
	Scale(pDC, 1.75, 1.75, false);

	

	pDC->PlayMetaFile(m_hCactusPartLight, CRect(-10, -20, 10, 20));
	Scale(pDC, 1 / 1.75, 1 / 1.75, false);

	pDC->SetWorldTransform(&rKruzic);
	
	Rotate(pDC, 45, false);
	Translate(pDC, 0, -37, false);
	Scale(pDC, 1.75, 1.75, false);
	pDC->PlayMetaFile(m_hCactusPart, CRect(-10, -20, 10, 20));
	Scale(pDC, 1 / 1.75, 1 / 1.75, false);


	//-------------------------------------------------------------------KRUZIC SKROOOOZ DESNO

	Translate(pDC, 0, -37, false);
	//Scale(pDC, 1 / 1.75, 1 / 1.75, false);
	pDC->Ellipse(-12, -12, 12, 12); //kruzic

	XFORM rrKruzic;
	pDC->GetWorldTransform(&rrKruzic);

	Rotate(pDC, -45, false);
	Translate(pDC, 0, -37, false);
	Scale(pDC, 1.75, 1.75, false);
	pDC->PlayMetaFile(m_hCactusPart, CRect(-10, -20, 10, 20));
	Scale(pDC, 1/1.75, 1/1.75, false);

	pDC->SetWorldTransform(&rrKruzic);

	Rotate(pDC, 45, false);
	Translate(pDC, 0, -37, false);
	Scale(pDC, 1.75, 1.75, false);
	pDC->PlayMetaFile(m_hCactusPart, CRect(-10, -20, 10, 20));
	Scale(pDC, 1 / 1.75, 1 / 1.75, false);



	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	pDC->SetWorldTransform(&oldXForm);
	
	drawGrid(pDC);
}

// Clab2View printing

BOOL Clab2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Clab2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Clab2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Clab2View diagnostics

#ifdef _DEBUG
void Clab2View::AssertValid() const
{
	CView::AssertValid();
}

void Clab2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Clab2Doc* Clab2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Clab2Doc)));
	return (Clab2Doc*)m_pDocument;
}
#endif //_DEBUG


// Clab2View message handlers

void Clab2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == 'G' || nChar == 'g')
	{
		m_showGid = !m_showGid;
		Invalidate();
	}

	if (nChar == 'N' || nChar == 'n')
	{
		m_lightPartAngle1 -= 5.0f;
		Invalidate();
	}

	if (nChar == 'M' || nChar == 'm')
	{
		m_lightPartAngle1 += 5.0f;
		Invalidate();
	}

	if (nChar == 'J' || nChar == 'j')
	{
		m_lightPartAngle2 -= 5.0f;
		Invalidate();
	}

	if (nChar == 'K' || nChar == 'k')
	{
		m_lightPartAngle2 += 5.0f;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
