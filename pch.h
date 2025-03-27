

#ifndef PCH_H
#define PCH_H


#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define MyAssert(x) if(!(x)) { __debugbreak(); }
#endif // _DEBUG

#include <windows.h>
#include <stdio.h>

#define MyAssert(x) 

#endif // PCH_H

