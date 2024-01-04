#ifndef _MUDDLL_GLOBALS_H_
#define _MUDDLL_GLOBALS_H_

#ifdef MUD_EXPORTS
# define MUDCALL __fastcall
# define MUDEXPORT __declspec(dllexport)
#else
# define MUDCALL __fastcall
# define MUDEXPORT __declspec(dllimport)
#endif


#endif // _MUDDLL_GLOBALS_H_