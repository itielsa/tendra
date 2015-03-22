/* $Id$ */

/*
 * Copyright 2002-2015, The TenDRA Project.
 * Copyright 1997, United Kingdom Secretary of State for Defence.
 *
 * See doc/copyright/ for the full copyright terms.
 */

#ifndef READER_H
#define READER_H


struct reader_dispatch {
	void (*dispatch_unit   )(void);
	void (*dispatch_capsule)(void);
};

extern struct reader_dispatch reader_dispatch;


#endif

