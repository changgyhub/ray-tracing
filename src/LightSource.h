// LightSource.h: interface for the CLightSource class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "v3.h"

class CLightSource
{
public:
	CLightSource(float,float,float,int);
	CLightSource(float,float,float,float,float,float,int);
	void TurnOn();
	void TurnOff();
	void SetColor(float,float,float);
	void Draw();
	virtual ~CLightSource();

	int		m_nID;
	V3		position;
	V3		color;
	bool	m_bON;		// on or off

	void	Select();
	void	UnSelect();
	bool	IsSelected();

private:
	bool	m_bSelected;
};

typedef CLightSource LIGHTSOURCE, *LPLIGHTSOURCE;
