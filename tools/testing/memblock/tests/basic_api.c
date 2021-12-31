// SPDX-License-Identifier: GPL-2.0-or-later
#include <string.h>
#include "basic_api.h"
#include <linux/memblock.h>

#define EXPECTED_MEMBLOCK_REGIONS			128
#define EXPECTED_PHYSMEM_REGIONS			4

static int memblock_add_simple_check(void);
static int memblock_add_node_simple_check(void);
static int memblock_add_disjoint_check(void);
static int memblock_add_overlap_top_check(void);
static int memblock_add_overlap_bottom_check(void);
static int memblock_add_within_check(void);
static int memblock_add_twice_check(void);

static int memblock_reserve_simple_check(void);
static int memblock_reserve_disjoint_check(void);
static int memblock_reserve_overlap_top_check(void);
static int memblock_reserve_overlap_bottom_check(void);
static int memblock_reserve_within_check(void);
static int memblock_reserve_twice_check(void);

static int memblock_remove_simple_check(void);
static int memblock_remove_overlap_top_check(void);
static int memblock_remove_overlap_bottom_check(void);
static int memblock_remove_within_check(void);
static int memblock_remove_absent_check(void);

/*
 * memblock initialization tests
 */
int memblock_init_checks(void)
{
	reset_memblock();
	memblock_basic_init_check();
	memblock_physmem_init_check();

	return 0;
}

int memblock_basic_init_check(void)
{
	assert(memblock.memory.regions);
	assert(memblock.memory.cnt == 1);
	assert(memblock.memory.max == EXPECTED_MEMBLOCK_REGIONS);
	assert(strcmp(memblock.memory.name, "memory") == 0);

	assert(memblock.reserved.regions);
	assert(memblock.reserved.cnt == 1);
	assert(memblock.memory.max == EXPECTED_MEMBLOCK_REGIONS);
	assert(strcmp(memblock.reserved.name, "reserved") == 0);

	assert(!memblock.bottom_up);
	assert(memblock.current_limit == MEMBLOCK_ALLOC_ANYWHERE);

	return 0;
}

int memblock_physmem_init_check(void)
{
#ifdef CONFIG_HAVE_MEMBLOCK_PHYS_MAP
	assert(physmem.regions);
	assert(physmem.cnt == 1);
	assert(physmem.max == EXPECTED_PHYSMEM_REGIONS);
	assert(strcmp(physmem.name, "physmem") == 0);
#endif
	return 0;
}

/*
 * memblock_add and memblock_add_node tests
 */
int memblock_add_checks(void)
{
	memblock_add_simple_check();
	memblock_add_node_simple_check();
	memblock_add_disjoint_check();
	memblock_add_overlap_top_check();
	memblock_add_overlap_bottom_check();
	memblock_add_within_check();
	memblock_add_twice_check();

	return 0;
}

static int memblock_add_simple_check(void)
{
	reset_memblock();

	memblock_add(0x2, 0xA);
	struct memblock_region *r = &memblock.memory.regions[0];

	assert(r->base == 0x2);
	assert(r->size == 0xA);

	return 0;
}

static int memblock_add_node_simple_check(void)
{
#ifdef CONFIG_NUMA
	struct memblock_region *r = &memblock.memory.regions[0];

	reset_memblock();
	memblock_add_node(0x2, 0xA, 1, MEMBLOCK_HOTPLUG);

	assert(r->base == 0x2);
	assert(r->size == 0xA);
	assert(r->nid == 1);
	assert(r->flags == MEMBLOCK_HOTPLUG);
#endif

	return 0;
}

static int memblock_add_disjoint_check(void)
{
	struct region r1 = {0x1, 0x2};
	struct region r2 = {0x5, 0x2};
	struct memblock_region *rgn1 = &memblock.memory.regions[0];
	struct memblock_region *rgn2 = &memblock.memory.regions[1];

	reset_memblock();
	memblock_add(r1.base, r1.size);
	memblock_add(r2.base, r2.size);

	assert(rgn1->base == 0x1);
	assert(rgn1->size == 0x2);

	assert(rgn2->base == 0x5);
	assert(rgn2->size == 0x2);

	return 0;
}

static int memblock_add_overlap_top_check(void)
{
	struct region r1 = {0x5, 0x4};
	struct region r2 = {0x0, 0x7};
	struct memblock_region *r = &memblock.memory.regions[0];

	reset_memblock();
	memblock_add(r1.base, r1.size);
	memblock_add(r2.base, r2.size);

	assert(r->base == 0x0);
	assert(r->size == 0x9);

	return 0;
}

static int memblock_add_overlap_bottom_check(void)
{
	struct region r1 = {0x1, 0xA};
	struct region r2 = {0x9, 0xB};
	struct memblock_region *r = &memblock.memory.regions[0];

	reset_memblock();
	memblock_add(r1.base, r1.size);
	memblock_add(r2.base, r2.size);

	assert(r->base == 0x1);
	assert(r->size == 0x13);

	return 0;
}

static int memblock_add_within_check(void)
{
	struct region r1 = {0x1, 0xA};
	struct region r2 = {0x2, 0x3};
	struct memblock_region *r = &memblock.memory.regions[0];

	reset_memblock();
	memblock_add(r1.base, r1.size);
	memblock_add(r2.base, r2.size);

	assert(r->base == 0x1);
	assert(r->size == 0xA);

	return 0;
}

static int memblock_add_twice_check(void)
{
	reset_memblock();

	memblock_add(0x0, 0x2);
	memblock_add(0x0, 0x2);

	assert(memblock.memory.cnt == 1);

	return 0;
}

