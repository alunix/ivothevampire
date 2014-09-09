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
#include "GameProc.h"
#include "Sprite.h"
#include "Animation.h"
#include "Vampire.h"
#include "Font.h"

namespace VampireIvo {



////////////////////////////////////////////////////////////////////////////
//// globals
SDL_Surface   *g_pScreen = NULL;
int           g_magenta = MAGENTA_565;

int           g_screenwidth = -1;
int           g_screenheight = -1;
__player      g_player;
Uint32        g_lastTimeSpawn = 0U;  
Uint32        g_TimeToRespawn = 0U;
Uint32        g_TimeToRespawnMin = DEFAULT_RESPAWN_TIME_MIN;
Uint32        g_TimeToRespawnMax = DEFAULT_RESPAWN_TIME_MAX;
int           g_GameState = GS_MAINMENU; //GS_GAMEPLAY;
float         g_vampire_min_speed = DEFAULT_VAMP_MIN_SPEED;
float         g_vampire_max_speed = DEFAULT_VAMP_MAX_SPEED;
Uint32        g_uiTimeLeft = 0U;
bool          g_soundinitialized = false;

bool          g_bShowFps = true;
int           g_nFps = 0;
__mouse       g_mouse;
float         g_mpf = 0.0f; // motion per frame
bool          g_bRunning = false;
bool          g_LogOpened = false;
ofstream      g_LogStream;

// game objects
CVampires     g_vampires;
CFont         *g_pFont = NULL;
CSprite       *g_vampire_fly = NULL;
CSprite       *g_vampire_die = NULL;
CSprite       *g_pbackground = NULL;
CSprite       *g_pSplash = NULL;    
CSprite       *g_pMouse = NULL;  
CSprite       *g_pTop = NULL, *g_pBottom = NULL;

// sound
__sound       sounds[SND_LAST];
bool          g_bMusic = false;
#ifdef WITH_SDLMIXER
Mix_Music     *music = NULL;
#endif



////////////////////////////////////////////////////////////////////////////


    
bool InitGame( int screen_width, int screen_height, int bpp, bool bFullscreen /*= false*/ )
{
    
    OpenLog();

    //----- init video

    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) 
    {
        fprintf (stderr, "Error intializing!");
        LOG( "Error intializing SDL!" );
        return false;
    }
     
    atexit( SDL_Quit );

    //----- defaults
    memset( &g_player, 0, sizeof(g_player ) );

    srand( (unsigned)SDL_GetTicks() );
    g_screenwidth = screen_width;
    g_screenheight = screen_height;

    //----- init SDL
    if ( bFullscreen )
    {
        g_pScreen = SDL_SetVideoMode( screen_width, screen_height, bpp, 
            SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_ANYFORMAT |  SDL_FULLSCREEN ); 
    }
    else
    {
        g_pScreen = SDL_SetVideoMode( screen_width, screen_height, bpp, 
            SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_ANYFORMAT );
    }

    // 16bit-mode-check
    if ( g_pScreen->format->BytesPerPixel == 2 )
    {
        if ( g_pScreen->format->Rmask == 0xF800 )
            g_magenta = MAGENTA_565;
        else
            g_magenta = MAGENTA_555;
    }
    else 
    {
        g_magenta = 0x00ff00ff;
    }

    if ( NULL == g_pScreen )
    {
        LOG( "Error intializing SDL video screen!" );
        fprintf (stderr, "Error intializing video screen!");
        return false;
    }

    // hide cursor
    SDL_ShowCursor( SDL_DISABLE );

#ifdef WITH_SDLMIXER

    LOG("Initializing SDL_mixer ..." );

    string strLog;

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
    {
        strLog = string("SDL Error: ...failed to open SDL_INIT_AUDIO:  ");
        strLog.append(SDL_GetError());
        LOG(strLog);
        // XXX Should we continue?
    }

    int flags_required = MIX_INIT_MOD;
    int flags_supported = Mix_Init(flags_required);

