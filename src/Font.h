// Font.h: interface for the Font class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FONT_H__C1B6695D_3811_4E8E_8B66_1CA0733C21DC__INCLUDED_)
#define AFX_FONT_H__C1B6695D_3811_4E8E_8B66_1CA0733C21DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFont  
{
public:
	CFont( char *szFontFile, int char_width, int char_height, int color_key = -1 );
	virtual ~CFont();

	void BlitText( int x, int y, char *szText, SDL_Surface *pScreen );
	void Destroy();

	int getWidth() { return m_charWidth; };
	int getHeihgt() { return m_charHeight; };

private:
	SDL_Surface *m_pImage;
	int m_imgWidth, m_imgHeight;
	int m_charWidth, m_charHeight;	// golemina na bukvite

};

#endif // !defined(AFX_FONT_H__C1B6695D_3811_4E8E_8B66_1CA0733C21DC__INCLUDED_)
