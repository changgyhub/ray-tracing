// ray.h : main header file for the RAY application
//

#pragma once

enum {OBJ_DUMMY1=0,OBJ_DUMMY2=1,OBJ_CUBE=2,OBJ_PYR=3,OBJ_ELLI=4,OBJ_CONE=5,OBJ_CYL=6,OBJ_ICOSA=7,OBJ_TETRA=8,OBJ_OCTA=9,OBJ_DODECA=10};  //object types
enum {IOB_SELECT,IOB_TRI,IOB_QUADR};	// intrinic object types
enum INTERFACE_MODE {MODE_NAVIGATE=0,MODE_SELECT,MODE_ZOOM};	// interface modes

#define MAX_DIST 1000000
#define TOLERANCE 0.001
//#define DotProduct(x,y)		(x[0]*y[0]+x[1]*y[1]+x[2]*y[2])
//#define Magnitude(v)		((float)sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]))
#define M_PI 3.141592654f


#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRayApp:
// See ray.cpp for the implementation of this class
//

class CRayApp : public CWinApp
{
public:
	CRayApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRayApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CRayApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