    if(flags_supported & flags_required != flags_required) {
        strLog.clear();
        strLog.append("SDL_mixer failed to initialize! Game will start without sound.");
        strLog.append("SDL_mixer error: ");
        strLog.append(SDL_GetError());
        LOG(strLog);
    }
    else
    {
        int mixrate = getenv("SW_SND_22KHZ")
                && !strncmp(getenv("SW_SND_22KHZ"), "1", 1) ? 22050 : 44100;

        /* initialize sdl mixer, open up the audio device */
        if (Mix_OpenAudio(mixrate, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
            strLog.clear();
            strLog.append("SDL_mixer failed to initialize!.");
            strLog.append("SDL_mixer error: ");
            strLog.append(SDL_GetError());
            LOG(strLog);
        }
        else
        {
            // Get specs information
            int audio_rate;
            int audio_channels;
            Uint16 audio_format;

            if (0 != Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels)) {
                int bits = audio_format & 0xFF;
                int sample_size = bits / 8 + audio_channels;
                int rate = audio_rate;

                stringstream ss;
                ss << "SDL_mixer: Opened audio at " << audio_rate << " Hz " << bits << " bit " 
                    << (audio_channels > 1 ? "stereo" : "mono");
                LOG(ss.str());
            }

            LOG("SDL_mixer initialized successfully.");

            g_soundinitialized = true;
        }
    }

#endif

    string strAssetsPath;
    char *env_assets_path = getenv("VAMPIREIVO_ASSETS");
    if (env_assets_path != NULL) 
    {
        strAssetsPath = string(env_assets_path);
    }    

    //----- init font
    g_pFont = new CFont(string(strAssetsPath) + "gfx/font.bmp", 16, 16, 0x00000000);

    //----- init gfx
    const char *szSplash[255] = { "gfx/splash.bmp" };
    g_pSplash = new CSprite(strAssetsPath, szSplash, 1);

    const char *szTop[255] = { "gfx/top.bmp" };
    g_pTop = new CSprite(strAssetsPath, szTop, 1);
    
    const char *szBottom[255] = { "gfx/bottom.bmp" };
    g_pBottom = new CSprite(strAssetsPath, szBottom, 1 );

    const char *szBackground[255] = { "gfx/backg.bmp" };
    g_pbackground = new CSprite(strAssetsPath, szBackground, 1);

    const char *szMouseCursor[255] = { "gfx/mernik.bmp" };
    g_pMouse = new CSprite(strAssetsPath, szMouseCursor, 1, MAGENTA, true);

    // load vampire sprites
    const char *szVampAnim[255] = { "gfx/ivo0.bmp", "gfx/ivo1.bmp", "gfx/ivo2.bmp"};
    g_vampire_fly = new CSprite(strAssetsPath, szVampAnim, 3, MAGENTA, true);

    const char *szVampDie[255] = { "gfx/ivodown.bmp" };
    g_vampire_die = new CSprite(strAssetsPath, szVampDie, 1, MAGENTA);

    // load sounds
    LoadSound(string(strAssetsPath) + "sfx/AHH_02.wav", true);
    LoadSound(string(strAssetsPath) + "sfx/Machine_Gun2.wav", true);

#ifdef WITH_SDLMIXER
    // if ( (music = FMUSIC_LoadSong( "sfx/39.mid" )) == NULL )
    // {
    //     LOG( "Failed to load sound - sfx/39.mid" );
    //     g_bMusic = false;
    // }
    // else {
    //     g_bMusic = true;
    // }
#endif

    return (g_bRunning = true);
}

void FreeGame()
{
    LOG( ("Releasing game...") );

    g_vampire_fly->Destroy();
    g_vampire_die->Destroy();
    g_pbackground->Destroy();
    g_pMouse->Destroy();
    g_pSplash->Destroy();
    g_pTop->Destroy();
    g_pBottom->Destroy();

    delete g_pTop;
    delete g_pBottom;
    delete g_vampire_fly;
    delete g_vampire_die;
    delete g_pbackground;
    delete g_pMouse;
    delete g_pSplash;
    
    g_pFont->Destroy();
    delete g_pFont;

    SDL_FreeSurface( g_pScreen );

#ifdef WITH_SDLMIXER
    // free sounds
    if ( g_soundinitialized )
    {
        LOG( ("Releasing sounds...") );
        for( int i = 0; i < SND_LAST; i++ )
        {
            sounds[i].Release();
        }

        // if ( g_bMusic ) {
        //     FMUSIC_FreeSong( music );
        // }

        Mix_Quit();
        LOG("SDL_mixer closed.");
    }
#endif

    LOG( ("Game closed.") );

    g_LogStream.flush();
    g_LogStream.close();
}

