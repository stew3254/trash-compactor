%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "y.tab.h"
%}

number  	  [0-9]+
ident       [a-z]+
whitespace  [ \t]+

%%

{whitespace}    {;}
"program"       { fprintf(stderr, "[PROGRAM]"); return PROGRAM; }
"begin"         { fprintf(stderr, "[BEGIN]"); return BBEGIN; }
"end"           { fprintf(stderr, "[END]"); return END; }

"<"             { fprintf(stderr, "[RELOP:%c]", yylval.opval = LT); return RELOP; }
"<="            { fprintf(stderr, "[RELOP:%c]", yylval.opval = LE); return RELOP; }
">"             { fprintf(stderr, "[RELOP:%c]", yylval.opval = GT); return RELOP; }
">="            { fprintf(stderr, "[RELOP:%c]", yylval.opval = GE); return RELOP; }
"=="            { fprintf(stderr, "[RELOP:%c]", yylval.opval = EQ); return RELOP; }
"<>"            { fprintf(stderr, "[RELOP:%c]", yylval.opval = NE); return RELOP; }

"+"             { fprintf(stderr, "[ADDOP:%d]", yylval.opval = PLUS); return ADDOP; }
"-"             { fprintf(stderr, "[ADDOP:%d]", yylval.opval = MINUS); return ADDOP; }
"or"            { fprintf(stderr, "[ADDOP:%d]", yylval.opval = OR); return ADDOP; }
"*"             { fprintf(stderr, "[MULOP:%d]", yylval.opval = STAR); return MULOP; }
"/"             { fprintf(stderr, "[MULOP:%d]", yylval.opval = SLASH); return MULOP; }
"and"           { fprintf(stderr, "[MULOP:%c]", yylval.opval = AND); return MULOP; }

{number}        { fprintf(stderr, "[INUM:%d]", yylval.ival = atoi(yytext)); return INUM; }
{number}"."{number}  { fprintf(stderr, "[RNUM:%f]", yylval.rval = atof(yytext)); return RNUM; }
{ident}         { fprintf(stderr, "[ID:%s]", yylval.sval = strdup(yytext)); return ID; }
"\n"            { fprintf(stderr, "[EOL]\n"); }
.             	{ fprintf(stderr, "[%c]", yytext[0]); return yytext[0]; }

%%

int yyerror(char *msg) {
  fprintf(stderr, "ERROR: %s\n", msg);
  exit(1);
}
