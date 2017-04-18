// Texture.cpp: implementation of the CTexture class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Texture.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "math.h"
#include "JpegFile.h"
#include "Bmpfile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTexture::CTexture(CString infn)
{	
	m_strFilename=infn;
}

CTexture::CTexture()
{
	m_bReady=FALSE;
}

CTexture::~CTexture()
{
	glDeleteTextures(1,&m_uTxName);
	delete m_pData;
}

void CTexture::Set()
{
	glBindTexture( GL_TEXTURE_2D, m_uTxName ); 
}

void CTexture::GetColorAt(float s,float t,float *color)
{
	int x,y;
	float ts,tt;

	ts=s+(float)fabs(floor(s));
	tt=t+(float)fabs(floor(t));
	ts=(float)fmod(ts,1.0f);
	tt=(float)fmod(tt,1.0f);

	x=(int)((float)ts*m_nTxDim);
	y=(int)((float)tt*m_nTxDim);

	color[0]=(float)m_pData[m_nTxDim*y*3+x*3]/255.0f;
	color[1]=(float)m_pData[m_nTxDim*y*3+x*3+1]/255.0f;
	color[2]=(float)m_pData[m_nTxDim*y*3+x*3+2]/255.0f;
}

int CTexture::Read(CString strFileName)
{
	BYTE* bBuf;
	unsigned int uTxWidth,uTxHeight;
	unsigned char *pData;

	if (strFileName.Right(3).CompareNoCase("jpg")==0){
		bBuf=JpegFile::JpegFileToRGB(strFileName, &uTxWidth, &uTxHeight);
	} else {
		BMPFile bmpf;
		bBuf=bmpf.LoadBMP(strFileName, &uTxWidth, &uTxHeight);
	}
	if (bBuf==NULL) return 1;

	pData=(unsigned char*)bBuf;
	JpegFile::VertFlipBuf(bBuf, uTxWidth * 3, uTxHeight);

	m_nWidth=(int)uTxWidth;
	m_nHeight=(int)uTxHeight;
	int j=strFileName.ReverseFind('\\');
	if (j==-1) j=strFileName.ReverseFind('/');
	if (j==-1){
		m_strFilename=strFileName;
	} else {
		m_strFilename=strFileName.Mid(j+1);
	}

	if (m_nWidth>=512 && m_nHeight>=512) m_nTxDim=512;
	else if (m_nWidth>=256 && m_nHeight>=256) m_nTxDim=256;
	else if (m_nWidth>=128 && m_nHeight>=128) m_nTxDim=128;
	else if (m_nWidth>=64 && m_nHeight>=64) m_nTxDim=64;
	else if (m_nWidth>=32 && m_nHeight>=32) m_nTxDim=32;
	else if (m_nWidth>=16 && m_nHeight>=16) m_nTxDim=16;
	else if (m_nWidth>=8 && m_nHeight>=8) m_nTxDim=8;
	else if (m_nWidth>=4 && m_nHeight>=4) m_nTxDim=4;

	
	glGenTextures(1,&m_uTxName);
	glBindTexture(GL_TEXTURE_2D,m_uTxName); 

	glPixelStorei(GL_PACK_ALIGNMENT,1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glPixelStorei(GL_PACK_ALIGNMENT,1);

	m_pData=new unsigned char[m_nTxDim*m_nTxDim*3];
	gluScaleImage(GL_RGB,m_nWidth,m_nHeight,GL_UNSIGNED_BYTE,(void*)pData,m_nTxDim,m_nTxDim,GL_UNSIGNED_BYTE,(void*)m_pData);
	delete pData;
	glTexImage2D( GL_TEXTURE_2D,0,3,m_nTxDim,m_nTxDim,0,GL_RGB ,GL_UNSIGNED_BYTE,m_pData);	
	
	return 0;

}


void CTexture::DefineAgain()
{
	glGenTextures(1,&m_uTxName);
	glBindTexture(GL_TEXTURE_2D,m_uTxName); 

	glPixelStorei(GL_PACK_ALIGNMENT,1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glPixelStorei(GL_PACK_ALIGNMENT,1);

	glTexImage2D( GL_TEXTURE_2D,0,3,m_nTxDim,m_nTxDim,0,GL_RGB ,GL_UNSIGNED_BYTE,m_pData);
}