void ResetGame()
{
    // reset game stuff
    g_vampires.Destroy();
    memset( &g_player, 0, sizeof(g_player) );

    g_TimeToRespawnMin = DEFAULT_RESPAWN_TIME_MIN;
    g_TimeToRespawnMax = DEFAULT_RESPAWN_TIME_MAX;
    g_vampire_min_speed = DEFAULT_VAMP_MIN_SPEED;
    g_vampire_max_speed = DEFAULT_VAMP_MAX_SPEED;
}

void StartGameLoop()
{
    float ftimediff = 0.0f;
    float ftimer = 0.0f;
    float ffpstime = 0.0f;
    int frames = 0;
    Uint32 curTime = 0U;
    Uint32 uiGameEndTimer = 0U;

    Uint32 uiVampireTime = 0U;

    // play music
#ifdef WITH_SDLMIXER
    // if ( g_bMusic )
    // {
    //     FMUSIC_SetLooping( music, true );
    //     FMUSIC_PlaySong( music );
    // }
#endif

    while( g_bRunning )
    {

        ////////////////////////////////////////////////////////////////////
        // do hires timing
        ftimediff = SDL_GetTicks() - ftimer;
        g_mpf = ftimediff / 1000.0f;
        ftimer = (float)SDL_GetTicks();
        // calc fps
        if ( ftimer - ffpstime >= 1000 )
        {
            g_nFps = frames;
            frames = 0;
            ffpstime = (float)SDL_GetTicks();
        }
        frames++;


        ////////////////////////////////////////////////////////////////////
        // keys events
        SDL_Event event;

        while ( SDL_PollEvent( &event ) )
        {
    
            if ( event.type == SDL_QUIT )  
            {  
                g_bRunning = false;  
            }

            if ( event.type == SDL_KEYDOWN )
            {
                if ( event.key.keysym.sym == SDLK_ESCAPE ) 
                { 
                    if ( GS_MAINMENU == g_GameState )
                        g_bRunning = false;
                    else
                    {
                        ResetGame();
                        g_GameState = GS_MAINMENU;
                    }
                }
                else if ( event.key.keysym.sym == SDLK_s )
                {
#ifdef WITH_SDLMIXER                    
                    // if ( g_bMusic)
                    // {
                    //     if ( FMUSIC_IsPlaying(music) )
                    //         FMUSIC_StopSong( music );
                    //     else
                    //     {
                    //         FMUSIC_SetLooping( music, true );
                    //         FMUSIC_PlaySong( music );
                    //     }
                    // }
#endif                    
                } //end KEY_s
            }
        }


        ////////////////////////////////////////////////////////////////////
        // update mouse state
        //SDL_PumpEvents();

        Uint8 button_mask = SDL_GetMouseState( &g_mouse.x, &g_mouse.y );

        // check mouse buttons
        if ( button_mask & SDL_BUTTON(1) )      // left
        {
            if ( g_mouse.left == __mouse::BUTTON_UNPRESSED )
                g_mouse.left = __mouse::BUTTON_DOWN;
        }
        else if ( button_mask & SDL_BUTTON(3) ) // right (2 or 3 ?!)
        {
            if ( g_mouse.right == __mouse::BUTTON_UNPRESSED )
                g_mouse.right = __mouse::BUTTON_DOWN;       }
        else
        {
            if ( g_mouse.left == __mouse::BUTTON_DOWN )
                g_mouse.left = __mouse::BUTTON_UP;
            else if ( g_mouse.left == __mouse::BUTTON_UP )
                g_mouse.left = __mouse::BUTTON_UNPRESSED;

            if ( g_mouse.right == __mouse::BUTTON_DOWN )
                g_mouse.right = __mouse::BUTTON_UP;
            else if ( g_mouse.right == __mouse::BUTTON_UP )
                g_mouse.right = __mouse::BUTTON_UNPRESSED;  

        }

        
        switch( g_GameState )
        {

        case GS_MAINMENU:

            if ( g_mouse.left == __mouse::BUTTON_UP )
            {
                g_GameState = GS_GAMEPLAY;

                // setup time
                g_uiTimeLeft = SDL_GetTicks() + DEFAULT_TIME_TO_LIVE;
            }

            break;

        case GS_GAMEPLAY:

            if ( g_mouse.left == __mouse::BUTTON_UP  && 
                 g_mouse.y > 64 && g_mouse.y < (g_screenheight - 64) )
            {
                // play sound
                PlaySound( SND_SHOOT );

                int kills = g_vampires.CheckHit( g_mouse.x, g_mouse.y, 
                    g_pMouse->getWidth(), g_pMouse->getHeight() );

                if ( kills > 0 )
                {
                    g_vampire_min_speed += 0.5f;
                    g_vampire_max_speed += 0.5f;
                
                    g_TimeToRespawnMax -= (kills * 25);
                    g_TimeToRespawnMin -= (kills * 25);
                    if ( g_TimeToRespawnMin <= 250 )
                        g_TimeToRespawnMin = 250;
                    if ( g_TimeToRespawnMax <= 300 )
                        g_TimeToRespawnMax = 300;

                    g_player.hits += kills;
                }
                
                g_player.shots++; 
            }

            ////////////////////////////////////////////////////////////////////
            // update game logics
            curTime = SDL_GetTicks();

            // minimize vampire time to appear over time (with 25ms)
            if ( uiVampireTime < curTime )
            {
                g_TimeToRespawnMax -= 30;
                g_TimeToRespawnMin -= 30;
                uiVampireTime = curTime + 1000U;
            }

            // spawn a vampire
            if ( g_TimeToRespawn < curTime )
            {
                g_vampires.CreateVampire( g_vampire_fly, g_vampire_die );
                g_TimeToRespawn = curTime + intGetRnd( g_TimeToRespawnMin, g_TimeToRespawnMax ); 
            }

            // check for 'Game Over'
            if ( g_vampires.getSize() >= MAX_ENEMIES_BEFORE_DEATH )
            {
                g_GameState = GS_GAMEOVER;
                uiGameEndTimer = SDL_GetTicks() + 10000;
            }

            // check for game end
            if ( g_uiTimeLeft < curTime )
            {
                g_GameState = GS_WIN;
                uiGameEndTimer = SDL_GetTicks() + 10000;
            }

            break;

        case GS_WIN:
            if ( uiGameEndTimer < SDL_GetTicks() )
            {
                g_GameState = GS_MAINMENU;
                ResetGame();
            }

            break;

        case GS_GAMEOVER:
            if ( uiGameEndTimer < SDL_GetTicks() )
            {
                g_GameState = GS_MAINMENU;
                ResetGame();

#ifdef WITH_SDLMIXER
                // if ( FMUSIC_IsPlaying(music) )
                // {
                //     FMUSIC_StopSong( music );
                //     FMUSIC_SetLooping( music, true );
                //     FMUSIC_PlaySong( music );
                // }
#endif                
            }
            
            break;
        }


        ////////////////////////////////////////////////////////////////////
        // update graphics frame
        UpdateFrame();
    }

}

