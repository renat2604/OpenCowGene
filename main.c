#include "ocgv2.h"

#include <stdint.h>
#include <math.h>

#define WORLD_SIZE 100
#define WORLD_RESOLUTION 10
#define SIMULATION_LENGTH 20

const size_t WORLD_DIM = WORLD_SIZE * WORLD_RESOLUTION;

size_t RESOURCE_MAP_ID;
size_t CLIMAT_MAP_ID;

uint8_t CURRENT_SEASON = 0;
size_t SEASON_MAP_ID[4]; // Index in the next order: Winter, Spring, Summer, Autumn 

void SimulationInit(World2D*);
void SimulationTick(World2D*);

int main() {
	World2D world;

	SimulationInit(&world);
	
	for (size_t i = 0; i < SIMULATION_LENGTH; ++i) {
		SimulationTick(&world);
	}
	return 0;
}

void SimulationInit(World2D *world) {
	World2DInit(world);
	
	// Resource map init
	World2DAddFND(world, 3, (const size_t[]){WORLD_DIM, WORLD_DIM, 1}, &RESOURCE_MAP_ID);
	FloatNDArrayFill(
		World2DGetFND(world, RESOURCE_MAP_ID), 
		1.0f
	);
	
	// Climat map init
	World2DAddFND(world, 3, (const size_t[]){WORLD_DIM, WORLD_DIM, 1}, &CLIMAT_MAP_ID);
	FloatNDArray *climat = World2DGetFND(world, CLIMAT_MAP_ID);
	/// Inverted double pole gradient
	for (size_t i = 0; i < climat->size; i++) {
		size_t y = i / climat->shape[1];
		float t = (float)y / (climat->shape[0] - 1);
		climat->data[i] = 1.0f - fabsf(2.0f * t - 1.0f);
	}

	for (size_t i = 0; i < 4; i++) {
		World2DAddFND(
			world, 
			3, 
			(const size_t[]){WORLD_DIM, WORLD_DIM, 1}, 
			&SEASON_MAP_ID[i]
		);
		FloatNDArray *currMap = World2DGetFND(world, SEASON_MAP_ID[i]);
		switch (i) {
		case 0:
			FloatNDArrayFill(
				currMap,
				0.0f
			);
			break;
		case 1:
			FloatNDArrayFill(
				currMap,
				0.5f
			);
			break;
		case 2:
			FloatNDArrayFill(
				currMap,
				1.0f
			);
			break;
		case 3:
			FloatNDArrayFill(
				currMap,
				0.2f
			);
			break;
		}

	}

	World2DFree(world);
}

void SimulationTick(World2D *world) {
	FloatNDArray *resMap = World2DGetFND(world, RESOURCE_MAP_ID);
	FloatNDArray *climatMap = World2DGetFND(world, CLIMAT_MAP_ID);
	FloatNDArray *seasonMaps[4];
	for (size_t i = 0; i < 4; i++) {
		seasonMaps[i] = World2DGetFND(world, SEASON_MAP_ID[i]);
	}

	// Resource grow
	FloatNDArray growStep;
	FloatNDArrayFill(&growStep, 0.0f);
	FloatNDArrayAdd(&growStep, seasonMaps[CURRENT_SEASON], &growStep);
	for (size_t i = 0; i < growStep.size; i++) {
		growStep.data[i] *= climatMap->data[i];
	}
	FloatNDArrayAdd(resMap, &growStep, resMap);
}
