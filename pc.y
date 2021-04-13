%{

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "y.tab.h"

int yylex();
int yyerror(char *msg);

extern FILE *yyin;

%}

%union {
  int ival; // INUM
  float rval; // RNUM
  int opval; // ADDOP, MULOP
  char *sval; // ID
};

%token <ival> INUM
%token <rval> RNUM
%token <sval> ID
%token <OPVAL> RELOP
%token LT LE GT GE NE EQ
%token <opval> ADDOP
%token PLUS MINUS OR
%token <opval> MULOP
%token STAR SLASH AND
%token NOT


%token PROGRAM
%token BBEGIN
%token END

%%

start: PROGRAM ID '(' ')' ';'
  BBEGIN
    expr_list
  END
  '.'
  ;

expr_list: expr expr_list
  |
  ;


expr: simple_expr
  | simple_expr RELOP simple_expr
  ;

simple_expr: term
  | ADDOP term
  | simple_expr ADDOP term
  ;

term: factor
  | term MULOP factor
  ;

factor: ID
  | ID '(' expr_list ')'
  | ID '[' expr ']'
  | INUM
  | RNUM
  | '(' expr ')'
  | NOT factor
  ;

%%

int main(int argc, char **argv) {
  if (argc > 1) {
    fprintf(stderr, "I should be working\n");
    yyin = fopen(argv[1], "r");
  }
  return yyparse();
}