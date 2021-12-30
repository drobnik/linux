// SPDX-License-Identifier: GPL-2.0-or-later
#include "tests/test.h"
#include <string.h>

#define INIT_MEMBLOCK_REGIONS			128
#define INIT_PHYSMEM_REGIONS			4
#define INIT_MEMBLOCK_RESERVED_REGIONS		INIT_MEMBLOCK_REGIONS

static void reset_regions_array(struct memblock_region *regions, int size)
{
	int i;
	struct memblock_region r = {0, 0, 0};

	for (i = 0; i < size; i++)
		memmove(&regions[i], &r, sizeof(r));
}

void reset_memblock(void)
{
	reset_regions_array(memblock.memory.regions, memblock.memory.cnt);
	reset_regions_array(memblock.reserved.regions, memblock.reserved.cnt);

	memblock.memory.cnt	= 1;
	memblock.memory.max	= INIT_MEMBLOCK_REGIONS;
	memblock.memory.name	= "memory";
	memblock.memory.total_size = 0;

	memblock.reserved.cnt	= 1;
	memblock.reserved.max	= INIT_MEMBLOCK_RESERVED_REGIONS;
	memblock.reserved.name	= "reserved";
	memblock.reserved.total_size = 0;

	memblock.bottom_up	= false;
	memblock.current_limit	= MEMBLOCK_ALLOC_ANYWHERE;

#ifdef CONFIG_HAVE_MEMBLOCK_PHYS_MAP
	reset_regions_array(physmem.regions, physmem.cnt);

	physmem.cnt = 1;
	physmem.max = INIT_PHYSMEM_REGIONS;
	physmem.name = "physmem";
	physmem.total_size = 0;
#endif
}
