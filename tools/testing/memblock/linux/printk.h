/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _PRINTK_H
#define _PRINTK_H

#include <stdio.h>
#include <asm/bug.h>

/*
 *
 * memblock_dbg calls printk down the line, which uses different
 * format specifiers than printf. They can't be easily translated
 * on the fly but, as we are not using this output, we can ignore
 * this mismatch
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
#define printk printf
#pragma GCC diagnostic push

#define pr_info printk
#define pr_debug printk
#define pr_cont printk
#define pr_err printk
#define pr_warn printk

#endif
