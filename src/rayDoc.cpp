// rayDoc.cpp : implementation of the CRayDoc class
//

#include "stdafx.h"
#include "gl/gl.h"
#include "ray.h"
#include "Texture.h"
#include "LightSource.h"
#include "Vertex.h"
#include "Primitive.h"
#include "rayDoc.h"
#include "rayView.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRayDoc

IMPLEMENT_DYNCREATE(CRayDoc, CDocument)

BEGIN_MESSAGE_MAP(CRayDoc, CDocument)
	//{{AFX_MSG_MAP(CRayDoc)
	ON_COMMAND(ID_CONE, OnCone)
	ON_COMMAND(ID_CUBE, OnCube)
	ON_COMMAND(ID_CYLINDER, OnCylinder)
	ON_COMMAND(ID_ELLIPSOID, OnEllipsoid)
	ON_COMMAND(ID_DODECAHEDRON, OnDodecahedron)
	ON_COMMAND(ID_ICOSAHEDRON, OnIcosahedron)
	ON_COMMAND(ID_OCTAHEDRON, OnOctahedron)
	ON_COMMAND(ID_PRYAMID, OnPryamid)
	ON_COMMAND(ID_TETRAHEDRON, OnTetrahedron)
	ON_COMMAND(ID_LIGHT, OnLight)
	ON_COMMAND(ID_DELETE, OnDelete)
	ON_COMMAND(ID_SELECTALL, OnSelectall)
	ON_COMMAND(ID_PM_DULLRED, OnPmDullred)
	ON_COMMAND(ID_PM_GLASS, OnPmGlass)
	ON_COMMAND(ID_PM_GOLDEN, OnPmGolden)
	ON_COMMAND(ID_PM_MIRROR, OnPmMirror)
	ON_COMMAND(ID_PM_SHINYGREEN, OnPmShinygreen)
	ON_COMMAND(ID_PM_TRANSBLUE, OnPmTransblue)
