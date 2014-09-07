/*

The MIT License

Copyright (c) 2005 Petar Petrov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include "vampire_ivo.h"
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