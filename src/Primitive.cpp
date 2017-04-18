// Primitive.cpp: implementation of the CPrimitive class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ray.h"
#include "Vertex.h"
#include "LightSource.h"
#include "Primitive.h"
#include "Texture.h"
#include "RayDoc.h"
#include "RayView.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include "code.h"
#define GOLDENRATIO		1.6180339887498948482045868343656f		// golden ratio
#define RGOLDENRATIO	0.6180339887498948482045868343656f		// one over golden ratio
#define ONEOVERGRSQR	0.3819660112501051517954131656341f		// one over golden ratio square

extern int boxedge[12][2];
extern int boxface[6][5];
extern BOOL SHOWTX;

CPrimitive::CPrimitive(int type,long id)
{
	m_nID=id;
	m_nType=(short)type;
	m_bSelected=FALSE;

	float vt[20][3]={{RGOLDENRATIO,RGOLDENRATIO,RGOLDENRATIO},{RGOLDENRATIO,RGOLDENRATIO,-RGOLDENRATIO},
	{RGOLDENRATIO,-RGOLDENRATIO,RGOLDENRATIO},{RGOLDENRATIO,-RGOLDENRATIO,-RGOLDENRATIO},
	{-RGOLDENRATIO,RGOLDENRATIO,RGOLDENRATIO},{-RGOLDENRATIO,RGOLDENRATIO,-RGOLDENRATIO},
	{-RGOLDENRATIO,-RGOLDENRATIO,RGOLDENRATIO},{-RGOLDENRATIO,-RGOLDENRATIO,-RGOLDENRATIO},
	{ONEOVERGRSQR,1,0},{-ONEOVERGRSQR,1,0},{ONEOVERGRSQR,-1,0},{-ONEOVERGRSQR,-1,0},
	{1,0,ONEOVERGRSQR},{1,0,-ONEOVERGRSQR},{-1,0,ONEOVERGRSQR},{-1,0,-ONEOVERGRSQR},
	{0,ONEOVERGRSQR,1},{0,-ONEOVERGRSQR,1},{0,ONEOVERGRSQR,-1},{0,-ONEOVERGRSQR,-1}};

	switch (type){
	case OBJ_CUBE:
		m_nNumTriangle=12;

		// -ve x direction
		m_vertices.push_back(VERTEX(-1,-1,-1,-1,.0,.0));
		m_vertices.push_back(VERTEX(-1,-1,1,-1,.0,.0));
		m_vertices.push_back(VERTEX(-1,1,-1,-1,.0,.0));
		m_vertices.push_back(VERTEX(-1,-1,1,-1,.0,.0));
		m_vertices.push_back(VERTEX(-1,1,1,-1,.0,.0));
		m_vertices.push_back(VERTEX(-1,1,-1,-1,.0,.0));

		// +ve x direction
		m_vertices.push_back(VERTEX(1,1,1,1,.0,.0));
		m_vertices.push_back(VERTEX(1,-1,1,1,.0,.0));
		m_vertices.push_back(VERTEX(1,1,-1,1,.0,.0));
		m_vertices.push_back(VERTEX(1,1,-1,1,.0,.0));
		m_vertices.push_back(VERTEX(1,-1,1,1,.0,.0));
		m_vertices.push_back(VERTEX(1,-1,-1,1,.0,.0));

		// +ve y direction
		m_vertices.push_back(VERTEX(-1,1,-1,0,1,.0));
		m_vertices.push_back(VERTEX(-1,1,1,0,1,.0));
		m_vertices.push_back(VERTEX(1,1,-1,0,1,.0));
		m_vertices.push_back(VERTEX(1,1,-1,0,1,.0));
		m_vertices.push_back(VERTEX(-1,1,1,0,1,.0));
		m_vertices.push_back(VERTEX(1,1,1,0,1,.0));

		// -ve y direction
		m_vertices.push_back(VERTEX(-1,-1,1,0,-1,.0));
		m_vertices.push_back(VERTEX(-1,-1,-1,0,-1,.0));
		m_vertices.push_back(VERTEX(1,-1,1,0,-1,.0));
		m_vertices.push_back(VERTEX(1,-1,-1,0,-1,.0));
		m_vertices.push_back(VERTEX(1,-1,1,0,-1,.0));
		m_vertices.push_back(VERTEX(-1,-1,-1,0,-1,.0));

		// +ve z direction
		m_vertices.push_back(VERTEX(-1,-1,1,0,0,1));
		m_vertices.push_back(VERTEX(1,-1,1,0,0,1));
		m_vertices.push_back(VERTEX(-1,1,1,0,0,1));
		m_vertices.push_back(VERTEX(1,1,1,0,0,1));
		m_vertices.push_back(VERTEX(-1,1,1,0,0,1));
		m_vertices.push_back(VERTEX(1,-1,1,0,0,1));

		// -ve z direction
		m_vertices.push_back(VERTEX(-1,1,-1,0,0,-1));
		m_vertices.push_back(VERTEX(1,1,-1,0,0,-1));
		m_vertices.push_back(VERTEX(-1,-1,-1,0,0,-1));
		m_vertices.push_back(VERTEX(1,-1,-1,0,0,-1));
		m_vertices.push_back(VERTEX(-1,-1,-1,0,0,-1));
		m_vertices.push_back(VERTEX(1,1,-1,0,0,-1));



		// -ve x direction
		m_objvertices.push_back(VERTEX(-1,-1,-1,-1,.0,.0,.0f,.0f));
		m_objvertices.push_back(VERTEX(-1,-1,1,-1,.0,.0,1.0f,.0f));
		m_objvertices.push_back(VERTEX(-1,1,-1,-1,.0,.0,.0f,1.0f));
		m_objvertices.push_back(VERTEX(-1,-1,1,-1,.0,.0,1.0f,.0f));
		m_objvertices.push_back(VERTEX(-1,1,1,-1,.0,.0,1.0f,1.0f));
		m_objvertices.push_back(VERTEX(-1,1,-1,-1,.0,.0,.0f,1.0f));

		// +ve x direction
		m_objvertices.push_back(VERTEX(1,1,1,1,.0,.0,.0f,1.0f));
		m_objvertices.push_back(VERTEX(1,-1,1,1,.0,.0,.0f,.0f));
		m_objvertices.push_back(VERTEX(1,1,-1,1,.0,.0,1.0f,1.0f));
		m_objvertices.push_back(VERTEX(1,1,-1,1,.0,.0,1.0f,1.0f));
		m_objvertices.push_back(VERTEX(1,-1,1,1,.0,.0,.0f,.0f));
		m_objvertices.push_back(VERTEX(1,-1,-1,1,.0,.0,1.0f,.0f));

		// +ve y direction
		m_objvertices.push_back(VERTEX(-1,1,-1,0,1,.0,.0f,.0f));
		m_objvertices.push_back(VERTEX(-1,1,1,0,1,.0,1.0f,.0f));
		m_objvertices.push_back(VERTEX(1,1,-1,0,1,.0,.0f,1.0f));
		m_objvertices.push_back(VERTEX(1,1,-1,0,1,.0,.0f,1.0f));
		m_objvertices.push_back(VERTEX(-1,1,1,0,1,.0,1.0f,.0f));
		m_objvertices.push_back(VERTEX(1,1,1,0,1,.0,1.0f,1.0f));

		// -ve y direction
		m_objvertices.push_back(VERTEX(-1,-1,1,0,-1,.0,1.0f,1.0f));
		m_objvertices.push_back(VERTEX(-1,-1,-1,0,-1,.0,.0f,1.0f));
		m_objvertices.push_back(VERTEX(1,-1,1,0,-1,.0,1.0f,.0f));
		m_objvertices.push_back(VERTEX(1,-1,-1,0,-1,.0,.0f,.0f));
		m_objvertices.push_back(VERTEX(1,-1,1,0,-1,.0,1.0f,.0f));
		m_objvertices.push_back(VERTEX(-1,-1,-1,0,-1,.0,.0f,1.0f));

		// +ve z direction
		m_objvertices.push_back(VERTEX(-1,-1,1,0,0,1,.0f,.0f));
		m_objvertices.push_back(VERTEX(1,-1,1,0,0,1,1.0f,.0f));
		m_objvertices.push_back(VERTEX(-1,1,1,0,0,1,.0f,1.0f));
		m_objvertices.push_back(VERTEX(1,1,1,0,0,1,1.0f,1.0f));
		m_objvertices.push_back(VERTEX(-1,1,1,0,0,1,.0f,1.0f));
		m_objvertices.push_back(VERTEX(1,-1,1,0,0,1,1.0f,.0f));

		// -ve z direction
		m_objvertices.push_back(VERTEX(-1,1,-1,0,0,-1,1.0f,1.0f));
		m_objvertices.push_back(VERTEX(1,1,-1,0,0,-1,.0f,1.0f));
		m_objvertices.push_back(VERTEX(-1,-1,-1,0,0,-1,1.0f,.0f));
		m_objvertices.push_back(VERTEX(1,-1,-1,0,0,-1,.0f,.0f));
		m_objvertices.push_back(VERTEX(-1,-1,-1,0,0,-1,1.0f,.0f));
		m_objvertices.push_back(VERTEX(1,1,-1,0,0,-1,.0f,1.0f));

		m_OBa=m_OBb=m_OBc=1.0f;
		m_OBd=m_OBe=m_OBf=m_OBg=m_OBh=m_OBj=.0f;
		m_OBk=-3.0f;

		m_nType=OBJ_CUBE;
		m_nIType=IOB_TRI;

		// shiny white
		m_Ambient[0]=m_Ambient[1]=m_Ambient[2]=.0f;m_Ambient[3]=1.0;
		m_Diffuse[0]=m_Diffuse[1]=m_Diffuse[2]=1.0;m_Diffuse[3]=1.0;
		m_Specular[0]=m_Specular[1]=m_Specular[2]=1.0;m_Specular[3]=1.0;
		m_Shininess=100;
		m_RefrIndex=1.0;
		m_Reflectance=.5;
		m_Opacity=1.0;

		m_pTX=NULL;

		m_matrix[1]=m_matrix[2]=m_matrix[3]=m_matrix[4]=m_matrix[6]=m_matrix[7]=m_matrix[8]=m_matrix[9]=m_matrix[11]=m_matrix[12]=m_matrix[13]=m_matrix[14]=.0f;
		m_matrix[0]=m_matrix[5]=m_matrix[10]=m_matrix[15]=1.0f;
		InvMatrix44(m_matrix,m_invmatrix);

		m_OBoxCorner[0][0]=-1;m_OBoxCorner[0][1]=1;m_OBoxCorner[0][2]=-1;
		m_OBoxCorner[1][0]=-1;m_OBoxCorner[1][1]=1;m_OBoxCorner[1][2]=1;
		m_OBoxCorner[2][0]=1;m_OBoxCorner[2][1]=1;m_OBoxCorner[2][2]=1;
		m_OBoxCorner[3][0]=1;m_OBoxCorner[3][1]=1;m_OBoxCorner[3][2]=-1;
		m_OBoxCorner[4][0]=-1;m_OBoxCorner[4][1]=-1;m_OBoxCorner[4][2]=-1;
		m_OBoxCorner[5][0]=-1;m_OBoxCorner[5][1]=-1;m_OBoxCorner[5][2]=1;
		m_OBoxCorner[6][0]=1;m_OBoxCorner[6][1]=-1;m_OBoxCorner[6][2]=1;
		m_OBoxCorner[7][0]=1;m_OBoxCorner[7][1]=-1;m_OBoxCorner[7][2]=-1;

		Transform();
		FindTBCorners();

		break;

	case OBJ_PYR:
		m_nNumTriangle=6;

		// -ve z direction
		m_vertices.push_back(VERTEX(-1,1,.0,0,0,-1));
		m_vertices.push_back(VERTEX(1,1,.0,0,0,-1));
		m_vertices.push_back(VERTEX(-1,-1,.0,0,0,-1));
		m_vertices.push_back(VERTEX(1,-1,.0,0,0,-1));
		m_vertices.push_back(VERTEX(-1,-1,.0,0,0,-1));
		m_vertices.push_back(VERTEX(1,1,.0,0,0,-1));


		m_vertices.push_back(VERTEX(1,-1,.0,.0f,-.81649658f,0.5573503f));
		m_vertices.push_back(VERTEX(0,0,1.4242356f,.0f,-.81649658f,0.5573503f));
		m_vertices.push_back(VERTEX(-1,-1,.0,.0f,-.81649658f,0.5573503f));

		m_vertices.push_back(VERTEX(-1,-1,.0,-.81649658f,.0f,0.5573503f));
		m_vertices.push_back(VERTEX(0,0,1.4242356f,-.81649658f,.0f,0.5573503f));
		m_vertices.push_back(VERTEX(-1,1,.0,-.81649658f,.0f,0.5573503f));

		m_vertices.push_back(VERTEX(-1,1,.0,.0f,.81649658f,0.5573503f));
		m_vertices.push_back(VERTEX(0,0,1.4242356f,.0f,.81649658f,0.5573503f));
		m_vertices.push_back(VERTEX(1,1,.0,.0f,.81649658f,0.5573503f));

		m_vertices.push_back(VERTEX(1,1,.0,.81649658f,.0f,0.5573503f));
		m_vertices.push_back(VERTEX(0,0,1.4242356f,.81649658f,.0f,0.5573503f));
		m_vertices.push_back(VERTEX(1,-1,.0,.81649658f,.0f,0.5573503f));


		// -ve z direction
		m_objvertices.push_back(VERTEX(-1,1,0,0,0,-1,1.0f,1.0f));
		m_objvertices.push_back(VERTEX(1,1,0,0,0,-1,.0f,1.0f));
		m_objvertices.push_back(VERTEX(-1,-1,0,0,0,-1,1.0f,.0f));
		m_objvertices.push_back(VERTEX(1,-1,0,0,0,-1,.0f,.0f));
		m_objvertices.push_back(VERTEX(-1,-1,0,0,0,-1,1.0f,.0f));
		m_objvertices.push_back(VERTEX(1,1,0,0,0,-1,.0f,1.0f));

		m_objvertices.push_back(VERTEX(1,-1,.0,.0f,-.81649658f,0.5573503f,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(0,0,1.4242356f,.0f,-.81649658f,0.5573503f,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(-1,-1,.0,.0f,-.81649658f,0.5573503f,.0f,0.0f));

		m_objvertices.push_back(VERTEX(-1,-1,.0,-.81649658f,.0f,0.5573503f,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(0,0,1.4242356f,-.81649658f,.0f,0.5573503f,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(-1,1,.0,-.81649658f,.0f,0.5573503f,.0f,0.0f));

		m_objvertices.push_back(VERTEX(-1,1,.0,.0f,.81649658f,0.5573503f,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(0,0,1.4242356f,.0f,.81649658f,0.5573503f,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(1,1,.0,.0f,.81649658f,0.5573503f,.0f,0.0f));

		m_objvertices.push_back(VERTEX(1,1,.0,.81649658f,.0f,0.5573503f,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(0,0,1.4242356f,.81649658f,.0f,0.5573503f,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(1,-1,.0,.81649658f,.0f,0.5573503f,.0f,0.0f));

		m_OBa=m_OBb=m_OBc=1.0f;
		m_OBd=m_OBe=m_OBf=m_OBg=m_OBh=m_OBj=.0f;
		m_OBk=-3.0f;

		m_nType=OBJ_PYR;
		m_nIType=IOB_TRI;

		// shiny white
		m_Ambient[0]=m_Ambient[1]=m_Ambient[2]=.0f;m_Ambient[3]=1.0;
		m_Diffuse[0]=m_Diffuse[1]=m_Diffuse[2]=1.0;m_Diffuse[3]=1.0;
		m_Specular[0]=m_Specular[1]=m_Specular[2]=1.0;m_Specular[3]=1.0;
		m_Shininess=100;
		m_RefrIndex=1.0;
		m_Reflectance=.5;
		m_Opacity=1.0;

		m_pTX=NULL;

		m_matrix[1]=m_matrix[2]=m_matrix[3]=m_matrix[4]=m_matrix[6]=m_matrix[7]=m_matrix[8]=m_matrix[9]=m_matrix[11]=m_matrix[12]=m_matrix[13]=m_matrix[14]=.0f;
		m_matrix[0]=m_matrix[5]=m_matrix[10]=m_matrix[15]=1.0f;
		InvMatrix44(m_matrix,m_invmatrix);

		m_OBoxCorner[0][0]=-1;m_OBoxCorner[0][1]=1;m_OBoxCorner[0][2]=0;
		m_OBoxCorner[1][0]=-1;m_OBoxCorner[1][1]=1;m_OBoxCorner[1][2]=2;
		m_OBoxCorner[2][0]=1;m_OBoxCorner[2][1]=1;m_OBoxCorner[2][2]=2;
		m_OBoxCorner[3][0]=1;m_OBoxCorner[3][1]=1;m_OBoxCorner[3][2]=0;
		m_OBoxCorner[4][0]=-1;m_OBoxCorner[4][1]=-1;m_OBoxCorner[4][2]=0;
		m_OBoxCorner[5][0]=-1;m_OBoxCorner[5][1]=-1;m_OBoxCorner[5][2]=2;
		m_OBoxCorner[6][0]=1;m_OBoxCorner[6][1]=-1;m_OBoxCorner[6][2]=2;
		m_OBoxCorner[7][0]=1;m_OBoxCorner[7][1]=-1;m_OBoxCorner[7][2]=0;

		Transform();
		FindTBCorners();

		break;

	case OBJ_TETRA:			// tetrahedron
		m_nNumTriangle=4;

		m_vertices.push_back(VERTEX(-1,-1,1));
		m_vertices.push_back(VERTEX(-1,1,-1));
		m_vertices.push_back(VERTEX(1,-1,-1));
		FindNormal(0,1,2,m_vertices);

		m_vertices.push_back(VERTEX(-1,1,-1));
		m_vertices.push_back(VERTEX(-1,-1,1));
		m_vertices.push_back(VERTEX(1,1,1));
		FindNormal(3,4,5,m_vertices);

		m_vertices.push_back(VERTEX(1,-1,-1));
		m_vertices.push_back(VERTEX(1,1,1));
		m_vertices.push_back(VERTEX(-1,-1,1));
		FindNormal(6,7,8,m_vertices);

		m_vertices.push_back(VERTEX(1,1,1));
		m_vertices.push_back(VERTEX(1,-1,-1));
		m_vertices.push_back(VERTEX(-1,1,-1));
		FindNormal(9,10,11,m_vertices);


		m_objvertices.push_back(VERTEX(-1,-1,1,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(-1,1,-1,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(1,-1,-1,.0f,0.0f));
		FindNormal(0,1,2,m_objvertices);

		m_objvertices.push_back(VERTEX(-1,1,-1,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(-1,-1,1,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(1,1,1,.0f,0.0f));
		FindNormal(3,4,5,m_objvertices);

		m_objvertices.push_back(VERTEX(1,-1,-1,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(1,1,1,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(-1,-1,1,.0f,0.0f));
		FindNormal(6,7,8,m_objvertices);

		m_objvertices.push_back(VERTEX(1,1,1,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(1,-1,-1,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(-1,1,-1,.0f,0.0f));
		FindNormal(9,10,11,m_objvertices);

		m_OBa=m_OBb=m_OBc=1.0f;
		m_OBd=m_OBe=m_OBf=m_OBg=m_OBh=m_OBj=.0f;
		m_OBk=-3.0f;

		m_nType=OBJ_TETRA;
		m_nIType=IOB_TRI;

		// shiny white
		m_Ambient[0]=m_Ambient[1]=m_Ambient[2]=.0f;m_Ambient[3]=1.0;
		m_Diffuse[0]=m_Diffuse[1]=m_Diffuse[2]=1.0;m_Diffuse[3]=1.0;
		m_Specular[0]=m_Specular[1]=m_Specular[2]=1.0;m_Specular[3]=1.0;
		m_Shininess=100;
		m_RefrIndex=1.0;
		m_Reflectance=.5;
		m_Opacity=1.0;

		m_pTX=NULL;

		m_matrix[1]=m_matrix[2]=m_matrix[3]=m_matrix[4]=m_matrix[6]=m_matrix[7]=m_matrix[8]=m_matrix[9]=m_matrix[11]=m_matrix[12]=m_matrix[13]=m_matrix[14]=.0f;
		m_matrix[0]=m_matrix[5]=m_matrix[10]=m_matrix[15]=1.0f;
		InvMatrix44(m_matrix,m_invmatrix);

		m_OBoxCorner[0][0]=-1;m_OBoxCorner[0][1]=1;m_OBoxCorner[0][2]=-1;
		m_OBoxCorner[1][0]=-1;m_OBoxCorner[1][1]=1;m_OBoxCorner[1][2]=1;
		m_OBoxCorner[2][0]=1;m_OBoxCorner[2][1]=1;m_OBoxCorner[2][2]=1;
		m_OBoxCorner[3][0]=1;m_OBoxCorner[3][1]=1;m_OBoxCorner[3][2]=-1;
		m_OBoxCorner[4][0]=-1;m_OBoxCorner[4][1]=-1;m_OBoxCorner[4][2]=-1;
		m_OBoxCorner[5][0]=-1;m_OBoxCorner[5][1]=-1;m_OBoxCorner[5][2]=1;
		m_OBoxCorner[6][0]=1;m_OBoxCorner[6][1]=-1;m_OBoxCorner[6][2]=1;
		m_OBoxCorner[7][0]=1;m_OBoxCorner[7][1]=-1;m_OBoxCorner[7][2]=-1;

		Transform();
		FindTBCorners();

		break;

	case OBJ_OCTA:			// octahedron, 8 faced platonic solm_nID
		m_nNumTriangle=8;

		m_vertices.push_back(VERTEX(1,0,0));
		m_vertices.push_back(VERTEX(0,1,0));
		m_vertices.push_back(VERTEX(0,0,1));
		FindNormal(0,1,2,m_vertices);

		m_vertices.push_back(VERTEX(0,1,0));
		m_vertices.push_back(VERTEX(1,0,0));
		m_vertices.push_back(VERTEX(0,0,-1));
		FindNormal(3,4,5,m_vertices);

		m_vertices.push_back(VERTEX(0,-1,0));
		m_vertices.push_back(VERTEX(1,0,0));
		m_vertices.push_back(VERTEX(0,0,1));
		FindNormal(6,7,8,m_vertices);

		m_vertices.push_back(VERTEX(1,0,0));
		m_vertices.push_back(VERTEX(0,-1,0));
		m_vertices.push_back(VERTEX(0,0,-1));
		FindNormal(9,10,11,m_vertices);

		m_vertices.push_back(VERTEX(0,1,0));
		m_vertices.push_back(VERTEX(-1,0,0));
		m_vertices.push_back(VERTEX(0,0,1));
		FindNormal(12,13,14,m_vertices);

		m_vertices.push_back(VERTEX(-1,0,0));
		m_vertices.push_back(VERTEX(0,1,0));
		m_vertices.push_back(VERTEX(0,0,-1));
		FindNormal(15,16,17,m_vertices);

		m_vertices.push_back(VERTEX(-1,0,0));
		m_vertices.push_back(VERTEX(0,-1,0));
		m_vertices.push_back(VERTEX(0,0,1));
		FindNormal(18,19,20,m_vertices);

		m_vertices.push_back(VERTEX(0,-1,0));
		m_vertices.push_back(VERTEX(-1,0,0));
		m_vertices.push_back(VERTEX(0,0,-1));
		FindNormal(21,22,23,m_vertices);



		m_objvertices.push_back(VERTEX(1,0,0,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(0,1,0,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(0,0,1,.0f,0.0f));
		FindNormal(0,1,2,m_objvertices);

		m_objvertices.push_back(VERTEX(0,1,0,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(1,0,0,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(0,0,-1,.0f,0.0f));
		FindNormal(3,4,5,m_objvertices);

		m_objvertices.push_back(VERTEX(0,-1,0,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(1,0,0,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(0,0,1,.0f,0.0f));
		FindNormal(6,7,8,m_objvertices);

		m_objvertices.push_back(VERTEX(1,0,0,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(0,-1,0,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(0,0,-1,.0f,0.0f));
		FindNormal(9,10,11,m_objvertices);

		m_objvertices.push_back(VERTEX(0,1,0,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(-1,0,0,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(0,0,1,.0f,0.0f));
		FindNormal(12,13,14,m_objvertices);

		m_objvertices.push_back(VERTEX(-1,0,0,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(0,1,0,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(0,0,-1,.0f,0.0f));
		FindNormal(15,16,17,m_objvertices);

		m_objvertices.push_back(VERTEX(-1,0,0,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(0,-1,0,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(0,0,1,.0f,0.0f));
		FindNormal(18,19,20,m_objvertices);

		m_objvertices.push_back(VERTEX(0,-1,0,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(-1,0,0,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(0,0,-1,.0f,0.0f));
		FindNormal(21,22,23,m_objvertices);

		m_OBa=m_OBb=m_OBc=1.0f;
		m_OBd=m_OBe=m_OBf=m_OBg=m_OBh=m_OBj=.0f;
		m_OBk=-1.0f;

		m_nType=OBJ_OCTA;
		m_nIType=IOB_TRI;

		// shiny white
		m_Ambient[0]=m_Ambient[1]=m_Ambient[2]=.0f;m_Ambient[3]=1.0;
		m_Diffuse[0]=m_Diffuse[1]=m_Diffuse[2]=1.0;m_Diffuse[3]=1.0;
		m_Specular[0]=m_Specular[1]=m_Specular[2]=1.0;m_Specular[3]=1.0;
		m_Shininess=100;
		m_RefrIndex=1.0;
		m_Reflectance=.5;
		m_Opacity=1.0;

		m_pTX=NULL;

		m_matrix[1]=m_matrix[2]=m_matrix[3]=m_matrix[4]=m_matrix[6]=m_matrix[7]=m_matrix[8]=m_matrix[9]=m_matrix[11]=m_matrix[12]=m_matrix[13]=m_matrix[14]=.0f;
		m_matrix[0]=m_matrix[5]=m_matrix[10]=m_matrix[15]=1.0f;
		InvMatrix44(m_matrix,m_invmatrix);

		m_OBoxCorner[0][0]=-1;m_OBoxCorner[0][1]=1;m_OBoxCorner[0][2]=-1;
		m_OBoxCorner[1][0]=-1;m_OBoxCorner[1][1]=1;m_OBoxCorner[1][2]=1;
		m_OBoxCorner[2][0]=1;m_OBoxCorner[2][1]=1;m_OBoxCorner[2][2]=1;
		m_OBoxCorner[3][0]=1;m_OBoxCorner[3][1]=1;m_OBoxCorner[3][2]=-1;
		m_OBoxCorner[4][0]=-1;m_OBoxCorner[4][1]=-1;m_OBoxCorner[4][2]=-1;
		m_OBoxCorner[5][0]=-1;m_OBoxCorner[5][1]=-1;m_OBoxCorner[5][2]=1;
		m_OBoxCorner[6][0]=1;m_OBoxCorner[6][1]=-1;m_OBoxCorner[6][2]=1;
		m_OBoxCorner[7][0]=1;m_OBoxCorner[7][1]=-1;m_OBoxCorner[7][2]=-1;

		Transform();
		FindTBCorners();

		break;

	case OBJ_ICOSA:			// icosahedron, 20 faced platonic solm_nID
		m_nNumTriangle=20;

		m_vertices.push_back(VERTEX(1,RGOLDENRATIO,0));
		m_vertices.push_back(VERTEX(0,1,RGOLDENRATIO));
		m_vertices.push_back(VERTEX(RGOLDENRATIO,0,1));
		FindNormal(0,1,2,m_vertices);

		m_vertices.push_back(VERTEX(1,RGOLDENRATIO,0));
		m_vertices.push_back(VERTEX(RGOLDENRATIO,0,-1));
		m_vertices.push_back(VERTEX(0,1,-RGOLDENRATIO));
		FindNormal(3,4,5,m_vertices);

		m_vertices.push_back(VERTEX(1,-RGOLDENRATIO,0));
		m_vertices.push_back(VERTEX(RGOLDENRATIO,0,1));
		m_vertices.push_back(VERTEX(0,-1,RGOLDENRATIO));
		FindNormal(6,7,8,m_vertices);

		m_vertices.push_back(VERTEX(1,-RGOLDENRATIO,0));
		m_vertices.push_back(VERTEX(0,-1,-RGOLDENRATIO));
		m_vertices.push_back(VERTEX(RGOLDENRATIO,0,-1));
		FindNormal(9,10,11,m_vertices);

		m_vertices.push_back(VERTEX(-1,RGOLDENRATIO,0));
		m_vertices.push_back(VERTEX(-RGOLDENRATIO,0,1));
		m_vertices.push_back(VERTEX(0,1,RGOLDENRATIO));
		FindNormal(12,13,14,m_vertices);

		m_vertices.push_back(VERTEX(-1,RGOLDENRATIO,0));
		m_vertices.push_back(VERTEX(0,1,-RGOLDENRATIO));
		m_vertices.push_back(VERTEX(-RGOLDENRATIO,0,-1));
		FindNormal(15,16,17,m_vertices);

		m_vertices.push_back(VERTEX(-1,-RGOLDENRATIO,0));
		m_vertices.push_back(VERTEX(0,-1,RGOLDENRATIO));
		m_vertices.push_back(VERTEX(-RGOLDENRATIO,0,1));
		FindNormal(18,19,20,m_vertices);

		m_vertices.push_back(VERTEX(-1,-RGOLDENRATIO,0));
		m_vertices.push_back(VERTEX(-RGOLDENRATIO,0,-1));
		m_vertices.push_back(VERTEX(0,-1,-RGOLDENRATIO));
		FindNormal(21,22,23,m_vertices);


		m_vertices.push_back(VERTEX(1,RGOLDENRATIO,0));
		m_vertices.push_back(VERTEX(0,1,-RGOLDENRATIO));
		m_vertices.push_back(VERTEX(0,1,RGOLDENRATIO));
		FindNormal(24,25,26,m_vertices);

		m_vertices.push_back(VERTEX(-1,RGOLDENRATIO,0));
		m_vertices.push_back(VERTEX(0,1,RGOLDENRATIO));
		m_vertices.push_back(VERTEX(0,1,-RGOLDENRATIO));
		FindNormal(27,28,29,m_vertices);

		m_vertices.push_back(VERTEX(1,-RGOLDENRATIO,0));
		m_vertices.push_back(VERTEX(0,-1,RGOLDENRATIO));
		m_vertices.push_back(VERTEX(0,-1,-RGOLDENRATIO));
		FindNormal(30,31,32,m_vertices);

		m_vertices.push_back(VERTEX(-1,-RGOLDENRATIO,0));
		m_vertices.push_back(VERTEX(0,-1,-RGOLDENRATIO));
		m_vertices.push_back(VERTEX(0,-1,RGOLDENRATIO));
		FindNormal(33,34,35,m_vertices);


		m_vertices.push_back(VERTEX(RGOLDENRATIO,0,1));
		m_vertices.push_back(VERTEX(1,-RGOLDENRATIO,0));
		m_vertices.push_back(VERTEX(1,RGOLDENRATIO,0));
		FindNormal(36,37,38,m_vertices);

		m_vertices.push_back(VERTEX(RGOLDENRATIO,0,-1));
		m_vertices.push_back(VERTEX(1,RGOLDENRATIO,0));
		m_vertices.push_back(VERTEX(1,-RGOLDENRATIO,0));
		FindNormal(39,40,41,m_vertices);

		m_vertices.push_back(VERTEX(-RGOLDENRATIO,0,1));
		m_vertices.push_back(VERTEX(-1,RGOLDENRATIO,0));
		m_vertices.push_back(VERTEX(-1,-RGOLDENRATIO,0));
		FindNormal(42,43,44,m_vertices);

		m_vertices.push_back(VERTEX(-RGOLDENRATIO,0,-1));
		m_vertices.push_back(VERTEX(-1,-RGOLDENRATIO,0));
		m_vertices.push_back(VERTEX(-1,RGOLDENRATIO,0));
		FindNormal(45,46,47,m_vertices);


		m_vertices.push_back(VERTEX(0,1,RGOLDENRATIO));
		m_vertices.push_back(VERTEX(-RGOLDENRATIO,0,1));
		m_vertices.push_back(VERTEX(RGOLDENRATIO,0,1));
		FindNormal(48,49,50,m_vertices);

		m_vertices.push_back(VERTEX(0,-1,RGOLDENRATIO));
		m_vertices.push_back(VERTEX(RGOLDENRATIO,0,1));
		m_vertices.push_back(VERTEX(-RGOLDENRATIO,0,1));
		FindNormal(51,52,53,m_vertices);

		m_vertices.push_back(VERTEX(0,1,-RGOLDENRATIO));
		m_vertices.push_back(VERTEX(RGOLDENRATIO,0,-1));
		m_vertices.push_back(VERTEX(-RGOLDENRATIO,0,-1));
		FindNormal(54,55,56,m_vertices);

		m_vertices.push_back(VERTEX(0,-1,-RGOLDENRATIO));
		m_vertices.push_back(VERTEX(-RGOLDENRATIO,0,-1));
		m_vertices.push_back(VERTEX(RGOLDENRATIO,0,-1));
		FindNormal(57,58,59,m_vertices);



		m_objvertices.push_back(VERTEX(1,RGOLDENRATIO,0,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(0,1,RGOLDENRATIO,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(RGOLDENRATIO,0,1,.0f,0.0f));
		FindNormal(0,1,2,m_objvertices);

		m_objvertices.push_back(VERTEX(1,RGOLDENRATIO,0,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(RGOLDENRATIO,0,-1,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(0,1,-RGOLDENRATIO,.0f,0.0f));
		FindNormal(3,4,5,m_objvertices);

		m_objvertices.push_back(VERTEX(1,-RGOLDENRATIO,0,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(RGOLDENRATIO,0,1,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(0,-1,RGOLDENRATIO,.0f,0.0f));
		FindNormal(6,7,8,m_objvertices);

		m_objvertices.push_back(VERTEX(1,-RGOLDENRATIO,0,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(0,-1,-RGOLDENRATIO,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(RGOLDENRATIO,0,-1,.0f,0.0f));
		FindNormal(9,10,11,m_objvertices);

		m_objvertices.push_back(VERTEX(-1,RGOLDENRATIO,0,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(-RGOLDENRATIO,0,1,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(0,1,RGOLDENRATIO,.0f,0.0f));
		FindNormal(12,13,14,m_objvertices);

		m_objvertices.push_back(VERTEX(-1,RGOLDENRATIO,0,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(0,1,-RGOLDENRATIO,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(-RGOLDENRATIO,0,-1,.0f,0.0f));
		FindNormal(15,16,17,m_objvertices);

		m_objvertices.push_back(VERTEX(-1,-RGOLDENRATIO,0,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(0,-1,RGOLDENRATIO,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(-RGOLDENRATIO,0,1,.0f,0.0f));
		FindNormal(18,19,20,m_objvertices);

		m_objvertices.push_back(VERTEX(-1,-RGOLDENRATIO,0,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(-RGOLDENRATIO,0,-1,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(0,-1,-RGOLDENRATIO,.0f,0.0f));
		FindNormal(21,22,23,m_objvertices);


		m_objvertices.push_back(VERTEX(1,RGOLDENRATIO,0,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(0,1,-RGOLDENRATIO,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(0,1,RGOLDENRATIO,.0f,0.0f));
		FindNormal(24,25,26,m_objvertices);

		m_objvertices.push_back(VERTEX(-1,RGOLDENRATIO,0,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(0,1,RGOLDENRATIO,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(0,1,-RGOLDENRATIO,.0f,0.0f));
		FindNormal(27,28,29,m_objvertices);

		m_objvertices.push_back(VERTEX(1,-RGOLDENRATIO,0,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(0,-1,RGOLDENRATIO,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(0,-1,-RGOLDENRATIO,.0f,0.0f));
		FindNormal(30,31,32,m_objvertices);

		m_objvertices.push_back(VERTEX(-1,-RGOLDENRATIO,0,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(0,-1,-RGOLDENRATIO,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(0,-1,RGOLDENRATIO,.0f,0.0f));
		FindNormal(33,34,35,m_objvertices);


		m_objvertices.push_back(VERTEX(RGOLDENRATIO,0,1,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(1,-RGOLDENRATIO,0,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(1,RGOLDENRATIO,0,.0f,0.0f));
		FindNormal(36,37,38,m_objvertices);

		m_objvertices.push_back(VERTEX(RGOLDENRATIO,0,-1,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(1,RGOLDENRATIO,0,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(1,-RGOLDENRATIO,0,.0f,0.0f));
		FindNormal(39,40,41,m_objvertices);

		m_objvertices.push_back(VERTEX(-RGOLDENRATIO,0,1,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(-1,RGOLDENRATIO,0,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(-1,-RGOLDENRATIO,0,.0f,0.0f));
		FindNormal(42,43,44,m_objvertices);

		m_objvertices.push_back(VERTEX(-RGOLDENRATIO,0,-1,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(-1,-RGOLDENRATIO,0,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(-1,RGOLDENRATIO,0,.0f,0.0f));
		FindNormal(45,46,47,m_objvertices);


		m_objvertices.push_back(VERTEX(0,1,RGOLDENRATIO,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(-RGOLDENRATIO,0,1,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(RGOLDENRATIO,0,1,.0f,0.0f));
		FindNormal(48,49,50,m_objvertices);

		m_objvertices.push_back(VERTEX(0,-1,RGOLDENRATIO,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(RGOLDENRATIO,0,1,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(-RGOLDENRATIO,0,1,.0f,0.0f));
		FindNormal(51,52,53,m_objvertices);

		m_objvertices.push_back(VERTEX(0,1,-RGOLDENRATIO,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(RGOLDENRATIO,0,-1,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(-RGOLDENRATIO,0,-1,.0f,0.0f));
		FindNormal(54,55,56,m_objvertices);

		m_objvertices.push_back(VERTEX(0,-1,-RGOLDENRATIO,1.0f,0.0f));
		m_objvertices.push_back(VERTEX(-RGOLDENRATIO,0,-1,.5f,.8660254f));
		m_objvertices.push_back(VERTEX(RGOLDENRATIO,0,-1,.0f,0.0f));
		FindNormal(57,58,59,m_objvertices);

		m_OBa=m_OBb=m_OBc=1.0f;
		m_OBd=m_OBe=m_OBf=m_OBg=m_OBh=m_OBj=.0f;
		m_OBk=-1.3819660112501051517954131656336f;

		m_nType=OBJ_ICOSA;
		m_nIType=IOB_TRI;

		// shiny white
		m_Ambient[0]=m_Ambient[1]=m_Ambient[2]=.0f;m_Ambient[3]=1.0;
		m_Diffuse[0]=m_Diffuse[1]=m_Diffuse[2]=1.0;m_Diffuse[3]=1.0;
		m_Specular[0]=m_Specular[1]=m_Specular[2]=1.0;m_Specular[3]=1.0;
		m_Shininess=100;
		m_RefrIndex=1.0;
		m_Reflectance=.5;
		m_Opacity=1.0;

		m_pTX=NULL;

		m_matrix[1]=m_matrix[2]=m_matrix[3]=m_matrix[4]=m_matrix[6]=m_matrix[7]=m_matrix[8]=m_matrix[9]=m_matrix[11]=m_matrix[12]=m_matrix[13]=m_matrix[14]=.0f;
		m_matrix[0]=m_matrix[5]=m_matrix[10]=m_matrix[15]=1.0f;
		InvMatrix44(m_matrix,m_invmatrix);

		m_OBoxCorner[0][0]=-1;m_OBoxCorner[0][1]=1;m_OBoxCorner[0][2]=-1;
		m_OBoxCorner[1][0]=-1;m_OBoxCorner[1][1]=1;m_OBoxCorner[1][2]=1;
		m_OBoxCorner[2][0]=1;m_OBoxCorner[2][1]=1;m_OBoxCorner[2][2]=1;
		m_OBoxCorner[3][0]=1;m_OBoxCorner[3][1]=1;m_OBoxCorner[3][2]=-1;
		m_OBoxCorner[4][0]=-1;m_OBoxCorner[4][1]=-1;m_OBoxCorner[4][2]=-1;
		m_OBoxCorner[5][0]=-1;m_OBoxCorner[5][1]=-1;m_OBoxCorner[5][2]=1;
		m_OBoxCorner[6][0]=1;m_OBoxCorner[6][1]=-1;m_OBoxCorner[6][2]=1;
		m_OBoxCorner[7][0]=1;m_OBoxCorner[7][1]=-1;m_OBoxCorner[7][2]=-1;

		Transform();
		FindTBCorners();

		break;

	case OBJ_DODECA:			// dodecahedron, 12 faced platonic solm_nID
		m_nNumTriangle=36;

		m_vertices.push_back(VERTEX(vt[1][0],vt[1][1],vt[1][2]));
		m_vertices.push_back(VERTEX(vt[8][0],vt[8][1],vt[8][2]));
		m_vertices.push_back(VERTEX(vt[0][0],vt[0][1],vt[0][2]));
		FindNormal(0,1,2,m_vertices);
		m_vertices.push_back(VERTEX(vt[1][0],vt[1][1],vt[1][2]));
		m_vertices.push_back(VERTEX(vt[0][0],vt[0][1],vt[0][2]));
		m_vertices.push_back(VERTEX(vt[12][0],vt[12][1],vt[12][2]));
		FindNormal(3,4,5,m_vertices);
		m_vertices.push_back(VERTEX(vt[1][0],vt[1][1],vt[1][2]));
		m_vertices.push_back(VERTEX(vt[12][0],vt[12][1],vt[12][2]));
		m_vertices.push_back(VERTEX(vt[13][0],vt[13][1],vt[13][2]));
		FindNormal(6,7,8,m_vertices);

		m_vertices.push_back(VERTEX(vt[4][0],vt[4][1],vt[4][2]));
		m_vertices.push_back(VERTEX(vt[9][0],vt[9][1],vt[9][2]));
		m_vertices.push_back(VERTEX(vt[5][0],vt[5][1],vt[5][2]));
		FindNormal(9,10,11,m_vertices);
		m_vertices.push_back(VERTEX(vt[4][0],vt[4][1],vt[4][2]));
		m_vertices.push_back(VERTEX(vt[5][0],vt[5][1],vt[5][2]));
		m_vertices.push_back(VERTEX(vt[15][0],vt[15][1],vt[15][2]));
		FindNormal(12,13,14,m_vertices);
		m_vertices.push_back(VERTEX(vt[4][0],vt[4][1],vt[4][2]));
		m_vertices.push_back(VERTEX(vt[15][0],vt[15][1],vt[15][2]));
		m_vertices.push_back(VERTEX(vt[14][0],vt[14][1],vt[14][2]));
		FindNormal(15,16,17,m_vertices);

		m_vertices.push_back(VERTEX(vt[2][0],vt[2][1],vt[2][2]));
		m_vertices.push_back(VERTEX(vt[10][0],vt[10][1],vt[10][2]));
		m_vertices.push_back(VERTEX(vt[3][0],vt[3][1],vt[3][2]));
		FindNormal(18,19,20,m_vertices);
		m_vertices.push_back(VERTEX(vt[2][0],vt[2][1],vt[2][2]));
		m_vertices.push_back(VERTEX(vt[3][0],vt[3][1],vt[3][2]));
		m_vertices.push_back(VERTEX(vt[13][0],vt[13][1],vt[13][2]));
		FindNormal(21,22,23,m_vertices);
		m_vertices.push_back(VERTEX(vt[2][0],vt[2][1],vt[2][2]));
		m_vertices.push_back(VERTEX(vt[13][0],vt[13][1],vt[13][2]));
		m_vertices.push_back(VERTEX(vt[12][0],vt[12][1],vt[12][2]));
		FindNormal(24,25,26,m_vertices);

		m_vertices.push_back(VERTEX(vt[7][0],vt[7][1],vt[7][2]));
		m_vertices.push_back(VERTEX(vt[11][0],vt[11][1],vt[11][2]));
		m_vertices.push_back(VERTEX(vt[6][0],vt[6][1],vt[6][2]));
		FindNormal(24,28,29,m_vertices);
		m_vertices.push_back(VERTEX(vt[7][0],vt[7][1],vt[7][2]));
		m_vertices.push_back(VERTEX(vt[6][0],vt[6][1],vt[6][2]));
		m_vertices.push_back(VERTEX(vt[14][0],vt[14][1],vt[14][2]));
		FindNormal(30,31,32,m_vertices);
		m_vertices.push_back(VERTEX(vt[7][0],vt[7][1],vt[7][2]));
		m_vertices.push_back(VERTEX(vt[14][0],vt[14][1],vt[14][2]));
		m_vertices.push_back(VERTEX(vt[15][0],vt[15][1],vt[15][2]));
		FindNormal(33,34,35,m_vertices);




		m_vertices.push_back(VERTEX(vt[2][0],vt[2][1],vt[2][2]));
		m_vertices.push_back(VERTEX(vt[12][0],vt[12][1],vt[12][2]));
		m_vertices.push_back(VERTEX(vt[0][0],vt[0][1],vt[0][2]));
		FindNormal(36,37,38,m_vertices);
		m_vertices.push_back(VERTEX(vt[2][0],vt[2][1],vt[2][2]));
		m_vertices.push_back(VERTEX(vt[0][0],vt[0][1],vt[0][2]));
		m_vertices.push_back(VERTEX(vt[16][0],vt[16][1],vt[16][2]));
		FindNormal(39,40,41,m_vertices);
		m_vertices.push_back(VERTEX(vt[2][0],vt[2][1],vt[2][2]));
		m_vertices.push_back(VERTEX(vt[16][0],vt[16][1],vt[16][2]));
		m_vertices.push_back(VERTEX(vt[17][0],vt[17][1],vt[17][2]));
		FindNormal(42,43,44,m_vertices);

		m_vertices.push_back(VERTEX(vt[1][0],vt[1][1],vt[1][2]));
		m_vertices.push_back(VERTEX(vt[13][0],vt[13][1],vt[13][2]));
		m_vertices.push_back(VERTEX(vt[3][0],vt[3][1],vt[3][2]));
		FindNormal(45,46,47,m_vertices);
		m_vertices.push_back(VERTEX(vt[1][0],vt[1][1],vt[1][2]));
		m_vertices.push_back(VERTEX(vt[3][0],vt[3][1],vt[3][2]));
		m_vertices.push_back(VERTEX(vt[19][0],vt[19][1],vt[19][2]));
		FindNormal(48,49,50,m_vertices);
		m_vertices.push_back(VERTEX(vt[1][0],vt[1][1],vt[1][2]));
		m_vertices.push_back(VERTEX(vt[19][0],vt[19][1],vt[19][2]));
		m_vertices.push_back(VERTEX(vt[18][0],vt[18][1],vt[18][2]));
		FindNormal(51,52,53,m_vertices);

		m_vertices.push_back(VERTEX(vt[4][0],vt[4][1],vt[4][2]));
		m_vertices.push_back(VERTEX(vt[14][0],vt[14][1],vt[14][2]));
		m_vertices.push_back(VERTEX(vt[6][0],vt[6][1],vt[6][2]));
		FindNormal(54,55,56,m_vertices);
		m_vertices.push_back(VERTEX(vt[4][0],vt[4][1],vt[4][2]));
		m_vertices.push_back(VERTEX(vt[6][0],vt[6][1],vt[6][2]));
		m_vertices.push_back(VERTEX(vt[17][0],vt[17][1],vt[17][2]));
		FindNormal(57,58,59,m_vertices);
		m_vertices.push_back(VERTEX(vt[4][0],vt[4][1],vt[4][2]));
		m_vertices.push_back(VERTEX(vt[17][0],vt[17][1],vt[17][2]));
		m_vertices.push_back(VERTEX(vt[16][0],vt[16][1],vt[16][2]));
		FindNormal(60,61,62,m_vertices);

		m_vertices.push_back(VERTEX(vt[7][0],vt[7][1],vt[7][2]));
		m_vertices.push_back(VERTEX(vt[15][0],vt[15][1],vt[15][2]));
		m_vertices.push_back(VERTEX(vt[5][0],vt[5][1],vt[5][2]));
		FindNormal(63,64,65,m_vertices);
		m_vertices.push_back(VERTEX(vt[7][0],vt[7][1],vt[7][2]));
		m_vertices.push_back(VERTEX(vt[5][0],vt[5][1],vt[5][2]));
		m_vertices.push_back(VERTEX(vt[18][0],vt[18][1],vt[18][2]));
		FindNormal(66,67,68,m_vertices);
		m_vertices.push_back(VERTEX(vt[7][0],vt[7][1],vt[7][2]));
		m_vertices.push_back(VERTEX(vt[18][0],vt[18][1],vt[18][2]));
		m_vertices.push_back(VERTEX(vt[19][0],vt[19][1],vt[19][2]));
		FindNormal(69,70,71,m_vertices);




		m_vertices.push_back(VERTEX(vt[4][0],vt[4][1],vt[4][2]));
		m_vertices.push_back(VERTEX(vt[16][0],vt[16][1],vt[16][2]));
		m_vertices.push_back(VERTEX(vt[0][0],vt[0][1],vt[0][2]));
		FindNormal(72,73,74,m_vertices);
		m_vertices.push_back(VERTEX(vt[4][0],vt[4][1],vt[4][2]));
		m_vertices.push_back(VERTEX(vt[0][0],vt[0][1],vt[0][2]));
		m_vertices.push_back(VERTEX(vt[8][0],vt[8][1],vt[8][2]));
		FindNormal(75,76,77,m_vertices);
		m_vertices.push_back(VERTEX(vt[4][0],vt[4][1],vt[4][2]));
		m_vertices.push_back(VERTEX(vt[8][0],vt[8][1],vt[8][2]));
		m_vertices.push_back(VERTEX(vt[9][0],vt[9][1],vt[9][2]));
		FindNormal(78,79,80,m_vertices);

		m_vertices.push_back(VERTEX(vt[2][0],vt[2][1],vt[2][2]));
		m_vertices.push_back(VERTEX(vt[17][0],vt[17][1],vt[17][2]));
		m_vertices.push_back(VERTEX(vt[6][0],vt[6][1],vt[6][2]));
		FindNormal(81,82,83,m_vertices);
		m_vertices.push_back(VERTEX(vt[2][0],vt[2][1],vt[2][2]));
		m_vertices.push_back(VERTEX(vt[6][0],vt[6][1],vt[6][2]));
		m_vertices.push_back(VERTEX(vt[11][0],vt[11][1],vt[11][2]));
		FindNormal(84,85,86,m_vertices);
		m_vertices.push_back(VERTEX(vt[2][0],vt[2][1],vt[2][2]));
		m_vertices.push_back(VERTEX(vt[11][0],vt[11][1],vt[11][2]));
		m_vertices.push_back(VERTEX(vt[10][0],vt[10][1],vt[10][2]));
		FindNormal(87,88,89,m_vertices);

		m_vertices.push_back(VERTEX(vt[1][0],vt[1][1],vt[1][2]));
		m_vertices.push_back(VERTEX(vt[18][0],vt[18][1],vt[18][2]));
		m_vertices.push_back(VERTEX(vt[5][0],vt[5][1],vt[5][2]));
		FindNormal(90,91,92,m_vertices);
		m_vertices.push_back(VERTEX(vt[1][0],vt[1][1],vt[1][2]));
		m_vertices.push_back(VERTEX(vt[5][0],vt[5][1],vt[5][2]));
		m_vertices.push_back(VERTEX(vt[9][0],vt[9][1],vt[9][2]));
		FindNormal(93,94,95,m_vertices);
		m_vertices.push_back(VERTEX(vt[1][0],vt[1][1],vt[1][2]));
		m_vertices.push_back(VERTEX(vt[9][0],vt[9][1],vt[9][2]));
		m_vertices.push_back(VERTEX(vt[8][0],vt[8][1],vt[8][2]));
		FindNormal(96,97,98,m_vertices);

		m_vertices.push_back(VERTEX(vt[7][0],vt[7][1],vt[7][2]));
		m_vertices.push_back(VERTEX(vt[19][0],vt[19][1],vt[19][2]));
		m_vertices.push_back(VERTEX(vt[3][0],vt[3][1],vt[3][2]));
		FindNormal(99,100,101,m_vertices);
		m_vertices.push_back(VERTEX(vt[7][0],vt[7][1],vt[7][2]));
		m_vertices.push_back(VERTEX(vt[3][0],vt[3][1],vt[3][2]));
		m_vertices.push_back(VERTEX(vt[10][0],vt[10][1],vt[10][2]));
		FindNormal(102,103,104,m_vertices);
		m_vertices.push_back(VERTEX(vt[7][0],vt[7][1],vt[7][2]));
		m_vertices.push_back(VERTEX(vt[10][0],vt[10][1],vt[10][2]));
		m_vertices.push_back(VERTEX(vt[11][0],vt[11][1],vt[11][2]));
		FindNormal(105,106,107,m_vertices);









		m_objvertices.push_back(VERTEX(vt[1][0],vt[1][1],vt[1][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[8][0],vt[8][1],vt[8][2],.809017f,.0f));
		m_objvertices.push_back(VERTEX(vt[0][0],vt[0][1],vt[0][2],1.0f,.587785f));
		FindNormal(0,1,2,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[1][0],vt[1][1],vt[1][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[0][0],vt[0][1],vt[0][2],1.0f,.587785f));
		m_objvertices.push_back(VERTEX(vt[12][0],vt[12][1],vt[12][2],.5f,.951056f));
		FindNormal(3,4,5,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[1][0],vt[1][1],vt[1][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[12][0],vt[12][1],vt[12][2],.5f,.951056f));
		m_objvertices.push_back(VERTEX(vt[13][0],vt[13][1],vt[13][2],.0f,.587785f));
		FindNormal(6,7,8,m_objvertices);

		m_objvertices.push_back(VERTEX(vt[4][0],vt[4][1],vt[4][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[9][0],vt[9][1],vt[9][2],.809017f,.0f));
		m_objvertices.push_back(VERTEX(vt[5][0],vt[5][1],vt[5][2],1.0f,.587785f));
		FindNormal(9,10,11,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[4][0],vt[4][1],vt[4][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[5][0],vt[5][1],vt[5][2],1.0f,.587785f));
		m_objvertices.push_back(VERTEX(vt[15][0],vt[15][1],vt[15][2],.5f,.951056f));
		FindNormal(12,13,14,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[4][0],vt[4][1],vt[4][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[15][0],vt[15][1],vt[15][2],.5f,.951056f));
		m_objvertices.push_back(VERTEX(vt[14][0],vt[14][1],vt[14][2],.0f,.587785f));
		FindNormal(15,16,17,m_objvertices);

		m_objvertices.push_back(VERTEX(vt[2][0],vt[2][1],vt[2][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[10][0],vt[10][1],vt[10][2],.809017f,.0f));
		m_objvertices.push_back(VERTEX(vt[3][0],vt[3][1],vt[3][2],1.0f,.587785f));
		FindNormal(18,19,20,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[2][0],vt[2][1],vt[2][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[3][0],vt[3][1],vt[3][2],1.0f,.587785f));
		m_objvertices.push_back(VERTEX(vt[13][0],vt[13][1],vt[13][2],.5f,.951056f));
		FindNormal(21,22,23,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[2][0],vt[2][1],vt[2][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[13][0],vt[13][1],vt[13][2],.5f,.951056f));
		m_objvertices.push_back(VERTEX(vt[12][0],vt[12][1],vt[12][2],.0f,.587785f));
		FindNormal(24,25,26,m_objvertices);

		m_objvertices.push_back(VERTEX(vt[7][0],vt[7][1],vt[7][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[11][0],vt[11][1],vt[11][2],.809017f,.0f));
		m_objvertices.push_back(VERTEX(vt[6][0],vt[6][1],vt[6][2],1.0f,.587785f));
		FindNormal(24,28,29,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[7][0],vt[7][1],vt[7][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[6][0],vt[6][1],vt[6][2],1.0f,.587785f));
		m_objvertices.push_back(VERTEX(vt[14][0],vt[14][1],vt[14][2],.5f,.951056f));
		FindNormal(30,31,32,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[7][0],vt[7][1],vt[7][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[14][0],vt[14][1],vt[14][2],.5f,.951056f));
		m_objvertices.push_back(VERTEX(vt[15][0],vt[15][1],vt[15][2],.0f,.587785f));
		FindNormal(33,34,35,m_objvertices);




		m_objvertices.push_back(VERTEX(vt[2][0],vt[2][1],vt[2][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[12][0],vt[12][1],vt[12][2],.809017f,.0f));
		m_objvertices.push_back(VERTEX(vt[0][0],vt[0][1],vt[0][2],1.0f,.587785f));
		FindNormal(36,37,38,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[2][0],vt[2][1],vt[2][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[0][0],vt[0][1],vt[0][2],1.0f,.587785f));
		m_objvertices.push_back(VERTEX(vt[16][0],vt[16][1],vt[16][2],.5f,.951056f));
		FindNormal(39,40,41,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[2][0],vt[2][1],vt[2][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[16][0],vt[16][1],vt[16][2],.5f,.951056f));
		m_objvertices.push_back(VERTEX(vt[17][0],vt[17][1],vt[17][2],.0f,.587785f));
		FindNormal(42,43,44,m_objvertices);

		m_objvertices.push_back(VERTEX(vt[1][0],vt[1][1],vt[1][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[13][0],vt[13][1],vt[13][2],.809017f,.0f));
		m_objvertices.push_back(VERTEX(vt[3][0],vt[3][1],vt[3][2],1.0f,.587785f));
		FindNormal(45,46,47,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[1][0],vt[1][1],vt[1][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[3][0],vt[3][1],vt[3][2],1.0f,.587785f));
		m_objvertices.push_back(VERTEX(vt[19][0],vt[19][1],vt[19][2],.5f,.951056f));
		FindNormal(48,49,50,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[1][0],vt[1][1],vt[1][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[19][0],vt[19][1],vt[19][2],.5f,.951056f));
		m_objvertices.push_back(VERTEX(vt[18][0],vt[18][1],vt[18][2],.0f,.587785f));
		FindNormal(51,52,53,m_objvertices);

		m_objvertices.push_back(VERTEX(vt[4][0],vt[4][1],vt[4][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[14][0],vt[14][1],vt[14][2],.809017f,.0f));
		m_objvertices.push_back(VERTEX(vt[6][0],vt[6][1],vt[6][2],1.0f,.587785f));
		FindNormal(54,55,56,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[4][0],vt[4][1],vt[4][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[6][0],vt[6][1],vt[6][2],1.0f,.587785f));
		m_objvertices.push_back(VERTEX(vt[17][0],vt[17][1],vt[17][2],.5f,.951056f));
		FindNormal(57,58,59,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[4][0],vt[4][1],vt[4][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[17][0],vt[17][1],vt[17][2],.5f,.951056f));
		m_objvertices.push_back(VERTEX(vt[16][0],vt[16][1],vt[16][2],.0f,.587785f));
		FindNormal(60,61,62,m_objvertices);

		m_objvertices.push_back(VERTEX(vt[7][0],vt[7][1],vt[7][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[15][0],vt[15][1],vt[15][2],.809017f,.0f));
		m_objvertices.push_back(VERTEX(vt[5][0],vt[5][1],vt[5][2],1.0f,.587785f));
		FindNormal(63,64,65,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[7][0],vt[7][1],vt[7][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[5][0],vt[5][1],vt[5][2],1.0f,.587785f));
		m_objvertices.push_back(VERTEX(vt[18][0],vt[18][1],vt[18][2],.5f,.951056f));
		FindNormal(66,67,68,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[7][0],vt[7][1],vt[7][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[18][0],vt[18][1],vt[18][2],.5f,.951056f));
		m_objvertices.push_back(VERTEX(vt[19][0],vt[19][1],vt[19][2],.0f,.587785f));
		FindNormal(69,70,71,m_objvertices);




		m_objvertices.push_back(VERTEX(vt[4][0],vt[4][1],vt[4][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[16][0],vt[16][1],vt[16][2],.809017f,.0f));
		m_objvertices.push_back(VERTEX(vt[0][0],vt[0][1],vt[0][2],1.0f,.587785f));
		FindNormal(72,73,74,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[4][0],vt[4][1],vt[4][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[0][0],vt[0][1],vt[0][2],1.0f,.587785f));
		m_objvertices.push_back(VERTEX(vt[8][0],vt[8][1],vt[8][2],.5f,.951056f));
		FindNormal(75,76,77,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[4][0],vt[4][1],vt[4][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[8][0],vt[8][1],vt[8][2],.5f,.951056f));
		m_objvertices.push_back(VERTEX(vt[9][0],vt[9][1],vt[9][2],.0f,.587785f));
		FindNormal(78,79,80,m_objvertices);

		m_objvertices.push_back(VERTEX(vt[2][0],vt[2][1],vt[2][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[17][0],vt[17][1],vt[17][2],.809017f,.0f));
		m_objvertices.push_back(VERTEX(vt[6][0],vt[6][1],vt[6][2],1.0f,.587785f));
		FindNormal(81,82,83,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[2][0],vt[2][1],vt[2][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[6][0],vt[6][1],vt[6][2],1.0f,.587785f));
		m_objvertices.push_back(VERTEX(vt[11][0],vt[11][1],vt[11][2],.5f,.951056f));
		FindNormal(84,85,86,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[2][0],vt[2][1],vt[2][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[11][0],vt[11][1],vt[11][2],.5f,.951056f));
		m_objvertices.push_back(VERTEX(vt[10][0],vt[10][1],vt[10][2],.0f,.587785f));
		FindNormal(87,88,89,m_objvertices);

		m_objvertices.push_back(VERTEX(vt[1][0],vt[1][1],vt[1][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[18][0],vt[18][1],vt[18][2],.809017f,.0f));
		m_objvertices.push_back(VERTEX(vt[5][0],vt[5][1],vt[5][2],1.0f,.587785f));
		FindNormal(90,91,92,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[1][0],vt[1][1],vt[1][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[5][0],vt[5][1],vt[5][2],1.0f,.587785f));
		m_objvertices.push_back(VERTEX(vt[9][0],vt[9][1],vt[9][2],.5f,.951056f));
		FindNormal(93,94,95,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[1][0],vt[1][1],vt[1][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[9][0],vt[9][1],vt[9][2],.5f,.951056f));
		m_objvertices.push_back(VERTEX(vt[8][0],vt[8][1],vt[8][2],.0f,.587785f));
		FindNormal(96,97,98,m_objvertices);

		m_objvertices.push_back(VERTEX(vt[7][0],vt[7][1],vt[7][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[19][0],vt[19][1],vt[19][2],.809017f,.0f));
		m_objvertices.push_back(VERTEX(vt[3][0],vt[3][1],vt[3][2],1.0f,.587785f));
		FindNormal(99,100,101,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[7][0],vt[7][1],vt[7][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[3][0],vt[3][1],vt[3][2],1.0f,.587785f));
		m_objvertices.push_back(VERTEX(vt[10][0],vt[10][1],vt[10][2],.5f,.951056f));
		FindNormal(102,103,104,m_objvertices);
		m_objvertices.push_back(VERTEX(vt[7][0],vt[7][1],vt[7][2],.190983f,.0f));
		m_objvertices.push_back(VERTEX(vt[10][0],vt[10][1],vt[10][2],.5f,.951056f));
		m_objvertices.push_back(VERTEX(vt[11][0],vt[11][1],vt[11][2],.0f,.587785f));
		FindNormal(105,106,107,m_objvertices);


		m_OBa=m_OBb=m_OBc=1.0f;
		m_OBd=m_OBe=m_OBf=m_OBg=m_OBh=m_OBj=.0f;
		m_OBk=-1.1458980337503154553862394969028f;

		m_nType=OBJ_DODECA;
		m_nIType=IOB_TRI;

		// shiny white
		m_Ambient[0]=m_Ambient[1]=m_Ambient[2]=.0f;m_Ambient[3]=1.0;
		m_Diffuse[0]=m_Diffuse[1]=m_Diffuse[2]=1.0;m_Diffuse[3]=1.0;
		m_Specular[0]=m_Specular[1]=m_Specular[2]=1.0;m_Specular[3]=1.0;
		m_Shininess=100;
		m_RefrIndex=1.0;
		m_Reflectance=.5;
		m_Opacity=1.0;

		m_pTX=NULL;

		m_matrix[1]=m_matrix[2]=m_matrix[3]=m_matrix[4]=m_matrix[6]=m_matrix[7]=m_matrix[8]=m_matrix[9]=m_matrix[11]=m_matrix[12]=m_matrix[13]=m_matrix[14]=.0f;
		m_matrix[0]=m_matrix[5]=m_matrix[10]=m_matrix[15]=1.0f;
		InvMatrix44(m_matrix,m_invmatrix);

		m_OBoxCorner[0][0]=-1;m_OBoxCorner[0][1]=1;m_OBoxCorner[0][2]=-1;
		m_OBoxCorner[1][0]=-1;m_OBoxCorner[1][1]=1;m_OBoxCorner[1][2]=1;
		m_OBoxCorner[2][0]=1;m_OBoxCorner[2][1]=1;m_OBoxCorner[2][2]=1;
		m_OBoxCorner[3][0]=1;m_OBoxCorner[3][1]=1;m_OBoxCorner[3][2]=-1;
		m_OBoxCorner[4][0]=-1;m_OBoxCorner[4][1]=-1;m_OBoxCorner[4][2]=-1;
		m_OBoxCorner[5][0]=-1;m_OBoxCorner[5][1]=-1;m_OBoxCorner[5][2]=1;
		m_OBoxCorner[6][0]=1;m_OBoxCorner[6][1]=-1;m_OBoxCorner[6][2]=1;
		m_OBoxCorner[7][0]=1;m_OBoxCorner[7][1]=-1;m_OBoxCorner[7][2]=-1;

		Transform();
		FindTBCorners();

		break;

	case OBJ_ELLI:

		m_OQa=m_OQb=m_OQc=1.0f;
		m_OQd=m_OQe=m_OQf=m_OQg=m_OQh=m_OQj=.0f;
		m_OQk=-1.0f;

		m_matrix[1]=m_matrix[2]=m_matrix[3]=m_matrix[4]=m_matrix[6]=m_matrix[7]=m_matrix[8]=m_matrix[9]=m_matrix[11]=m_matrix[12]=m_matrix[13]=m_matrix[14]=.0f;
		m_matrix[0]=m_matrix[5]=m_matrix[10]=m_matrix[15]=1.0f;

		InvMatrix44(m_matrix,m_invmatrix);
		Transform();

		m_Za=-1.0f;
		m_Zb=1.0f;

		m_nType=OBJ_ELLI;
		m_nIType=IOB_QUADR;

		// shiny white
		m_Ambient[0]=m_Ambient[1]=m_Ambient[2]=.0f;m_Ambient[3]=1.0;
		m_Diffuse[0]=m_Diffuse[1]=m_Diffuse[2]=1.0f;m_Diffuse[3]=1.0f;
		m_Specular[0]=m_Specular[1]=m_Specular[2]=1.0f;m_Specular[3]=1.0f;
		m_Shininess=100;
		m_RefrIndex=1.0f;
		m_Reflectance=.5f;
		m_Opacity=1.0f;

		m_pTX=NULL;

		m_OBoxCorner[0][0]=-1;m_OBoxCorner[0][1]=1;m_OBoxCorner[0][2]=-1;
		m_OBoxCorner[1][0]=-1;m_OBoxCorner[1][1]=1;m_OBoxCorner[1][2]=1;
		m_OBoxCorner[2][0]=1;m_OBoxCorner[2][1]=1;m_OBoxCorner[2][2]=1;
		m_OBoxCorner[3][0]=1;m_OBoxCorner[3][1]=1;m_OBoxCorner[3][2]=-1;
		m_OBoxCorner[4][0]=-1;m_OBoxCorner[4][1]=-1;m_OBoxCorner[4][2]=-1;
		m_OBoxCorner[5][0]=-1;m_OBoxCorner[5][1]=-1;m_OBoxCorner[5][2]=1;
		m_OBoxCorner[6][0]=1;m_OBoxCorner[6][1]=-1;m_OBoxCorner[6][2]=1;
		m_OBoxCorner[7][0]=1;m_OBoxCorner[7][1]=-1;m_OBoxCorner[7][2]=-1;

		FindTBCorners();

		break;
	case OBJ_CONE:

		m_OQa=m_OQb=1.0f;
		m_OQc=-.25f;
		m_OQd=m_OQe=m_OQf=m_OQg=m_OQh=m_OQj=m_OQk=.0f;

		m_matrix[1]=m_matrix[2]=m_matrix[3]=m_matrix[4]=m_matrix[6]=m_matrix[7]=m_matrix[8]=m_matrix[9]=m_matrix[11]=m_matrix[12]=m_matrix[13]=m_matrix[14]=.0f;
		m_matrix[0]=m_matrix[5]=m_matrix[10]=m_matrix[15]=1.0f;

		InvMatrix44(m_matrix,m_invmatrix);
		Transform();

		m_Za=.0f;
		m_Zb=2.0f;

		m_nType=OBJ_CONE;
		m_nIType=IOB_QUADR;

		// shiny white
		m_Ambient[0]=m_Ambient[1]=m_Ambient[2]=.0f;m_Ambient[3]=1.0;
		m_Diffuse[0]=m_Diffuse[1]=m_Diffuse[2]=1.0;m_Diffuse[3]=1.0;
		m_Specular[0]=m_Specular[1]=m_Specular[2]=1.0;m_Specular[3]=1.0;
		m_Shininess=100;
		m_RefrIndex=1.0;
		m_Reflectance=.5;
		m_Opacity=1.0;

		m_pTX=NULL;

		m_OBoxCorner[0][0]=-1;m_OBoxCorner[0][1]=1;m_OBoxCorner[0][2]=0;
		m_OBoxCorner[1][0]=-1;m_OBoxCorner[1][1]=1;m_OBoxCorner[1][2]=2;
		m_OBoxCorner[2][0]=1;m_OBoxCorner[2][1]=1;m_OBoxCorner[2][2]=2;
		m_OBoxCorner[3][0]=1;m_OBoxCorner[3][1]=1;m_OBoxCorner[3][2]=0;
		m_OBoxCorner[4][0]=-1;m_OBoxCorner[4][1]=-1;m_OBoxCorner[4][2]=0;
		m_OBoxCorner[5][0]=-1;m_OBoxCorner[5][1]=-1;m_OBoxCorner[5][2]=2;
		m_OBoxCorner[6][0]=1;m_OBoxCorner[6][1]=-1;m_OBoxCorner[6][2]=2;
		m_OBoxCorner[7][0]=1;m_OBoxCorner[7][1]=-1;m_OBoxCorner[7][2]=0;

		FindTBCorners();
		break;

	case OBJ_CYL:
		m_OQa=m_OQb=1.0f;
		m_OQc=m_OQd=m_OQe=m_OQf=m_OQg=m_OQh=m_OQj=.0f;
		m_OQk=-1.0f;

		m_matrix[1]=m_matrix[2]=m_matrix[3]=m_matrix[4]=m_matrix[6]=m_matrix[7]=m_matrix[8]=m_matrix[9]=m_matrix[11]=m_matrix[12]=m_matrix[13]=m_matrix[14]=.0f;
		m_matrix[0]=m_matrix[5]=m_matrix[10]=m_matrix[15]=1.0f;

		InvMatrix44(m_matrix,m_invmatrix);
		Transform();

		m_Za=-1.0f;
		m_Zb=1.0f;

		m_nType=OBJ_CYL;
		m_nIType=IOB_QUADR;

		// shiny white
		m_Ambient[0]=m_Ambient[1]=m_Ambient[2]=.0f;m_Ambient[3]=1.0;
		m_Diffuse[0]=m_Diffuse[1]=m_Diffuse[2]=1.0;m_Diffuse[3]=1.0;
		m_Specular[0]=m_Specular[1]=m_Specular[2]=1.0;m_Specular[3]=1.0;
		m_Shininess=100;
		m_RefrIndex=1.0;
		m_Reflectance=.5;
		m_Opacity=1.0;

		m_pTX=NULL;

		m_OBoxCorner[0][0]=-1;m_OBoxCorner[0][1]=1;m_OBoxCorner[0][2]=-1;
		m_OBoxCorner[1][0]=-1;m_OBoxCorner[1][1]=1;m_OBoxCorner[1][2]=1;
		m_OBoxCorner[2][0]=1;m_OBoxCorner[2][1]=1;m_OBoxCorner[2][2]=1;
		m_OBoxCorner[3][0]=1;m_OBoxCorner[3][1]=1;m_OBoxCorner[3][2]=-1;
		m_OBoxCorner[4][0]=-1;m_OBoxCorner[4][1]=-1;m_OBoxCorner[4][2]=-1;
		m_OBoxCorner[5][0]=-1;m_OBoxCorner[5][1]=-1;m_OBoxCorner[5][2]=1;
		m_OBoxCorner[6][0]=1;m_OBoxCorner[6][1]=-1;m_OBoxCorner[6][2]=1;
		m_OBoxCorner[7][0]=1;m_OBoxCorner[7][1]=-1;m_OBoxCorner[7][2]=-1;

		FindTBCorners();
		break;

	default:
		break;
	}
}

CPrimitive::~CPrimitive()
{
	m_vertices.clear();
	m_objvertices.clear();
}

void CPrimitive::DrawSelBox()
{
	int i;

	glDisable(GL_LIGHTING);
	glColor3f(.0f,.5f,.5f);

	GLboolean bBlend;
	glGetBooleanv(GL_BLEND,&bBlend);
	if (bBlend){
		glLineWidth(1.5);
	} else {
		glLineWidth(1.0);
	}

	for (i=0;i<12;i++){
		glLoadName(10000+m_nID*12+i);
		glBegin(GL_LINES);
		glVertex3fv(m_TBoxCorner[boxedge[i][0]]);
		glVertex3fv(m_TBoxCorner[boxedge[i][1]]);
		glEnd();
	}

	glLineWidth(1.0);
	glEnable(GL_LIGHTING);
	glLoadName(-1);
}

//////////////////////////////////////////////////////////////////////
// render the om_Bject
void CPrimitive::Draw(int style,list<LPPRIMITIVE>& sel)
{
	GLUquadricObj *qu;
	CTexture *t;

	////////////////////////////
	//add your codes here
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,m_Ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,m_Diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,m_Specular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,m_Shininess);
	
	switch (m_nIType){
	case IOB_TRI:
		if (m_bSelected==TRUE){
			glLoadName(-1);
			glDepthMask(0);
			glDisable(GL_LIGHTING);
			glColor3f(1.0,.0,.0);
			glLineWidth(5.0);
			for(list<VERTEX>::iterator vi=m_vertices.begin(); vi!=m_vertices.end();){
				glBegin(GL_LINE_LOOP);
					glVertex3fv((*vi).position); vi++;
					glVertex3fv((*vi).position); vi++;
					glVertex3fv((*vi).position); vi++;
				glEnd();
			}
			glLineWidth(1.0);
			glEnable(GL_LIGHTING);
			glDepthMask(1);
		}

		if (m_pTX!=NULL && style==1){
			glEnable(GL_TEXTURE_2D);
			t=(CTexture*)m_pTX;
			t->Set();
		}

		glLoadName(m_nID);
		glBegin(GL_TRIANGLES);
		for(list<VERTEX>::iterator vi=m_vertices.begin(); vi!=m_vertices.end(); vi++){
			glNormal3fv((*vi).normal);
			if (m_pTX!=NULL){
				glTexCoord2f((*vi).s,(*vi).t);
			}
			glVertex3fv((*vi).position);
		}
		glEnd();

		glDisable(GL_TEXTURE_2D);
		break;

	case IOB_QUADR:
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glMultMatrixf(m_matrix);
		qu=gluNewQuadric();
		
		switch(m_nType){

		case OBJ_ELLI:			
			if (m_bSelected==TRUE){
				glLoadName(-1);
				glDepthMask(0);
				glDisable(GL_LIGHTING);
				glColor3f(1.0,.0,.0);
				glLineWidth(5.0);
				gluQuadricDrawStyle(qu,GLU_LINE);
				gluSphere(qu,1.0f,30,20);
				glLineWidth(1.0);
				glEnable(GL_LIGHTING);
				glDepthMask(1);
			}
			glLoadName(m_nID);
			if (m_pTX!=NULL && style==1){
				glEnable(GL_TEXTURE_2D);
				t=(CTexture*)m_pTX;
				t->Set();
				gluQuadricTexture(qu,GL_TRUE);
			}
			gluQuadricDrawStyle(qu,GLU_FILL);
			gluSphere(qu,1.0f,30,20);			
			break;

		case OBJ_CONE:			
			if (m_bSelected==TRUE){
				glLoadName(-1);
				glDepthMask(0);
				glDisable(GL_LIGHTING);
				glColor3f(1.0,.0,.0);
				glLineWidth(5.0);
				glPushMatrix();
				gluQuadricDrawStyle(qu,GLU_LINE);
				gluCylinder(qu,.0f,1.0f,2.0f,30,20);
				glTranslatef(.0f,.0f,2.0f);
				gluDisk(qu,.0f,1.0f,30,5);
				glPopMatrix();
				glLineWidth(1.0);
				glEnable(GL_LIGHTING);
				glDepthMask(1);
			}
			glLoadName(m_nID);
			if (m_pTX!=NULL && style==1){
				glEnable(GL_TEXTURE_2D);
				t=(CTexture*)m_pTX;
				t->Set();
				gluQuadricTexture(qu,GL_TRUE);
			}
			gluQuadricDrawStyle(qu,GLU_FILL);
			gluCylinder(qu,.0f,1.0f,2.0f,30,20);
			glTranslatef(.0f,.0f,2.0f);
			gluDisk(qu,.0f,1.0f,30,5);			
			break;

		case OBJ_CYL:			
			if (m_bSelected==TRUE){
				glLoadName(-1);
				glDepthMask(0);
				glDisable(GL_LIGHTING);
				glColor3f(1.0,.0,.0);
				glLineWidth(5.0);
				gluQuadricDrawStyle(qu,GLU_LINE);
				glPushMatrix();
				glTranslatef(.0,.0,-1);
				gluCylinder(qu,1.0f,1.0f,2.0,30,10);
				glScalef(1.0f,1.0f,-1.0f);
				gluDisk(qu,.0f,1.0f,30,5);
				glScalef(1.0f,1.0f,-1.0f);
				glTranslatef(.0,.0,2);
				gluDisk(qu,.0f,1.0f,30,5);
				glLineWidth(1.0);
				glEnable(GL_LIGHTING);
				glDepthMask(1);
				glPopMatrix();
			}
			glLoadName(m_nID);
			if (m_pTX!=NULL && style==1){
				glEnable(GL_TEXTURE_2D);
				t=(CTexture*)m_pTX;
				t->Set();
				gluQuadricTexture(qu,GL_TRUE);
			}
			gluQuadricDrawStyle(qu,GLU_FILL);
			glTranslatef(.0,.0,-1);
			gluCylinder(qu,1.0f,1.0f,2.0,30,10);
			glScalef(1.0f,1.0f,-1.0f);
			gluDisk(qu,.0f,1.0f,30,5);
			glScalef(1.0f,1.0f,-1.0f);
			glTranslatef(.0,.0,2);
			gluDisk(qu,.0f,1.0f,30,5);			
			break;
		}
		gluDeleteQuadric(qu);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		break;
	}
	/////////////////
	//if (m_bSelected==TRUE){
	//	DrawSelBox();
	//}

	glLoadName(-1);
}


//////////////////////////////////////////////////////////////////////
// mark om_Bject as selected
void CPrimitive::Select()
{
	m_bSelected=TRUE;
}

//////////////////////////////////////////////////////////////////////
// mark om_Bject as not selected
void CPrimitive::UnSelect()
{
	m_bSelected=FALSE;
}

//////////////////////////////////////////////////////////////////////
// test if om_Bject is selected
bool CPrimitive::IsSelected()
{
	return m_bSelected;
}

//////////////////////////////////////////////////////////////////////
// add offset to all m_pVertices of the object
void CPrimitive::SetOffset(float xoff,float yoff,float zoff)
{
	m_matrix[12]=xoff;
	m_matrix[13]=yoff;
	m_matrix[14]=zoff;
	InvMatrix44(m_matrix,m_invmatrix);
	Transform();
	FindTBCorners();
}

//////////////////////////////////////////////////////////////////////
// 4x4 matrix multiplication
void CPrimitive::MultMatrix44(float *im,float *m,float *rm)
{

    rm[0] = m[0] * im[0] + m[1] * im[4] + m[2] * im[8] + m[3] * im[12];
    rm[1] = m[0] * im[1] + m[1] * im[5] + m[2] * im[9] + m[3] * im[13];
    rm[2] = m[0] * im[2] + m[1] * im[6] + m[2] * im[10] + m[3] * im[14];
    rm[3] = m[0] * im[3] + m[1] * im[7] + m[2] * im[11] + m[3] * im[15];
    rm[4] = m[4] * im[0] + m[5] * im[4] + m[6] * im[8] + m[7] * im[12];
    rm[5] = m[4] * im[1] + m[5] * im[5] + m[6] * im[9] + m[7] * im[13];
    rm[6] = m[4] * im[2] + m[5] * im[6] + m[6] * im[10] + m[7] * im[14];
    rm[7] = m[4] * im[3] + m[5] * im[7] + m[6] * im[11] + m[7] * im[15];
    rm[8] = m[8] * im[0] + m[9] * im[4] + m[10] * im[8] + m[11] * im[12];
    rm[9] = m[8] * im[1] + m[9] * im[5] + m[10] * im[9] + m[11] * im[13];
    rm[10] = m[8] * im[2] + m[9] * im[6] + m[10] * im[10] + m[11] * im[14];
    rm[11] = m[8] * im[3] + m[9] * im[7] + m[10] * im[11] + m[11] * im[15];
    rm[12] = m[12] * im[0] + m[13] * im[4] + m[14] * im[8] + m[15] * im[12];
    rm[13] = m[12] * im[1] + m[13] * im[5] + m[14] * im[9] + m[15] * im[13];
    rm[14] = m[12] * im[2] + m[13] * im[6] + m[14] * im[10] + m[15] * im[14];
    rm[15] = m[12] * im[3] + m[13] * im[7] + m[14] * im[11] + m[15] * im[15];

}

//////////////////////////////////////////////////////////////////////
// 4x4 matrix inverse
void CPrimitive::InvMatrix44(float *m,float *rm)
{
    float det,rdet;
	float im[16];
	det = m[0] * (m[5] * (m[10] * m[15] - m[14] * m[11]) - m[6] * (m[9] * m[15] - m[13] * m[11]) + m[7] * (m[9] * m[14] - m[13] * m[10]));
    det = det - m[1] * (m[4] * (m[10] * m[15] - m[14] * m[11]) - m[6] * (m[8] * m[15] - m[12] * m[11]) + m[7] * (m[8] * m[14] - m[12] * m[10]));
    det = det + m[2] * (m[4] * (m[9] * m[15] - m[13] * m[11]) - m[5] * (m[8] * m[15] - m[12] * m[11]) + m[7] * (m[8] * m[13] - m[12] * m[9]));
    det = det - m[3] * (m[4] * (m[9] * m[14] - m[13] * m[10]) - m[5] * (m[8] * m[14] - m[12] * m[10]) + m[6] * (m[8] * m[13] - m[12] * m[9]));
	
	rdet=1.0f/det;

    im[0] = m[5] * (m[10] * m[15] - m[14] * m[11]) - m[6] * (m[9] * m[15] - m[13] * m[11]) + m[7] * (m[9] * m[14] - m[13] * m[10]);
    im[1] = -(m[4] * (m[10] * m[15] - m[14] * m[11]) - m[6] * (m[8] * m[15] - m[12] * m[11]) + m[7] * (m[8] * m[14] - m[12] * m[10]));
    im[2] = m[4] * (m[9] * m[15] - m[13] * m[11]) - m[5] * (m[8] * m[15] - m[12] * m[11]) + m[7] * (m[8] * m[13] - m[12] * m[9]);
    im[3] = -(m[4] * (m[9] * m[14] - m[13] * m[10]) - m[5] * (m[8] * m[14] - m[12] * m[10]) + m[6] * (m[8] * m[13] - m[12] * m[9]));
    im[4] = -(m[1] * (m[10] * m[15] - m[14] * m[11]) - m[2] * (m[9] * m[15] - m[13] * m[11]) + m[3] * (m[9] * m[14] - m[13] * m[10]));
    im[5] = m[0] * (m[10] * m[15] - m[14] * m[11]) - m[2] * (m[8] * m[15] - m[12] * m[11]) + m[3] * (m[8] * m[14] - m[12] * m[10]);
    im[6] = -(m[0] * (m[9] * m[15] - m[13] * m[11]) - m[1] * (m[8] * m[15] - m[12] * m[11]) + m[3] * (m[8] * m[13] - m[12] * m[9]));
    im[7] = m[0] * (m[9] * m[14] - m[13] * m[10]) - m[1] * (m[8] * m[14] - m[12] * m[10]) + m[2] * (m[8] * m[13] - m[12] * m[9]);
    im[8] = m[1] * (m[6] * m[15] - m[14] * m[7]) - m[2] * (m[5] * m[15] - m[13] * m[7]) + m[3] * (m[5] * m[14] - m[13] * m[6]);
    im[9] = -(m[0] * (m[6] * m[15] - m[14] * m[7]) - m[2] * (m[4] * m[15] - m[12] * m[7]) + m[3] * (m[4] * m[14] - m[12] * m[6]));
    im[10] = m[0] * (m[5] * m[15] - m[13] * m[7]) - m[1] * (m[4] * m[15] - m[12] * m[7]) + m[3] * (m[4] * m[13] - m[12] * m[5]);
    im[11] = -(m[0] * (m[5] * m[14] - m[13] * m[6]) - m[1] * (m[4] * m[14] - m[12] * m[6]) + m[2] * (m[4] * m[13] - m[12] * m[5]));
    im[12] = -(m[1] * (m[6] * m[11] - m[10] * m[7]) - m[2] * (m[5] * m[11] - m[9] * m[7]) + m[3] * (m[5] * m[10] - m[9] * m[6]));
    im[13] = m[0] * (m[6] * m[11] - m[10] * m[7]) - m[2] * (m[4] * m[11] - m[8] * m[7]) + m[3] * (m[4] * m[10] - m[8] * m[6]);
    im[14] = -(m[0] * (m[5] * m[11] - m[9] * m[7]) - m[1] * (m[4] * m[11] - m[8] * m[7]) + m[3] * (m[4] * m[9] - m[8] * m[5]));
    im[15] = m[0] * (m[5] * m[10] - m[9] * m[6]) - m[1] * (m[4] * m[10] - m[8] * m[6]) + m[2] * (m[4] * m[9] - m[8] * m[5]);

    im[0]*=rdet;
    im[1]*=rdet;
    im[2]*=rdet;
    im[3]*=rdet;
    im[4]*=rdet;
    im[5]*=rdet;
    im[6]*=rdet;
    im[7]*=rdet;
    im[8]*=rdet;
    im[9]*=rdet;
    im[10]*=rdet;
    im[11]*=rdet;
    im[12]*=rdet;
    im[13]*=rdet;
    im[14]*=rdet;
    im[15]*=rdet;

	TransMatrix44(im,rm);
}

//////////////////////////////////////////////////////////////////////
// 4x4 matrix multiplication
void CPrimitive::TransMatrix44(float *m,float *tm)
{
	tm[0]=m[0];
	tm[1]=m[4];
	tm[2]=m[8];
	tm[3]=m[12];
	tm[4]=m[1];
	tm[5]=m[5];
	tm[6]=m[9];
	tm[7]=m[13];
	tm[8]=m[2];
	tm[9]=m[6];
	tm[10]=m[10];
	tm[11]=m[14];
	tm[12]=m[3];
	tm[13]=m[7];
	tm[14]=m[11];
	tm[15]=m[15];
}

//////////////////////////////////////////////////////////////////////
// generate transformed primitive from object space one form
void CPrimitive::Transform()
{
	float inQ[16],tmp[16],Q[16],transM[16];

	switch(m_nIType){
	case IOB_QUADR:

		inQ[0]=m_OQa;
		inQ[5]=m_OQb;
		inQ[10]=m_OQc;
		inQ[15]=m_OQk;
		inQ[1]=inQ[4]=m_OQd;
		inQ[6]=inQ[9]=m_OQe;
		inQ[2]=inQ[8]=m_OQf;
		inQ[3]=inQ[12]=m_OQg;
		inQ[7]=inQ[13]=m_OQh;
		inQ[11]=inQ[14]=m_OQj;

		TransMatrix44(m_invmatrix,transM);

		MultMatrix44(transM,inQ,tmp);
		MultMatrix44(tmp,m_invmatrix,Q);

		m_Qa=Q[0];
		m_Qb=Q[5];
		m_Qc=Q[10];
		m_Qk=Q[15];
		m_Qd=Q[4];
		m_Qe=Q[9];
		m_Qf=Q[8];
		m_Qg=Q[12];
		m_Qh=Q[13];
		m_Qj=Q[14];
		break;

	case IOB_TRI:
		list<VERTEX>::iterator vi=m_vertices.begin();
		for(list<VERTEX>::iterator ovi=m_objvertices.begin(); ovi!=m_objvertices.end(); ovi++){
			V3 t2;

			MatrixMultVector3(m_matrix,(float*)(*ovi).position,(float*)t2);

			t2+=V3(m_matrix[12],m_matrix[13],m_matrix[14]);
			
			(*vi).position=t2;
			(*vi).s=(*ovi).s;
			(*vi).t=(*ovi).t;

			vi++;
		}

		for(list<VERTEX>::iterator vi=m_vertices.begin(); vi!=m_vertices.end();){
			V3 t0,t1,t2;
			t0=(*vi).position; vi++;
			t1=(*vi).position-t0; vi++;
			t2=(*vi).position-t0; vi++;

			t0=t1.cross(t2);
			t0.normalize();

			vi--;
			vi--;
			vi--;

			(*vi).normal=t0; vi++;
			(*vi).normal=t0; vi++;
			(*vi).normal=t0; vi++;
		}

		// boundm_OQg ellipsom_nID
		inQ[0]=m_OBa;
		inQ[5]=m_OBb;
		inQ[10]=m_OBc;
		inQ[15]=m_OBk;
		inQ[1]=inQ[4]=m_OBd;
		inQ[6]=inQ[9]=m_OBe;
		inQ[2]=inQ[8]=m_OBf;
		inQ[3]=inQ[12]=m_OBg;
		inQ[7]=inQ[13]=m_OBh;
		inQ[11]=inQ[14]=m_OBj;

		TransMatrix44(m_invmatrix,transM);

		MultMatrix44(transM,inQ,tmp);
		MultMatrix44(tmp,m_invmatrix,Q);

		m_Ba=Q[0];
		m_Bb=Q[5];
		m_Bc=Q[10];
		m_Bk=Q[15];
		m_Bd=Q[4];
		m_Be=Q[9];
		m_Bf=Q[8];
		m_Bg=Q[12];
		m_Bh=Q[13];
		m_Bj=Q[14];

		break;
	}
}

//////////////////////////////////////////////////////////////////////
// find transformed bounding box corners
void CPrimitive::FindTBCorners()
{
	int i;

	for (i=0;i<8;i++){
		float u[4],v[4];
		u[0]=m_OBoxCorner[i][0];
		u[1]=m_OBoxCorner[i][1];
		u[2]=m_OBoxCorner[i][2];
		u[3]=1.0f;

		MatrixMultVector4(m_matrix,u,v);

		m_TBoxCorner[i][0]=v[0];
		m_TBoxCorner[i][1]=v[1];
		m_TBoxCorner[i][2]=v[2];
	}
}

//////////////////////////////////////////////////////////////////////
// m_Backup M, for rotation and translation
void CPrimitive::BackupM()
{
	int i;

	for (i=0;i<16;i++){
		m_oldmatrix[i]=m_matrix[i];
	}
}

//////////////////////////////////////////////////////////////////////
// restore M, for rotation and translation
void CPrimitive::RestoreM()
{
	int i;

	for (i=0;i<16;i++){
		m_matrix[i]=m_oldmatrix[i];
	}
	InvMatrix44(m_matrix,m_invmatrix);
	Transform();
	FindTBCorners();
}

//////////////////////////////////////////////////////////////////////
// replace M by NewM
void CPrimitive::LoadM(float *NewM)
{
	int i;

	for (i=0;i<16;i++){
		m_matrix[i]=NewM[i];
	}
	InvMatrix44(m_matrix,m_invmatrix);
	Transform();
	FindTBCorners();
}

//////////////////////////////////////////////////////////////////////
// multiply a rotation matrix RM to M
void CPrimitive::MultM(float *RM)
{
	float tmpM[16];

	MultMatrix44(RM,m_matrix,tmpM);
	LoadM(tmpM);
}

//////////////////////////////////////////////////////////////////////
// return M
void CPrimitive::GetM(float *rM)
{
	int i;

	for (i=0;i<16;i++){
		rM[i]=m_matrix[i];
	}
}


CPrimitive *CPrimitive::Clone()
{
	CPrimitive *p=new CPrimitive(m_nType,m_nID);
	int i;

	p->m_nType=m_nType;
	p->m_nIType=m_nIType;
	p->m_nID=m_nID;

	p->m_Qa=m_Qa;
	p->m_Qb=m_Qb;
	p->m_Qc=m_Qc;
	p->m_Qd=m_Qd;
	p->m_Qe=m_Qe;
	p->m_Qf=m_Qf;
	p->m_Qg=m_Qg;
	p->m_Qh=m_Qh;
	p->m_Qj=m_Qj;
	p->m_Qk=m_Qk;

	p->m_OQa=m_OQa;
	p->m_OQb=m_OQb;
	p->m_OQc=m_OQc;
	p->m_OQd=m_OQd;
	p->m_OQe=m_OQe;
	p->m_OQf=m_OQf;
	p->m_OQg=m_OQg;
	p->m_OQh=m_OQh;
	p->m_OQj=m_OQj;
	p->m_OQk=m_OQk;
	p->m_Za=m_Za;
	p->m_Zb=m_Zb;

	for (i=0;i<16;i++){
		p->m_matrix[i]=m_matrix[i];
		p->m_invmatrix[i]=m_invmatrix[i];
	}

	p->m_nNumTriangle=m_nNumTriangle;
	if (m_nIType==IOB_TRI){
		list<VERTEX>::iterator vi=m_vertices.begin();
		for(list<VERTEX>::iterator pvi=p->m_vertices.begin(); pvi!=p->m_vertices.end(); pvi++){			
			(*vi)=(*pvi);
			vi++;
		}

		vi=m_objvertices.begin();
		for(list<VERTEX>::iterator pvi=p->m_objvertices.begin(); pvi!=p->m_objvertices.end(); pvi++){			
			(*vi)=(*pvi);
			vi++;
		}
	}

	p->m_Ba=m_Ba;
	p->m_Bb=m_Bb;
	p->m_Bc=m_Bc;
	p->m_Bd=m_Bd;
	p->m_Be=m_Be;
	p->m_Bf=m_Bf;
	p->m_Bg=m_Bg;
	p->m_Bh=m_Bh;
	p->m_Bj=m_Bj;
	p->m_Bk=m_Bk;

	p->m_OBa=m_OBa;
	p->m_OBb=m_OBb;
	p->m_OBc=m_OBc;
	p->m_OBd=m_OBd;
	p->m_OBe=m_OBe;
	p->m_OBf=m_OBf;
	p->m_OBg=m_OBg;
	p->m_OBh=m_OBh;
	p->m_OBj=m_OBj;
	p->m_OBk=m_OBk;

	for (i=0;i<8;i++){
		p->m_OBoxCorner[i]=m_OBoxCorner[i];
		p->m_TBoxCorner[i]=m_TBoxCorner[i];
	}

	for (i=0;i<4;i++){
		p->m_Ambient[i]=m_Ambient[i];
		p->m_Diffuse[i]=m_Diffuse[i];
		p->m_Specular[i]=m_Specular[i];
	}

	p->m_Shininess=m_Shininess;
	p->m_RefrIndex=m_RefrIndex;
	p->m_Reflectance=m_Reflectance;
	p->m_Opacity=m_Opacity;

	p->m_pTX=m_pTX;

	p->m_bSelected=FALSE;

	return p;
}

void CPrimitive::FindNormal(int id0, int id1, int id2, list<VERTEX>& vlist)
{
	list<VERTEX>::iterator i0=vlist.begin();
	list<VERTEX>::iterator i1=vlist.begin();
	list<VERTEX>::iterator i2=vlist.begin();

	for(int i=0;i<id0;i++) i0++;
	for(int i=0;i<id1;i++) i1++;
	for(int i=0;i<id2;i++) i2++;

	V3 vect1,vect2,norm;

	vect1=(*i0).position-(*i1).position;
	vect2=(*i2).position-(*i1).position;

	norm=vect2.cross(vect1);
	norm.normalize();

	(*i0).normal=(*i1).normal=(*i2).normal=norm;
}
float  CPrimitive::rayIntersect(V3 rayStart, V3 rayDir, V3& interp, V3& intern)
{
	float equationCoeffs[16]={m_Qa,m_Qd,m_Qf,m_Qg,m_Qd,m_Qb,m_Qe,m_Qh,m_Qf,m_Qe,m_Qc,m_Qj,m_Qg,m_Qh,m_Qj,m_Qk};
	if (m_nIType==1)
	{
		float t=-1;
		float tmin=MAX_DIST;
		V3 tempIntersection;
		for(list<VERTEX>::iterator vi=m_vertices.begin(); vi!=m_vertices.end(); vi++){
			V3 p0 = vi->position;
			vi++;
			V3 p1 = vi->position;
			vi++;
			V3 p2 = vi->position;
			float tempT;
			if(IntersectTriangle(rayStart,rayDir, p0, p1,p2, tempT,tempIntersection))
			{
				if(tempT<tmin&&tempT>0.01)
				{
					tmin=tempT;
					t=tempT;
					intern = (p1-p0).cross(p2-p0);
					if(intern.dot(vi->normal)<0)intern=-intern;
					intern.normalize();
					interp=tempIntersection;
				}
			}
		}
		return t;
	}
if (m_nIType==2)
{
	

		float _top[4]={0,0,m_Zb ,1};
		float _bott[4]={0,0,m_Za, 1};
		float top_point[4];
		float bott_point[4];

		V3 top_normal;
		V3 bott_normal;

		if(m_nType == OBJ_CONE || m_nType == OBJ_CYL)
		{
			float local_tempP1[4]={1,0,m_Zb ,1};
			float local_tempP2[4]={0,1,m_Zb ,1};
			float global_tempP1[4];
			float global_tempP2[4];
			MatrixMultVector(m_matrix, _top, top_point);
			MatrixMultVector(m_matrix, _bott, bott_point);
			MatrixMultVector(m_matrix, local_tempP1, global_tempP1);
			MatrixMultVector(m_matrix, local_tempP2, global_tempP2);
			V3 tempE1(global_tempP1[0]-top_point[0],global_tempP1[1]-top_point[1],global_tempP1[2]-top_point[2]);
			V3 tempE2(global_tempP2[0]-top_point[0],global_tempP2[1]-top_point[1],global_tempP2[2]-top_point[2]);
			top_normal=tempE1.cross(tempE2);
			top_normal.normalize();
			bott_normal= -top_normal;

		}

		float top_t, bott_t;
		float top_intersect[4];
		float bott_intersect[4];

		//////////////////
		///0 compute  intersection with surface
		V3 intersection;
		float quad_t;
		if(IntersectQuadratic(rayStart,rayDir, equationCoeffs, quad_t, intersection))
		{
			// transform intersection to object coordinate
			float local_p[4];
			float global_p[4]={intersection[0], intersection[1], intersection[2], 1};
			MatrixMultVector(m_invmatrix, global_p, local_p);
			// if in bounding box
			if( local_p[2]<=m_Zb&&local_p[2]>=m_Za)
			{

				interp = intersection;
				float temp[4];
				MatrixMultVector(equationCoeffs,global_p,temp);
				intern[0] = 2*temp[0];
				intern[1] = 2*temp[1];
				intern[2] = 2*temp[2];

				intern.normalize();

				//if(intern.dot(rayDir)< 0)
					return quad_t;

			}
		}

		//////1 compute intersection with top plane

		if(m_nType == OBJ_CONE  || m_nType == OBJ_CYL)
		{	
			V3 planePoint(top_point[0],top_point[1],top_point[2]);
			if(!IntersectPlane(rayStart, rayDir,top_normal, planePoint,  interp,top_t))return -1;

			// check if in the bound

			float local_top_intersect[4];
			top_intersect[0]=interp[0];
			top_intersect[1]=interp[1];
			top_intersect[2]=interp[2];
			top_intersect[3]=1;
			MatrixMultVector(m_invmatrix, top_intersect, local_top_intersect);


			if(local_top_intersect[0] * local_top_intersect[0] + local_top_intersect[1]*local_top_intersect[1]+ (local_top_intersect[2]-m_Zb)*(local_top_intersect[2]-m_Zb) < 1 )
			{

				intern = top_normal;
				//if(intern.dot(rayDir)< 0)
					return top_t;

			}

		}// top plane


		////////2 check intersection with bottom plane

		if(m_nType == OBJ_CYL)
		{
			V3 planePoint(bott_point[0],bott_point[1],bott_point[2]);
			if(!IntersectPlane(rayStart, rayDir,bott_normal, planePoint,  interp,bott_t))return -1;

			bott_intersect[0] = interp[0];
			bott_intersect[1] = interp[1];
			bott_intersect[2] = interp[2];
			bott_intersect[3]=1;
			// check if in the bound

			float local_bott_intersect[4];
			MatrixMultVector(m_invmatrix, bott_intersect, local_bott_intersect);



			if(local_bott_intersect[0] * local_bott_intersect[0] +local_bott_intersect[1]*local_bott_intersect[1]+(local_bott_intersect[2] - m_Za) * (local_bott_intersect[2] - m_Za) < 1 )
			{

				intern = bott_normal;

				//if(intern.dot(rayDir)< 0)
				
					return bott_t;
				

			}
		}// bottom plane


	//}// other type
}
	return -1;

}
void CPrimitive::GetDiffuse(V3 point,V3& diffuse)
{
	if(m_nType==OBJ_ELLI&&m_pTX!=NULL)
	{
		GetTextureValueAt(point,diffuse);
	}
	else
	{
		diffuse[0]=m_Diffuse[0];
		diffuse[1]=m_Diffuse[1];
		diffuse[2]=m_Diffuse[2];
	}
}
void CPrimitive::GetAmbient(V3 point,V3& ambient)
{
	if(m_nType==OBJ_ELLI&&m_pTX!=NULL)
	{
		ambient[0]=0;
		ambient[1]=0;
		ambient[2]=0;
	}
	else
	{
		ambient[0]=m_Ambient[0];
		ambient[1]=m_Ambient[1];
		ambient[2]=m_Ambient[2];
	}
}
void CPrimitive::GetSpecular(V3 point,V3& specular)
{
	if(m_nType==OBJ_ELLI&&m_pTX!=NULL)
	{
		
		GetTextureValueAt(point,specular);
	}
	else
	{
		specular[0]=m_Specular[0];
		specular[1]=m_Specular[1];
		specular[2]=m_Specular[2];
	}
}
void	CPrimitive::GetTextureValueAt(V3& point,V3& color){
	CTexture *t;
	if(m_pTX==NULL)return;
	float tempColor[4];
	switch(m_nType){

		case OBJ_ELLI:	
			float local_p[4];
			float global_p[4]={point[0], point[1], point[2], 1};
			MatrixMultVector(m_invmatrix, global_p, local_p);
			float theta;
			if(fabs(local_p[0])<0.00001)
			{
				if(local_p[0]>0)theta=0;
				else theta=3*M_PI/2;
			}
			else{
				theta=atan(local_p[1]/local_p[0]);			
				if(local_p[0]<0)theta=theta+M_PI;
				else if(local_p[1]<0)theta=theta+2*M_PI;
				theta-=M_PI/2;//(0,1) is 0
				if(theta<0)theta+=2*M_PI;
			}
			float fai=asin(local_p[2]);
			float u,v;
			u=theta/(2*M_PI);
			v=(fai+M_PI/2)/M_PI;			
			t=(CTexture*)m_pTX;
			
			t->GetColorAt(u,v,tempColor);
			color[0]=tempColor[0];
			color[1]=tempColor[1];
			color[2]=tempColor[2];
			break;
	}
}