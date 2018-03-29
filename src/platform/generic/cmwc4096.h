#ifndef CMWC4096_H_INCLUDED
#define CMWC4096_H_INCLUDED

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// CMWC working parts
#define CMWC_CYCLE 4096 // as Marsaglia recommends
#define CMWC_C_MAX 809430660 // as Marsaglia recommends
struct cmwc_state {
	uint32_t Q[CMWC_CYCLE];
	uint32_t c;	// must be limited with CMWC_C_MAX
	unsigned i;
};

void initCMWC(struct cmwc_state *state, unsigned int seed);
uint32_t randCMWC(struct cmwc_state *state);

#endif
