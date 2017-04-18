// PreferencesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ray.h"
#include "PreferencesDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreferencesDialog dialog


CPreferencesDialog::CPreferencesDialog(CWnd* pParent /*=NULL*/,int inmaxlevel,BOOL inshowtx,BOOL inshowsummary,int injpgquality,int inzoomfactor,int inviewangle,BOOL inblending)
	: CDialog(CPreferencesDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPreferencesDialog)
	m_bShowSummary = inshowsummary;
	m_bShowTexture = inshowtx;
	m_nRayTraceLevel = inmaxlevel;
	m_nJPGQuality = injpgquality;
	m_nZoomFactor = inzoomfactor;
	m_nViewAngle = inviewangle;
	m_bBlending = inblending;
	//}}AFX_DATA_INIT
}


void CPreferencesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferencesDialog)
	DDX_Check(pDX, IDC_SHOWSUMMARY, m_bShowSummary);
	DDX_Check(pDX, IDC_SHOWTX, m_bShowTexture);
	DDX_Check(pDX, IDC_BLENDING, m_bBlending);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPreferencesDialog, CDialog)
	//{{AFX_MSG_MAP(CPreferencesDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferencesDialog message handlers

BOOL CPreferencesDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CSliderCtrl *sc;
	sc=(CSliderCtrl*)GetDlgItem(IDC_RTLEVEL);
	sc->SetRange(0,20);
	sc->SetPos(m_nRayTraceLevel);
	
	sc=(CSliderCtrl*)GetDlgItem(IDC_JPGQUALITY);
	sc->SetRange(1,100);
	sc->SetTicFreq(10);
	sc->SetPos(m_nJPGQuality);
	
	sc=(CSliderCtrl*)GetDlgItem(IDC_ZOOMFACT);
	sc->SetRange(1,5);
	sc->SetPos(m_nZoomFactor);
	
	sc=(CSliderCtrl*)GetDlgItem(IDC_VIEWANGLE);
	sc->SetRange(10,120);
	sc->SetTicFreq(10);
	sc->SetPos(m_nViewAngle);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreferencesDialog::OnOK() 
{
	CSliderCtrl *sc;
	sc=(CSliderCtrl*)GetDlgItem(IDC_RTLEVEL);
	m_nRayTraceLevel=sc->GetPos();
	sc=(CSliderCtrl*)GetDlgItem(IDC_JPGQUALITY);
	m_nJPGQuality=sc->GetPos();
	sc=(CSliderCtrl*)GetDlgItem(IDC_ZOOMFACT);
	m_nZoomFactor=sc->GetPos();
	sc=(CSliderCtrl*)GetDlgItem(IDC_VIEWANGLE);
	m_nViewAngle=sc->GetPos();
	
	CDialog::OnOK();
}
