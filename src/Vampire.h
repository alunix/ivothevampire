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

#if !defined(AFX_VAMPIRE_H__BC4C3FF1_9255_4420_818B_3A7C2A106C03__INCLUDED_)
#define AFX_VAMPIRE_H__BC4C3FF1_9255_4420_818B_3A7C2A106C03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


namespace VampireIvo {


class CVampire;
class CAnimation;
class CSprite;


class CVampires
{

public:

    enum // move methods
    {
        MV_HORIZONTAL = 0,
        MV_VERTICAL = 1,
        MV_SINUS_HORIZ = 2,
        MV_SINUS_VERT = 3
    };



    CVampires();
    virtual ~CVampires();

    void Destroy();
    bool CreateVampire( CSprite *pSpriteFly, CSprite *pSpriteDie );
    int CheckHit( int mx, int my, int mw, int mh );
    void Update( SDL_Surface *pScreen, float *mpf );
    
    int getSize() { return m_vampires.size(); };

private:
    vector<CVampire*> m_vampires;



};


class CVampire  
{
public:
    CVampire();
    virtual ~CVampire();

    float m_x, m_y;
    float m_speed;
    float m_angle;
    float m_speed_angle;

    int   m_life;
    int   m_move_method;
    
    CAnimation *m_panimFly;
    CAnimation *m_panimDie;

    bool m_bKilled;

};



} //end namespace

#endif // !defined(AFX_VAMPIRE_H__BC4C3FF1_9255_4420_818B_3A7C2A106C03__INCLUDED_)
