
#include "ocgsim.h"
#include <stdint.h>

#define SIMULATION_LENGTH 1000

int main() {
	OCGSim sim;
	sim.WORLD_SIZE = 100;
	sim.WORLD_RESOULUTION = 10;
	sim.CURRENT_SEASON = 0;
	
	if (OCGSimInit(&sim) != 0)
		return -1;

	for (size_t i = 0; i < SIMULATION_LENGTH; ++i) {
		if (OCGSimTick(&sim) != 0) {
			OCGSimFree(&sim);
			return -1;
		}
	}
	OCGSimFree(&sim);
	return 0;
}

