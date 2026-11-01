/* ocg2d.h -- OpenCowGene Core library version 2
 * License: BSD-3-Clause
 * Author: Nikita (nst) Stavtsev
 */

#ifndef OCG2D_H
#define OCG2D_H

#include "defines.h"

#include <ocgmath.h>

#include <stdlib.h>
#include <unistd.h>

#define WORLD_MAX_FNDS 32

// World declaration
DEFINE_STRUCT_TYPE(World2D);
struct World2D {
	FloatNDArray fnd[WORLD_MAX_FNDS];
	size_t cfnd;
};

void World2DInit(World2D *world);
void World2DFree(World2D *world);
int World2DAddFND(World2D *world, size_t ndim, const size_t *shape, size_t *outid);
FloatNDArray* World2DGetFND(World2D *world, size_t id);


#endif