void UpdateFrame()
{
    char buf[255];

    switch( g_GameState )
    {

    case GS_MAINMENU:
        g_pSplash->Update( 0, 0, 0, g_pScreen );

        // update mouse cursor
        g_pMouse->Update( g_mouse.x, g_mouse.y, 0, g_pScreen );

        break;

    case GS_GAMEPLAY:
    case GS_GAMEOVER:
    case GS_WIN:
        // blit background
        g_pbackground->Update( 0, 0, 0, g_pScreen );

        // blit vampires
        g_vampires.Update( g_pScreen, &g_mpf );

        // update borders
        g_pTop->Update( 0, 0, 0, g_pScreen );
        g_pBottom->Update( 0, g_screenheight - 64, 0, g_pScreen );

        // update mouse cursor
        g_pMouse->Update( g_mouse.x, g_mouse.y, 0, g_pScreen );


        // update stats
        sprintf( buf, "VAMPIRES: %d/%d", g_vampires.getSize(), MAX_ENEMIES_BEFORE_DEATH );
        g_pFont->BlitText( 320, 3 + g_pFont->getHeihgt() + g_pFont->getHeihgt(), 
            buf, g_pScreen );
        
        if ( GS_GAMEOVER == g_GameState )
        {
            g_pFont->BlitText( 240, 300, (char *)"GAME OVER YOU SUCKER", g_pScreen );

            // print stats
            sprintf( buf, "KILLS: %d", g_player.hits );
            g_pFont->BlitText( 300, 300 + g_pFont->getHeihgt() * 2, buf, g_pScreen );
            sprintf( buf, "SHOTS: %d", g_player.shots );
            g_pFont->BlitText( 300, 300 + g_pFont->getHeihgt() * 3, buf, g_pScreen );
            float perc = 100.0f / (float)( (float)g_player.shots / (float)g_player.hits );
            sprintf( buf, "SUCCESS: %d%c", (int)perc, '%' );
            g_pFont->BlitText( 300, 300 + g_pFont->getHeihgt() * 4, buf, g_pScreen );
        }
        else if ( GS_WIN == g_GameState )
        {
            g_pFont->BlitText( 304, 300, (char *)"YOU WIN MASTER", g_pScreen );
            // print stats
            sprintf( buf, "KILLS: %d", g_player.hits );
            g_pFont->BlitText( 300, 300 + g_pFont->getHeihgt() * 2, buf, g_pScreen );
            sprintf( buf, "SHOTS: %d", g_player.shots );
            g_pFont->BlitText( 300, 300 + g_pFont->getHeihgt() * 3, buf, g_pScreen );
            float perc = 100.0f / (float)( (float)g_player.shots / (float)g_player.hits );
            sprintf( buf, "SUCCESS: %d%c", (int)perc, '%' );
            g_pFont->BlitText( 300, 300 + g_pFont->getHeihgt() * 4, buf, g_pScreen );
        }
        else
        {
            int time_left = ( g_uiTimeLeft - SDL_GetTicks() ) / 1000U; 
            int min = time_left / 60;
            int sec = time_left % 60;
            sprintf( buf, "TIME LEFT: %.2d:%.2d",  min, sec  );
            g_pFont->BlitText( 288, 3 + g_pFont->getHeihgt(), buf, g_pScreen );
        }

        break;

    }


    if ( g_bShowFps )
    {
        sprintf( buf, "%d FPS", g_nFps );
        g_pFont->BlitText( 5, g_screenheight - 15, buf, g_pScreen );
    }

    SDL_Flip( g_pScreen );
}

