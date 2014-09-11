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

/*

  "Ivo the Vampire" - shoot'em up game

  greetings from the Pi-Conuslt.bg team - 03.2005


  beta release: 21.03.2005
  final release: ??.03.2005
  edited release: Sep.2014

  code: Petar Petrov
  gfx & sfx: Valentin Bojilov

*/

#include "vampire_ivo.h"
#include "GameProc.h"

int main(int argc, char* argv[])
{

#ifdef LINUX_BUILD
  setenv("SDL_VIDEO_CENTERED", "1", 1);
#else
  _putenv("SDL_VIDEO_CENTERED=1");
#endif

    bool bFullscreen = false;
    bool bShowFPS = false;

    if (argc > 1) {
        for (int i = 1; i < argc; i++) 
        {
            if (!strncmp(argv[i], "-fps", 4)) 
            {
                bShowFPS = true;
                continue;
            }
            else if (!strncmp(argv[i], "-force-fullscreen", 17)
              || !strncmp(argv[i], "-fullscreen", 11)) 
            {
                bFullscreen = true;
                continue;
            }

            perror("Unknown command line parameter passed!");
            exit(0);
        }
    }    
  
    if (!VampireIvo::InitGame(800, 600, 16, bFullscreen, bShowFPS)) {
        return -1;
    }

    VampireIvo::StartGameLoop();
    
    VampireIvo::FreeGame();
    
    return 0;
}
