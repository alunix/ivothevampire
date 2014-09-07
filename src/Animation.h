// Animation.h: interface for the CAnimation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANIMATION_H__9F65541F_5D9D_4BF0_ABF1_6995B847A5BF__INCLUDED_)
#define AFX_ANIMATION_H__9F65541F_5D9D_4BF0_ABF1_6995B847A5BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace VampireIvo {

class CSprite;


class CAnimation  
{
public:
	CAnimation( CSprite *pSprite, float animspeed );
	virtual ~CAnimation();

	void Update( int x, int y, SDL_Surface *pScreen, float *mpf );

	int getWidth();
	int getHeight();
	int* getMask();

private:
	float m_curframe;
	float m_animspeed;
	int m_numframes;

	CSprite *m_pSprite;

};


} // end namespace

#endif // !defined(AFX_ANIMATION_H__9F65541F_5D9D_4BF0_ABF1_6995B847A5BF__INCLUDED_)
