# Copyright 2002-2011, The TenDRA Project.
# Copyright 1997, United Kingdom Secretary of State for Defence.
#
# See doc/copyright/ for the full copyright terms.


# CAE, Networking Services, Issue 4

+IMPLEMENT "cae/xns4-ar", "netinet/in.h.ts", "in_t" ;

# explicitly uint32_t for XNS5, instead of in_addr_t as here.
+TYPEDEF in_addr_t ~ne_addr_t;

+SUBSET "ndb_generic" := {

	+USE "c/c89", "stddef.h.ts", "size_t" (!?) ;

	+FIELD struct hostent {
		char *h_name ;
		char **h_aliases ;
		int h_addrtype ;
		int h_length ;
		char **h_addr_list ;
	} ;

	+FIELD struct netent {
		char *n_name ;
		char **n_aliases ;
		int n_addrtype ;
		~ne_addr_t n_net ;
	} ;

	+FIELD struct protoent {
		char *p_name ;
		char **p_aliases ;
		int p_proto ;
	} ;

	+FIELD struct servent {
		char *s_name ;
		char **s_aliases ;
		int s_port ;
		char *s_proto ;
	} ;

	+CONST int IPPORT_RESERVED ;

	+EXP (extern) int h_errno ;

	+CONST int HOST_NOT_FOUND, NO_DATA, NO_RECOVERY, TRY_AGAIN ;

	+FUNC void endhostent ( void ) ;
	+FUNC void endnetent ( void ) ;
	+FUNC void endprotoent ( void ) ;
	+FUNC void endservent ( void ) ;
	+FUNC struct hostent *gethostbyaddr ( const void *, size_t, int ) ;
	+FUNC struct hostent *gethostbyname ( const char * ) ;
	+FUNC struct hostent *gethostent ( void ) ;
	+FUNC struct netent *getnetbyaddr ( ~ne_addr_t, int ) ;
	+FUNC struct netent *getnetbyname ( const char * ) ;
	+FUNC struct netent *getnetent ( void ) ;
	+FUNC struct protoent *getprotobyname ( const char * ) ;
	+FUNC struct protoent *getprotobynumber ( int ) ;
	+FUNC struct protoent *getprotoent ( void ) ;
	+FUNC struct servent *getservbyname ( const char *, const char * ) ;
	+FUNC struct servent *getservbyport ( int, const char * ) ;
	+FUNC struct servent *getservent ( void ) ;
	+FUNC void sethostent ( int ) ;
	+FUNC void setnetent ( int ) ;
	+FUNC void setprotoent ( int ) ;
	+FUNC void setservent ( int ) ;

} ;