bool OpenLog()
{
    if ( !g_LogOpened )
    {
        g_LogStream.open( "vampire_ivo.log", ios::out );
        if ( g_LogStream.fail() )
            return g_LogOpened = false;

        g_LogStream << "Vampire Ivo " << VER_MAJ << "." << VER_MIN << " --- Log File ";
        g_LogOpened = true;
    }

    return g_LogOpened;
}

void LogMessage( const string &msg )
{
    if ( g_LogOpened )
    {
        g_LogStream << endl << msg;
        g_LogStream.flush();
    }
}

int Collide( SDL_Rect *r_result, SDL_Rect *r1, SDL_Rect *r2 )
{

    if ( 
         ((r1->x > r2->x && r1->x < r2->w) || (r1->w > r2->x && r1->w < r2->w)) &&
         ((r1->y > r2->y && r1->y < r2->h) || (r1->h > r2->y && r1->h < r2->h)) || 
         ((r2->x > r1->x && r2->x < r1->w) || (r2->w > r1->x && r2->w < r1->w)) &&
         ((r2->y > r1->y && r2->y < r1->h) || (r2->h > r1->y && r2->h < r1->h))
         )
    {
        
            if ( r_result != NULL )
            {
                if ( r1->x < r2->x )
                    r_result->x = r2->x;
                else
                    r_result->x = r1->x;
            
                if ( r1->y < r2->y )
                    r_result->y = r2->y;
                else
                    r_result->y = r1->y;

                if ( r1->w > r2->w )
                    r_result->w = r2->w;
                else
                    r_result->w = r1->w;

                if ( r1->h > r2->h )
                    r_result->h = r2->h;
                else
                    r_result->h = r1->h;
            }

            return 1;
    }

    return 0;
}

