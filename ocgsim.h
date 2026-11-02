#ifndef OCGSIM_H
#define OCGSIM_H

#include "ocg2d.h"
#include <math.h>

#define SEASONS_AMOUNT 4
#define SEASONS_LENGTH 31
typedef struct OCGSim OCGSim;

/// TODO: USER DEFINED FUNCTIONS FORWARD DECLARATION ///////
int OCGSimWorld2DUserInit(World2D *world, void *context);///
int OCGSimWorld2DUserStep(World2D *world, void *context);///
////////////////////////////////////////////////////////////

// NOTE: SIMULATION STRUCTURE
struct OCGSim {
	size_t WORLD_SIZE;
	size_t WORLD_RESOULUTION;

	size_t RESOURCE_MAP_ID;
	size_t CLIMAT_MAP_ID;
	
	size_t CURRENT_SEASON;
	size_t TICKS_ELAPSED;
	size_t SEASON_MAP_IDs[SEASONS_AMOUNT];

	World2D *world;
};

// NOTE: ALLOCATION OF SIMULATION
int OCGSimInit(OCGSim *sim) {
	if (!sim)
		return -1;

	sim->TICKS_ELAPSED = 0;

	// WARNING: IF YOU DECLARE ANY OCG STRUCTURE. 
	// BE SURE THAT YOU'VE INITIALIZED EVERY NEEDED 
	// STRUCT FIELD
	sim->world = (World2D*)malloc(sizeof(World2D));
	if (sim->world == NULL)
		return -1;

	sim->world->UserInitCallback = OCGSimWorld2DUserInit;
	sim->world->UserStepCallback = OCGSimWorld2DUserStep;
	sim->world->SContext = sim;

	if (World2DInit(sim->world) != 0) {
		World2DFree(sim->world);
		free(sim->world);
		sim->world = NULL;
		return -1;
	}

	return 0;
}
// NOTE: DEALLOCATION OF SIMULATION
void OCGSimFree(OCGSim *sim) {
	if (!sim)
		return;
	if (sim->world)
		World2DFree(sim->world);
	free(sim->world);
}

// NOTE: MAIN SIMULATION TICK STEP FUNCTION
int OCGSimTick(OCGSim *sim) {
	if (!sim)
		return -1;

	if (World2DStep(sim->world) != 0)
		return -1;

	sim->TICKS_ELAPSED++;
	if (sim->TICKS_ELAPSED % SEASONS_LENGTH == 0)
		sim->CURRENT_SEASON = (sim->CURRENT_SEASON + 1) % SEASONS_AMOUNT;
	
	return 0;
}

// TODO: USER DEFINED WORLD2D INIT FUNCTION
int OCGSimWorld2DUserInit(World2D *world, void *context) {
	OCGSim *sim = context;
	if (!world || !sim) {
		return -1;
	}
	
	size_t WORLD_DIM = sim->WORLD_SIZE * sim->WORLD_RESOULUTION;

	/* Resource map init */
	if (World2DAddFND(
		world, 3, 
		(const size_t[]){WORLD_DIM, WORLD_DIM, 1}, 
		&sim->RESOURCE_MAP_ID) != 0
	)
		return -1;
	
	FloatNDA *resource = World2DGetFND(world, sim->RESOURCE_MAP_ID);
	if (resource == NULL)
		return -1;
	FloatNDAFill(resource, 1.0f);


	/* Climat map init */
	if (World2DAddFND(
		world, 3, 
		(const size_t[]){WORLD_DIM, WORLD_DIM, 1}, 
		&sim->CLIMAT_MAP_ID) != 0
	)
		return -1;
	FloatNDA *climat = World2DGetFND(world, sim->CLIMAT_MAP_ID);
	if (climat == NULL)
		return -1;

	/// Inverted double pole gradient
	for (size_t i = 0; i < climat->size; i++) {
		size_t y = i / climat->shape[1];
		float t = (float)y / (climat->shape[0] - 1);
		climat->data[i] = 1.0f - fabsf(2.0f * t - 1.0f);
	}


	/* Init of season affection map */
	for (size_t i = 0; i < SEASONS_AMOUNT; i++) {
		if (World2DAddFND(
			world, 
			3, 
			(const size_t[]){WORLD_DIM, WORLD_DIM, 1}, 
			&sim->SEASON_MAP_IDs[i]) != 0
		)
			return -1;
		FloatNDA *currMap = World2DGetFND(world, sim->SEASON_MAP_IDs[i]);
		if (currMap == NULL)
			return -1;
		switch (i) {
		case 0:
			FloatNDAFill(currMap, 0.0f);
			break;
		case 1:
			FloatNDAFill(currMap, 0.5f);
			break;
		case 2:
			FloatNDAFill(currMap, 1.0f);
			break;
		case 3:
			FloatNDAFill(currMap, 0.2f);
			break;
		}
	}

	return 0;
}

// TODO: USER DEFINED WORLD2D STEP FUNCTION
int OCGSimWorld2DUserStep(World2D *world, void *context) {
	OCGSim *sim = context;
	if (!world || !sim)
		return -1;
	
	FloatNDA *resMap = World2DGetFND(world, sim->RESOURCE_MAP_ID);
	FloatNDA *climatMap = World2DGetFND(world, sim->CLIMAT_MAP_ID);
	if (resMap == NULL || climatMap == NULL)
		return -1;

	FloatNDA *seasonMaps[SEASONS_AMOUNT];
	for (size_t i = 0; i < SEASONS_AMOUNT; i++) {
		seasonMaps[i] = World2DGetFND(world, sim->SEASON_MAP_IDs[i]);
		if (seasonMaps[i] == NULL)
			return -1;
	}

	// Resource grow
	if (sim->CURRENT_SEASON >= SEASONS_AMOUNT)
		return -1;

	FloatNDA growStep;
	if (FloatNDAInit(&growStep, climatMap->ndim, climatMap->shape) != 0) return -1;
	FloatNDAFill(&growStep, 0.0f);

	if (FloatNDAAdd(&growStep, seasonMaps[sim->CURRENT_SEASON], &growStep) != 0) { 
		FloatNDAFree(&growStep);
		return -1;
	}

	for (size_t i = 0; i < growStep.size; i++) {
		growStep.data[i] *= climatMap->data[i];
	}
	
	if (FloatNDAAdd(resMap, &growStep, resMap)) {
		FloatNDAFree(&growStep);
		return -1;
	}
	FloatNDAFree(&growStep);

	return 0;
}


#endif
