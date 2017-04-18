// Vertex.cpp: implementation of the CVertex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Vertex.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVertex::CVertex(float inx,float iny,float inz,float innx,float inny,float innz)
{
	position=V3(inx,iny,inz);
	normal=V3(innx,inny,innz);
}

CVertex::CVertex(float inx,float iny,float inz,float innx,float inny,float innz,float ints,float intt)
{
	position=V3(inx,iny,inz);
	normal=V3(innx,inny,innz);
	s=ints;
	t=intt;
}

CVertex::CVertex(float inx,float iny,float inz)
{
	position=V3(inx,iny,inz);
}

CVertex::CVertex(float inx,float iny,float inz,float ints,float intt)
{
	position=V3(inx,iny,inz);
	s=ints;
	t=intt;
}

CVertex::~CVertex()
{
}

void CVertex::SetTxCood(float ints,float intt)
{
	s=ints;
	t=intt;
}

