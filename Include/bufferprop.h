#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "MKL25Z4.h"

typedef struct
{
	int8_t *buffs;
	__uint16_t front;
	__uint16_t back;
	__uint16_t size;
	__uint16_t count;
} __attribute__((packed))ring_wbells;

ring_wbells* buffinit(__uint16_t length);

int8_t buffinsert(ring_wbells *ring1, int8_t data);

int8_t buffread(ring_wbells *ring1, int8_t *data);

ring_wbells* buffresize(ring_wbells *ring1, __uint16_t length);