int ClipRect( int *x , int *y, SDL_Rect *rSurf )
{

    SDL_Rect  rWld, rResult, rDest;
    int    tx = *x, ty = *y;

    //tx -= rWorld.x;
    //ty -= rWorld.y;
 

    SetRect( &rWld, 0, 0, g_screenwidth, g_screenheight );
    //SetRect( &rDest, tx, ty, tx + rSurf->w, ty + rSurf->x );
    SetRect( &rDest, tx, ty, tx + rSurf->w, ty + rSurf->h );
 
    if (  Collide( &rResult, &rWld, &rDest ) )
    {
        rSurf->y = rResult.y - rDest.y;
        rSurf->x = rResult.x - rDest.x;
        rSurf->w = rResult.w - rDest.x;
        rSurf->h = rResult.h - rDest.y;
  
        *x = rResult.x;
        *y = rResult.y;

        return true;
    }
    
    return false;
}

void SetRect( SDL_Rect *rect, int x, int y, int width, int height )
{
    rect->x = x;
    rect->y = y;
    rect->w = width;
    rect->h = height;
}

void MakeBoolMask( SDL_Surface *surf, int *&mask, int back_color )
{

    register int  i,j,w,h,pos;
    Uint16        *pixel;

    
    if ( SDL_MUSTLOCK( surf ) )
    {
        if ( SDL_LockSurface( surf ) < 0 ) 
        {
            LOG( "MakeBoolMask() - Could not lock surface !" );
            return;
        }
    }

    pixel = (Uint16 *)((Uint8 *)surf->pixels );
    w = surf->w;
    h = surf->h;

    mask = (int *) new int[w*h];
    if ( mask == NULL )
    {
        LOG( "MakeBoolMask() - Could not allocate memory for image mask !");
        return;
    }
    
    for ( j = 0; j < h; j++ )
    {
        for ( i = 0; i < w; i++, pixel++ )
        {
            pos = w * j + i; 
            mask[pos] = ( *pixel != back_color ) ? 1 : 0;
        }

        // premseti pad-a
        pixel = (Uint16 *)((Uint8 *)surf->pixels + j * surf->pitch );
    }

    if ( SDL_MUSTLOCK( surf ) )
    {
        SDL_UnlockSurface( surf );
    }
}

int Collide( SDL_Rect *r1, int *mask1, SDL_Rect *r2, int *mask2 )
{

    SDL_Rect  rt1, rt2, rret;
    SDL_Rect  rSurf1, rSurf2;
    int       col_width, col_height;
    int       x_off1, y_off1;
    int       x_off2, y_off2;
    bool      bcollision = false;
    int       *pm1, *pm2;
    int       w1, w2; //, h1, h2;

    SetRect( &rt1, r1->x, r1->y, r1->w, r1->h );
    SetRect( &rt2, r2->x, r2->y, r2->w, r2->h );

    w1 = r1->w - r1->x;
    w2 = r2->w - r2->x;
    //h1 = r1->h - r1->y;
    //h2 = r2->h - r2->y;

    if ( Collide( &rret, &rt1, &rt2 ) )
    {
        // get overlappings
        rSurf1.x = rret.x - rt1.x;
        rSurf1.y = rret.y - rt1.y;
        rSurf1.w = rret.w - rt1.x;
        rSurf1.h = rret.h - rt1.y;
        x_off1 = ( rSurf1.y * w1 ) + rSurf1.x;
        
        rSurf2.x = rret.x - rt2.x;
        rSurf2.y = rret.y - rt2.y;
        rSurf2.w = rret.w - rt2.x;
        rSurf2.h = rret.h - rt2.y;
        x_off2 = ( rSurf2.y * w2 ) + rSurf2.x;

        // dimensii na intersekciqta
        col_width = (rret.w - rret.x);
        col_height = (rret.h - rret.y);

        for ( int j = 0; j < col_height; j++ )
        {
            // y - posiciq
            y_off1 = j * w1 + x_off1;
            y_off2 = j * w2 + x_off2;
            pm1 = &mask1[ y_off1 ];
            pm2 = &mask2[ y_off2 ];

            for ( int i = 0; i < col_width; i++, pm1++, pm2++ )
            {
                if ( (*pm1) && (*pm2) )
                    bcollision = true;
            }

            // zatvori cikyla
            if ( bcollision ) break;
        }
    
        if ( bcollision ) return 1;

    }

    // no collision
    return 0;
}