/*
 * memblock_reserve tests
 */
int memblock_reserve_checks(void)
{
	memblock_reserve_simple_check();
	memblock_reserve_disjoint_check();
	memblock_reserve_overlap_top_check();
	memblock_reserve_overlap_bottom_check();
	memblock_reserve_within_check();
	memblock_reserve_twice_check();

	return 0;
}

static int memblock_reserve_simple_check(void)
{
	reset_memblock();

	memblock_reserve(0x1, 0x8);
	struct memblock_region *r = &memblock.reserved.regions[0];

	assert(r->base == 0x1);
	assert(r->size == 0x8);

	return 0;
}

static int memblock_reserve_disjoint_check(void)
{
	struct region r1 = {0x1, 0x1};
	struct region r2 = {0x4, 0x1};
	struct memblock_region *rgn1 = &memblock.reserved.regions[0];
	struct memblock_region *rgn2 = &memblock.reserved.regions[1];

	reset_memblock();
	memblock_reserve(r1.base, r1.size);
	memblock_reserve(r2.base, r2.size);

	assert(rgn1->base == r1.base);
	assert(rgn1->size == r1.size);

	assert(rgn2->base == r2.base);
	assert(rgn2->size == r2.size);

	return 0;
}

static int memblock_reserve_overlap_top_check(void)
{
	struct region r1 = {0x2, 0x7};
	struct region r2 = {0x0, 0x3};
	struct memblock_region *r = &memblock.reserved.regions[0];

	reset_memblock();
	memblock_reserve(r1.base, r1.size);
	memblock_reserve(r2.base, r2.size);

	assert(r->base == 0x0);
	assert(r->size == 0x9);

	return 0;
}

static int memblock_reserve_overlap_bottom_check(void)
{
	struct region r1 = {0x0, 0xA};
	struct region r2 = {0x9, 0x3};
	struct memblock_region *r = &memblock.reserved.regions[0];

	reset_memblock();
	memblock_reserve(r1.base, r1.size);
	memblock_reserve(r2.base, r2.size);

	assert(r->base == 0x0);
	assert(r->size == 0xC);

	return 0;
}

static int memblock_reserve_within_check(void)
{
	struct region r1 = {0x2, 0x1};
	struct region r2 = {0x0, 0x5};
	struct memblock_region *r = &memblock.memory.regions[0];

	reset_memblock();
	memblock_add(r1.base, r1.size);
	memblock_add(r2.base, r2.size);

	assert(r->base == 0x0);
	assert(r->size == 0x5);

	return 0;
}

static int memblock_reserve_twice_check(void)
{
	reset_memblock();

	memblock_reserve(0x0, 0x2);
	memblock_reserve(0x0, 0x2);

	assert(memblock.reserved.cnt == 1);

	return 0;
}

/*
 * memblock_remove tests
 */
int memblock_remove_checks(void)
{
	memblock_remove_simple_check();
	memblock_remove_absent_check();
	memblock_remove_overlap_top_check();
	memblock_remove_overlap_bottom_check();
	memblock_remove_within_check();

	return 0;
}

static int memblock_remove_simple_check(void)
{
	struct region r1 = {0x2, 0x1};
	struct region r2 = {0x1, 0x3};
	struct memblock_region *r = &memblock.memory.regions[0];

	reset_memblock();
	memblock_add(r1.base, r1.size);
	memblock_add(r2.base, r2.size);
	memblock_remove(r1.base, r1.size);

	/* expect an overwrite with regions[1], i.e. removing regions[0] */
	assert(r->base == 0x1);
	assert(r->size == 0x3);

	return 0;
}

static int memblock_remove_absent_check(void)
{
	struct region r1 = {0x3, 0x2};
	struct region r2 = {0x0, 0x2};
	struct memblock_region *r = &memblock.memory.regions[0];

	reset_memblock();
	memblock_add(r1.base, r1.size);
	memblock_remove(r2.base, r2.size);

	assert(r->base == 0x3);
	assert(r->size == 0x2);

	return 0;
}

static int memblock_remove_overlap_top_check(void)
{
	struct region r1 = {0x2, 0x7};
	struct region r2 = {0x0, 0x4};
	struct memblock_region *r = &memblock.memory.regions[0];

	reset_memblock();
	memblock_add(r1.base, r1.size);
	memblock_remove(r2.base, r2.size);

	assert(r->base == 0x4);
	assert(r->size == 0x5);

	return 0;
}

static int memblock_remove_overlap_bottom_check(void)
{
	struct region r1 = {0x1, 0xF};
	struct region r2 = {0x9, 0xA};
	struct memblock_region *r = &memblock.memory.regions[0];

	reset_memblock();
	memblock_add(r1.base, r1.size);
	memblock_remove(r2.base, r2.size);

	assert(r->base == 0x1);
	assert(r->size == 0x8);

	return 0;
}

static int memblock_remove_within_check(void)
{
	struct region r1 = {0x1, 0xF};
	struct region r2 = {0x3, 0x2};
	struct memblock_region *rgn1 = &memblock.memory.regions[0];
	struct memblock_region *rgn2 = &memblock.memory.regions[1];

	reset_memblock();
	memblock_add(r1.base, r1.size);
	memblock_remove(r2.base, r2.size);

	assert(rgn1->base == 0x1);
	assert(rgn1->size == 0x2);

	assert(rgn2->base == 0x5);
	assert(rgn2->size == 0xB);

	return 0;
}
