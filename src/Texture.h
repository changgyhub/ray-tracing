// Texture.h: interface for the CTexture class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CTexture
{
public:
	void DefineAgain();
	BOOL m_bReady;
	unsigned long m_uLID;
	int Read(CString strFileName);
	CTexture();
	CTexture(CString infn);
	virtual ~CTexture();

	int m_nWidth,m_nHeight;	// texture dimension
	int m_nTxDim;
	unsigned char *m_pData;	// data, length: 3wh for type 0, wh for type 1
	CString m_strFilename;	// file name, used to identity texture
	unsigned int m_uTxName;	// texture name returned by opengl

	void	Set();			// set this texture to current
	void	GetColorAt(float,float,float*);	//return color3f at given coordinate, but no maxification ot minification, just find the nearest value
};

typedef CTexture TEXTURE, *LPTEXTURE;