int intGetRnd( int min_val, int max_val )
{
    int range = max_val - min_val;
    int num = rand() % range;
    return ( num + min_val );
}

float fGetRnd( float min_val, float max_val )
{
    return ( ( max_val - min_val ) * (float)rand()/(float)RAND_MAX ) + min_val;
}

Uint16  GetDistance( int x1, int y1, int x2, int y2 )
{
    //return (Uint16)sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
    return 0;
}

float  fGetDistance( float x1, float y1, float x2, float y2 )
{
    return (float)sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
}

float fRangeGetXY(float in, float inMin, float inMax, float min, float max)
{
    float inRange = (inMax - inMin);
    float newRange = (max - min);
    float result = (((in - inMin) * newRange) / inRange) + min;

    return result;
}

int RangeGetXY(int in, int inMin, int inMax, int min, int max)
{
    return (int)fRangeGetXY((float)in, (float)inMin, (float)inMax, (float)min, (float)max);
}

//------------------------ SOUND FUNCTIONS


int LoadSound(const string soundFilePath, bool buffered_sound)
{
#ifdef WITH_SDLMIXER    

    if ( !g_soundinitialized ) 
        return -1;

    Uint32         i = 0;
    static int     cur_channel = 0;
    __sound        *ptr_snd = sounds;
    bool           bsnd_loaded = false;


    while ( i < SND_LAST )
    {
        if ( !ptr_snd->loaded )
        {
            LOG("Loading sound - " + soundFilePath);

            ptr_snd->sound = Mix_LoadWAV(soundFilePath.c_str());
            if (!ptr_snd->sound) {
                stringstream ss;
                ss << "...failed to load sound file : " << soundFilePath;
                ss << "SDL_mixer error: " << Mix_GetError();
                LOG(ss.str());
                return -1;
            }
            
            if ( !buffered_sound )
            {
                ptr_snd->buffered = false;
                ptr_snd->play_channel = cur_channel;
                cur_channel++;
            }
            else 
            {
                ptr_snd->buffered = true;
            }
            
            ptr_snd->loaded = true;
            
            bsnd_loaded = true;
            break;
        }

        i++;
        ptr_snd++;
    }

    if ( !bsnd_loaded )
    {
        LOG("Sound load error: No slots available!");
        return -1;
    }    
#endif
}

void PlaySound( int snd_index, int position )
{
#ifdef WITH_SDLMIXER
    Uint8 left_pos = 127;
    Uint8 right_pos = 127;
    if ( position != -1 )
    {
        right_pos = (Uint8)fRangeGetXY(position, 0, 800, 0.0f, 255.0);
        left_pos = 255 - right_pos;
    }

//  if ( sounds[snd_index].buffered ) {
        int channel = Mix_PlayChannel(-1, sounds[snd_index].sound, 0);
        if (position != -1) {
            Mix_SetPanning(channel, left_pos, right_pos);
        }
//  }
//  else
//  {
//      Mix_HaltChannel( sounds[snd_index].play_channel );
//      Mix_PlayChannel( sounds[snd_index].play_channel, sounds[snd_index].sound, 0 );
//  }
#endif
}

void __sound::Release()
{
#ifdef WITH_SDLMIXER
    Mix_FreeChunk(sound);
#endif    
}

} //end namespace