//	ON_COMMAND(ID_ROTATEX, OnRotatex)
//	ON_COMMAND(ID_ROTATEY, OnRotatey)
//	ON_COMMAND(ID_ROTATEZ, OnRotatez)
	ON_COMMAND(ID_REMOVETRANS, OnRemovetrans)
	ON_COMMAND(ID_DUPLICATE, OnDuplicate)
	ON_COMMAND(ID_LIGHT_SWITCH, OnLightSwitch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRayDoc construction/destruction

CRayDoc::CRayDoc()
{
	m_camera.viewpoint=V3(.0f,.0f,10.0f);
	m_camera.los=V3(.0f,.0f,-1.0f);
	m_camera.huv=V3(.0f,1.0f,.0f);
	m_camera.rhv=V3(1.0f,.0f,.0f);
	m_camera.viewangle=40;

	m_nObjIDCount=1;
	m_nLightIDCount=1;
	
	CLightSource *s=new CLightSource(20,20,20,1.0,1.0,1.0,m_nLightIDCount++);
	m_lightsources.push_back(s);

	m_nSelectedLight=0;	// 0 means no light is selected
}

CRayDoc::~CRayDoc()
{
	RemoveAll();
}

BOOL CRayDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	RemoveAll();

	m_camera.viewpoint=V3(.0f,.0f,10.0f);
	m_camera.los=V3(.0f,.0f,-1.0f);
	m_camera.huv=V3(.0f,1.0f,.0f);
	m_camera.rhv=V3(1.0f,.0f,.0f);
	m_camera.viewangle=40;

	m_nObjIDCount=1;
	m_nLightIDCount=1;
	
	CLightSource *s=new CLightSource(20,20,20,1.0,1.0,1.0,m_nLightIDCount++);
	m_lightsources.push_back(s);

	POSITION pos=GetFirstViewPosition();	
	CRayView* pView = (CRayView*)GetNextView(pos);
	::PostMessage(pView->m_hWnd,WM_USER+1,0,0);
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRayDoc serialization

void CRayDoc::Serialize(CArchive& ar)
{
	char c[200];
	int nObjCnt,nLightCnt,nTxCnt,i,j;
	CPrimitive	*pPrim;
	CTexture *pTexture;
	CLightSource *pLight;

	if (ar.IsStoring())
	{
		nObjCnt=m_objects.size();
		if (nObjCnt==0){
			AfxMessageBox("Cannot save empty scene.",MB_ICONEXCLAMATION | MB_OK);
			return;
		}

		ar.WriteString("RAY150\15\12");		// signature

		CString cstr=CString();
		cstr.Format("%f %f %f // view point\15\12",m_camera.viewpoint[0],m_camera.viewpoint[1],m_camera.viewpoint[2]);
		ar.WriteString((LPCTSTR)cstr);
		cstr.Format("%f %f %f // line of sight\15\12",m_camera.los[0],m_camera.los[1],m_camera.los[2]);
		ar.WriteString((LPCTSTR)cstr);
		cstr.Format("%f %f %f // head up vector\15\12",m_camera.huv[0],m_camera.huv[1],m_camera.huv[2]);
		ar.WriteString((LPCTSTR)cstr);
		cstr.Format("%f %f %f // right hand vector\15\12",m_camera.rhv[0],m_camera.rhv[1],m_camera.rhv[2]);
		ar.WriteString((LPCTSTR)cstr);
		cstr.Format("%f // vertical view angle\15\12",m_camera.viewangle);
		ar.WriteString((LPCTSTR)cstr);

		nLightCnt=m_lightsources.size();
		cstr.Format("%d // number of light sources\15\12",nLightCnt);
		ar.WriteString((LPCTSTR)cstr);

		for(list<LPLIGHTSOURCE>::iterator li=m_lightsources.begin(); li!=m_lightsources.end(); li++){
			cstr.Format("%f %f %f // light source position\15\12",(*li)->position.x,(*li)->position.y,(*li)->position.z);
			ar.WriteString((LPCTSTR)cstr);
			cstr.Format("%f %f %f // light source color\15\12",(*li)->color.r,(*li)->color.g,(*li)->color.b);
			ar.WriteString((LPCTSTR)cstr);
			cstr.Format("%d // light source status\15\12",(*li)->m_bON);
			ar.WriteString((LPCTSTR)cstr);
		}

		cstr.Format("%d // number of objects\15\12",nObjCnt);
		ar.WriteString((LPCTSTR)cstr);

		for(list<LPPRIMITIVE>::iterator oi=m_objects.begin(); oi!=m_objects.end(); oi++){
			cstr.Format("%d // type\15\12",(*oi)->m_nType);
			ar.WriteString((LPCTSTR)cstr);

			cstr.Format("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f // transform M\15\12",
				(*oi)->m_matrix[0],(*oi)->m_matrix[1],(*oi)->m_matrix[2],(*oi)->m_matrix[3],(*oi)->m_matrix[4],(*oi)->m_matrix[5],(*oi)->m_matrix[6],(*oi)->m_matrix[7],
				(*oi)->m_matrix[8],(*oi)->m_matrix[9],(*oi)->m_matrix[10],(*oi)->m_matrix[11],(*oi)->m_matrix[12],(*oi)->m_matrix[13],(*oi)->m_matrix[14],(*oi)->m_matrix[15]);
			ar.WriteString((LPCTSTR)cstr);

			// material settings
			cstr.Format("%f %f %f // ambient\15\12",(*oi)->m_Ambient[0],(*oi)->m_Ambient[1],(*oi)->m_Ambient[2]);
			ar.WriteString((LPCTSTR)cstr);
			cstr.Format("%f %f %f // diffuse\15\12",(*oi)->m_Diffuse[0],(*oi)->m_Diffuse[1],(*oi)->m_Diffuse[2]);
			ar.WriteString((LPCTSTR)cstr);
			cstr.Format("%f %f %f // specular\15\12",(*oi)->m_Specular[0],(*oi)->m_Specular[1],(*oi)->m_Specular[2]);
			ar.WriteString((LPCTSTR)cstr);

			cstr.Format("%f // shininess\15\12",(*oi)->m_Shininess);
			ar.WriteString((LPCTSTR)cstr);
			cstr.Format("%f // refractive index\15\12",(*oi)->m_RefrIndex);
			ar.WriteString((LPCTSTR)cstr);
			cstr.Format("%f // reflectance\15\12",(*oi)->m_Reflectance);
			ar.WriteString((LPCTSTR)cstr);
			cstr.Format("%f // opacity\15\12",(*oi)->m_Opacity);
			ar.WriteString((LPCTSTR)cstr);

			pTexture=(CTexture*)(*oi)->m_pTX;
			if (pTexture!=NULL){
				j=pTexture->m_strFilename.ReverseFind('\\');
				if (j==-1) j=pTexture->m_strFilename.ReverseFind('/');
				if (j==-1){
					cstr.Format("%s\15\12",LPCTSTR(pTexture->m_strFilename));
				} else {
					cstr.Format("%s\15\12",pTexture->m_strFilename.Mid(j+1));
				}
				ar.WriteString((LPCTSTR)cstr);
			} else {
				cstr.Format("\15\12");
				ar.WriteString((LPCTSTR)cstr);
			}
			cstr.Format("\15\12");
			ar.WriteString((LPCTSTR)cstr);
		}
		
		ar.WriteString("RAY150_EOF\15\12");		// signature

		SetModifiedFlag(FALSE);
	}
	else
	{
		ar.ReadString(c,100);			// read signature
		CString cstr=CString(c);
		if (cstr.Left(6)!=_T("RAY150")){
			AfxMessageBox("Unmatched scene file, open abort.\nSignature missing",MB_ICONEXCLAMATION | MB_OK);
			return;
		} else {
			int tmpi1;
			float tmpf1,tmpf2,tmpf3,tmpM[16];
			CTexture	*t,*rt;

			RemoveAll();
			POSITION pos=GetFirstViewPosition();	
			CRayView* pView = (CRayView*)GetNextView(pos);
			::PostMessage(pView->m_hWnd,WM_USER+1,0,0);

			for(list<LPLIGHTSOURCE>::iterator li=m_lightsources.begin(); li!=m_lightsources.end(); li++){
				delete (*li);
			}
			m_lightsources.clear();


			ar.ReadString(c,100);
			sscanf_s(c,"%f %f %f",&m_camera.viewpoint[0],&m_camera.viewpoint[1],&m_camera.viewpoint[2]);
			ar.ReadString(c,100);
			sscanf_s(c,"%f %f %f",&m_camera.los[0],&m_camera.los[1],&m_camera.los[2]);
			ar.ReadString(c,100);
			sscanf_s(c,"%f %f %f",&m_camera.huv[0],&m_camera.huv[1],&m_camera.huv[2]);
			ar.ReadString(c,100);
			sscanf_s(c,"%f %f %f",&m_camera.rhv[0],&m_camera.rhv[1],&m_camera.rhv[2]);
			ar.ReadString(c,100);	// read windows size in old format
			sscanf_s(c,"%f",&m_camera.viewangle);


			ar.ReadString(c,100);
			sscanf_s(c,"%d",&nLightCnt);

			for (i=0;i<nLightCnt;i++){
				ar.ReadString(c,100);
				sscanf_s(c,"%f %f %f",&tmpf1,&tmpf2,&tmpf3);
				pLight=new CLightSource(tmpf1,tmpf2,tmpf3,m_nLightIDCount++);
				ar.ReadString(c,100);
				sscanf_s(c,"%f %f %f",&tmpf1,&tmpf2,&tmpf3);
				pLight->SetColor(tmpf1,tmpf2,tmpf3);
				ar.ReadString(c,100);
				sscanf_s(c,"%d",&tmpi1);
				if (tmpi1==0){
					pLight->TurnOff();
				} else {
					pLight->TurnOn();
				}
				m_lightsources.push_back(pLight);
			}

			ar.ReadString(c,100);
			sscanf_s(c,"%d",&nObjCnt);

			for (i=0;i<nObjCnt;i++){
				ar.ReadString(c,100);
				sscanf_s(c,"%d",&tmpi1);
				pPrim=new CPrimitive(tmpi1,m_nObjIDCount++);

				ar.ReadString(c,200);
				sscanf_s(c,"%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
						tmpM,tmpM+1,tmpM+2,tmpM+3,tmpM+4,tmpM+5,tmpM+6,tmpM+7,
						tmpM+8,tmpM+9,tmpM+10,tmpM+11,tmpM+12,tmpM+13,tmpM+14,tmpM+15);

				ar.ReadString(c,100);
				sscanf_s(c,"%f %f %f",&pPrim->m_Ambient[0],&pPrim->m_Ambient[1],&pPrim->m_Ambient[2]);
				ar.ReadString(c,100);
				sscanf_s(c,"%f %f %f",&pPrim->m_Diffuse[0],&pPrim->m_Diffuse[1],&pPrim->m_Diffuse[2]);
				ar.ReadString(c,100);
				sscanf_s(c,"%f %f %f",&pPrim->m_Specular[0],&pPrim->m_Specular[1],&pPrim->m_Specular[2]);

				ar.ReadString(c,100);
				sscanf_s(c,"%f",&pPrim->m_Shininess);
				ar.ReadString(c,100);
				sscanf_s(c,"%f",&pPrim->m_RefrIndex);
				ar.ReadString(c,100);
				sscanf_s(c,"%f",&pPrim->m_Reflectance);
				ar.ReadString(c,100);
				sscanf_s(c,"%f",&pPrim->m_Opacity);
				pPrim->m_Ambient[3]=pPrim->m_Opacity;
				pPrim->m_Diffuse[3]=pPrim->m_Opacity;
				pPrim->m_Specular[3]=pPrim->m_Opacity;

				pPrim->LoadM(tmpM);

				ar.ReadString(c,100);

				CString c1=CString(c);
				if (c[0]!=0){	// read texture
					rt=NULL;
					nTxCnt=m_textures.size();
					for(list<LPTEXTURE>::iterator ti=m_textures.begin(); ti!=m_textures.end(); ti++){
						if (c1==(*ti)->m_strFilename){
							rt=(*ti);
							break;
						}
					}

					if (rt==NULL){
						t=new CTexture(c1);

						int j=ar.m_strFileName.ReverseFind('\\');
						if (j==-1) j=ar.m_strFileName.ReverseFind('/');
						if (j!=-1){
							c1=ar.m_strFileName.Left(j+1)+c1;
						}


						if (t->Read(c1)==1){
							AfxMessageBox("Cannot open texture file",MB_OK | MB_ICONEXCLAMATION);
							rt=NULL;
						} else {
							m_textures.push_back(t);
							rt=t;
						}
					}
					pPrim->m_pTX=rt;
				} else {
					pPrim->m_pTX=NULL;
				}

				ar.ReadString(c,100);

				m_objects.push_back(pPrim);

			}


		}
	
		SetModifiedFlag(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRayDoc diagnostics

#ifdef _DEBUG
void CRayDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRayDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRayDoc commands

void CRayDoc::RedrawScene()
{
	POSITION pos=GetFirstViewPosition();	
	CRayView* pView = (CRayView*)GetNextView(pos);
	::PostMessage(pView->m_hWnd,WM_USER+1,0,0);
}

void CRayDoc::FindRHV()
{
	m_camera.rhv=m_camera.los.cross(m_camera.huv);
	m_camera.rhv.normalize();
}

void CRayDoc::FindLOS()
{
	m_camera.los=m_camera.huv.cross(m_camera.rhv);
	m_camera.los.normalize();
}

void CRayDoc::FindHUV()
{
	m_camera.huv=m_camera.rhv.cross(m_camera.los);
	m_camera.huv.normalize();
}

void CRayDoc::RemoveAll()
{
	for(list<LPPRIMITIVE>::iterator oi=m_objects.begin(); oi!=m_objects.end(); oi++) delete (*oi);
	m_objects.clear();

	for(list<LPLIGHTSOURCE>::iterator li=m_lightsources.begin(); li!=m_lightsources.end(); li++) delete (*li);
	m_lightsources.clear();

	for(list<LPTEXTURE>::iterator ti=m_textures.begin(); ti!=m_textures.end(); ti++) delete (*ti);
	m_textures.clear();
}

void CRayDoc::OnCone() 
{
	CPrimitive *pPrim=new CPrimitive(OBJ_CONE,m_nObjIDCount++);
	pPrim->SetOffset(m_camera.viewpoint[0]+m_camera.los[0]*10,m_camera.viewpoint[1]+m_camera.los[1]*10,m_camera.viewpoint[2]+m_camera.los[2]*10);
	m_objects.push_back(pPrim);
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::OnCube() 
{
	CPrimitive *pPrim=new CPrimitive(OBJ_CUBE,m_nObjIDCount++);
	pPrim->SetOffset(m_camera.viewpoint[0]+m_camera.los[0]*10,m_camera.viewpoint[1]+m_camera.los[1]*10,m_camera.viewpoint[2]+m_camera.los[2]*10);
	m_objects.push_back(pPrim);
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::OnCylinder() 
{
	CPrimitive *pPrim=new CPrimitive(OBJ_CYL,m_nObjIDCount++);
	pPrim->SetOffset(m_camera.viewpoint[0]+m_camera.los[0]*10,m_camera.viewpoint[1]+m_camera.los[1]*10,m_camera.viewpoint[2]+m_camera.los[2]*10);
	m_objects.push_back(pPrim);
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::OnEllipsoid() 
{
	CPrimitive *pPrim=new CPrimitive(OBJ_ELLI,m_nObjIDCount++);
	pPrim->SetOffset(m_camera.viewpoint[0]+m_camera.los[0]*10,m_camera.viewpoint[1]+m_camera.los[1]*10,m_camera.viewpoint[2]+m_camera.los[2]*10);
	m_objects.push_back(pPrim);
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::OnDodecahedron() 
{
	CPrimitive *pPrim=new CPrimitive(OBJ_DODECA,m_nObjIDCount++);
	pPrim->SetOffset(m_camera.viewpoint[0]+m_camera.los[0]*10,m_camera.viewpoint[1]+m_camera.los[1]*10,m_camera.viewpoint[2]+m_camera.los[2]*10);
	m_objects.push_back(pPrim);
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::OnIcosahedron() 
{
	CPrimitive *pPrim=new CPrimitive(OBJ_ICOSA,m_nObjIDCount++);
	pPrim->SetOffset(m_camera.viewpoint[0]+m_camera.los[0]*10,m_camera.viewpoint[1]+m_camera.los[1]*10,m_camera.viewpoint[2]+m_camera.los[2]*10);
	m_objects.push_back(pPrim);
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::OnOctahedron() 
{
	CPrimitive *pPrim=new CPrimitive(OBJ_OCTA,m_nObjIDCount++);
	pPrim->SetOffset(m_camera.viewpoint[0]+m_camera.los[0]*10,m_camera.viewpoint[1]+m_camera.los[1]*10,m_camera.viewpoint[2]+m_camera.los[2]*10);
	m_objects.push_back(pPrim);
	UpdateAllViews(NULL);
}

void CRayDoc::OnPryamid() 
{
	CPrimitive *pPrim=new CPrimitive(OBJ_PYR,m_nObjIDCount++);
	pPrim->SetOffset(m_camera.viewpoint[0]+m_camera.los[0]*10,m_camera.viewpoint[1]+m_camera.los[1]*10,m_camera.viewpoint[2]+m_camera.los[2]*10);
	m_objects.push_back(pPrim);
	UpdateAllViews(NULL);
}

void CRayDoc::OnTetrahedron() 
{
	CPrimitive *pPrim=new CPrimitive(OBJ_TETRA,m_nObjIDCount++);
	pPrim->SetOffset(m_camera.viewpoint[0]+m_camera.los[0]*10,m_camera.viewpoint[1]+m_camera.los[1]*10,m_camera.viewpoint[2]+m_camera.los[2]*10);
	m_objects.push_back(pPrim);
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::OnLight() 
{
	CLightSource *pLight=new CLightSource(m_camera.viewpoint[0]-m_camera.los[0]*20,m_camera.viewpoint[1]-m_camera.los[1]*20,m_camera.viewpoint[2]-m_camera.los[2]*20,m_nLightIDCount++);
	m_lightsources.push_back(pLight);
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::GenerateBigBoundingBox()
{
	int i,cnt;
	float xmin,xmax,ymin,ymax,zmin,zmax;

	xmin=MAX_DIST;
	xmax=-MAX_DIST;
	ymin=MAX_DIST;
	ymax=-MAX_DIST;
	zmin=MAX_DIST;
	zmax=-MAX_DIST;

	cnt=m_selectedobjects.size();

	if (cnt==0) return;
	
	for(list<LPPRIMITIVE>::iterator oi=m_selectedobjects.begin(); oi!=m_selectedobjects.end(); oi++){
		(*oi)->FindTBCorners();
		for (i=0;i<8;i++){
			if ((*oi)->m_TBoxCorner[i][0]<xmin) xmin=(*oi)->m_TBoxCorner[i][0];
			if ((*oi)->m_TBoxCorner[i][0]>xmax) xmax=(*oi)->m_TBoxCorner[i][0];
			if ((*oi)->m_TBoxCorner[i][1]<ymin) ymin=(*oi)->m_TBoxCorner[i][1];
			if ((*oi)->m_TBoxCorner[i][1]>ymax) ymax=(*oi)->m_TBoxCorner[i][1];
			if ((*oi)->m_TBoxCorner[i][2]<zmin) zmin=(*oi)->m_TBoxCorner[i][2];
			if ((*oi)->m_TBoxCorner[i][2]>zmax) zmax=(*oi)->m_TBoxCorner[i][2];
		}
	}

	m_BigBoxCorner[0][0]=xmin;m_BigBoxCorner[0][1]=ymax;m_BigBoxCorner[0][2]=zmin;
	m_BigBoxCorner[1][0]=xmin;m_BigBoxCorner[1][1]=ymax;m_BigBoxCorner[1][2]=zmax;
	m_BigBoxCorner[2][0]=xmax;m_BigBoxCorner[2][1]=ymax;m_BigBoxCorner[2][2]=zmax;
	m_BigBoxCorner[3][0]=xmax;m_BigBoxCorner[3][1]=ymax;m_BigBoxCorner[3][2]=zmin;
	m_BigBoxCorner[4][0]=xmin;m_BigBoxCorner[4][1]=ymin;m_BigBoxCorner[4][2]=zmin;
	m_BigBoxCorner[5][0]=xmin;m_BigBoxCorner[5][1]=ymin;m_BigBoxCorner[5][2]=zmax;
	m_BigBoxCorner[6][0]=xmax;m_BigBoxCorner[6][1]=ymin;m_BigBoxCorner[6][2]=zmax;
	m_BigBoxCorner[7][0]=xmax;m_BigBoxCorner[7][1]=ymin;m_BigBoxCorner[7][2]=zmin;

	m_RotateCentre[0]=(xmin+xmax)/2.0f;
	m_RotateCentre[1]=(ymin+ymax)/2.0f;
	m_RotateCentre[2]=(zmin+zmax)/2.0f;

}


void CRayDoc::OnDelete() 
{
	for(list<LPPRIMITIVE>::iterator oi=m_selectedobjects.begin(); oi!=m_selectedobjects.end(); oi++){
		delete (*oi);
		m_objects.remove((*oi));
	}
	m_selectedobjects.clear();

	for(list<LPLIGHTSOURCE>::iterator li=m_lightsources.begin(); li!=m_lightsources.end(); li++){
		if ((*li)->IsSelected()==TRUE){
			delete (*li);
			m_lightsources.erase(li);
			break;
		}
	}

	GenerateBigBoundingBox();
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::OnSelectall() 
{
	UnSelectAll();
	for(list<LPPRIMITIVE>::iterator oi=m_objects.begin(); oi!=m_objects.end(); oi++){
		(*oi)->Select();
		m_selectedobjects.push_back((*oi));
	}
	GenerateBigBoundingBox();
}

void CRayDoc::SelectOne(void *r)
{
	CPrimitive *pPrim;
	pPrim=(CPrimitive*)r;

	UnSelectAll();
	pPrim->Select();
	m_selectedobjects.push_back(pPrim);
	GenerateBigBoundingBox();
}

void CRayDoc::UnSelectAll()
{
	for(list<LPPRIMITIVE>::iterator oi=m_objects.begin(); oi!=m_objects.end(); oi++) (*oi)->UnSelect();
	m_selectedobjects.clear();
	GenerateBigBoundingBox();

	for(list<LPPRIMITIVE>::iterator oi=m_objects.begin(); oi!=m_objects.end(); oi++) (*oi)->UnSelect();
	for(list<LPLIGHTSOURCE>::iterator i=m_lightsources.begin(); i!=m_lightsources.end(); i++)(*i)->UnSelect();
}

void CRayDoc::UnSelectOne(void *r)
{
	CPrimitive* p=(CPrimitive*)r;

	p->UnSelect();
	for(list<LPPRIMITIVE>::iterator oi=m_selectedobjects.begin(); oi!=m_selectedobjects.end(); oi++){
		if ((*oi)==p){
			m_selectedobjects.erase(oi);
			break;
		}
	}
	GenerateBigBoundingBox();
}

void CRayDoc::SelectOneMore(void* r)
{
	CPrimitive *p;
	p=(CPrimitive*)r;

	if (p->IsSelected()==TRUE) return;
	p->Select();
	m_selectedobjects.push_back(p);
	GenerateBigBoundingBox();
}

void CRayDoc::OnPmDullred() 
{
	if (m_selectedobjects.size()==0) return;
	for(list<LPPRIMITIVE>::iterator oi=m_selectedobjects.begin(); oi!=m_selectedobjects.end(); oi++){
		(*oi)->m_Ambient[0]=.0f;(*oi)->m_Ambient[1]=.0f;(*oi)->m_Ambient[2]=.0f;
		(*oi)->m_Diffuse[0]=.5f;(*oi)->m_Diffuse[1]=.0f;(*oi)->m_Diffuse[2]=.0f;
		(*oi)->m_Specular[0]=.0f;(*oi)->m_Specular[1]=.0f;(*oi)->m_Specular[2]=.0f;
		(*oi)->m_Shininess=128;
		(*oi)->m_RefrIndex=1.0;
		(*oi)->m_Reflectance=.0;
		(*oi)->m_Opacity=1.0;
	}
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::OnPmGlass() 
{
	if (m_selectedobjects.size()==0) return;
	for(list<LPPRIMITIVE>::iterator oi=m_selectedobjects.begin(); oi!=m_selectedobjects.end(); oi++){
		(*oi)->m_Ambient[0]=.0f;(*oi)->m_Ambient[1]=.0f;(*oi)->m_Ambient[2]=.0f;
		(*oi)->m_Diffuse[0]=.0f;(*oi)->m_Diffuse[1]=.0f;(*oi)->m_Diffuse[2]=.0f;
		(*oi)->m_Specular[0]=1.0f;(*oi)->m_Specular[1]=1.0f;(*oi)->m_Specular[2]=1.0f;
		(*oi)->m_Shininess=128;
		(*oi)->m_RefrIndex=1.5f;
		(*oi)->m_Reflectance=1.0;
		(*oi)->m_Opacity=.0;
	}
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::OnPmGolden() 
{
	if (m_selectedobjects.size()==0) return;
	for(list<LPPRIMITIVE>::iterator oi=m_selectedobjects.begin(); oi!=m_selectedobjects.end(); oi++){
		(*oi)->m_Ambient[0]=.0f;(*oi)->m_Ambient[1]=.0f;(*oi)->m_Ambient[2]=.0f;
		(*oi)->m_Diffuse[0]=.5f;(*oi)->m_Diffuse[1]=.4f;(*oi)->m_Diffuse[2]=.0f;
		(*oi)->m_Specular[0]=1.0f;(*oi)->m_Specular[1]=1.0f;(*oi)->m_Specular[2]=.0f;
		(*oi)->m_Shininess=128;
		(*oi)->m_RefrIndex=1.0;
		(*oi)->m_Reflectance=1.0;
		(*oi)->m_Opacity=1.0;
	}
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::OnPmMirror() 
{
	if (m_selectedobjects.size()==0) return;
	for(list<LPPRIMITIVE>::iterator oi=m_selectedobjects.begin(); oi!=m_selectedobjects.end(); oi++){
		(*oi)->m_Ambient[0]=.0f;(*oi)->m_Ambient[1]=.0f;(*oi)->m_Ambient[2]=.0f;
		(*oi)->m_Diffuse[0]=.0f;(*oi)->m_Diffuse[1]=.0f;(*oi)->m_Diffuse[2]=.0f;
		(*oi)->m_Specular[0]=.0f;(*oi)->m_Specular[1]=.0f;(*oi)->m_Specular[2]=.0f;
		(*oi)->m_Shininess=128;
		(*oi)->m_RefrIndex=1.0;
		(*oi)->m_Reflectance=1.0;
		(*oi)->m_Opacity=1.0;
	}
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::OnPmShinygreen() 
{
	if (m_selectedobjects.size()==0) return;
	for(list<LPPRIMITIVE>::iterator oi=m_selectedobjects.begin(); oi!=m_selectedobjects.end(); oi++){
		(*oi)->m_Ambient[0]=.0f;(*oi)->m_Ambient[1]=.0f;(*oi)->m_Ambient[2]=.0f;
		(*oi)->m_Diffuse[0]=.0f;(*oi)->m_Diffuse[1]=.5f;(*oi)->m_Diffuse[2]=.0f;
		(*oi)->m_Specular[0]=1.0f;(*oi)->m_Specular[1]=1.0f;(*oi)->m_Specular[2]=1.0f;
		(*oi)->m_Shininess=128;
		(*oi)->m_RefrIndex=1.0;
		(*oi)->m_Reflectance=1.0;
		(*oi)->m_Opacity=1.0;
	}
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::OnPmTransblue() 
{
	if (m_selectedobjects.size()==0) return;
	for(list<LPPRIMITIVE>::iterator oi=m_selectedobjects.begin(); oi!=m_selectedobjects.end(); oi++){
		(*oi)->m_Ambient[0]=.0f;(*oi)->m_Ambient[1]=.0f;(*oi)->m_Ambient[2]=.0f;
		(*oi)->m_Diffuse[0]=.0f;(*oi)->m_Diffuse[1]=.0f;(*oi)->m_Diffuse[2]=.2f;
		(*oi)->m_Specular[0]=.0f;(*oi)->m_Specular[1]=.0f;(*oi)->m_Specular[2]=.0f;
		(*oi)->m_Shininess=128;
		(*oi)->m_RefrIndex=1.5;
		(*oi)->m_Reflectance=.5;
		(*oi)->m_Opacity=.5;
	}
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::OnRemovetrans() 
{
	int j;
	float oldtrans[3];

	if (m_selectedobjects.size()==0) return;

	for(list<LPPRIMITIVE>::iterator oi=m_selectedobjects.begin(); oi!=m_selectedobjects.end(); oi++){
		oldtrans[0]=(*oi)->m_matrix[12];
		oldtrans[1]=(*oi)->m_matrix[13];
		oldtrans[2]=(*oi)->m_matrix[14];
		for (j=0;j<16;j++) (*oi)->m_matrix[j]=.0f;
		(*oi)->m_matrix[0]=(*oi)->m_matrix[5]=(*oi)->m_matrix[10]=(*oi)->m_matrix[15]=1.0f;
		(*oi)->m_matrix[12]=oldtrans[0];
		(*oi)->m_matrix[13]=oldtrans[1];
		(*oi)->m_matrix[14]=oldtrans[2];

		(*oi)->InvMatrix44((*oi)->m_matrix,(*oi)->m_invmatrix);
		(*oi)->Transform();
		(*oi)->FindTBCorners();
	}

	GenerateBigBoundingBox();
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::OnDuplicate() 
{
	CPrimitive *p;

	if (m_selectedobjects.size()==0) return;

	for(list<LPPRIMITIVE>::iterator oi=m_selectedobjects.begin(); oi!=m_selectedobjects.end(); oi++){
		p=(*oi)->Clone();
		p->m_nID=m_nObjIDCount++;
		m_objects.push_back(p);
	}

	GenerateBigBoundingBox();
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::LightColor(float r,float g,float b)
{
	if (m_nSelectedLight==0) return;
	
	for(list<LPLIGHTSOURCE>::iterator li=m_lightsources.begin(); li!=m_lightsources.end(); li++){
		if ((*li)->m_nID==m_nSelectedLight){
			(*li)->SetColor(r,g,b);
			break;
		}
	}

	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::SelectedLightColor(float *r, float *g, float *b)
{
	if (m_nSelectedLight==0) return;
	
	for(list<LPLIGHTSOURCE>::iterator li=m_lightsources.begin(); li!=m_lightsources.end(); li++){
		if ((*li)->m_nID==m_nSelectedLight){
			if ((*li)!=NULL){
				*r=(*li)->color.r;
				*g=(*li)->color.g;
				*b=(*li)->color.b;
			} else
			{
				*r=1.0;
				*g=1.0;
				*b=1.0;
			}
			break;
		}
	}
}

void CRayDoc::OnLightSwitch() 
{
	for(list<LPLIGHTSOURCE>::iterator li=m_lightsources.begin(); li!=m_lightsources.end(); li++){
		if ((*li)->m_nID==m_nSelectedLight){
			if ((*li)->m_bON==TRUE) (*li)->m_bON=FALSE;
			else (*li)->m_bON=TRUE;
			break;
		}
	}
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::SelectedObjectsMiscMaterialProp(float *data)
{
	int cnt;
	CPrimitive *q;

	cnt=m_selectedobjects.size();
	if (cnt==0) return;

	if (cnt==1){
		list<LPPRIMITIVE>::iterator oi=m_selectedobjects.begin();
		q=(*oi);
		data[0]=q->m_Ambient[0];
		data[1]=q->m_Ambient[1];
		data[2]=q->m_Ambient[2];
		data[3]=q->m_Diffuse[0];
		data[4]=q->m_Diffuse[1];
		data[5]=q->m_Diffuse[2];
		data[6]=q->m_Specular[0];
		data[7]=q->m_Specular[1];
		data[8]=q->m_Specular[2];
		data[9]=q->m_Shininess;
		data[10]=q->m_Opacity;
		data[11]=q->m_RefrIndex;
		data[12]=q->m_Reflectance;
	} else {
		data[0]=.0f;
		data[1]=.0f;
		data[2]=.0f;
		data[3]=1.0f;
		data[4]=1.0f;
		data[5]=1.0f;
		data[6]=1.0f;
		data[7]=1.0f;
		data[8]=1.0f;
		data[9]=100;
		data[10]=1.0f;
		data[11]=1.0f;
		data[12]=.5f;
	}
}

void CRayDoc::MiscMaterialProp(float *data)
{
	if (m_selectedobjects.size()==0) return;

	for(list<LPPRIMITIVE>::iterator oi=m_selectedobjects.begin(); oi!=m_selectedobjects.end(); oi++){
		(*oi)->m_Ambient[0]=data[0];
		(*oi)->m_Ambient[1]=data[1];
		(*oi)->m_Ambient[2]=data[2];
		(*oi)->m_Diffuse[0]=data[3];
		(*oi)->m_Diffuse[1]=data[4];
		(*oi)->m_Diffuse[2]=data[5];
		(*oi)->m_Specular[0]=data[6];
		(*oi)->m_Specular[1]=data[7];
		(*oi)->m_Specular[2]=data[8];
		(*oi)->m_Shininess=data[9];
		(*oi)->m_RefrIndex=data[10];
		(*oi)->m_Reflectance=data[11];
		(*oi)->m_Opacity=data[12];
		(*oi)->m_Ambient[3]=(*oi)->m_Opacity;
		(*oi)->m_Diffuse[3]=(*oi)->m_Opacity;
		(*oi)->m_Specular[3]=(*oi)->m_Opacity;
	}

	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::SelectedObjectsBackupM()
{
	if (m_selectedobjects.size()==0) return;

	for(list<LPPRIMITIVE>::iterator oi=m_selectedobjects.begin(); oi!=m_selectedobjects.end(); oi++){
		(*oi)->BackupM();
	}
}

CPrimitive* CRayDoc::ObjectWithIDExists(unsigned short id)
{
	for(list<LPPRIMITIVE>::iterator oi=m_objects.begin(); oi!=m_objects.end(); oi++){
		if ((*oi)->m_nID==id){
			return (*oi);
		}
	}
	return NULL;
}

CLightSource* CRayDoc::SelectLightWithID(unsigned short id)
{
	for(list<LPLIGHTSOURCE>::iterator li=m_lightsources.begin(); li!=m_lightsources.end(); li++){
		if ((*li)->m_nID==id){
			if ((*li)->IsSelected()==FALSE){
				(*li)->Select();
				m_nSelectedLight=(*li)->m_nID;
			}
			return (*li);
		}
	}
	return NULL;
}

void CRayDoc::SelectedObjectsRestoreM()
{
	for(list<LPPRIMITIVE>::iterator oi=m_selectedobjects.begin(); oi!=m_selectedobjects.end(); oi++){
		(*oi)->RestoreM();
	}
	GenerateBigBoundingBox();
}

void CRayDoc::ViewpointWalk(int sx, int ex, int sy, int ey)
{
	m_camera.los+=m_camera.rhv*(float)(ex-sx)/5000.0f;
	m_camera.los.normalize();
	FindRHV();
	FindHUV();
	m_camera.viewpoint+=m_camera.los*(float)(ey-sy)/500.0f;
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::ViewpointSlide(int sx, int ex, int sy, int ey)
{
	m_camera.viewpoint+=(m_camera.huv*(float)(ey-sy)+m_camera.rhv*(float)(ex-sx))/500.0f;
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::ViewpointElevate(int sx, int ex, int sy, int ey)
{
	m_camera.los+=m_camera.rhv*(float)(ex-sx)/5000.0f;
	m_camera.los+=m_camera.huv*(float)(ey-sy)/5000.0f;
	m_camera.los.normalize();
	FindRHV();
	FindHUV();
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CRayDoc::ViewpointTwist(int sx, int ex)
{
	m_camera.huv+=m_camera.rhv*(float)(ex-sx)/5000.0f;
	m_camera.huv.normalize();
	FindRHV();
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}
