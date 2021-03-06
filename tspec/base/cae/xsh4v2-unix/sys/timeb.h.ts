# Copyright 2002-2011, The TenDRA Project.
# Copyright 1997, United Kingdom Secretary of State for Defence.
#
# See doc/copyright/ for the full copyright terms.


+IMPLEMENT "c/c89", "time.h.ts", "time_t" ;

+FIELD struct timeb {
    time_t time ;
    unsigned short millitm ;
    short timezone ;
    short dstflag ;
} ;

+FUNC int ftime ( struct timeb * ) ;
