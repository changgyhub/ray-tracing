//////////////////////////////////////////////////////////////////////
// Primitive.h: interface for the CPrimitive class.
//

#pragma once

#include <list>
using namespace std;

#include "Vertex.h"

class CPrimitive
{
public:
	short	m_nType;							// 0-group node, 2-cubes, 4-ellipsoids, 5-cones, 6-cylinder,7-icosa, 8-tetra, 9-octa, 10-dodeca
	short	m_nIType;							// 1-triangles, 2-quadrics
	long	m_nID;								// unique object id

	// quadric related parametrics
	float	m_Qa,m_Qb,m_Qc,m_Qd,m_Qe,m_Qf,m_Qg,m_Qh,m_Qj,m_Qk;	// quadric equation ax^2+by^2+cz^2+2dxy+2eyz+2fxz+2gx+2hy+2jz+k=0
	float	m_Za,m_Zb;							// z-range of quadrics in object space

	// triangles related parameters
	int		m_nNumTriangle;						// number of triangles, cube=12, tetrahedron=4, isodecahedron=20, etc

	list<VERTEX> m_vertices;					// transformed triangle vertices
	list<VERTEX> m_objvertices;					// object space triangle vertices

	// transformations
	float	m_matrix[16];						// 4x4 transform matrix
	float	m_invmatrix[16];					// inverse of M

	// bounding ellipsoid
	float	m_Ba,m_Bb,m_Bc,m_Bd,m_Be,m_Bf,m_Bg,m_Bh,m_Bj,m_Bk;		// bounding ellipsoid coefficients
	float	m_OBa,m_OBb,m_OBc,m_OBd,m_OBe,m_OBf,m_OBg,m_OBh,m_OBj,m_OBk;	// object space bounding ellipsoid
	// bounding boxes
	V3	m_OBoxCorner[8];						// object space bounding box corners
	V3	m_TBoxCorner[8];						// transformed bounding box corners
	
	// material
	float	m_Ambient[4];						// ambient component, amb[3] not-used
	float	m_Diffuse[4];						// diffuse component, dif[3] not-used
	float	m_Specular[4];						// specular component, dif[3] not-used
	float	m_Shininess;						// shininess range 1-128
	float	m_RefrIndex;						// refractive index > 1.0
	float	m_Reflectance;						// range 0-1
	float	m_Opacity;							// range 0-1
	void GetDiffuse(V3 point,V3& diffuse);//get diffuse component. if it has texture, return texture color.
	void GetAmbient(V3 point,V3& ambient);//get ambient component. if it has texture, return texture color.
	void GetSpecular(V3 point,V3& specular);//get specular component. if it has texture, return texture color.
	// mapping, following pointers point to CTexture objects
	void	*m_pTX;								// color texture

	float	m_oldmatrix[16];					// rotation matrix backup
	bool	m_bSelected;
	
	CPrimitive(int,long);
	virtual ~CPrimitive();

	// vector, matrix operations
	void	FindNormal(int id0, int id1, int id2, list<VERTEX>& vlist);
	void	MultMatrix44(float*,float*,float*);
	void	InvMatrix44(float*,float*);
	void	TransMatrix44(float*,float*);

	// find bounding boxes
	void	FindTBCorners();		// find transformed bounding box

	// transform M related
	void	SetOffset(float,float,float);
	void	Transform();
	void	BackupM();
	void	RestoreM();
	void	GetM(float *);
	void	LoadM(float*);
	void	MultM(float*);

	float  rayIntersect(V3 rayStart, V3 rayDir, V3& interp, V3& intern);
	void	DrawSelBox();			// draw bounding boxes
	void	Draw(int,list<CPrimitive*>&);	// draw object

	void	Select();
	void	UnSelect();
	bool	IsSelected();
	void	GetTextureValueAt(V3& point,V3& color);
	CPrimitive	*Clone();			// self duplicate

private:
	float	m_OQa,m_OQb,m_OQc,m_OQd,m_OQe,m_OQf,m_OQg,m_OQh,m_OQj,m_OQk;	// intrinic quadric equation, not transformed
};

typedef CPrimitive PRIMITIVE, *LPPRIMITIVE;
