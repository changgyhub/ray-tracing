// Material.cpp : implementation file
//

#include "stdafx.h"
#include "ray.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "Material.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaterial dialog


CMaterial::CMaterial(CWnd* pParent /*=NULL*/,float *data)
	: CDialog(CMaterial::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMaterial)
	m_nShininess = (int)data[9];
	m_nOpacity = (int)(data[10]*100);
	m_nRefractiveIndex = (int)(data[11]*100);
	m_nReflectance = (int)(data[12]*100);
	//}}AFX_DATA_INIT

	m_nAmbient[0]=(int)(data[0]*255);
	m_nAmbient[1]=(int)(data[1]*255);
	m_nAmbient[2]=(int)(data[2]*255);
	m_nDiffuse[0]=(int)(data[3]*255);
	m_nDiffuse[1]=(int)(data[4]*255);
	m_nDiffuse[2]=(int)(data[5]*255);
	m_nSpecular[0]=(int)(data[6]*255);
	m_nSpecular[1]=(int)(data[7]*255);
	m_nSpecular[2]=(int)(data[8]*255);

	m_nPrevShininess=0;
	m_nPrevOpacity=0;
}


void CMaterial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);	
}


BEGIN_MESSAGE_MAP(CMaterial, CDialog)
	//{{AFX_MSG_MAP(CMaterial)
	ON_BN_CLICKED(IDC_AMBIENT, OnAmbient)
	ON_BN_CLICKED(IDC_DIFFUSE, OnDiffuse)
	ON_BN_CLICKED(IDC_SPECULAR, OnSpecular)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaterial message handlers

BOOL CMaterial::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CSliderCtrl *sc;
	sc=(CSliderCtrl*)GetDlgItem(IDC_SHININESS);
	sc->SetRange(1,128);
	sc->SetPos(m_nShininess);

	sc=(CSliderCtrl*)GetDlgItem(IDC_OPACITY);
	sc->SetRange(0,100);
	sc->SetPos(m_nOpacity);

	sc=(CSliderCtrl*)GetDlgItem(IDC_REFRACTIDX);
	sc->SetRange(100,500);
	sc->SetPos(m_nRefractiveIndex);

	sc=(CSliderCtrl*)GetDlgItem(IDC_REFLECTANCE);
	sc->SetRange(0,100);
	sc->SetPos(m_nReflectance);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CMaterial::OnAmbient() 
{
	CColorDialog *cdlg;
	COLORREF color;

	cdlg=new CColorDialog(RGB(m_nAmbient[0],m_nAmbient[1],m_nAmbient[2]),CC_ANYCOLOR | CC_RGBINIT | CC_FULLOPEN,this);

	if (cdlg->DoModal()==IDOK){
		color=cdlg->GetColor();
		m_nAmbient[0]=GetRValue(color);
		m_nAmbient[1]=GetGValue(color);
		m_nAmbient[2]=GetBValue(color);
	}
	delete cdlg;
	Invalidate();
}

void CMaterial::OnDiffuse() 
{
	CColorDialog *cdlg;
	COLORREF color;

	cdlg=new CColorDialog(RGB(m_nDiffuse[0],m_nDiffuse[1],m_nDiffuse[2]),CC_ANYCOLOR | CC_RGBINIT | CC_FULLOPEN,this);

	if (cdlg->DoModal()==IDOK){
		color=cdlg->GetColor();
		m_nDiffuse[0]=GetRValue(color);
		m_nDiffuse[1]=GetGValue(color);
		m_nDiffuse[2]=GetBValue(color);
	}
	delete cdlg;
	Invalidate();
}

void CMaterial::OnSpecular() 
{
	CColorDialog *cdlg;
	COLORREF color;

	cdlg=new CColorDialog(RGB(m_nSpecular[0],m_nSpecular[1],m_nSpecular[2]),CC_ANYCOLOR | CC_RGBINIT | CC_FULLOPEN,this);

	if (cdlg->DoModal()==IDOK){
		color=cdlg->GetColor();
		m_nSpecular[0]=GetRValue(color);
		m_nSpecular[1]=GetGValue(color);
		m_nSpecular[2]=GetBValue(color);
	}
	delete cdlg;
	Invalidate();
}

