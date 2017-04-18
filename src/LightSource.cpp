// LightSource.cpp: implementation of the CLightSource class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LightSource.h"
#include <gl/gl.h>
#include <gl/glu.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLightSource::CLightSource(float inx,float iny,float inz,int inid)
{
	position=V3(inx,iny,inz);
	color=V3(1,1,1);	// default color white
	m_bON=true;
	m_nID=inid;
	m_bSelected=false;
}

CLightSource::CLightSource(float inx,float iny,float inz,float inr,float ing,float inb,int inid)
{
	position=V3(inx,iny,inz);
	color=V3(inr,ing,inb);
	m_bON=true;
	m_nID=inid;
	m_bSelected=false;
}

CLightSource::~CLightSource()
{

}

void CLightSource::TurnOn()
{
	m_bON=TRUE;
}

void CLightSource::TurnOff()
{
	m_bON=FALSE;
}

void CLightSource::SetColor(float inr,float ing,float inb)
{
	color=V3(inr,ing,inb);
}

void CLightSource::Draw()
{
	GLUquadricObj *qu;

	glDisable(GL_LIGHTING);

	if (m_bSelected==TRUE){
		glColor3f(.0,.5,.5);
		glLoadName(60018+3*m_nID);
		glBegin(GL_LINES);
		glVertex3fv(position-V3(10,0,0));
		glVertex3fv(position+V3(10,0,0));
		
		glEnd();
		glColor3f(.5,.0,.5);
		glLoadName(60018+3*m_nID+1);
		glBegin(GL_LINES);
		glVertex3fv(position-V3(0,10,0));
		glVertex3fv(position+V3(0,10,0));
		
		glEnd();
		glColor3f(.5,.5,.0);
		glLoadName(60018+3*m_nID+2);
		glBegin(GL_LINES);
		glVertex3fv(position-V3(0,0,10));
		glVertex3fv(position+V3(0,0,10));
		
		glEnd();
	}

	glLoadName(1000+m_nID);
	glPushMatrix();
	glTranslatef(position.x,position.y,position.z);
	qu=gluNewQuadric();

	if (m_bSelected==TRUE){
		glDepthMask(0);
		glColor3f(.0,1.0,.0);
		glLineWidth(5.0);
		gluQuadricDrawStyle(qu,GLU_LINE);
		gluSphere(qu,1.0f,10,10);
		glLineWidth(1.0);
		glDepthMask(1);
	}
	
	
	glColor3fv(color);
	if (m_bON==TRUE){
		gluQuadricDrawStyle(qu,GLU_FILL);
	} else {
		gluQuadricDrawStyle(qu,GLU_LINE);
	}
	gluSphere(qu,1.0f,10,10);
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glLoadName(-1);
}

void CLightSource::Select()
{
	m_bSelected=TRUE;
}

void CLightSource::UnSelect()
{
	m_bSelected=FALSE;
}

bool CLightSource::IsSelected()
{
	return m_bSelected;
}