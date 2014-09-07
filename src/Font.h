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

#if !defined(AFX_FONT_H__C1B6695D_3811_4E8E_8B66_1CA0733C21DC__INCLUDED_)
#define AFX_FONT_H__C1B6695D_3811_4E8E_8B66_1CA0733C21DC__INCLUDED_

class CFont  
{
public:
    CFont( const char *szFontFile, int char_width, int char_height, int color_key = -1 );
    virtual ~CFont();

    void BlitText( int x, int y, char *szText, SDL_Surface *pScreen );
    void Destroy();

    int getWidth() { return m_charWidth; };
    int getHeihgt() { return m_charHeight; };

private:
    SDL_Surface *m_pImage;
    int m_imgWidth, m_imgHeight;
    int m_charWidth, m_charHeight;  // golemina na bukvite

};

#endif // !defined(AFX_FONT_H__C1B6695D_3811_4E8E_8B66_1CA0733C21DC__INCLUDED_)
