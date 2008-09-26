/* 
 *  Copyright (c) 2002-2008 The TenDRA Project <http://www.tendra.org/>.
 *  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 * 
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. Neither the name of The TenDRA Project nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific, prior written permission.
 * 
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
 *  IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 *  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 *  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * 
 *     		 Crown Copyright (c) 1997
 *     
 *     This TenDRA(r) Computer Program is subject to Copyright
 *     owned by the United Kingdom Secretary of State for Defence
 *     acting through the Defence Evaluation and Research Agency
 *     (DERA).  It is made available to Recipients with a
 *     royalty-free licence for its use, reproduction, transfer
 *     to other parties and amendment for any purpose not excluding
 *     product development provided that any such use et cetera
 *     shall be deemed to be acceptance of the following conditions:-
 *     
 *         (1) Its Recipients shall ensure that this Notice is
 *         reproduced upon any copies or amended versions of it;
 *     
 *         (2) Any amended version of it shall be clearly marked to
 *         show both the nature of and the organisation responsible
 *         for the relevant amendment or amendments;
 *     
 *         (3) Its onward transfer from a recipient to another
 *         party shall be deemed to be that party's acceptance of
 *         these conditions;
 *     
 *         (4) DERA gives no warranty or assurance as to its
 *         quality or suitability for any purpose and DERA accepts
 *         no liability whatsoever in relation to any use to which
 *         it may be put.
 * 
 * $Id$
 * 
 */


/*
 * Automatically generated by lexi version 2.0
 */

#ifndef LEXI_GENERATED_HEADER_lexi__INCLUDED
#define LEXI_GENERATED_HEADER_lexi__INCLUDED


#include <stdio.h> /*TODO This is necessary because lexi automatically prepend #include "lexer.h" to the header. Maybe we should remove this unecessary feature*/

extern char token_buff [];
extern char *token_end;
extern int crt_lex_token;
extern int saved_lex_token;
extern unsigned int number_buffer;
extern struct lexi_state lexer_state;

extern FILE *lex_input;

/*
     PARSER MACROS
*/

#define CURRENT_LXI_TERMINAL	crt_lex_token
#define ADVANCE_LXI_LEXER		crt_lex_token = lexi_read_token(&lexer_state)
#define SAVE_LXI_LEXER(T)       (saved_lex_token = crt_lex_token,\
                                  crt_lex_token = (T))
#define RESTORE_LXI_LEXER          (crt_lex_token = saved_lex_token)
#include <stdbool.h>

/*
 * This struct holds state for the lexer; its representation is
 * private, but present here for ease of allocation.
 */
struct lexi_state {
	int (*zone_function)(struct lexi_state *);

	/*
	 * Lexi's buffer is a simple stack.
	 */
	int buffer[4];
	int buffer_index;
};

/* Read a character */
int lexi_readchar(struct lexi_state *state);

/* Push a character to lexi's buffer */
void lexi_push(struct lexi_state *state, const int c);

/* Pop a character from lexi's buffer */
int lexi_pop(struct lexi_state *state);

/* Flush lexi's buffer */
void lexi_flush(struct lexi_state *state);

#ifndef LEXI_EOF
#define LEXI_EOF 256
#endif

enum lexi_groups {
	lexi_group_white = 0x0001,
	lexi_group_alpha = 0x0002,
	lexi_group_digit = 0x0004,
	lexi_group_alphanum = 0x0008,
	lexi_group_alphanumhyphen = 0x0010,
};

/* true if the given character is present in the given group */
bool lexi_group(enum lexi_groups group, int c);

/* Identify a keyword */
int lexi_keyword(const char *identifier, int notfound);

/* Identify a token */
int lexi_read_token(struct lexi_state *state);

/* Initialise a lexi_state structure */
void lexi_init(struct lexi_state *state);

#endif


