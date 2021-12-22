/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _TOOLS_SLAB_H
#define _TOOLS_SLAB_H

#include <linux/types.h>
#include <linux/gfp.h>

#define SLAB_PANIC 2
#define SLAB_RECLAIM_ACCOUNT    0x00020000UL

void *kmalloc(size_t size, gfp_t gfp);
void kfree(void *p);

static inline void *kzalloc(size_t size, gfp_t gfp)
{
	return kmalloc(size, gfp | __GFP_ZERO);
}

#endif
