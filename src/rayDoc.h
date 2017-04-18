// rayDoc.h : interface of the CRayDoc class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <list>
using namespace std;

#include "v3.h"
#include "Texture.h"
typedef struct _CAMERAINFO{
	V3	viewpoint;			// viewpoint position
	V3	huv;				// head up vector (unit vector)
	V3	los;				// line of sight (unit vector)
	V3	rhv;				// right hand vector (unit vector)
	float	viewangle;		// view angle	
} CAMERAINFO;


class CRayDoc : public CDocument
{
protected: // create from serialization only
	CRayDoc();
	DECLARE_DYNCREATE(CRayDoc)

// Attributes
public:
	CAMERAINFO m_camera;

	list<LPPRIMITIVE>	m_objects;
	list<LPLIGHTSOURCE> m_lightsources;
	list<LPTEXTURE>		m_textures;

	list<LPPRIMITIVE>	m_selectedobjects;

	V3		m_BigBoxCorner[8];						// big bounding box corners of selected objects
	V3		m_RotateCentre;								// center of rotation	
	int		m_nSelectedLight;							// selected light's id, not actually used

private:
	long	m_nObjIDCount;								// object id count, used to find a unique id
	long	m_nLightIDCount;							// light id count, used to find a unique id

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRayDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ViewpointTwist(int sx, int ex);
	void ViewpointElevate(int sx,int ex,int sy,int ey);
	void ViewpointSlide(int sx,int ex,int sy,int ey);
	void ViewpointWalk(int sx,int ex,int sy,int ey);
	void SelectedObjectsRestoreM();
	CLightSource* SelectLightWithID(unsigned short id);
	CPrimitive* ObjectWithIDExists(unsigned short id);
	void SelectedObjectsBackupM();
	void MiscMaterialProp(float* data);
	void SelectedObjectsMiscMaterialProp(float *data);
	void SelectedLightColor(float *r,float *g, float* b);
	void LightColor(float r,float g,float b);
	void SelectOneMore(void* r);
	void UnSelectOne(void* r);
	void UnSelectAll();
	void SelectOne(void *r);
	void GenerateBigBoundingBox();
	void RemoveAll();
	void FindHUV();
	void FindLOS();
	void FindRHV();
	//void XProduct(float *u, float *v,float *w);
	//void Normalize(float *v);
	void RedrawScene();
	virtual ~CRayDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRayDoc)
	afx_msg void OnCone();
	afx_msg void OnCube();
	afx_msg void OnCylinder();
	afx_msg void OnEllipsoid();
	afx_msg void OnDodecahedron();
	afx_msg void OnIcosahedron();
	afx_msg void OnOctahedron();
	afx_msg void OnPryamid();
	afx_msg void OnTetrahedron();
	afx_msg void OnLight();
	afx_msg void OnDelete();
	afx_msg void OnSelectall();
	afx_msg void OnPmDullred();
	afx_msg void OnPmGlass();
	afx_msg void OnPmGolden();
	afx_msg void OnPmMirror();
	afx_msg void OnPmShinygreen();
	afx_msg void OnPmTransblue();
//	afx_msg void OnRotatex();
//	afx_msg void OnRotatey();
//	afx_msg void OnRotatez();
	afx_msg void OnRemovetrans();
	afx_msg void OnDuplicate();
	afx_msg void OnLightSwitch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
