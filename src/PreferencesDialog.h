
#pragma once

// PreferencesDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPreferencesDialog dialog

class CPreferencesDialog : public CDialog
{
// Construction
public:
	int m_nViewAngle;
	int m_nZoomFactor;
	int m_nJPGQuality;
	CPreferencesDialog(CWnd* pParent = NULL,int inmaxlevel=5,BOOL inshowtx=TRUE,BOOL inshowsummary=TRUE,int injpgquality=100,int inzoomfactor=4,int inviewangle=40,BOOL inblending=FALSE);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPreferencesDialog)
	enum { IDD = IDD_PREFERENCES };
	BOOL	m_bShowSummary;
	BOOL	m_bShowTexture;
	int		m_nRayTraceLevel;
	BOOL	m_bBlending;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreferencesDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPreferencesDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


