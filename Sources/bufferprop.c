#include "bufferprop.h"

ring_wbells* buffinit(__uint16_t length)
{
	if(length == 0)
	{
		return NULL;
	}

	ring_wbells* ring1 = (ring_wbells *)malloc(length + sizeof(ring_wbells));
	if(ring1 == NULL)
	{
		return NULL;
	}
	else
	{
		(ring1) -> front = 0;
		(ring1) -> back = 0;
		(ring1) -> size = length;
		(ring1) -> buffs = (int8_t *)(ring1 + 1);
		(ring1) -> count = 0;
		return ring1;
	}
}

int8_t buffinsert(ring_wbells *ring1, int8_t data)
{
	if(ring1 -> count == ring1 -> size)
	{
		return -1;
	}
	ring1 -> buffs[ring1 -> front] = data;
	ring1 -> front = (ring1 -> front + 1)%(ring1 -> size);
	ring1 -> count++;
	return 0;
}

int8_t buffread(ring_wbells *ring1, int8_t *data)
{
	if(ring1 -> count == 0)
	{
		return -1;
	}
	*data = ring1 -> buffs[ring1 -> back];
	ring1 -> back = (ring1 -> back + 1)%(ring1 -> size);
	ring1 -> count--;
	return 0;
}

ring_wbells* buffresize(ring_wbells* ring1, __uint16_t length)
{
	ring_wbells *tmp;

	if(length==0 || length < ((ring1) -> count))
	{
		return NULL;
	}
	else if(length > ((ring1) -> count))
	{
		tmp = (ring_wbells *)realloc(ring1, length + sizeof(ring_wbells));
		if(tmp == NULL)
		{
			return NULL;
		}
		(ring1) = (ring_wbells *)tmp;
		if((ring1)->count != 0 && (ring1)->front==0)
		{
			(ring1)->front = (ring1)->size;
		}
		(ring1) -> size = length;
		(ring1) -> buffs = (int8_t *)(ring1 + 1);
	}
	return ring1;
}
