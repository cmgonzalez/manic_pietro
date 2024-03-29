#ifndef GAME_ZX
#define GAME_ZX

#include <arch/zx.h>
#include <intrinsic.h>
#include <z80.h>
#include "globals.h"

extern void            zx_isr(void);

extern void            zx_print_chr(unsigned char ui_row, unsigned char ui_col, unsigned char val) __z88dk_callee;
extern void            zx_print_int(unsigned char ui_row, unsigned char ui_col, unsigned int val) __z88dk_callee;
extern void            zx_print_str(unsigned char ui_row, unsigned char ui_col, unsigned char *s) __z88dk_callee;


// INLINE

#define zx_clock()          intrinsic_load16(_curr_time)
#define zx_set_clock(a)     intrinsic_store16(_curr_time,a)
#define zx_paper_fill(a)    zx_cls(a)
#define zx_print_ink(a)     (screen_ink = (a))
#define zx_print_paper(a)   (screen_paper = (a))

#endif
