#pragma once
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include "v3.h"
#include "Lightsource.h"
#include "Primitive.h"
#include "rayDoc.h"
#include "rayView.h"
using namespace std;


#define MAX_DIST	1000000

extern int winWidth;		// window width
extern int winHeight;	// window height

extern V3 ViewPoint;		// view point
extern V3 ImageLL;		// coordinates at lower left corner of image
extern V3 ImageLR;		// coordinates at lower right corner of image
extern V3 ImageUL;		// coordinates at upper left corner of image
extern V3 ImageUR;		// coordinates at upper right corner of image

extern int MaxTraceDepth;			// depth of recursive ray-tracing

// scene objects
extern vector<CLightSource *> vLightSource;		// array of light sources
extern vector<CPrimitive *> vObjects;				// array of objects
/************************************************************************
initialize function
***********************************************************************/
void initVariables(CRayDoc * pDoc, CRayView * view);
/***********************************
useful functions
 ***********************************/
void MatrixMultVector(float *m,float *v,float *rv);//rv=m*v
void VectorMultMatrix(float *v,float *m,float *lv);//lv=v^Tm
float VectorMultVector(float *v1,float *v2);//v3=v1^Tv2

/***********************************
 ray trace functions
 ***********************************/
void RayTracing(V3 * colorMap);
void Trace(V3& rayStart, V3& rayDir, int depth, V3& color);
void Shade(CPrimitive *obj,V3& rayStart, V3& rayDir, V3& intersection, V3& normal, int depth, V3& color);
bool Intersect(V3 rayStart, V3 rayDir, CPrimitive *&objHit, V3& intersection, V3& normal);
bool  IntersectQuadratic(V3 rayStart,V3 rayDir, float * coeffMatrix, float& t,V3& intersection);
bool  IntersectTriangle(V3 rayStart,V3 rayDir, V3 v0, V3 v1,V3 v2, float& t,V3& intersection);
bool  IntersectPlane(V3 rayStart, V3 rayDir,V3 planeNormal, V3 planePoint, V3& interp,float& t);
