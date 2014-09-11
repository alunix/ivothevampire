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

#if !defined(AFX_GAMEPROC_H__5F71E7E8_88EB_4CC9_B4CA_159FD2D597DD__INCLUDED_)
#define AFX_GAMEPROC_H__5F71E7E8_88EB_4CC9_B4CA_159FD2D597DD__INCLUDED_

namespace VampireIvo {

#ifdef _DEBUG
#define DBG(msg)                LogMessage(msg)
#else
#define DBG(msg)
#endif

#define LOG(msg)                LogMessage(msg)
#define _PI                     3.141592f
#define _PI_2                   1.570796f
#define MAGENTA                 0xff00ff
#define MAGENTA_565             0xF81F      
#define MAGENTA_555             0x7C1F

#define DEFAULT_RESPAWN_TIME_MIN    1850U
#define DEFAULT_RESPAWN_TIME_MAX    2250U
#define MAX_ENEMIES_BEFORE_DEATH    20
#define DEFAULT_VAMP_MIN_SPEED      80.0f
#define DEFAULT_VAMP_MAX_SPEED      120.0f
#define DEFAULT_TIME_TO_LIVE        60000U      // time to survive

#define ASSETS_ENV_VARIABLE "VAMPIREIVO_ASSETS" // where to look for gfx & sfx

struct __player
{
    int hits;
    int shots;
    int score;
};


struct __mouse
{
    enum
    {
        BUTTON_UNPRESSED = 0,
        BUTTON_DOWN,
        BUTTON_UP
    };
        
    int x, y;
    Uint8 left;
    Uint8 right;
};

struct __sound
{
#ifdef WITH_SDLMIXER    
    Mix_Chunk       *sound;
#endif  
    bool            buffered;               // bufferiran li shte e zvukyt 
    int             play_channel;           // ne-bufferiran zvuk se nujdae ot otdelen kanal
    bool            loaded;
    
    void Release();

};

// sounds
enum
{
    SND_AAH = 0,
    SND_SHOOT,
    SND_LAST
};

// gamestates
enum 
{
    GS_MAINMENU = 0,
    GS_GAMEPLAY,
    GS_WIN,
    GS_GAMEOVER
};


//--- game functions
bool InitGame( int screen_width, int screen_height, int bpp, 
    bool bFullscreen = false, bool bShowFPS = false );
void FreeGame();
void StartGameLoop();
void UpdateFrame();
void ResetGame();

//--- logging
bool OpenLog();
void LogMessage(const string &msg);

//--- graphics
void MakeBoolMask( SDL_Surface *surf, int *&mask, int back_color ); 
int Collide( SDL_Rect *r1, int *mask1, SDL_Rect *r2, int *mask2 ); // pixel perfect
int ClipRect( int *x , int *y, SDL_Rect *rSurf );
void SetRect( SDL_Rect *rect, int x, int y, int width, int height );

//--- physics
int Collide( SDL_Rect *r_result, SDL_Rect *r1, SDL_Rect *r2 );
int intGetRnd( int min_val, int max_val );
float fGetRnd( float min_val, float max_val );
Uint16 GetDistance( int x1, int y1, int x2, int y2 );
float  fGetDistance( float x1, float y1, float x2, float y2 );
float fRangeGetXY(float in, float inMin, float inMax, float min, float max);
int RangeGetXY(int in, int inMin, int inMax, int min, int max);

//--- sound 
int LoadSound(const string soundFilePath, bool buffered_sound );
void PlaySound( int snd_index, int position = -1 );

} // end namespace


#endif // !defined(AFX_GAMEPROC_H__5F71E7E8_88EB_4CC9_B4CA_159FD2D597DD__INCLUDED_)
