/*
 * Copyright (c) 2002, 2003, 2004 The TenDRA Project <http://www.tendra.org/>.
 * All rights reserved.
 *
 * This code is derived from software contributed to The TenDRA Project by
 * Jeroen Ruigrok van der Werven.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of The TenDRA Project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific, prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
 * IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id$
 */
/*
    		 Crown Copyright (c) 1997

    This TenDRA(r) Computer Program is subject to Copyright
    owned by the United Kingdom Secretary of State for Defence
    acting through the Defence Evaluation and Research Agency
    (DERA).  It is made available to Recipients with a
    royalty-free licence for its use, reproduction, transfer
    to other parties and amendment for any purpose not excluding
    product development provided that any such use et cetera
    shall be deemed to be acceptance of the following conditions:-

        (1) Its Recipients shall ensure that this Notice is
        reproduced upon any copies or amended versions of it;

        (2) Any amended version of it shall be clearly marked to
        show both the nature of and the organisation responsible
        for the relevant amendment or amendments;

        (3) Its onward transfer from a recipient to another
        party shall be deemed to be that party's acceptance of
        these conditions;

        (4) DERA gives no warranty or assurance as to its
        quality or suitability for any purpose and DERA accepts
        no liability whatsoever in relation to any use to which
        it may be put.
*/


#include "config.h"
#include "types.h"
#include "ascii.h"
#include "file.h"
#include "pretty.h"
#include "tree.h"
#include "utility.h"


/*
    MACRO DEFINITIONS

    BUFFSIZE gives the size of the input buffer.  HI_MASK and LO_MASK
    are bit masks used to extract bits 8-15 and 0-7 respectively from
    an integer.
*/

#define BUFFSIZE	((size_t)1024)
#define HI_MASK		((unsigned)0xff00)
#define LO_MASK		((unsigned)0x00ff)


/*
    INPUT AND OUTPUT FILES AND BUFFERS

    The input TDF capsule is read from tdf_file.  The pretty-printed
    output is placed into pp_file.  The input uses the buffer
    inbuffer.
*/

FILE *pp_file;
static FILE *tdf_file;
static char inbuffer[BUFFSIZE];
static int ib_ptr, ib_size;


/*
    POSITION IN INPUT FILE

    The current position in the input file is recorded by means of a
    place.
*/

place here;


/*
    INPUT AND OUTPUT VARIABLES

    printflag is used to switch the printing on or off.  The present
    column number in the output file is given by column.  The maximum
    value attained by column is recorded in maximum.  The last character
    output is held in lastc.  The flag read_error is used to indicate
    that an error has occurred in reading the input file.
*/

int printflag = 1;
int column;
int maximum;
int lastc = 0;
int read_error = 0;
int dump = 0;


/*
    OPEN FILES

    The file name1 is opened for input and name2 for output.  If name2
    is the null string, the standard output is used.
*/

void
open_files(char *name1, char *name2)
{
    tdf_file = fopen(name1, "rb");
    if (tdf_file == null)fatal_error("Can't open %s", name1);
    if (name2 == null)dflag = 0;
    if (dflag) {
	pp_file = fopen(name2, "w");
	if (pp_file == null)fatal_error("Can't open %s", name2);
    } else {
	pp_file = stdout;
    }
    here.byte = 0;
    here.bit = 0;
    ib_size = 0;
    return;
}


/*
    READ THE NEXT BYTE FROM THE INPUT FILE

    This routine reads the next byte from the input file, putting it
    into the worksp field of here.
*/

