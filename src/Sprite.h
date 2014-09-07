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

#if !defined(AFX_SPRITE_H__D8351040_15CC_44FD_AB01_2E4B42B02E42__INCLUDED_)
#define AFX_SPRITE_H__D8351040_15CC_44FD_AB01_2E4B42B02E42__INCLUDED_

namespace VampireIvo {

class CSprite  
{
public:
    CSprite( const char **lpszFilePaths, int num_frames, int color_key = -1, bool bMakeMask = false );
    virtual ~CSprite();

    void Update( int x, int y, int curframe, SDL_Surface *pScreen );
    void Destroy();

    int getWidth() { return m_pWidths[m_curframe]; };
    int getHeight() { return m_pHeights[m_curframe]; };
    int getFrames() { return m_numframes; };
    int* getMask();
    SDL_Surface* getSDL_Surface() { return m_pImages[m_curframe]; };

private:

    SDL_Surface **m_pImages;
    int **m_pMask;
    int m_numframes;
    int m_curframe;
    bool m_bMask;


    int *m_pWidths, 
        *m_pHeights;

};


} //end namespace

#endif // !defined(AFX_SPRITE_H__D8351040_15CC_44FD_AB01_2E4B42B02E42__INCLUDED_)
