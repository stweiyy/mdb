/**
 * This part is mostly from SQLite 
 * Do the syntax analysis job
*/

%include {
#ifdef __cplusplus
extern "C" {
#endif

#include "../mdbtype.h"
#include "../mdbproto.h"

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
}
#endif
}

%token_prefix TK_
%token_type {Token}
%default_type {Token}
%extra_argument {Parse *pParse}

%syntax_error{
	printf("syntax error!\n");
}

%name mdbParser

%nonassoc END_OF_FILE ILLEGAL SPACE UNCLOSED_STRING COMMENT FUNCTION COLUMN
	AGG_FUNCTION AGG_COLUMN CONST_FUNC.

input ::= cmdlist.
cmdlist ::= cmdlist ecmd.
cmdlist ::= ecmd.
ecmd ::= SEMI.
ecmd ::= explain cmdx SEMI.
cmdx ::= cmd.
explain ::= . {mdbBeginParse(pParse,0);}

////////////////////////Create table syntax///////////////////////////////////
cmd ::=create_table create_table_args.

create_table ::=CREATE(X) TABLE nm(Y).


create_table_args ::=LP columnlist conslist_opt(X) RP(Y).


columnlist ::=columnlist COMMA column.
columnlist ::=column.

column(A) ::=columnid(X) type carglist.

columnid(A) ::=nm(X).

%type id {Token}
id(A) ::= ID(X). {A=X;}

%type ids {Token}
ids(A) ::= ID(X). {A=X;}
ids(A) ::= STRING(X). {A=X;}


%type nm {Token}
nm(A) ::= ID(X). {A=X;}
nm(A) ::= STRING(X). {A=X;}


%type typetoken {Token}

type ::=.
type ::=typetoken(X). 

typetoken(A) ::=typename(X). {A=X;}
typetoken(A) ::=typename(X) LP signed RP(Y).

typetoken(A) ::=typename(X) LP signed COMMA signed RP(Y).

%type typename {Token}
typename(A) ::=ids(X). {A=X;}

%type signed {int}
signed(A) ::=plus_num(X). {A=atoi(reinterpret_cast<const char*>(X.z));}
signed(A) ::=minus_num(X). {A=-atoi(reinterpret_cast<const char*>(X.z));}

carglist ::=carglist carg.
carglist ::=.
carg ::=CONSTRAINT nm ccons.
carg ::=ccons.
carg ::=DEFAULT id(X).

ccons ::=NULL onconf.
%type onconf {int}
onconf(A) ::=.


conslist_opt(A) ::=.
/////////////////the pragma command/////////////////////////

plus_num(A) ::=plus_opt number(X). {A=X;}
minus_num(A) ::=MINUS number(X). {A=X;}
number(A) ::=INTEGER(X). {A=X;}
number(A) ::=FLOAT(X). {A=X;}
plus_opt ::=PLUS.
plus_opt ::=.

