/* SPDX-License-Identifier: GPL-2.0 */
#ifndef SLAB_RADIX_H
#define SLAB_RADIX_H

#include "../../include/linux/slab.h"

void *kmem_cache_alloc(struct kmem_cache *cachep, int flags);
void kmem_cache_free(struct kmem_cache *cachep, void *objp);

struct kmem_cache *kmem_cache_create(const char *name, unsigned int size,
			unsigned int align, unsigned int flags,
			void (*ctor)(void *));

#endif
