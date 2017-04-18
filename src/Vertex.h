// Vertex.h: interface for the CVertex class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "v3.h"

class CVertex
{
public:
	V3 position;		// coordinate
	V3 normal;
	float s,t;			// texture coordinate
public:
	CVertex(float,float,float);
	CVertex(float,float,float,float,float);
	CVertex(float,float,float,float,float,float);
	CVertex(float,float,float,float,float,float,float,float);
	void	SetTxCood(float,float);
	virtual ~CVertex();
};

typedef CVertex VERTEX, *LPVERTEX;
