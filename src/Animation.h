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
