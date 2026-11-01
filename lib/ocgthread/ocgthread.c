/* OpenCowGene Threading (ocgthread.h)
 * License: BSD-3-Clause
 * Author: Nikita (nst) Stavtsev
 *
 * Threading library for OCG needs
 */

#include "ocgthread.h"
#include <stdlib.h>

#ifdef _WIN32
#else

int ThreadCreate(Thread *thread, ThreadFunction func, void *arg) {
	return pthread_create(thread, NULL, func, arg);
}

int ThreadJoin(Thread thread) {
	return pthread_join(thread, NULL);
}

#endif
