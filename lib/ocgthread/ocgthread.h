/* OpenCowGene Threading (ocgthread.h)
 * License: BSD-3-Clause
 * Author: Nikita (nst) Stavtsev
 *
 * Threading library for OCG needs
 */

#ifndef OCGTHREAD_H
#define OCGTHREAD_H

#ifdef _WIN32
	#include <windows.h>
	typedef HANDLE Thread;
#else
	#include <pthread.h>
	typedef pthread_t Thread;
#endif

typedef void *(*ThreadFunction)(void*);

int ThreadCreate(Thread *thread, ThreadFunction func, void *argument);
int ThreadJoin(Thread thread);


#endif
