// vampire_ivo.cpp : Defines the entry point for the console application.
//

/*

  "Vampire Ivo" - shoot'em up game

  greetings from the Pi-Conuslt.bg team - 03.2005


  beta release: 21.03.2005
  final release: ??.03.2005

  code: Peter Petrov
  gfx & sfx: Valentin Bojilov

*/


#include "stdafx.h"
#include "GameProc.h"


int main(int argc, char* argv[])
{

	if ( !VampireIvo::InitGame( 800, 600, 16, true ) )
		return -1;

	VampireIvo::StartGameLoop();
	
	VampireIvo::FreeGame();
	
	return 0;
}

