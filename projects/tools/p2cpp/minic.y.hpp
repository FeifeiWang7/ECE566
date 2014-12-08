/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     SEMICOLON = 258,
     COMMA = 259,
     COLON = 260,
     LBRACE = 261,
     RBRACE = 262,
     LPAREN = 263,
     RPAREN = 264,
     LBRACKET = 265,
     RBRACKET = 266,
     ASSIGN = 267,
     PLUS = 268,
     MINUS = 269,
     STAR = 270,
     DIV = 271,
     MOD = 272,
     LESSTHAN = 273,
     GT = 274,
     LTE = 275,
     MINIC_GTE = 276,
     EQ = 277,
     NEQ = 278,
     NOT = 279,
     LOGICAL_AND = 280,
     LOGICAL_OR = 281,
     BITWISE_OR = 282,
     BITWISE_XOR = 283,
     LSHIFT = 284,
     RSHIFT = 285,
     BITWISE_INVERT = 286,
     DOT = 287,
     ARROW = 288,
     AMPERSAND = 289,
     QUESTION_MARK = 290,
     FOR = 291,
     WHILE = 292,
     IF = 293,
     ELSE = 294,
     DO = 295,
     STRUCT = 296,
     SIZEOF = 297,
     RETURN = 298,
     BREAK = 299,
     CONTINUE = 300,
     INT = 301,
     VOID = 302,
     STATIC = 303,
     AUTO = 304,
     EXTERN = 305,
     TYPEDEF = 306,
     CONST = 307,
     VOLATILE = 308,
     ENUM = 309,
     UNION = 310,
     REGISTER = 311,
     NUMBER = 312,
     IDENT = 313,
     LOWER_THAN_ELSE = 314
   };
#endif
/* Tokens.  */
#define SEMICOLON 258
#define COMMA 259
#define COLON 260
#define LBRACE 261
#define RBRACE 262
#define LPAREN 263
#define RPAREN 264
#define LBRACKET 265
#define RBRACKET 266
#define ASSIGN 267
#define PLUS 268
#define MINUS 269
#define STAR 270
#define DIV 271
#define MOD 272
#define LESSTHAN 273
#define GT 274
#define LTE 275
#define MINIC_GTE 276
#define EQ 277
#define NEQ 278
#define NOT 279
#define LOGICAL_AND 280
#define LOGICAL_OR 281
#define BITWISE_OR 282
#define BITWISE_XOR 283
#define LSHIFT 284
#define RSHIFT 285
#define BITWISE_INVERT 286
#define DOT 287
#define ARROW 288
#define AMPERSAND 289
#define QUESTION_MARK 290
#define FOR 291
#define WHILE 292
#define IF 293
#define ELSE 294
#define DO 295
#define STRUCT 296
#define SIZEOF 297
#define RETURN 298
#define BREAK 299
#define CONTINUE 300
#define INT 301
#define VOID 302
#define STATIC 303
#define AUTO 304
#define EXTERN 305
#define TYPEDEF 306
#define CONST 307
#define VOLATILE 308
#define ENUM 309
#define UNION 310
#define REGISTER 311
#define NUMBER 312
#define IDENT 313
#define LOWER_THAN_ELSE 314




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 55 "minic.y"
{
  int num;
  char * id;
  Type*  type;
  Value* value;
  paramlist_t *params;
}
/* Line 1529 of yacc.c.  */
#line 175 "minic.y.hpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

