/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _SLAB_MEMBLOCK_H
#define _SLAB_MEMBLOCK_H

#include "../../include/linux/slab.h"
#include <linux/types.h>
#include <linux/gfp.h>

#define kzalloc_node(size, flags, node) kmalloc(size, flags)

enum slab_state {
	DOWN,
	PARTIAL,
	PARTIAL_NODE,
	UP,
	FULL
};

extern enum slab_state slab_state;

bool slab_is_available(void);

#endif
