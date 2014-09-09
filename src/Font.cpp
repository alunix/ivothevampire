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
#include "Font.h"
#include "GameProc.h"


CFont::CFont(const string strFontFile, int char_width, int char_height, int color_key)
{


    SDL_Surface *pTemp;
    char buf[255];

    m_charWidth = char_width;
    m_charHeight = char_height;

    if ( (pTemp = SDL_LoadBMP( strFontFile.c_str() ) ) == NULL )
    {
        sprintf( buf, "Failed to load font - %s", strFontFile.c_str() );
        VampireIvo::LOG( string(buf) );
        return;
    }

    // set color key
    if ( color_key != -1 )
    {
        SDL_SetColorKey( pTemp, SDL_SRCCOLORKEY, color_key );
    }

    // get dimensions
    SDL_Rect rect;
    SDL_GetClipRect( pTemp, &rect );
    m_imgWidth = rect.w;
    m_imgHeight = rect.h;

    m_pImage = SDL_DisplayFormat( pTemp );

    sprintf( buf, "Loaded font - %s", strFontFile.c_str() );
    VampireIvo::LOG( string(buf) );
}


CFont::~CFont()
{

}


void CFont::Destroy()
{
    SDL_FreeSurface( m_pImage );
}


void CFont::BlitText(int x, int y, char *szText, SDL_Surface *pScreen)
{

    char *p = szText;
    int n = strlen( p );
    bool bBlit = false;
    SDL_Rect rsrc, rdst;

    while( (n--) > 0 )
    {
        if ( (*p >= 'A' && *p <= 'Z') )
        {
            rsrc.x = (*p - 'A') * m_charWidth;
            rsrc.y = 0;
            rsrc.w = m_charWidth;
            rsrc.h = m_charHeight;
            bBlit = true;
        }
        else if ( *p >= '0' && *p <= '9')
        {
            rsrc.x = (*p - '0') * m_charWidth;
            rsrc.y = m_charHeight;
            rsrc.w = m_charWidth;
            rsrc.h = m_charHeight;
            bBlit = true;
        }
        else if ( *p == ':' )
        {
            rsrc.x = m_charWidth * 10;
            rsrc.y = m_charHeight;
            rsrc.w = m_charWidth;
            rsrc.h = m_charHeight;
            bBlit = true;
        }
        else if ( *p == '%' )
        {
            rsrc.x = m_charWidth * 11;
            rsrc.y = m_charHeight;
            rsrc.w = m_charWidth;
            rsrc.h = m_charHeight;
            bBlit = true;
        }
        else if ( *p == ' ' )
        {
            rsrc.x = m_charWidth * 13;
            rsrc.y = m_charHeight;
            rsrc.w = m_charWidth;
            rsrc.h = m_charHeight;
            bBlit = true;
        }
        else if ( *p == '/' )
        {
            rsrc.x = m_charWidth * 12;
            rsrc.y = m_charHeight;
            rsrc.w = m_charWidth;
            rsrc.h = m_charHeight;
            bBlit = true;
        }


        if ( bBlit )
        {
            rdst.x = x;
            rdst.y = y;
            rdst.w = m_charWidth;
            rdst.h = m_charHeight;

            SDL_BlitSurface( m_pImage, &rsrc, pScreen, &rdst );

            x += m_charWidth;       

            bBlit = false;
        }

        p++;
    }
}
