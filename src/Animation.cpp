// Animation.cpp: implementation of the CAnimation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Sprite.h"
#include "Animation.h"
#include "GameProc.h"


namespace VampireIvo {


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAnimation::CAnimation( CSprite *pSprite, float animspeed )
{
	try
	{
		m_pSprite = pSprite;
		m_animspeed = animspeed;
		m_curframe = 0.0f;
		m_numframes = m_pSprite->getFrames();
	}
	catch(...)
	{
		VampireIvo::LOG( "CAnimation::CAnimation() : Invalid sprite pointer !");
	}
}

CAnimation::~CAnimation()
{

}


void CAnimation::Update( int x, int y, SDL_Surface *pScreen, float *mpf )
{

	int curframe;

	m_curframe += ( (*mpf) * m_animspeed );
	if ( (int)m_curframe >= m_numframes )
		m_curframe = 0.0f;

	curframe = (int)m_curframe;

	
	m_pSprite->Update( x, y, curframe, pScreen );
}


int CAnimation::getWidth()
{
	return m_pSprite->getHeight();
}

int CAnimation::getHeight()
{
	return m_pSprite->getHeight();
}

int* CAnimation::getMask() 
{ 
	return m_pSprite->getMask(); 
}


} // end namespace