// Sprite.h: interface for the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRITE_H__D8351040_15CC_44FD_AB01_2E4B42B02E42__INCLUDED_)
#define AFX_SPRITE_H__D8351040_15CC_44FD_AB01_2E4B42B02E42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


namespace VampireIvo {



class CSprite  
{
public:
	CSprite( char **lpszFilePaths, int num_frames, int color_key = -1, bool bMakeMask = false );
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
