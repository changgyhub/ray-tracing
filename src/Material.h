#pragma once

// Material.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMaterial dialog

class CMaterial : public CDialog
{
// Construction
public:
	void RedrawPreview();
	BOOL bSetupPixelFormat();
	int m_nSpecular[3];
	int m_nDiffuse[3];
	int m_nAmbient[3];
	CMaterial(CWnd* pParent = NULL,float *data=NULL);   // standard constructor
	CClientDC *m_pDC;
	HGLRC	m_hRC;										// rendering context
	HDC		m_hDC;										// device context
	int		m_nPrevShininess;
	int		m_nPrevOpacity;

// Dialog Data
	//{{AFX_DATA(CMaterial)
	enum { IDD = IDD_MATERIAL };
	int		m_nShininess;
	int		m_nOpacity;
	int		m_nRefractiveIndex;
	int		m_nReflectance;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaterial)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMaterial)
	virtual BOOL OnInitDialog();
	afx_msg void OnAmbient();
	afx_msg void OnDiffuse();
	afx_msg void OnSpecular();
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