static void
next_byte(void)
{
    if (read_error) {
	here.worksp = 0xff;
    } else {
	/* refill input buffer if required */
	if (ib_size == 0) {
	    ib_ptr = 0;
	    ib_size = (int)fread(inbuffer, 1, BUFFSIZE, tdf_file);
	    if (ib_size == 0) {
		/* if we have reached the end of the file... */
		read_error = 1;
		if (!dump) {
		    out("<reading error>");
		    input_error("Reading error");
		}
	    }
	}
	/* get the next character */
	if (read_error) {
	    here.worksp = 0xff;
	} else {
	    char c = inbuffer[ib_ptr++];
	    here.worksp = (unsigned)(c & 0xff);
	    ib_size--;
	}
    }
    return;
}


/*
    FETCH THE NEXT n BITS FROM THE INPUT FILE

    This routine reads the next n bits from the input file and returns
    them as an integer.  n will rarely be as much as 8, so there is
    no problem with overflow.
*/

long
fetch(int n)
{
    int m;
    unsigned a = 0;
    while (n) {
	/* read the next byte if necessary */
	if (here.bit == 0)next_byte();
	/* m = number of bits we need from this byte */
	m = BYTESIZE - here.bit;
	if (n < m)m = n;
	/* extract m bytes from here.worksp */
	here.worksp <<= m;
	a = (a << m) + ((here.worksp & HI_MASK) >> BYTESIZE);
	here.worksp &= LO_MASK;
	n -= m;
	here.bit += m;
	if (here.bit == BYTESIZE) {
	    here.bit = 0;
	    here.byte++;
	}
    }
    return((long)a);
}


/*
    ALIGN INPUT FILE TO AN 8 BIT BOUNDARY

    Bits are read from the input file until it is on an 8 bit boundary.
*/

void
byte_align(void)
{
    while (!(here.bit == 0 || here.bit == BYTESIZE)) {
	IGNORE fetch(1);
    }
    return;
}


/*
    GO TO A GIVEN PLACE IN THE INPUT FILE

    The current position is set to the position indicated by the place p.
*/

void
set_place(place *p)
{
    int s;
    here.byte = p->byte;
    here.bit = 0;
    s = fseek(tdf_file, here.byte, SEEK_SET);
    if (s)fatal_error("Internal file seek error");
    ib_size = 0;
    IGNORE fetch(p->bit);
    return;
}


/*
    SKIP THE NEXT n BITS IN THE INPUT FILE

    If n is small, the next n bits are read but discarded.  Otherwise
    set_place is used.
*/

void
skip_bits(long n)
{
    if (read_error) return;
    if (n <= 4 * BYTESIZE) {
	IGNORE fetch((int)n);
    } else {
	place p;
	long b = posn(here) + n;
	p.byte = b / BYTESIZE;
	p.bit = (int)(b % BYTESIZE);
	set_place(&p);
    }
    return;
}


/*
    ARRAY OF SPACES

    The indentation spaces used during pretty-printing are stored in
    an array.
*/

static char *spaces1;


/*
    INITIALIZE SPACES

    This routine initializes the array of indentation spaces, spaces1.
*/

void
init_spaces(int d)
{
    int i, j;
    if (helpflag) {
	spaces1 = alloc_nof(char, 5000);
	/* every dth character should be a '.' */
	for (i = 0, j = 0; i < 5000; i++) {
	    if (++j == d) {
		spaces1[i] = '.';
		j = 0;
	    } else {
		spaces1[i] = ' ';
	    }
	}
    }
    return;
}


/*
    OUTPUT n SPACES

    This routine outputs n indentation spaces, using the array spaces1
    when appropriate.
*/

void
spaces(int n)
{
    if (n < 0)n = 0;
    if (printflag) {
	if (helpflag) {
	    if (n) {
		 IGNORE fwrite(spaces1, sizeof(char),
				  (size_t)n, pp_file);
	    }
	} else {
	    int i;
	    int q = (n / TBSIZE);
	    int r = (n % TBSIZE);
	    for (i = 0; i < q; i++)IGNORE fputc(TAB, pp_file);
	    for (i = 0; i < r; i++)IGNORE fputc(SPACE, pp_file);
	}
    }
    column = n;
    lastc = ' ';
    return;
}
