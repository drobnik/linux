# SPDX-License-Identifier: GPL-2.0

# Memblock simulator requires AddressSanitizer (libasan) library
CFLAGS += -I. -I../../include -Wall -O2 -fsanitize=address \
	  -fsanitize=undefined -D CONFIG_PHYS_ADDR_T_64BIT
LDFLAGS += -fsanitize=address -fsanitize=undefined
TARGETS = main
TEST_OFILES = tests/basic_api.o tests/test.o
DEP_OFILES = memblock.o mb_slab.o mmzone.o slab.o
OFILES = main.o $(DEP_OFILES) $(TEST_OFILES)
LOCAL_H = linux/memblock.h
EXTR_SRC = ../../../mm/memblock.c

ifeq ($(BUILD), 32)
	CFLAGS += -m32
	LDFLAGS += -m32
endif

# Process user parameters
include scripts/Makefile.include

main: $(OFILES)

$(OFILES): include

include: ../../../include/linux/memblock.h ../../include/linux/*.h \
	../../include/asm/*.h

	@mkdir -p linux
	@sed -e '/^#ifdef __KERNEL__/d' -e '/^#endif \/\* __KERNEL__ \*\//d' < $< > $(LOCAL_H)
	test -L asm/cache.h || ln -s ../../../../include/asm-generic/cache.h asm/cache.h
	test -L asm/cmpxchg.h || ln -s ../../../arch/x86/include/asm/cmpxchg.h asm/cmpxchg.h

memblock.c: $(EXTR_SRC)
	test -L memblock.c || ln -s $(EXTR_SRC) memblock.c

clean:
	$(RM) $(TARGETS) $(OFILES) $(LOCAL_H) memblock.c asm/cache.h asm/cmpxchg.h

help:
	@echo  'Memblock simulator'
	@echo  ''
	@echo  'Available targets:'
	@echo  '  main		  - Build the memblock simulator'
	@echo  '  clean		  - Remove generated files and symlinks in the directory'
	@echo  ''
	@echo  'Configuration:'
	@echo  '  make NUMA=1               - simulate enabled NUMA'
	@echo  '  make MOVABLE_NODE=1       - override `movable_node_is_enabled`'
	@echo  '                              definition to simulate movable NUMA nodes'
	@echo  '  make 32BIT_PHYS_ADDR_T=1  - Use 32 bit physical addresses'
	@echo  '  make MEMBLOCK_PHYSMEM=1   - Enable physmem, a collection of available'
	@echo  '                              physical memory, ignoring possible restrictions'
	@echo  '                              and memory hotplugging'

vpath %.c ../../lib

.PHONY: clean include help
