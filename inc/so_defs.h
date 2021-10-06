#ifndef __SO_DEF_H
#define __SO_DEF_H

/*
 * modulo para facilitar a detecção do
 * sistema operacional em que o programa
 * está sendo compilado
 */

#if defined(_WIN32) || defined(_WIN64)
	// Windows 32 & 64
	#define WINDOWS_OS
#else
#ifdef __posix
	#define UNIX_OS
#endif

#endif
