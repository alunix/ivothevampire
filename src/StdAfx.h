// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__7A1CBEFF_90C5_41A0_B5CB_E5298481A320__INCLUDED_)
#define AFX_STDAFX_H__7A1CBEFF_90C5_41A0_B5CB_E5298481A320__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers


#pragma comment( lib, "sdl.lib" )
#pragma comment( lib, "sdlmain.lib" )
#pragma comment( lib, "fmodvc.lib" )
//#pragma comment( lib, "sdl_image.lib" )

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

#include "SDL/Sdl.h"
#include "fmod/fmod.h"
#include "fmod/fmod_errors.h"


// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__7A1CBEFF_90C5_41A0_B5CB_E5298481A320__INCLUDED_)
