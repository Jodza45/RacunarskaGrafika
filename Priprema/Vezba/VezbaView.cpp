
// VezbaView.cpp : implementation of the CVezbaView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Vezba.h"
#endif

#include "VezbaDoc.h"
#include "VezbaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define KOCKICA 25
// CVezbaView

IMPLEMENT_DYNCREATE(CVezbaView, CView)

BEGIN_MESSAGE_MAP(CVezbaView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CVezbaView construction/destruction

// Ucitavamo sve puzle

CVezbaView::CVezbaView() noexcept
{
	LoadPuzzlePieces();
}

CVezbaView::~CVezbaView()
{
	for (int i = 0; i < MAX_PIECES; i++) {
		delete puzzlePieces[i];
	}
}

BOOL CVezbaView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/// lab

void CVezbaView::LoadPuzzlePieces()
{
	// ucitavamo deo po deo slagalice pomocu DImage f-je Load

	char s[] = "1.bmp";
	for (int i = 0; i < MAX_PIECES; i++) {
		s[0] = '0' + (i + 1);
		puzzlePieces[i] = new DImage();
		puzzlePieces[i]->Load(CString(s));
	}
}

void CVezbaView::DrawCenter(CDC* pDC)
{
	CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
	CBrush brush(RGB(255, 0, 0));

	CPen* oldPen = pDC->SelectObject(&pen);
	CBrush* oldBrush = pDC->SelectObject(&brush);

	pDC->Ellipse(-4, -4, 4, 4);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void CVezbaView::DrawAxes(CDC* pDC)
{
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	CPen* oldPen = pDC->SelectObject(&pen);

	// X osa
	pDC->MoveTo(0, 0);
	pDC->LineTo(50, 0);

	CPen penB(PS_SOLID, 1, RGB(0, 0, 255));
	CPen* oldPenB = pDC->SelectObject(&penB);
	// Y osa
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, -50);

	pDC->SelectObject(oldPen);
	//pDC->SelectObject(oldPenB);
}

void CVezbaView::DrawTransparent(CDC* pDC, DImage* img, bool isBlue) {

	
	int w = img->Width();// sirana originalne slike, kad kazem slika mislim sa sve pozadion i puzlom
	int h = img->Height();// visina originalne slike

	CBitmap srcBitmap;// bitmapa za sliku u boji
	srcBitmap.CreateCompatibleBitmap(pDC, w, h);//

	CBitmap maskBitmap;
	maskBitmap.CreateBitmap(w, h, 1, 1, nullptr); // monohromatska bitmapa (crno-bela)

	CDC* srcDC = new CDC(); // kao da pravis virtuelni ekran u memoriji, za sliku u boji
	srcDC->CreateCompatibleDC(pDC);

	CDC* dstDC = new CDC(); // kao da pravis virtuelni ekran u memoriji, za masku
	dstDC->CreateCompatibleDC(pDC);

	CBitmap* oldSrcBitmap = srcDC->SelectObject(&srcBitmap);// ovo je isti djavo ko za olovku brush i to ovi stari se prave jer je kao dobra praksa
	CBitmap* oldDstBitmap = dstDC->SelectObject(&maskBitmap);

	//pravi src i dest CRect koji ce da crta puzzlePieces pojedinacno u srcDC
	img->Draw(srcDC, CRect(0, 0, w, h), CRect(0, 0, w, h));//Ucitavamo originalnu sliku (sa sve zelenom pozadinom) na srcDC.
	// deo puzzle koji je plav
	if (isBlue)
		makeBlue(&srcBitmap);
	else
		makeGray(&srcBitmap);

	// sad je svaki deo puzle ili sive ili plave boje ali ima pozadinu
	// sada uklanjamo pozadinu postupkom sa slajdova

	// 1 korak - kopiramo RGB bitmapu na monohromatsku -> pozadina bela (1), objekat crn (0)

	COLORREF trColor = srcDC->GetPixel(0, 0); // uzimamo piksel 0,0
	COLORREF oldBgColor = srcDC->SetBkColor(trColor); // stavljamo pozadinu na boju piksela 0,0

	dstDC->BitBlt(0, 0, w, h, srcDC, 0, 0, SRCCOPY);// u dst je pozadina bela a puzla crna

	// 2 korak - vrsimo AND operaciju, pri cemu je sad objekat RGB, a pozadina crna

	// za novi srcDC, pozadina ce se tretirati kao crna boja (0), objekat kao bela (1)
	COLORREF oldTextColorSrc = srcDC->SetTextColor(RGB(255, 255, 255));// ja ovde takoreci bojim puzlicu u belo
	COLORREF oldBackgroundColorSrc = srcDC->SetBkColor(RGB(0, 0, 0));// a pozadina koja je bila nekad zelena ide u crno

	srcDC->BitBlt(0, 0, w, h, dstDC, 0, 0, SRCAND);// e sad ja kad endujem belu masku u obliku puzle sa puzlom ja cu samo puzlu da izdvojim

	
	Translate(pDC, -w / 2, -h / 2); // pomeramo koordinatni pocetak u centar slike

	pDC->BitBlt(0, 0, w, h, dstDC, 0, 0, SRCAND); // u pDC - pozadina ce biti bele boje, objekat crne

	pDC->BitBlt(0, 0, w, h, srcDC, 0, 0, SRCPAINT); // "lepimo" srcDC preko pDC, SRCPAINT radi OR

	Translate(pDC, w / 2, h / 2); // vracamo koordinatni pocetak u prethodni polozaj
	//DrawCenter(pDC);

	//srcDC->SetTextColor(oldTextColorSrc);
	srcDC->SetBkColor(oldBgColor);

	srcDC->SelectObject(oldSrcBitmap);
	dstDC->SelectObject(oldDstBitmap);

	srcDC->DeleteDC();
	delete srcDC;

	dstDC->DeleteDC();
	delete dstDC;
}
void CVezbaView::makeGray(CBitmap* bitmap) {// finalni izlaz ove metode ti je puzla koja je pretvorena u sivkastu boju ali jos uvek ima zelenu pozadinu
	BITMAP b;
	bitmap->GetBitmap(&b);

	BYTE* bits = new BYTE[b.bmWidthBytes * b.bmHeight];// alociram bits
	bitmap->GetBitmapBits(b.bmWidthBytes * b.bmHeight, bits);// ovde iz bitmape prebacujem sve bitove slike u novo alocirani niz bits
	COLORREF trColor = RGB(bits[2], bits[1], bits[0]);// Dakle: bits[0] je Plava prvog piksela, 
													  //bits[1] Zelena, bits[2] Crvena. 
													  // postoji i neki cetvrti magicni za alfa zato nam je dole u for korak +=4.
													  //Logika ovde je: Gornji levi piksel (0,0) je sigurno pozadina. jer sa ovim bits[2], bits[1], bits[0] mi sigurno pristupamo prvom pikselu na slici gore levo
	for (int i = 0; i < b.bmWidthBytes * b.bmHeight; i += 4)// korak je 4 jer na svaka 4 bita pocinje novi piksel
	{
		if (RGB(bits[i + 2], bits[i + 1], bits[i]) == trColor) continue;// ako smo na pozadini onda preskacemo pozadinu, 
		BYTE gr = min(255, (bits[i] + bits[i + 1] + bits[i + 2]) / 3 + 64);// racunas
		bits[i] = bits[i + 1] = bits[i + 2] = gr;
	}

	bitmap->SetBitmapBits(b.bmWidthBytes * b.bmHeight, bits);

	if (bits)
		delete[] bits;

	bits = nullptr;
}
void CVezbaView::makeBlue(CBitmap* bitmap) {// finalni izlaz ove metode ti je puzla koja je pretvorena u plavicuastu boju ali jos uvek ima zelenu pozadinu

	BITMAP b;
	bitmap->GetBitmap(&b); // u promenljivoj b dobijamo informacije o izvornoj bitmapi

	// bmWidthBytes - svaka scan-linija (vrsta u bitmapi) mora imati dužinu koja je celobrojni umnožak 32 bita 
	// pr. 4,8,12,16... u jednom redu * visina = matrica piksela
	BYTE* bits = new BYTE[b.bmWidthBytes * b.bmHeight]; // niz vrednosti
	bitmap->GetBitmapBits(b.bmWidthBytes * b.bmHeight, bits);

	// uzimamo vr. prvog piksela koji ce da bude pozadina
	COLORREF trColor = RGB(bits[2], bits[1], bits[0]);// ovde uzimas zelenu

	for (int i = 0; i < b.bmWidthBytes * b.bmHeight; i += 4) {
		if (RGB(bits[i + 2], bits[i + 1], bits[i]) == trColor) continue;
		// max(gr) = 255, if bits[i] > gr => bits[i] = 255 else bits[i] = gr
		BYTE gr = min(255, (bits[i] + bits[i + 1] + bits[i + 2]) / 3 + 64);
		bits[i] = 0;// ubacujem u plavi kanal
		bits[i + 1] = gr;
		bits[i + 2] = 0; // B,G,R - 2 preostala kanala se setuju na 0
	}

	bitmap->SetBitmapBits(b.bmWidthBytes * b.bmHeight, bits);// SetBitmapBits: Do sada smo menjali samo niz u RAM memoriji. 
															//Slika na ekranu/objektu se nije promenila. 
															// Ovom komandom kopiramo izmenjene podatke nazad u bitmap objekat.

	if (bits)
		delete[] bits;

	bits = nullptr;
}

void CVezbaView::DrawGrid(CDC* pDC)
{
	CPen pen(PS_SOLID, 2, RGB(229, 229, 229));
	CPen* old = pDC->SelectObject(&pen);

	for (int i = 0; i <= 500; i += 25)
	{
		pDC->MoveTo(0, i);
		pDC->LineTo(500, i);

		pDC->MoveTo(i, 0);
		pDC->LineTo(i, 500);
	}
	pDC->SelectObject(old);
}

void CVezbaView::DrawBackground(COLORREF backgroundColor, CDC* pDC)
{
	// boji celu pozadinu u boju koju ce da prosledimo sa backgroundColor
	CBrush brush(backgroundColor);
	CRect client;
	GetClientRect(&client); // ovo uzima ceo prozor, bukvalno onaj prozor koji ti se pojavi kad pokrenes program e njega uzme bez oni dugmici i to

	pDC->FillRect(client, &brush);// ovo popunjava taj prozor sa cetkicom kojoj smo dali boju gore
}

void CVezbaView::Translate(CDC* pDC, float dx, float dy, bool rightMultiply)
{
	XFORM form = { 1, 0, 0, 1, dx, dy };
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CVezbaView::Scale(CDC* pDC, float sx, float sy, bool rightMultiply)
{
	XFORM form = { sx, 0, 0, sy, 0, 0 };
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CVezbaView::Mirror(CDC* pDC, bool mx, bool my, bool rightMyltiply)
{
	Scale(pDC, mx ? -1 : 1, my ? -1 : 1, rightMyltiply);
}

void CVezbaView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float sindeg = sin(angle);
	float cosdeg = cos(angle);
	XFORM form = { cosdeg, sindeg, -sindeg, cosdeg, 0, 0 };
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CVezbaView::DrawMem(CDC* pDC)
{
	DrawBackground(RGB(255, 255, 255), pDC); // bojimo pozadinu u belo
	DrawGrid(pDC); // crtamo grid

	XFORM tr;
	pDC->GetWorldTransform(&tr);

	// 0,0
	DrawCenter(pDC);
	DrawAxes(pDC);
	Translate(pDC, 4 * KOCKICA + 5, 4 * KOCKICA + 5);
	DrawCenter(pDC);
	DrawAxes(pDC);
	Mirror(pDC, false, true);
	Rotate(pDC, -PI / 6 + RAD(5));
	DrawCenter(pDC);
	DrawAxes(pDC);
	DrawCenter(pDC);
	DrawAxes(pDC);
	DrawTransparent(pDC, puzzlePieces[0], false);
	pDC->SetWorldTransform(&tr);

	// 0,1

	Translate(pDC, 10 * KOCKICA + 9, 4 * KOCKICA + 1);
	Rotate(pDC, -PI / 4 - RAD(10));
	Mirror(pDC, false, true);
	DrawTransparent(pDC, puzzlePieces[6], false);
	pDC->SetWorldTransform(&tr);

	// 0,2

	Translate(pDC, 16 * KOCKICA + 9, 4 * KOCKICA - 6);
	Rotate(pDC, PI / 2 - RAD(14));
	Mirror(pDC, true, false);
	DrawTransparent(pDC, puzzlePieces[4], false);
	pDC->SetWorldTransform(&tr);

	// 1,0

	Translate(pDC, 4 * KOCKICA - 7, 10 * KOCKICA + 5);
	Rotate(pDC, -PI / 2 - RAD(17));
	Mirror(pDC, true, false);
	DrawTransparent(pDC, puzzlePieces[8], false);
	pDC->SetWorldTransform(&tr);

	// 1,1

	Translate(pDC, 10 * KOCKICA - 7, 10 * KOCKICA - 3);
	Rotate(pDC, -PI / 3 + RAD(2));
	Mirror(pDC, true, false);
	DrawTransparent(pDC, puzzlePieces[1], false);
	pDC->SetWorldTransform(&tr);

	// 1,2

	Translate(pDC, 16 * KOCKICA -6, 10 * KOCKICA - 1);
	Rotate(pDC, -PI / 2 + RAD(18));
	Mirror(pDC, true, false);
	DrawTransparent(pDC, puzzlePieces[2], false);
	pDC->SetWorldTransform(&tr);

	// 2,0

	Translate(pDC, 4 * KOCKICA - 7, 16 * KOCKICA -6);
	Rotate(pDC, -PI / 10 - RAD(1));
	Mirror(pDC, true, false);
	DrawTransparent(pDC, puzzlePieces[3], false);
	pDC->SetWorldTransform(&tr);

	// 2,1

	Translate(pDC, 10 * KOCKICA  -5, 16 * KOCKICA +6);
	Rotate(pDC, -PI / 2 - RAD(23));
	Mirror(pDC, true, false);
	DrawTransparent(pDC, puzzlePieces[5], true);
	pDC->SetWorldTransform(&tr);

	// 2,2

	Translate(pDC, 16 * KOCKICA + 3, 16 * KOCKICA + 8);
	Rotate(pDC, -PI + RAD(19));
	Mirror(pDC, true, false);
	DrawTransparent(pDC, puzzlePieces[7], false);
	pDC->SetWorldTransform(&tr);

}


void CVezbaView::OnDraw(CDC* pDC)
{
	CVezbaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);

	int pDCmode = pDC->SetGraphicsMode(GM_ADVANCED);

	CDC* memDC = new CDC(); // kontekst koji sluzi za crtanje u memoriju
	memDC->CreateCompatibleDC(pDC);

	int memDCmode = memDC->SetGraphicsMode(GM_ADVANCED);

	CBitmap memBitmap; // cuva sliku u vidu piksela, sluzi za crtanje u memoriju
	memBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* old = memDC->SelectObject(&memBitmap);

	DrawMem(memDC);

	//Translate(pDC, 250, 250);
	//Rotate(pDC, rotation * PI / 2);
	//Mirror(pDC, mx, my);
	//Translate(pDC, -250, -250);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);

	pDC->SetGraphicsMode(pDCmode);

	memDC->SetGraphicsMode(memDCmode);
	memDC->SelectObject(old);
	memDC->DeleteDC();
	delete memDC;
}

// CVezbaView printing

BOOL CVezbaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CVezbaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CVezbaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CVezbaView diagnostics

#ifdef _DEBUG
void CVezbaView::AssertValid() const
{
	CView::AssertValid();
}

void CVezbaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVezbaDoc* CVezbaView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVezbaDoc)));
	return (CVezbaDoc*)m_pDocument;
}
#endif //_DEBUG


// CVezbaView message handlers
