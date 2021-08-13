%{

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"
#include "hashmap.h"
#include "y.tab.h"

int yylex();
int yyerror(char *msg);

hashmap *symbol_table;

typedef struct Symbol {
    int type;
    union {
        int ival;
        float rval;
        int opval;
        char *sval;
    } attribute;
} symbol;

symbol *symbol_new(int type) {
  symbol *new_s = (symbol *) malloc(sizeof(symbol));
  new_s->type = type;
  return new_s;
}

// TODO fix this when you can tell it's a string for proper string copy
void *symbol_copy(const void *e) {
  symbol *s = (symbol*) e;
  // Create the new symbol
  symbol *new_s = symbol_new(s->type);
  new_s->attribute.sval = s->attribute.sval;
  return new_s;
}

// TODO fix this for when you can tell it's a string for proper deletion
void symbol_del(void *e) {
  // free the struct
  free(e);
}

extern FILE *yyin;

%}

%union {
  int ival; // INUM
  float rval; // RNUM
  int opval; // ADDOP, MULOP
  char *sval; // ID

  tree_node *tval; // Tree node
};

// Tokens
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
%token ASSIGNOP

%token START_COMMENT END_COMMENT
%token PROGRAM
%token BBEGIN END
%token IF THEN ELSE
%token WHILE DO

%token VAR
%token ARRAY OF
%token INTEGER REAL
%token FUNCTION PROCEDURE

%token DOTDOT

// Types for grammar rules
%type <tval> start
%type <tval> identifier_list
%type <tval> declarations
%type <tval> type
%type <tval> standard_type
%type <tval> subprogram_declarations
%type <tval> subprogram_declaration
%type <tval> subprogram_head
%type <tval> arguments
%type <tval> parameter_list
%type <tval> compound_statement
%type <tval> optional_statement
%type <tval> statement_list
%type <tval> statement
%type <tval> variable
%type <tval> for_loop
%type <tval> procedure_statement
%type <tval> expression_list
%type <tval> expression
%type <tval> simple_expression
%type <tval> term
%type <tval> factor
%%

start: PROGRAM ID '(' identifier_list ')' ';'
  declarations
  subprogram_declarations
  compound_statement
  '.'
  ;

identifier_list: ID
  | identifier_list ',' ID
  ;

declarations: declarations VAR identifier_list ':' type ';'
  |
  ;

type: standard_type
  | ARRAY '[' INUM DOTDOT INUM ']' OF standard_type
  ;

standard_type: INTEGER
  | REAL
  ;

subprogram_declarations: subprogram_declarations subprogram_declaration ';'
  |
  ;

subprogram_declaration: subprogram_head declarations subprogram_declarations compound_statement
  ;

subprogram_head: FUNCTION ID arguments ':' standard_type ';'
  | PROCEDURE ID arguments ';'
  ;

arguments: '(' parameter_list ')'
  |
  ;

parameter_list: identifier_list ':' type
  | parameter_list ';' identifier_list ':' type
  ;

compound_statement: BBEGIN optional_statements END
  ;

optional_statements: statement_list
  |
  ;

statement_list: statement
  | statement_list ';' statement
  ;

statement: variable ASSIGNOP expression
  | procedure_statement
  | compound_statement
  | IF expression THEN statement ELSE statement
  | IF expression THEN statement
  | WHILE expression DO statement
  ;

variable: ID
  | ID '[' expression ']'
  ;

procedure_statement: ID
  | ID '(' expression_list ')'
  ;

expression_list: expression expression_list
    { $$ = $1; }
  | expression_list ',' expression
	{ $$ = tree_make_from(symbol_new(COMMA), $1, $3); }
  ;

expression: simple_expression
    { $$ = $1; }
  | simple_expression RELOP simple_expression
//    { $$ = tree_make_from(symbol_new(RELOP), $1, $3); ((symbol *) $$->e)->attribute.opval = $2; }
    { $$ = tree_make_from(symbol_new(RELOP), $1, $3); printf("%d", typeof($$)); }
  ;

simple_expression: term
    { $$ = $1; }
  | ADDOP term
//    { $$ = tree_make_from(symbol_new(ADDOP), $2, NULL); ((symbol *) $$->e)->attribute.opval = $1; }
  | simple_expression ADDOP term
//    { $$ = tree_make_from(symbol_new(ADDOP), $1, $3); ((symbol *) $$->e)->attribute.opval = $2; }
  ;

term: factor
    { $$ = $1; }
  | term MULOP factor
//    { $$ = tree_make_from(MULOP, $1, $3); $$->e.opval = $2; }
  ;

factor: ID
    { $$ = NULL; }
  | ID '(' expression_list ')'
    { $$ = NULL; }
  | ID '[' expression ']'
    { $$ = NULL; }
  | INUM
//    { $$ = tree_make_from(INUM, NULL, NULL); $$->e.ival = $1; }
  | RNUM
//    { $$ = tree_make_from(RNUM, NULL, NULL); $$->e.rval = $1; }
  | '(' expression ')'
    { $$ = $2; }
  | NOT factor
    { $$ = NULL; }
  ;

%%

int main(int argc, char **argv) {
  // Initialize the symbol table
  printf("Hello there\n");
  symbol_table = map_new(symbol_copy, symbol_del);
  if (argc > 1) {
    yyin = fopen(argv[1], "r");
  }
  return yyparse();
}