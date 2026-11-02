/* ocg2d.h -- OpenCowGene Core library version 2
 * License: BSD-3-Clause
 * Author: Nikita (nst) Stavtsev
 */

#ifndef OCG2D_H
#define OCG2D_H

#include "ocgdef.h"
#include <ocgmath.h>

#include <stdlib.h>
#include <unistd.h>

#define WORLD_MAX_FNDS 32

// World declaration
DEFINE_STRUCT_TYPE(World2D);
DEFINE_CALLBACK(World2DUserInit, int, World2D *world, void *context);
DEFINE_CALLBACK(World2DUserStep, int, World2D *world, void *context);
struct World2D {
	FloatNDA fnd[WORLD_MAX_FNDS];
	size_t cfnd;
	void *SContext;
	World2DUserInit UserInitCallback;
	World2DUserStep UserStepCallback;
};

int World2DInit(World2D *world);
void World2DFree(World2D *world);

int World2DStep(World2D *world);

int World2DAddFND(World2D *world, size_t ndim, const size_t *shape, size_t *outid);
FloatNDA* World2DGetFND(World2D *world, size_t id);

// Cow agent declaration
DEFINE_STRUCT_TYPE(PCow2D);
struct PCow2D {
	float r[2];
};



#endif
