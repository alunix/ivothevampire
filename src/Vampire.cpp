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
#include "Vampire.h"
#include "Animation.h"
#include "Sprite.h"
#include "GameProc.h"


namespace VampireIvo {


extern int            g_screenwidth;
extern int            g_screenheight;
extern float          g_vampire_min_speed;
extern float          g_vampire_max_speed;
extern CSprite        *g_pMouse;


CVampires::CVampires()
{

}


CVampires::~CVampires()
{
    Destroy();
}


void CVampires::Destroy()
{

    // clear mem on exit
    if ( m_vampires.size() > 0 )
    {
        //LOG( "Killing alive vampires...");

        for( vector<CVampire*>::iterator it = m_vampires.begin(); it != m_vampires.end(); it++ )
        {
            CVampire *pv = (*it);

            delete pv->m_panimFly;
            delete pv->m_panimDie;

            delete pv;
            pv = NULL;
        }

        m_vampires.clear();

        //LOG( "Killing alive vampires...OK!");
    }

}


bool CVampires::CreateVampire( CSprite *pSpriteFly, CSprite *pSpriteDie )
{

    try
    {

        CVampire *pVampire = new CVampire();
        
        // setup vampire
        pVampire->m_panimFly = new CAnimation( pSpriteFly, 12 );
        pVampire->m_panimDie = new CAnimation( pSpriteDie, 0 );
        pVampire->m_move_method = VampireIvo::intGetRnd( 0, 4 );
        pVampire->m_life = 1;
        pVampire->m_bKilled = false;
        
        pVampire->m_angle = VampireIvo::fGetRnd( 0.0f, _PI );
        pVampire->m_speed_angle = VampireIvo::fGetRnd( 0.05f, 0.9f );
        
        //pVampire->m_move_method = MV_SINUS_HORIZ;
        switch( pVampire->m_move_method )
        {
        case MV_HORIZONTAL:
        case MV_SINUS_HORIZ:
            if ( intGetRnd( 0, 50 ) > 25 )
            {
                pVampire->m_x = fGetRnd( -250.0f, -100.0f );
                pVampire->m_y = fGetRnd( -50.0f, (float)(g_screenheight - 350) );
                pVampire->m_speed = fGetRnd( g_vampire_min_speed, g_vampire_max_speed );
            }
            else
            {
                pVampire->m_x = fGetRnd( (float)(g_screenwidth + 50), (float)(g_screenwidth + 150) );
                pVampire->m_y = fGetRnd( -50.0f, (float)(g_screenheight - 350) );
                pVampire->m_speed = -fGetRnd( g_vampire_min_speed, g_vampire_max_speed );
            }

            break;

        case MV_VERTICAL:
        case MV_SINUS_VERT:
            //if ( intGetRnd( 0, 50 ) > 25 )
            //{
                pVampire->m_x = fGetRnd( 5.0f, (float)(g_screenwidth - 200) );
                pVampire->m_y = fGetRnd( -550.0f, -150 );
                pVampire->m_speed = fGetRnd( g_vampire_min_speed, g_vampire_max_speed );
            //}
            /*else
            {
                pVampire->m_x = fGetRnd( 5.0f, (float)(g_screenwidth - 200) );
                pVampire->m_y = fGetRnd( (float)(g_screenheight) + 50, (float)g_screenheight + 550 );
                pVampire->m_speed = -fGetRnd( 50.0f, 100.0f );
            }*/
            break;

        default:
            VampireIvo::LOG( "CVampires::CreateVampire() : Error in  pVampire->m_move_method " );

        }
        

        m_vampires.push_back( pVampire );

    }
    catch( std::exception &e )
    {
        string strError("CVampires::CreateVampire() Exception - ");
        strError.append( e.what() );
        VampireIvo::LOG( strError );

        return false;
    }
    catch(...)
    {
        VampireIvo::LOG( string("CVampires::CreateVampire() Exception - Unknown") );

        return false;
    }

    return true;
}


void CVampires::Update( SDL_Surface *pScreen, float *mpf )
{

    CVampire *pv = NULL;

    vector<CVampire*>::iterator it_begin = m_vampires.begin();
    vector<CVampire*>::iterator it_end = m_vampires.end();
    vector<CVampire*>::iterator it;

    int nVampires = m_vampires.size();

    for( int i = 0; i < nVampires; i++ )
    {
        pv = m_vampires[i]; //(*it);
        
        if ( pv->m_life > 0 )
        {
            // trygvai nadolu
            if ( pv->m_bKilled )
            {
                pv->m_y += (pv->m_speed * (*mpf));
                pv->m_panimDie->Update( pv->m_x, pv->m_y, pScreen, mpf );

                if ( pv->m_y > g_screenheight )
                {
                    // remove from list
                    it = find( it_begin, it_end, pv );
                    if ( it != it_end )
                    {
                        m_vampires.erase( it );
                        delete pv->m_panimFly;
                        delete pv->m_panimDie;
                        delete pv;

                        //continue;
                    }
                }
            }
            else
            {
                // normal update
                switch( pv->m_move_method )
                {
                case MV_HORIZONTAL:
                    pv->m_x += (pv->m_speed * (*mpf));

                    // check bounds
                    if ( pv->m_speed > 0 )
                    {
                        if ( pv->m_x - 250 > g_screenwidth )
                            pv->m_life = 0;
                    }
                    else if  ( pv->m_speed < 0 )
                    {
                        if ( pv->m_x + 250 < 0 )
                            pv->m_life = 0;
                    }
                    
                    break;

                case MV_VERTICAL:
                    //pv->m_x += (pv->m_speed * (*mpf));
                    pv->m_y += (pv->m_speed * (*mpf));

                    // check bounds
                    if ( pv->m_speed > 0 )
                    {
                        if ( pv->m_y - 250 > g_screenheight )
                            pv->m_life = 0;
                    }
                    else if  ( pv->m_speed < 0 )
                    {
                        if ( pv->m_y + 250 < 0 )
                            pv->m_life = 0;
                    }

                    break;

                case MV_SINUS_HORIZ:
                    pv->m_x += (pv->m_speed * (*mpf));
                    pv->m_y += ((pv->m_speed * (*mpf)) * sin(pv->m_angle) );
                    pv->m_angle += pv->m_speed_angle;

                    if ( pv->m_angle > 2 * _PI )
                    {
                        pv->m_angle  -= (2 * _PI);
                    }

                    // check bounds
                    if ( pv->m_speed > 0 )
                    {
                        if ( pv->m_x - 250 > g_screenwidth )
                            pv->m_life = 0;
                    }
                    else if  ( pv->m_speed < 0 )
                    {
                        if ( pv->m_x + 250 < 0 )
                            pv->m_life = 0;
                    }

                    break;

                case MV_SINUS_VERT:
                    pv->m_x += ((pv->m_speed * (*mpf)) * sin(pv->m_angle) );
                    pv->m_y += (pv->m_speed * (*mpf));
                    pv->m_angle += pv->m_speed_angle;
                    if ( pv->m_angle > 2 * _PI )
                    {
                        pv->m_angle  -= (2 * _PI);
                    }
                    // check bounds
                    if ( pv->m_speed > 0 )
                    {
                        if ( pv->m_y - 250 > g_screenheight )
                            pv->m_life = 0;
                    }
                    else if  ( pv->m_speed < 0 )
                    {
                        if ( pv->m_y + 250 < 0 )
                            pv->m_life = 0;
                    }

                    break;

                }

                // blit
                pv->m_panimFly->Update( pv->m_x, pv->m_y, pScreen, mpf );

            }

        }
        else
        {
            // remove from list
            it = find( it_begin, it_end, pv );
            if ( it != it_end )
            {
                m_vampires.erase( it );
                delete pv->m_panimFly;
                delete pv->m_panimDie;
                delete pv;
            }
        }

        //it++;
    }

    
}


int CVampires::CheckHit( int mx, int my, int mw, int mh )
{

    //if ( m_vampires.size() < 1 )
    //  return false;
    int kills = 0;

    CVampire *pv = NULL;
    SDL_Rect r_cursor = { mx, my, (mx + mw-1), (my + mh-1) };

    vector<CVampire*>::iterator it_begin = m_vampires.begin();
    vector<CVampire*>::iterator it_end = m_vampires.end();
    //vector<CVampire*>::iterator it;
    int nSize = m_vampires.size();

    for( int i = 0; i < nSize; i++ )
    {
        pv = m_vampires[i];

        if ( !pv->m_bKilled )
        {

            SDL_Rect r_vampire;
            r_vampire.x = (int)pv->m_x;
            r_vampire.y = (int)pv->m_y;
            
            // ebati buga - SDL_Rect.w i SDL_Rect.h sa unsigned
            // i proverkata za otricatelni koordinati se ebe qko...ave ebati
            int w = (int)pv->m_x + pv->m_panimFly->getWidth();
            int h = (int)pv->m_y + pv->m_panimFly->getHeight();
            if ( w < 0 || h < 0 )
                continue;

            r_vampire.w = w;
            r_vampire.h = h;

#ifdef _DEBUG
    char buf[255];
    sprintf( buf, "VAMPIRE: x:%d y:%d w:%d h:%d",  r_vampire.x, r_vampire.y, pv->m_panimFly->getWidth(), pv->m_panimFly->getHeight() );
    LOG( buf );
    sprintf( buf, "VAMPIRE RECT: x:%d y:%d w:%d h:%d",  r_vampire.x, r_vampire.y, r_vampire.w, r_vampire.h );
    LOG( buf );
#endif
            
            //if ( VampireIvo::Collide( NULL, &r_vampire, &r_cursor ) )
            if ( VampireIvo::Collide( &r_vampire, pv->m_panimFly->getMask(), 
                                      &r_cursor, g_pMouse->getMask() ) )
            {
                pv->m_bKilled = true;
                kills++;

                if ( pv->m_speed < 0 )
                    pv->m_speed *= -1;

                pv->m_speed *= 3;

                VampireIvo::PlaySound( VampireIvo::SND_AAH );

                return kills; // remove to kill more with 1 shot
    /*
                if ( (--pv->m_life) <= 0 )
                {*/

    /*              try {

                    it = find( it_begin, it_end, pv );
                    if ( it != NULL )
                    {
                        m_vampires.erase( it );
                        delete pv;
                        kills++;

                        //PLAY SOUND

    #ifdef _DEBUG
                    LOG( "Vampire killed." );
    #endif
                    }

                    }
                    catch( std::exception &e )
                    {
                        string err( "CVampires::CheckHit() : Error while killing vampire - ");
                        err.append( e.what() );
                        LOG( err );
                        return kills;
                    }
                    
            
                }*/
            }

        }
    }

    return kills;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CVampire::CVampire()
{

}

CVampire::~CVampire()
{

}



} // end namespace