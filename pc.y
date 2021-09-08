%{

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "tree.h"
#include "table_stack.h"
#include "y.tab.h"

int yylex();
int yyerror(char *msg);

table_stack *symbol_table;

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
%token <opval> RELOP
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
%token IF THEN ELSE IF_THEN IF_THEN_ELSE
%token WHILE DO

%token VAR
%token ARRAY OF
%token INTEGER REAL
%token BOOL
%token FUNCTION PROCEDURE

%token TYPE
%token PARAMETER
%token DECLARATIONS SUBPROGRAM_DECLARATIONS SUBPROGRAM_DECLARATION
%token IDENTIFIER_LIST STATEMENT_LIST PARAMETER_LIST EXPRESSION_LIST
%token COMPOUND_STATEMENT

%token DOTDOT
%token COMMA

// Types for grammar rules
%type <tval> start
%type <tval> identifier_list
%type <tval> declarations
%type <ival> type
%type <ival> standard_type
%type <tval> subprogram_declarations
%type <tval> subprogram_declaration
%type <tval> subprogram_head
%type <tval> arguments
%type <tval> parameter_list
%type <tval> compound_statement
%type <tval> optional_statements
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
    {
      // Create tree
      tree_node *stmt = tree_make_from(symbol_new(COMPOUND_STATEMENT), $9, NULL);
      tree_node *sub_dec = tree_make_from(symbol_new(SUBPROGRAM_DECLARATIONS), $8, stmt);
      tree_node *dec = tree_make_from(symbol_new(DECLARATIONS), $7, sub_dec);
      tree_node *id_list = tree_make_from(symbol_new(IDENTIFIER_LIST), $4, dec);

      $$ = tree_make_from(symbol_new(PROGRAM), NULL, id_list);
      ((symbol*) $$->e)->attribute.sval = $2;

      // Remove the global scope
      table_stack_pop(symbol_table);
    }
;

identifier_list: ID
    {
      // Create the symbol
      symbol * s = symbol_new(ID);
      s->attribute.sval = $1;
      // Insert into the hashmap
      table_stack_insert(symbol_table, $1, s);
      // Add symbol into the tree
      $$ = tree_make_from(s, NULL, NULL);
    }
  | identifier_list ',' ID
    {
      // Create the symbol
      symbol * s = symbol_new(ID);
      s->attribute.sval = $3;
      // Insert into the hashmap
      table_stack_insert(symbol_table, $3, s);
      // Add symbol into the tree
      $$ = tree_make_from(symbol_new(COMMA), $1, tree_make_from(s, NULL, NULL));
    }
;

declarations: declarations VAR identifier_list ':' type ';'
    {
      // TODO assign identifier types
      // Create type and tree
      symbol *type_s = symbol_new(TYPE);
      type_s->attribute.sval = $5;
      tree_node *type = tree_make_from(type_s, NULL, NULL);

      // Create identifier list and tree
      symbol * id_list_s = symbol_new(IDENTIFIER_LIST);
      id_list_s->attribute.sval = $3;
      tree_node *id_list = tree_make_from(id_list_s, NULL, type);

      // Create tree
      symbol * dec_s = symbol_new(DECLARATIONS);
      dec_s->attribute.sval = $1;
      $$ = tree_make_from(dec_s, NULL, id_list);
    }
  | // Nothing to do
    {
      $$ = NULL;
    }
;

type: standard_type
    {
      $$ = 1;
    }
  | ARRAY '[' INUM DOTDOT INUM ']' OF standard_type
    {
      // Make a tree node with proper bound checking on the array
      $$ = ARRAY; // Incomplete
    }
  ;

standard_type: INTEGER
    {
      $$ = tree_make_from(symbol_new(INTEGER), NULL, NULL);
    }
  | REAL
    {
      $$ = tree_make_from(symbol_new(REAL), NULL, NULL);
    }
  ;

subprogram_declarations: subprogram_declarations subprogram_declaration ';'
    {
      $$ = tree_make_from(symbol_new(SUBPROGRAM_DECLARATIONS), $1, $2);
    }
  | // Nothing to do
    {
      $$ = NULL;
    }
  ;

subprogram_declaration: subprogram_head declarations subprogram_declarations compound_statement
    {
      // TODO make a tree here
      $$ = NULL;
    }
  ;

subprogram_head: FUNCTION ID arguments ':' standard_type ';'
    {
      symbol *sym = symbol_new(FUNCTION);
      sym->attribute.sval = $2;
      $$ = tree_make_from(sym, $3, $5);
    }
  | PROCEDURE ID arguments ';'
    {
      symbol *sym = symbol_new(PROCEDURE);
      sym->attribute.sval = $2;
      $$ = tree_make_from(sym, $3, NULL);
    }
  ;

arguments: '(' parameter_list ')'
    {
      $$ = $2;
    }
  | // Nothing to do
    {
      $$ = NULL;
    }
  ;

parameter_list: identifier_list ':' type
    {
      $$ = tree_make_from(symbol_new(PARAMETER), $1, $3);
    }
  | parameter_list ';' identifier_list ':' type
    {
      $$ = tree_make_from(symbol_new(PARAMETER), $1, tree_make_from(symbol_new(PARAMETER), $3, $5));
    }
  ;

compound_statement: BBEGIN optional_statements END
    {
      $$ = $2;
    }
  ;

optional_statements: statement_list
    {
      $$ = $1;
    }
  | // Nothing to do
    {
      $$ = NULL;
    }
  ;

statement_list: statement
    {
      $$ = $1;
    }
  | statement_list ';' statement
    {
      $$ = tree_make_from(symbol_new(STATEMENT_LIST), $1, $3);
    }
  ;

statement: variable ASSIGNOP expression
    {
      // Do semantic checking here
      $$ = tree_make_from(symbol_new(ASSIGNOP), $1, $3);
    }
  | procedure_statement
    {
      $$ = $1;
    }
  | compound_statement
    {
      $$ = $1;
    }
  | IF expression THEN statement ELSE statement
    {
      // TODO make this a tree
      // $$ = tree_make_from(symbol_new(IF_THEN_ELSE, $2, $4));
      $$ = NULL;
    }
  | IF expression THEN statement
    {
      $$ = tree_make_from(symbol_new(IF_THEN), $2, $4);
    }
  | WHILE expression DO statement
    {
      $$ = tree_make_from(symbol_new(WHILE), $2, $4);
    }
  ;

variable: ID
    {
      symbol *sym;
      // See if the symbol exists
      if (table_stack_get(symbol_table, $1, &sym) != 1) {
        // If it does add it
        tree_make_from(sym, NULL, NULL);
      } else {
        fprintf(stderr, "This is bad, symbol not found\n");
      }
    }
  | ID '[' expression ']'
    {
      $$ = NULL;
    }
;

procedure_statement: ID
    {
      $$ = tree_make_from(symbol_new(ID), NULL, NULL);
      ((symbol *) $$->e)->attribute.sval = $1;
    }
  | ID '(' expression_list ')'
    {
      $$ = tree_make_from(symbol_new(FUNCTION_CALL), $3, NULL);
      ((symbol *) $$->e)->attribute.sval = $1;
    }
;

expression_list: expression expression_list
    {
      $$ = $1;
    }
  | expression_list ',' expression
    {
      $$ = tree_make_from(symbol_new(COMMA), $1, $3);
    }
  ;

expression: simple_expression
    {
      $$ = $1;
    }
  | simple_expression RELOP simple_expression
    {
      $$ = tree_make_from(symbol_new(RELOP), $1, $3);
      ((symbol *) $$->e)->attribute.opval = $2;
    }
  ;

simple_expression: term
    {
      $$ = $1;
    }
  | ADDOP term
    {
      $$ = tree_make_from(symbol_new(ADDOP), $2, NULL);
      ((symbol *) $$->e)->attribute.opval = $1;
    }
  | simple_expression ADDOP term
    {
      $$ = tree_make_from(symbol_new(ADDOP), $1, $3);
      ((symbol *) $$->e)->attribute.opval = $2;
    }
  ;

term: factor
    {
      $$ = $1;
    }
  | term MULOP factor
    {
      $$ = tree_make_from(symbol_new(MULOP), $1, $3);
      ((symbol *) $$->e)->attribute.opval = $2;
    }
  ;

factor: ID
    {
      symbol *sym;
      // See if the symbol exists
      if (table_stack_get(symbol_table, $1, &sym) != 1) {
        // If it does add it
        tree_make_from(sym, NULL, NULL);
      } else {
        fprintf(stderr, "This is bad, symbol not found\n");
      }
    }
  | ID '(' expression_list ')'
    {
      $$ = NULL;
    }
  | ID '[' expression ']'
    {
      $$ = NULL;
    }
  | INUM
    {
      $$ = tree_make_from(symbol_new(INUM), NULL, NULL);
      ((symbol *) $$->e)->attribute.ival = $1;
    }
  | RNUM
    {
      $$ = tree_make_from(symbol_new(RNUM), NULL, NULL);
      ((symbol *) $$->e)->attribute.rval = $1;
    }
  | '(' expression ')'
    {
      $$ = $2;
    }
  | NOT factor
    {
      $$ = NULL;
    }
  ;

%%

int main(int argc, char **argv) {
  // Initialize the symbol table
  symbol_table = table_stack_new();
  // Add a global scope
  table_stack_add(symbol_table);
  if (argc > 1) {
    yyin = fopen(argv[1], "r");
  }

  return yyparse();
}