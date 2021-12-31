/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef _MEMBLOCK_BASIC_H
#define _MEMBLOCK_BASIC_H

#include <assert.h>
#include "test.h"

int memblock_init_checks(void);
int memblock_basic_init_check(void);
int memblock_physmem_init_check(void);

int memblock_add_checks(void);
int memblock_reserve_checks(void);

#endif