BOOL CMaterial::bSetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		  PFD_SUPPORT_OPENGL |          // support OpenGL
		  PFD_DOUBLEBUFFER,             // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int pixelformat;

	if ( (pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0 )
	{
		MessageBox("ChoosePixelFormat failed");
		return FALSE;
	}

	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		MessageBox("SetPixelFormat failed");
		return FALSE;
	}

	return TRUE;

}

void CMaterial::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	RedrawPreview();
}

int CMaterial::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	RECT rect;

	rect.left=13;
	rect.top=13;
	rect.right=263;
	rect.bottom=263;

	CWnd* wnd=new CWnd();
	wnd->Create(NULL,"Preview",WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE,rect,this,1000122,NULL);


	m_pDC = new CClientDC(wnd);

	ASSERT(m_pDC != NULL);

	if (!bSetupPixelFormat())
		return -1;

	m_hRC = wglCreateContext(m_pDC->GetSafeHdc());
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
	return 0;
}

void CMaterial::RedrawPreview()
{
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	glClearColor(.0,.0,.0,1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0,0,250,250);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,250,0,250);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	glColor4f(.3f,.3f,.3f,1.0f);
	glBegin(GL_QUADS);
		glVertex2i(0,0);
		glVertex2i(0,125);
		glVertex2i(125,125);
		glVertex2i(125,0);
		glVertex2i(125,125);
		glVertex2i(125,250);
		glVertex2i(250,250);
		glVertex2i(250,125);
	glEnd();
	glEnable(GL_LIGHTING);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(15,1,5,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,0,10,0,0,0,0,1,0);

	float flWhite[4]={1.0,1.0,1.0,1.0};
	float flPosition[4]={10,10,10.0,1.0};
	glLightfv(GL_LIGHT0,GL_POSITION,flPosition);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,flWhite);
	glLightfv(GL_LIGHT0,GL_SPECULAR,flWhite);
	glLightfv(GL_LIGHT0,GL_AMBIENT,flWhite);
	glEnable(GL_LIGHT0);


	float tmpcolor[4]={1.0,1.0,1.0,1.0};
	tmpcolor[3]=(float)m_nOpacity/100;
	tmpcolor[0]=(float)m_nAmbient[0]/255;
	tmpcolor[1]=(float)m_nAmbient[1]/255;
	tmpcolor[2]=(float)m_nAmbient[2]/255;
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,tmpcolor);
	tmpcolor[0]=(float)m_nDiffuse[0]/255;
	tmpcolor[1]=(float)m_nDiffuse[1]/255;
	tmpcolor[2]=(float)m_nDiffuse[2]/255;
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,tmpcolor);
	tmpcolor[0]=(float)m_nSpecular[0]/255;
	tmpcolor[1]=(float)m_nSpecular[1]/255;
	tmpcolor[2]=(float)m_nSpecular[2]/255;
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,tmpcolor);

	if (m_nShininess<=128) glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,(float)m_nShininess);
	else glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,128.0f);

	GLUquadricObj *qu;
	qu=gluNewQuadric();
	gluQuadricDrawStyle(qu,GLU_FILL);
	gluSphere(qu,1.0f,50,50);
	gluDeleteQuadric(qu);

	SwapBuffers(m_pDC->GetSafeHdc());
}

void CMaterial::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSliderCtrl *sc;
	sc=(CSliderCtrl*)GetDlgItem(IDC_SHININESS);
	m_nShininess=sc->GetPos();
	sc=(CSliderCtrl*)GetDlgItem(IDC_OPACITY);
	m_nOpacity=sc->GetPos();

	if (m_nPrevShininess!=m_nShininess){
		m_nPrevShininess=m_nShininess;
		RedrawPreview();
	} else if (m_nPrevOpacity!=m_nOpacity){
		m_nPrevOpacity=m_nOpacity;
		RedrawPreview();
	}
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMaterial::OnOK() 
{
	CSliderCtrl *sc;
	sc=(CSliderCtrl*)GetDlgItem(IDC_REFRACTIDX);
	m_nRefractiveIndex=sc->GetPos();
	sc=(CSliderCtrl*)GetDlgItem(IDC_REFLECTANCE);
	m_nReflectance=sc->GetPos();
	CDialog::OnOK();
}
