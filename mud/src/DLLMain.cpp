// mud.cpp : Defines the entry point for the DLL application.
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <scripthost.h>
#include <f5/f5classes.h>
#include <stdio.h>

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
  if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
    printf("HELLO FROM NEW MUD!\n");
  }
  return TRUE;
}
