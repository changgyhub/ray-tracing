// rayView.h : interface of the CRayView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "v3.h"
#include "ray.h"
typedef struct _VIEWINFO{
	V3		llp;				// visible region of the projection plane, lower left corner, world coordinate
	V3		lrp;				// visible region of the projection plane, lower right corner, world coordinate
	V3		ulp;				// visible region of the projection plane, upper left corner, world coordinate
	V3		urp;				// visible region of the projection plane, upper left corner, world coordinate
	int		sx1,sy1,sx2,sy2;	// select window screen coordinates
} VIEWINFO;

void MatrixMultVector4(float *m,float *v,float *rv);
void MatrixMultVector3(float *m,float *v,float *rv);

class CRayView : public CView
{

public:
	//////////////////////////////////////////////////////////////
	// assignment related functions
	// add code to the following 6 functions only

	void UserLeftButtonDown(int pickedid, int x, int y);		// called whenever the left mouse button is pressed
	void UserLeftButtonUp(int x, int y);						// called whenever the left mouse button is released
	void UserRightButtonDown(int pickedid, int x, int y);		// called whenever the right mouse button is pressed
	void UserRightButtonUp(int x, int y);						// called whenever the right mouse button is released
	void UserMouseMove(int x, int y);							// called whenever the mouse is moved over the 3D area
	void UserDrawControlHandle();								// custom control handle drawing, called in DrawScene function

	
	//////////////////////////////////////////////////////////////
	// control mode flags
	bool	m_bRotateObj;								// indicates object rotating mode
	bool	m_bMoveObj;									// indicates object moving mode
	bool	m_bRotateView;								// rotate view point about selected objects
	bool	m_bResize;									// resize objects




protected: // create from serialization only
	CRayView();
	DECLARE_DYNCREATE(CRayView)

// Attributes
public:
	CRayDoc* GetDocument();

	////////////////////////////////
	// interface related properties
	int		m_nLeft,m_nTop,m_nWidth,m_nHeight;			// windows position and size
	bool	m_bLButDown,m_bRButDown;					// mouse button down
	int		m_nSx,m_nSy,m_nEx,m_nEy,m_nCx,m_nCy,m_nPx,m_nPy,m_nDx,m_nDy;	// mouse positions S=start,E=end,C=current,P=previous,D=delta
	int		m_nRSx,m_nRSy,m_nREx,m_nREy,m_nRCx,m_nRCy,m_nRPx,m_nRPy; // right mouse positions
	
	HGLRC	m_hRC;										// rendering context
	HDC		m_hDC;										// device context

	VIEWINFO	m_viewinfo;

	CAMERAINFO	m_oldcamera;
	VIEWINFO	m_oldviewinfo;
	
	bool	m_bWinSelect;								// 'window' object select mode
	
	bool	m_bElevate;									// look up or down
	bool	m_bMove;									// moving forward or backward
	bool	m_bTwist;									// turn about line of sight
	bool	m_bSlide;									// move with view direction unchange	

	bool	m_bRender;									// indicate a render command is issued
	bool	m_bSelRenderWindow;							// indicates next mouse drag is to select render window

	bool	m_bMoveLight;								// move light source
		
	bool	m_bShowSummary;								// indicates whether shows summary after ray trace
	bool	m_bShowTexture;								// indicated whether shows texture in preview
	bool	m_bBlending;								// Alpha blending

	V3	m_flRLLP,m_flRLRP,m_flRULP,m_flRURP;// corners of projection region in 3D of real render area
	int		m_nRLeft,m_nRBottom,m_nRWidth,m_nRHeight;	// render area dimension in pixels
	unsigned long	*m_pULMap;							// pointer to memory

	// scene data structure
	INTERFACE_MODE		m_nCurrentMode;								// current mode, either navigate mode or select mode	
	
	CClientDC	*m_pDC;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRayView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_nZoomFactor;
	CString m_strRenderToFilename;
	bool m_bRenderToFile;
	int m_nJPGQuality;
	void OnUserNewDoc();
	void FindCorners();
	void WindowSelect(int x1,int y1,int x2,int y2);
	void RestoreViewpoint();
	void BackupViewpoint();
	int PickObject(int mx,int my);
	void DrawSelBox();
	void Render();
	void Init();
	void DrawScene();
	void CreateRGBPalette();
	unsigned char ComponentFromIndex(int i, UINT nbits, UINT shift);
	BOOL bSetupPixelFormat();
	virtual ~CRayView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:


// Generated message map functions
protected:
	//{{AFX_MSG(CRayView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnSelect();
	afx_msg void OnNavigate();
	afx_msg void OnUpdateSelect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNavigate(CCmdUI* pCmdUI);	
	afx_msg void OnMiscmat();
	afx_msg void OnPreferences();
	afx_msg void OnImportTx();	
	afx_msg void OnZoom();
	afx_msg void OnUpdateZoom(CCmdUI* pCmdUI);
	afx_msg void OnRemovetx();
	afx_msg void OnLightColor();
	afx_msg void OnExport();
	afx_msg void OnRender();
	afx_msg void OnRenderWindow();
	afx_msg void OnRenderfile();
	afx_msg void OnRerender();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in rayView.cpp
inline CRayDoc* CRayView::GetDocument()
   { return (CRayDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
