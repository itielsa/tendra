/*
 * Copyright (c) 2002-2005 The TenDRA Project <http://www.tendra.org/>.
 * All rights reserved.
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
#include<stdio.h>

#include <string.h>
#include <stddef.h>

#include "error.h"
#include "xalloc.h"

#include "char.h"

/*
    WHITE SPACE CHARACTERS

    This variable holds all the white space characters.
*/

letter *white_space = NULL;


/*
    LEXICAL PASS REPRESENTATIONS

    These variables describe the various lexical passes.
*/

static character passes [2] = {
    { LAST_LETTER, NULL, NULL, NULL, NULL },
    { LAST_LETTER, NULL, NULL, NULL, NULL }
};

/*character *pre_pass = passes;*/
zone  global_zone_v={"global",passes,passes+1,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
zone* global_zone=&global_zone_v;

/*
    ALLOCATE A NEW CHARACTER

    This routine allocates a new character with value c.
*/

static character *
new_char(letter c)
{
    character *p;
    static int chars_left = 0;
    static character *chars_free = NULL;
    if (chars_left == 0) {
	chars_left = 100;
	chars_free = xmalloc_nof(character, chars_left);
    }
    p = chars_free + (--chars_left);
    p->ch = c;
    p->cond = NULL;
    p->opt = NULL;
    p->next = NULL;
    p->definition = NULL;
    return(p);
}


/*
    ADD A CHARACTER

    This routine adds the string s (defined using data) to the lexical
    pass p.
*/

void
add_char(character *p, letter *s, char *cond, instructions_list* instlist, char* map)
{
    character *q;
    letter c = *s;
    if (p->next == NULL) {
	q = new_char(c);
	p->next = q;
    } else {
	character *r = NULL;
	for (q = p->next; q && (q->ch < c); q = q->opt)r = q;
	if (q && q->ch == c) {
	    /* already exists */
	} else {
	    q = new_char(c);
	    if (r == NULL) {
		q->opt = p->next;
		p->next = q;
	    } else {
		q->opt = r->opt;
		r->opt = q;
	    }
	}
    }
    if (c == LAST_LETTER) {
        if ((instlist && q->definition) || (map && q->map))
	    error(ERROR_SERIOUS, "TOKEN already defined");
        q->cond=cond;
        if(instlist) 
	    q->definition=instlist;
        else
	    q->map=map;
    }
    else 
      add_char(q, s + 1, cond, instlist, map);
    
    return;
}

/* 
    ALLOCATE A NEW ARG 

    This routine allocates a arg for a user provided function
*/
static arg*
new_arg(void) 
{
    arg *p;
    static int args_left = 0;
    static arg *args_free = NULL;
    if (args_left == 0) {
	args_left = 100;
	args_free = xmalloc_nof(arg, args_left);
    }
    p = args_free + (--args_left);
    p->next =NULL;
    return(p);
}

/*
    ADDS AN ARG

    This routines adds a new arg
 */
arg *
add_arg (arg_type t, unsigned int d)
{
    arg* p = new_arg();
    p->type = t;
    p->digit = d;
    return(p);
}

/*
    ALLOCATES A NEW ARGS LIST

    This routines allocates a new args list
*/
static args_list*
new_args_list (void) 
{
    args_list *p;
    static int args_lists_left = 0;
    static args_list *args_lists_free = NULL;
    if (args_lists_left == 0) {
	args_lists_left = 100;
	args_lists_free = xmalloc_nof(args_list, args_lists_left);
    }
    p = args_lists_free + (--args_lists_left);
    p->head =NULL;
    p->tail=&(p->head);
    return(p);
}


/*
    ADDS A NEW ARGS LIST

    This routines adds a new args list
*/
args_list*
add_args_list (void) 
{
    args_list *p=new_args_list();
    return(p);
}


/*
    ALLOCATE A NEW USER FUNCTION

    This routine allocates a new user provided function
*/
static user_function * 
new_user_function (void)
{
    user_function *p;
    static int user_functions_left = 0;
    static user_function *user_functions_free = NULL;
    if (user_functions_left == 0) {
	user_functions_left = 100;
	user_functions_free = xmalloc_nof(user_function, user_functions_left);
    }
    p = user_functions_free + (--user_functions_left);
    p->name=NULL;
    p->args=NULL;
    return p;
};
/*
    ALLOCATE ADDS USER FUNCTION

    This routine add a new user provided function
*/
user_function * 
add_user_function (char *name)
{
    user_function *p=new_user_function();
    p->name=name;
    return p;
};


/*
    ALLOCATE A NEW INSTRUCTION

    This routine allocates a new instrucion
*/

static instruction * 
new_instruction (instruction_type type) 
{
    instruction *p;
    static int instructions_left = 0;
    static instruction *instructions_free = NULL;
    if (instructions_left == 0) {
	instructions_left = 100;
	instructions_free = xmalloc_nof(instruction, instructions_left);
    }
    p = instructions_free + (--instructions_left);
    p->type=type;
    p->next=NULL;
    return p;
};

/*
    ADD  A NEW RETURN TOKEN INSTRUCTION

    This routine adds a new return token instruction
*/

instruction * 
add_instruction_return_token (char* name)
{
    instruction *p=new_instruction(return_token);
    p->name=name;
    return p;
};

/*
    ADD  A NEW FUNCTION INSTRUCTION

    This routine adds a new function instruction
*/

instruction * 
add_instruction_function (char* name, args_list* args) 
{
    instruction* p=new_instruction(apply_function);
    p->fun=add_user_function(name);
    p->fun->args=args;
    return p;
};

/*
    ADD  A NEW DO NOTHING INSTRUCTION

    This routine adds a new do nothing instruction
*/

instruction * 
add_instruction_donothing () 
{
    instruction* p=new_instruction(do_nothing);
    return p;
};

/*
    ADD  A NEW PUSH ZONE INSTRUCTION

    This routine adds a new push zone instruction
*/

instruction * 
add_instruction_pushzone (zone* z) 
{
    instruction* p=new_instruction(push_zone);
    p->z=z;
    return p;
};

/*
    ADD  A NEW PUSH ZONE INSTRUCTION

    This routine adds a new pop zone instruction
*/

instruction*
add_instruction_popzone (zone* z) 
{
    instruction* p=new_instruction(pop_zone);
    p->z=z;
    return p;
};

/*
    ALLOCATE A NEW INSTRUCTION

    This routine allocates a new instrucion
*/

static 
instructions_list*
new_instructions_list (void)
{
    instructions_list *p;
    static int instructions_list_left = 0;
    static instructions_list *instructions_list_free = NULL;
    if (instructions_list_left == 0) {
	instructions_list_left = 100;
	instructions_list_free = xmalloc_nof(instructions_list, instructions_list_left);
    }
    p = instructions_list_free + (--instructions_list_left);
    p->head=NULL;
    p->tail=&(p->head);
    return p;   
}

/*
    ADDS A NEW INSTRUCTIONS LIST

    This routine adds a ne instructions list.
*/

instructions_list*
add_instructions_list (void)
{
    instructions_list *p=new_instructions_list();
    return p;   
}


/*
    ALLOCATE A NEW ZONE

    This routine allocates a new zone
*/

static zone * 
new_zone (char* zid) 
{
    zone *p;
    static int zones_left = 0;
    static zone *zones_free = NULL;
    if (zones_left == 0) {
	zones_left = 100;
	zones_free = xmalloc_nof(zone, zones_left);
    }
    p = zones_free + (--zones_left);
    p->zone_name=zid;
    p->zone_main_pass=new_char(LAST_LETTER);
    p->zone_pre_pass=new_char(LAST_LETTER);

    p->keywords=NULL;
    
    p->default_actions=NULL;
    p->default_cond=NULL;

    p->entering_instructions=NULL;
    p->leaving_instructions=NULL;

    p->opt=NULL;
    p->next=NULL;
    return p;
};

/*
    FIND A  ZONE

    This routine finds a zone under the current one

*/

zone * 
find_zone (zone* z, char* zid) 
{
    zone* q;
    for(q=z->next; q!=NULL ; q=q->opt) {
        if(strcmp(q->zone_name,zid)==0)
	    break;
    }
    return q;
    }

/*
    ADD A ZONE

    This routine adds a new zone named zid under the current zone z
*/
zone* 
add_zone(zone* current_zone, char* zid,letter* e)
{
  zone* q;
  for(q=current_zone->next; q!=NULL; q=q->opt) {
    if(strcmp(q->zone_name,zid)==0) {
      error(ERROR_SERIOUS, "Zone %s already exists in this scope",zid);
      return NULL;
    }
  }

  q=new_zone(zid);
  q->opt=current_zone->next;
  current_zone->next=q;
  q->up=current_zone;
  instruction* inst = add_instruction_popzone(current_zone);
  instructions_list* inst_list=add_instructions_list();
  *(inst_list->tail)=inst;
  inst_list->tail=&(inst->next);
  add_char(q->zone_main_pass,e,NULL,inst_list,NULL);
  return q;
}


/*
    ARRAY OF ALL GROUPS

    This array gives all the character groups.
*/

char_group groups [ MAX_GROUPS ];
int no_groups = 0;


/*
    CREATE A NEW GROUP

    This routine creates a new character group with name nm and
    definition s.
*/

void
make_group(char *nm, letter *s)
{
    int i, n = no_groups;
    for (i = 0; i < n; i++) {
	if (!strcmp(nm, groups [i].name)) {
	    error(ERROR_SERIOUS, "Group '%s' already defined", nm);
	    return;
	}
    }
    if (n >= MAX_GROUPS) {
	error(ERROR_SERIOUS, "Too many groups defined (%d)", n);
	return;
    }
    groups [n].name = nm;
    groups [n].defn = s;
    no_groups = n + 1;
    return;
}


/*
    IS A LETTER IN A GROUP?

    This routine checks whether the letter c is in the list p.
*/

int
in_group(letter *p, letter c)
{
    letter a;
    if (p == NULL) return(0);
    while (a = *(p++), a != LAST_LETTER) {
	if (a == c) {
	    return(1);
	} else if (a == WHITE_LETTER) {
	    if (in_group(white_space, c)) return(1);
	} else if (a >= GROUP_LETTER) {
	    int n = (int)(a - GROUP_LETTER);
	    if (in_group(groups [n].defn, c)) return(1);
	}
    }
    return(0);
}


/*
    FIND AN ESCAPE SEQUENCE

    This routine finds the character corresponding to the escape sequence c.
*/

letter
find_escape(int c)
{
    letter a;
    switch (c) {
	case 'n': a = '\n'; break;
	case 't': a = '\t'; break;
	case 'v': a = '\v'; break;
	case 'f': a = '\f'; break;
	case 'r': a = '\r'; break;
	case '?': a = '?'; break;
	case '"': a = '"'; break;
	case '[': a = '['; break;
	case '\\': a = '\\'; break;
	case '\'': a = '\''; break;
	case 'e': a = EOF_LETTER; break;
	default : {
	    error(ERROR_SERIOUS, "Unknown escape sequence, '\\%c'",
		   (unsigned char)c);
	    a = (letter)(c & 0xff);
	    break;
	}
    }
    return(a);
}


/*
    TRANSLATE A STRING INTO A CHARACTER STRING

    This routine translates the string s into an array of letters.
*/

letter *
make_string(char *s)
{
    int i = 0, n = (int)strlen(s);
    letter *p = xmalloc_nof(letter, n + 1);
    while (*s) {
	letter a;
	char c = *(s++);
	if (c == '\\') {
	    c = *(s++);
	    a = find_escape(c);
	} else if (c == '[') {
	    int j;
	    size_t glen;
	    char *gnm = s;
	    while (*s && *s != ']')s++;
	    glen = (size_t)(s - gnm);
	    if (*s) {
		s++;
	    } else {
		error(ERROR_SERIOUS,
			"Unterminated character group name, '%s'", gnm);
	    }
	    for (j = 0; j < no_groups; j++) {
		if (strncmp(gnm, groups [j].name, glen) == 0) {
		    a = GROUP_LETTER + j;
		    break;
		}
	    }
	    if (j == no_groups) {
		if (strncmp(gnm, "white", glen) == 0) {
		    a = WHITE_LETTER;
		} else {
		    error(ERROR_SERIOUS, "Unknown character group, '%.*s'",
			   (int)glen, gnm);
		    a = '?';
		}
	    }
	    /* a is set */
	} else {
	    a = (letter)(c & 0xff);
	}
	p [i] = a;
	i++;
    }
    p [i] = LAST_LETTER;
    return(p);
}


/*
    LIST OF ALL KEYWORDS

    This variable gives a list of all the keywords.

keyword *keywords = NULL;
*/


/*
    ADD A KEYWORD

    This routine adds the keyword nm with its associated data to the list
    of all keywords.
*/

void
add_keyword(zone* z, char *nm, char* cond ,instruction* instr)
{
    static int keywords_left = 0;
    static keyword *keywords_free = NULL;
    keyword *p = z->keywords, *q = NULL;
    while (p) {
	int c = strcmp(nm, p->name);
	if (c == 0) {
	    error(ERROR_SERIOUS, "Keyword '%s' already defined", nm);
	    return;
	}
	if (c < 0)break;
	q = p;
	p = p->next;
    }
    if (keywords_left == 0) {
	keywords_left = 100;
	keywords_free = xmalloc_nof(keyword, keywords_left);
    }
    p = keywords_free + (--keywords_left);
    p->name = nm;
    p->instr = instr;
    p->cond = cond;
    p->done = 0;
    if (q == NULL) {
	p->next = z->keywords;
	z->keywords = p;
    } else {
	p->next = q->next;
	q->next = p;
    }
    return;
}


/*
	COUNT MAXIMUM TOKEN LENGTH

	Find the maximum token length within the given lexical pass.
*/
size_t
char_maxlength(character *c)
{
	character *p;
	size_t maxopt;
	size_t maxchar;

	maxopt = 0;
	for(p = c->next; p; p = p->opt) {
		size_t l;

		if(p->ch == LAST_LETTER) {
			continue;
		}

		l = char_maxlength(p) + 1;

		if(l > maxopt) {
			maxopt = l;
		}
	}

	return maxopt;
}

