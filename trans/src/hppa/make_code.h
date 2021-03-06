/*
 * Copyright 2002-2011, The TenDRA Project.
 * Copyright 1997, United Kingdom Secretary of State for Defence.
 *
 * See doc/copyright/ for the full copyright terms.
 */

#ifndef MAKECODE_INCLUDED
#define MAKECODE_INCLUDED

#include <reader/exp.h>

extern where nowhere;
extern makeans make_code(exp, space, where, int);
extern int repeat_level;
extern outofline * current_odd_bit;
extern int last_odd_bit;
extern bool doing_odd_bits;
extern int do_indexed_loads;

#endif /* MAKECODE_INCLUDED */
