/* A Bison parser, made by GNU Bison 3.7.6.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INUM = 258,                    /* INUM  */
    RNUM = 259,                    /* RNUM  */
    ID = 260,                      /* ID  */
    RELOP = 261,                   /* RELOP  */
    LT = 262,                      /* LT  */
    LE = 263,                      /* LE  */
    GT = 264,                      /* GT  */
    GE = 265,                      /* GE  */
    NE = 266,                      /* NE  */
    EQ = 267,                      /* EQ  */
    ADDOP = 268,                   /* ADDOP  */
    PLUS = 269,                    /* PLUS  */
    MINUS = 270,                   /* MINUS  */
    OR = 271,                      /* OR  */
    MULOP = 272,                   /* MULOP  */
    STAR = 273,                    /* STAR  */
    SLASH = 274,                   /* SLASH  */
    AND = 275,                     /* AND  */
    NOT = 276,                     /* NOT  */
    ASSIGNOP = 277,                /* ASSIGNOP  */
    PROGRAM = 278,                 /* PROGRAM  */
    BBEGIN = 279,                  /* BBEGIN  */
    END = 280,                     /* END  */
    IF = 281,                      /* IF  */
    THEN = 282,                    /* THEN  */
    ELSE = 283,                    /* ELSE  */
    WHILE = 284,                   /* WHILE  */
    DO = 285,                      /* DO  */
    VAR = 286,                     /* VAR  */
    ARRAY = 287,                   /* ARRAY  */
    OF = 288,                      /* OF  */
    INTEGER = 289,                 /* INTEGER  */
    REAL = 290,                    /* REAL  */
    FUNCTION = 291,                /* FUNCTION  */
    PROCEDURE = 292,               /* PROCEDURE  */
    DOTDOT = 293                   /* DOTDOT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define INUM 258
#define RNUM 259
#define ID 260
#define RELOP 261
#define LT 262
#define LE 263
#define GT 264
#define GE 265
#define NE 266
#define EQ 267
#define ADDOP 268
#define PLUS 269
#define MINUS 270
#define OR 271
#define MULOP 272
#define STAR 273
#define SLASH 274
#define AND 275
#define NOT 276
#define ASSIGNOP 277
#define PROGRAM 278
#define BBEGIN 279
#define END 280
#define IF 281
#define THEN 282
#define ELSE 283
#define WHILE 284
#define DO 285
#define VAR 286
#define ARRAY 287
#define OF 288
#define INTEGER 289
#define REAL 290
#define FUNCTION 291
#define PROCEDURE 292
#define DOTDOT 293

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 15 "pc.y"

  int ival; // INUM
  float rval; // RNUM
  int opval; // ADDOP, MULOP
  char *sval; // ID

#line 150 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
