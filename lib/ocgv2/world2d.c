/* world2d.c -- OpenCowGene Core library version 2
 * License: BSD-3-Clause
 * Author: Nikita (nst) Stavtsev
 */

#include "ocg2d.h"
#include <string.h>
#include <stdlib.h>

/* World2D related functions */
int World2DInit(World2D *world) {
	if (!world) 
		return -1;
	if (!world->UserInitCallback || !world->UserStepCallback || !world->SContext)
		return -1;
	
	world->cfnd = 0;

	if (world->UserInitCallback(world, world->SContext) != 0)
		return -1;
	return 0;
}

void World2DFree(World2D *world) {
	if (!world)
		return;

	for (size_t i = 0; i < world->cfnd; i++) {
		FloatNDAFree(&world->fnd[i]);
	}

	*world = (World2D){0};
}

int World2DStep(World2D *world) {
	if (!world || !world->UserStepCallback)
		return -1;

	if (world->UserStepCallback(world, world->SContext) != 0)
		return -1;
	return 0;
}

int World2DAddFND(World2D *world, size_t ndim, const size_t *shape, size_t *outid) {
	if (!world || world->cfnd >= WORLD_MAX_FNDS)
		return -1;

	size_t id = world->cfnd;

	if (FloatNDAInit(&world->fnd[id], ndim, shape) != 0)
		return -1;

	world->cfnd++;
	if (outid)
		*outid = id;
	return 0;
}

FloatNDA* World2DGetFND(World2D *world, size_t id) {
	if (!world || id >= world->cfnd) {
		return NULL;
	}

	return &world->fnd[id];
}
