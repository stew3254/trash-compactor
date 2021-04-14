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
%token ASSIGNOP

%token PROGRAM
%token BBEGIN END
%token IF THEN ELSE
%token WHILE DO

%token VAR
%token ARRAY OF
%token INTEGER REAL
%token FUNCTION PROCEDURE

%token DOTDOT

%%

start: PROGRAM ID '(' ')' ';'
  BBEGIN
    expression_list
  END
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
  |
  ;


expression: simple_expression
  | simple_expression RELOP simple_expression
  ;

simple_expression: term
  | ADDOP term
  | simple_expression ADDOP term
  ;

term: factor
  | term MULOP factor
  ;

factor: ID
  | ID '(' expression_list ')'
  | ID '[' expression ']'
  | INUM
  | RNUM
  | '(' expression ')'
  | NOT factor
  ;

%%

int main(int argc, char **argv) {
  if (argc > 1) {
    yyin = fopen(argv[1], "r");
  }
  return yyparse();
}