/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 1 "minic.y"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
//#include "llvm/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/IRBuilder.h"

#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/ToolOutputFile.h"


#include <memory>
#include <algorithm>

#include "list.h"
#include "symbol.h"

using namespace llvm;

int num_errors;

extern int yylex();   /* lexical analyzer generated from lex.l */

int yyerror(const char *error);
int parser_error(const char*);

void minic_abort();
char *get_filename();
int get_lineno();

int loops_found=0;

extern Module *M;

Function *Fun;
IRBuilder<> *Builder;

Value* BuildFunction(Type* RetType, const char *name, 
			   paramlist_t *params);



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

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
/* Line 193 of yacc.c.  */
#line 274 "minic.y.cpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 287 "minic.y.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   172

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  60
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  98
/* YYNRULES -- Number of states.  */
#define YYNSTATES  171

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   314

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    13,    21,    22,
      31,    36,    40,    42,    45,    47,    49,    50,    52,    57,
      63,    67,    70,    74,    76,    78,    80,    82,    84,    86,
      88,    90,    92,    95,   100,   101,   103,   104,   106,   108,
     111,   114,   117,   125,   126,   127,   135,   145,   146,   148,
     151,   155,   157,   159,   163,   165,   171,   173,   177,   179,
     183,   185,   189,   191,   195,   197,   201,   203,   207,   211,
     213,   217,   221,   225,   229,   231,   235,   239,   241,   245,
     249,   251,   255,   259,   263,   265,   267,   270,   272,   275,
     278,   281,   284,   287,   290,   292,   294,   296,   300
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      61,     0,    -1,    62,    -1,    61,    62,    -1,    63,    -1,
      66,    -1,    -1,    68,    58,     8,    70,     9,    64,    74,
      -1,    -1,    68,    15,    58,     8,    70,     9,    65,    74,
      -1,    68,    15,    69,     3,    -1,    68,    69,     3,    -1,
      66,    -1,    67,    66,    -1,    46,    -1,    58,    -1,    -1,
      71,    -1,    71,     4,    68,    69,    -1,    71,     4,    68,
      15,    69,    -1,    71,     4,    68,    -1,    68,    69,    -1,
      68,    15,    69,    -1,    68,    -1,    73,    -1,    74,    -1,
      80,    -1,    81,    -1,    85,    -1,    78,    -1,    79,    -1,
       3,    -1,    86,     3,    -1,     6,    75,    76,     7,    -1,
      -1,    67,    -1,    -1,    77,    -1,    72,    -1,    77,    72,
      -1,    44,     3,    -1,    45,     3,    -1,    38,     8,    86,
       9,    72,    39,    72,    -1,    -1,    -1,    37,     8,    82,
      86,     9,    83,    72,    -1,    36,     8,    84,     3,    84,
       3,    84,     9,    72,    -1,    -1,    86,    -1,    43,     3,
      -1,    43,    86,     3,    -1,    87,    -1,    88,    -1,   100,
      12,    87,    -1,    89,    -1,    89,    35,    86,     5,    88,
      -1,    90,    -1,    89,    26,    90,    -1,    91,    -1,    90,
      25,    91,    -1,    92,    -1,    91,    27,    92,    -1,    93,
      -1,    92,    28,    93,    -1,    94,    -1,    93,    34,    94,
      -1,    95,    -1,    94,    22,    95,    -1,    94,    23,    95,
      -1,    96,    -1,    95,    18,    96,    -1,    95,    19,    96,
      -1,    95,    20,    96,    -1,    95,    21,    96,    -1,    97,
      -1,    96,    29,    97,    -1,    96,    30,    97,    -1,    98,
      -1,    97,    13,    98,    -1,    97,    14,    98,    -1,    99,
      -1,    98,    15,    99,    -1,    98,    16,    99,    -1,    98,
      17,    99,    -1,   101,    -1,    58,    -1,    15,    58,    -1,
     102,    -1,    34,   103,    -1,    15,   103,    -1,    14,   101,
      -1,    13,   101,    -1,    31,   101,    -1,    24,   101,    -1,
     103,    -1,    58,    -1,   104,    -1,     8,    86,     9,    -1,
      57,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   130,   130,   131,   134,   146,   153,   152,   172,   171,
     187,   202,   218,   222,   229,   236,   243,   246,   253,   257,
     261,   265,   270,   275,   283,   284,   285,   286,   287,   288,
     289,   292,   296,   302,   309,   312,   319,   322,   328,   332,
     338,   343,   349,   356,   360,   356,   380,   387,   390,   396,
     401,   407,   413,   417,   424,   428,   434,   438,   445,   449,
     455,   459,   465,   469,   475,   479,   485,   489,   493,   499,
     503,   507,   511,   515,   521,   525,   529,   535,   539,   543,
     549,   553,   557,   561,   567,   571,   582,   595,   599,   604,
     609,   613,   617,   621,   628,   634,   643,   647,   653
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SEMICOLON", "COMMA", "COLON", "LBRACE",
  "RBRACE", "LPAREN", "RPAREN", "LBRACKET", "RBRACKET", "ASSIGN", "PLUS",
  "MINUS", "STAR", "DIV", "MOD", "LESSTHAN", "GT", "LTE", "MINIC_GTE",
  "EQ", "NEQ", "NOT", "LOGICAL_AND", "LOGICAL_OR", "BITWISE_OR",
  "BITWISE_XOR", "LSHIFT", "RSHIFT", "BITWISE_INVERT", "DOT", "ARROW",
  "AMPERSAND", "QUESTION_MARK", "FOR", "WHILE", "IF", "ELSE", "DO",
  "STRUCT", "SIZEOF", "RETURN", "BREAK", "CONTINUE", "INT", "VOID",
  "STATIC", "AUTO", "EXTERN", "TYPEDEF", "CONST", "VOLATILE", "ENUM",
  "UNION", "REGISTER", "NUMBER", "IDENT", "LOWER_THAN_ELSE", "$accept",
  "translation_unit", "external_declaration", "function_definition", "@1",
  "@2", "declaration", "declaration_list", "type_specifier", "declarator",
  "param_list_opt", "param_list", "statement", "expr_stmt",
  "compound_stmt", "declaration_list_opt", "statement_list_opt",
  "statement_list", "break_stmt", "continue_stmt", "selection_stmt",
  "iteration_stmt", "@3", "@4", "expr_opt", "jump_stmt", "expression",
  "assignment_expression", "conditional_expression",
  "logical_OR_expression", "logical_AND_expression",
  "inclusive_OR_expression", "exclusive_OR_expression", "AND_expression",
  "equality_expression", "relational_expression", "shift_expression",
  "additive_expression", "multiplicative_expression", "cast_expression",
  "lhs_expression", "unary_expression", "postfix_expression",
  "primary_expression", "constant", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    60,    61,    61,    62,    62,    64,    63,    65,    63,
      66,    66,    67,    67,    68,    69,    70,    70,    71,    71,
      71,    71,    71,    71,    72,    72,    72,    72,    72,    72,
      72,    73,    73,    74,    75,    75,    76,    76,    77,    77,
      78,    79,    80,    82,    83,    81,    81,    84,    84,    85,
      85,    86,    87,    87,    88,    88,    89,    89,    90,    90,
      91,    91,    92,    92,    93,    93,    94,    94,    94,    95,
      95,    95,    95,    95,    96,    96,    96,    97,    97,    97,
      98,    98,    98,    98,    99,   100,   100,   101,   101,   101,
     101,   101,   101,   101,   102,   103,   103,   103,   104
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     0,     7,     0,     8,
       4,     3,     1,     2,     1,     1,     0,     1,     4,     5,
       3,     2,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     4,     0,     1,     0,     1,     1,     2,
       2,     2,     7,     0,     0,     7,     9,     0,     1,     2,
       3,     1,     1,     3,     1,     5,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     3,     1,
       3,     3,     3,     3,     1,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     1,     1,     2,     1,     2,     2,
       2,     2,     2,     2,     1,     1,     1,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    14,     0,     2,     4,     5,     0,     1,     3,     0,
      15,     0,    15,     0,    16,    11,    16,    10,    23,     0,
      17,     0,     0,    15,    21,     6,     0,     8,    22,     0,
      20,     0,    34,     7,     0,    18,     9,    12,    35,     0,
      36,    19,    13,     0,    31,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,    95,
      38,    24,    25,     0,    37,    29,    30,    26,    27,    28,
       0,    51,    52,    54,    56,    58,    60,    62,    64,    66,
      69,    74,    77,    80,     0,    84,    87,    94,    96,     0,
       0,    95,    91,    90,    95,    89,    93,    92,    88,    47,
      43,     0,    49,     0,    40,    41,    33,    39,    32,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,    48,     0,     0,    50,    57,     0,    59,    61,    63,
      65,    67,    68,    70,    71,    72,    73,    75,    76,    78,
      79,    81,    82,    83,    53,    47,     0,     0,     0,     0,
      44,     0,    55,    47,     0,     0,     0,    45,    42,     0,
      46
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     4,    29,    31,     5,    38,     6,    11,
      19,    20,    60,    61,    62,    40,    63,    64,    65,    66,
      67,    68,   132,   164,   130,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -137
static const yytype_int16 yypact[] =
{
     -24,  -137,     4,  -137,  -137,  -137,   -10,  -137,  -137,   -46,
      25,    37,    34,    44,   -24,  -137,   -24,  -137,     1,    67,
      49,    71,    30,  -137,  -137,  -137,   -24,  -137,  -137,    89,
       6,    89,   -24,  -137,    30,  -137,  -137,  -137,   -24,    17,
       0,  -137,  -137,    30,  -137,    47,    55,    55,    -6,    55,
      55,     9,    92,    99,   101,    15,    94,   114,  -137,   107,
    -137,  -137,  -137,   135,     0,  -137,  -137,  -137,  -137,  -137,
     118,  -137,  -137,   -15,   120,   119,   121,   113,   112,    64,
      -4,   123,   116,  -137,   136,  -137,  -137,  -137,  -137,   141,
       9,  -137,  -137,  -137,   139,  -137,  -137,  -137,  -137,    47,
    -137,    47,  -137,   149,  -137,  -137,  -137,  -137,  -137,    55,
      47,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    47,  -137,
     150,  -137,    47,   145,  -137,   120,   151,   119,   121,   113,
     112,    64,    64,    -4,    -4,    -4,    -4,   123,   123,   116,
     116,  -137,  -137,  -137,  -137,    47,   146,     0,    55,   154,
    -137,   122,  -137,    47,     0,     0,   153,  -137,  -137,     0,
    -137
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -137,  -137,   156,  -137,  -137,  -137,     3,  -137,    82,    81,
     143,  -137,   -63,  -137,   -22,  -137,  -137,  -137,  -137,  -137,
    -137,  -137,  -137,  -137,  -136,  -137,   -45,    32,     5,  -137,
      56,    53,    54,    57,    58,    23,    10,    19,    20,   -34,
    -137,    76,  -137,    26,  -137
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -87
static const yytype_int16 yytable[] =
{
      89,   107,    45,    44,     7,     9,    32,    33,    45,    36,
     103,   109,    12,    46,    47,    48,    22,    45,   102,   159,
     110,    34,     1,    45,    49,   121,   122,   166,    46,    47,
      48,    50,    43,    14,    51,    37,    52,    53,    54,    49,
      15,    42,    16,    55,    56,    57,    50,    17,    10,    51,
       1,    58,    94,    26,   131,    45,   133,    58,    59,    23,
      46,    47,    48,    45,    23,   136,    58,    91,    46,    47,
      90,    49,    58,    59,    95,    23,    25,    98,    50,    49,
      27,    51,   117,   118,   119,   120,    50,   156,    23,    51,
      13,   151,   152,   153,   161,    32,    18,   104,    18,    24,
      99,   167,   168,    28,    58,    59,   170,   100,    30,   101,
     131,    35,    58,    91,    39,    41,    95,   105,   131,   -85,
      39,   108,    92,    93,    13,    96,    97,   143,   144,   145,
     146,   125,   126,   127,   115,   116,   123,   124,   141,   142,
     147,   148,   106,   149,   150,   111,   112,   114,   128,   113,
     129,   -86,   134,   155,   157,   160,   158,   163,     8,    21,
     154,   165,   169,   162,   137,   135,   138,     0,     0,     0,
     139,     0,   140
};

static const yytype_int16 yycheck[] =
{
      45,    64,     8,     3,     0,    15,     6,    29,     8,    31,
      55,    26,    58,    13,    14,    15,    15,     8,     3,   155,
      35,    15,    46,     8,    24,    29,    30,   163,    13,    14,
      15,    31,    15,     8,    34,    32,    36,    37,    38,    24,
       3,    38,     8,    43,    44,    45,    31,     3,    58,    34,
      46,    57,    58,     4,    99,     8,   101,    57,    58,    58,
      13,    14,    15,     8,    58,   110,    57,    58,    13,    14,
      15,    24,    57,    58,    48,    58,     9,    51,    31,    24,
       9,    34,    18,    19,    20,    21,    31,   132,    58,    34,
       9,   125,   126,   127,   157,     6,    14,     3,    16,    18,
       8,   164,   165,    22,    57,    58,   169,     8,    26,     8,
     155,    30,    57,    58,    32,    34,    90,     3,   163,    12,
      38,     3,    46,    47,    43,    49,    50,   117,   118,   119,
     120,    15,    16,    17,    22,    23,    13,    14,   115,   116,
     121,   122,     7,   123,   124,    25,    27,    34,    12,    28,
       9,    12,     3,     3,     9,     9,     5,     3,     2,    16,
     128,    39,     9,   158,   111,   109,   112,    -1,    -1,    -1,
     113,    -1,   114
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    46,    61,    62,    63,    66,    68,     0,    62,    15,
      58,    69,    58,    69,     8,     3,     8,     3,    68,    70,
      71,    70,    15,    58,    69,     9,     4,     9,    69,    64,
      68,    65,     6,    74,    15,    69,    74,    66,    67,    68,
      75,    69,    66,    15,     3,     8,    13,    14,    15,    24,
      31,    34,    36,    37,    38,    43,    44,    45,    57,    58,
      72,    73,    74,    76,    77,    78,    79,    80,    81,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,    86,
      15,    58,   101,   101,    58,   103,   101,   101,   103,     8,
       8,     8,     3,    86,     3,     3,     7,    72,     3,    26,
      35,    25,    27,    28,    34,    22,    23,    18,    19,    20,
      21,    29,    30,    13,    14,    15,    16,    17,    12,     9,
      84,    86,    82,    86,     3,    90,    86,    91,    92,    93,
      94,    95,    95,    96,    96,    96,    96,    97,    97,    98,
      98,    99,    99,    99,    87,     3,    86,     9,     5,    84,
       9,    72,    88,     3,    83,    39,    84,    72,    72,     9,
      72
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
#line 135 "minic.y"
    {
  /* finish compiling function */
  if(num_errors>100)
    {
      minic_abort();
    }
  else if(num_errors==0)
    {
      
    }
;}
    break;

  case 5:
#line 147 "minic.y"
    { 
  /* nothing to be done here */
;}
    break;

  case 6:
#line 153 "minic.y"
    {
  symbol_push_scope();
  /* This is a mid-rule action */
  BuildFunction((yyvsp[(1) - (5)].type),(yyvsp[(2) - (5)].id),(yyvsp[(4) - (5)].params));  
;}
    break;

  case 7:
#line 159 "minic.y"
    { 
  /* This is the rule completion */
  BasicBlock *BB = Builder->GetInsertBlock();
  if(!BB->getTerminator())
    {
      Builder->CreateRet(ConstantInt::getSigned(
						IntegerType::get(getGlobalContext(),64),0));
    }

  symbol_pop_scope();
  /* make sure basic block has a terminator (a return statement) */
;}
    break;

  case 8:
#line 172 "minic.y"
    {
  symbol_push_scope();
  BuildFunction(PointerType::get((yyvsp[(1) - (6)].type),0),(yyvsp[(3) - (6)].id),(yyvsp[(5) - (6)].params));
;}
    break;

  case 9:
#line 177 "minic.y"
    { 
  /* This is the rule completion */


  /* make sure basic block has a terminator (a return statement) */

  symbol_pop_scope();
;}
    break;

  case 10:
#line 188 "minic.y"
    {
  if (is_global_scope())
    {
      Twine name((yyvsp[(3) - (4)].id));
      new GlobalVariable(*M,(Type*)PointerType::get((yyvsp[(1) - (4)].type),0),false,GlobalValue::ExternalLinkage,(Constant*)NULL,name);
    } 
  else
    {
      symbol_insert((yyvsp[(3) - (4)].id),  /* map name to alloca */
		    Builder->CreateAlloca(PointerType::get((yyvsp[(1) - (4)].type),0),NULL,(yyvsp[(3) - (4)].id)), /* build alloca */
		    0);  /* not an arg */
    }

;}
    break;

  case 11:
#line 203 "minic.y"
    {
  if (is_global_scope())
    {
      Twine name((yyvsp[(2) - (3)].id));
      new GlobalVariable(*M,(Type*)(yyvsp[(1) - (3)].type),false,GlobalValue::ExternalLinkage,(Constant*)NULL,name);
    }
  else
    {
      symbol_insert((yyvsp[(2) - (3)].id),  /* map name to alloca */
		    Builder->CreateAlloca((yyvsp[(1) - (3)].type),NULL,(yyvsp[(2) - (3)].id)), /* build alloca */
		    0);  /* not an arg */
    }
;}
    break;

  case 12:
#line 219 "minic.y"
    {

;}
    break;

  case 13:
#line 223 "minic.y"
    {

;}
    break;

  case 14:
#line 230 "minic.y"
    {
  (yyval.type) = IntegerType::get(getGlobalContext(),32);
;}
    break;

  case 15:
#line 237 "minic.y"
    {
  (yyval.id) = (yyvsp[(1) - (1)].id);
;}
    break;

  case 16:
#line 243 "minic.y"
    { 
  (yyval.params) = NULL;
;}
    break;

  case 17:
#line 247 "minic.y"
    { 
  (yyval.params) = (yyvsp[(1) - (1)].params);
;}
    break;

  case 18:
#line 254 "minic.y"
    {
  (yyval.params) = push_param((yyvsp[(1) - (4)].params),(yyvsp[(4) - (4)].id),(yyvsp[(3) - (4)].type));
;}
    break;

  case 19:
#line 258 "minic.y"
    {
  (yyval.params) = push_param((yyvsp[(1) - (5)].params),(yyvsp[(5) - (5)].id),(Type*)PointerType::get((yyvsp[(3) - (5)].type),0));
;}
    break;

  case 20:
#line 262 "minic.y"
    {
  (yyval.params) = push_param((yyvsp[(1) - (3)].params),NULL,(yyvsp[(3) - (3)].type));
;}
    break;

  case 21:
#line 266 "minic.y"
    {
  /* create a parameter list with this as the first entry */
  (yyval.params) = push_param(NULL, (yyvsp[(2) - (2)].id), (yyvsp[(1) - (2)].type));
;}
    break;

  case 22:
#line 271 "minic.y"
    {
  /* create a parameter list with this as the first entry */
  (yyval.params) = push_param(NULL, (yyvsp[(3) - (3)].id), (Type*)PointerType::get((yyvsp[(1) - (3)].type),0));
;}
    break;

  case 23:
#line 276 "minic.y"
    {
  /* create a parameter list with this as the first entry */
  (yyval.params) = push_param(NULL, NULL, (yyvsp[(1) - (1)].type));
;}
    break;

  case 31:
#line 293 "minic.y"
    { 

;}
    break;

  case 32:
#line 297 "minic.y"
    { 

;}
    break;

  case 33:
#line 303 "minic.y"
    {

;}
    break;

  case 34:
#line 309 "minic.y"
    {

;}
    break;

  case 35:
#line 313 "minic.y"
    {

;}
    break;

  case 36:
#line 319 "minic.y"
    {

;}
    break;

  case 37:
#line 323 "minic.y"
    {

;}
    break;

  case 38:
#line 329 "minic.y"
    {

;}
    break;

  case 39:
#line 333 "minic.y"
    {

;}
    break;

  case 40:
#line 339 "minic.y"
    {

;}
    break;

  case 41:
#line 344 "minic.y"
    {

;}
    break;

  case 42:
#line 350 "minic.y"
    { 

;}
    break;

  case 43:
#line 356 "minic.y"
    { 
  // set up header basic block
  // make it the new insertion point

;}
    break;

  case 44:
#line 360 "minic.y"
    { 
  // set up loop body
  // BasicBlock *expr=Buider->GetInsertBlock(), *body, *after;

  // create new body and exit blocks

  // call push loop to record this loop's important blocks
  //push_loop(expr,body,body,after);
;}
    break;

  case 45:
#line 370 "minic.y"
    {
  // finish while loop rule
  /* finish loop */
  //loop_info_t info = get_loop();

  // insert back edge from body to header

  //pop_loop();
;}
    break;

  case 46:
#line 381 "minic.y"
    {
  // 566: add mid-rule actions to support for-statement
;}
    break;

  case 47:
#line 387 "minic.y"
    { 

;}
    break;

  case 48:
#line 391 "minic.y"
    { 

;}
    break;

  case 49:
#line 397 "minic.y"
    { 
  Builder->CreateRetVoid();

;}
    break;

  case 50:
#line 402 "minic.y"
    {
  Builder->CreateRet((yyvsp[(2) - (3)].value));
;}
    break;

  case 51:
#line 408 "minic.y"
    { 
  (yyval.value)=(yyvsp[(1) - (1)].value);
;}
    break;

  case 52:
#line 414 "minic.y"
    {
  (yyval.value)=(yyvsp[(1) - (1)].value);
;}
    break;

  case 53:
#line 418 "minic.y"
    {
  // build store using address from lhs_expression
;}
    break;

  case 54:
#line 425 "minic.y"
    {
  (yyval.value)=(yyvsp[(1) - (1)].value);
;}
    break;

  case 55:
#line 429 "minic.y"
    {
  // Hint: select instruction
;}
    break;

  case 56:
#line 435 "minic.y"
    {
  (yyval.value) = (yyvsp[(1) - (1)].value);
;}
    break;

  case 57:
#line 439 "minic.y"
    {
  // Hint: do not build control flow, build an 2 icmps and 2 selects and one or
  // Since we do not need to enforce short circuiting, you can take some short cuts!

;}
    break;

  case 58:
#line 446 "minic.y"
    {
  (yyval.value) = (yyvsp[(1) - (1)].value);
;}
    break;

  case 59:
#line 450 "minic.y"
    {
  // Just like LOGICAL_OR above but with LOGICAL_AND
;}
    break;

  case 60:
#line 456 "minic.y"
    {
    (yyval.value)=(yyvsp[(1) - (1)].value);
;}
    break;

  case 61:
#line 460 "minic.y"
    {
  // Implement
;}
    break;

  case 62:
#line 466 "minic.y"
    {
  (yyval.value) = (yyvsp[(1) - (1)].value);
;}
    break;

  case 63:
#line 470 "minic.y"
    {
  // Implement
;}
    break;

  case 64:
#line 476 "minic.y"
    {
  (yyval.value) = (yyvsp[(1) - (1)].value);
;}
    break;

  case 65:
#line 480 "minic.y"
    {
  // Implement
;}
    break;

  case 66:
#line 486 "minic.y"
    {
  (yyval.value) = (yyvsp[(1) - (1)].value);
;}
    break;

  case 67:
#line 490 "minic.y"
    {
  // Implement
;}
    break;

  case 68:
#line 494 "minic.y"
    {
  // Implement
;}
    break;

  case 69:
#line 500 "minic.y"
    {
    (yyval.value)=(yyvsp[(1) - (1)].value);
;}
    break;

  case 70:
#line 504 "minic.y"
    {
  // Implement: icmp
;}
    break;

  case 71:
#line 508 "minic.y"
    {
  // Implement: icmp
;}
    break;

  case 72:
#line 512 "minic.y"
    {
  // Implement
;}
    break;

  case 73:
#line 516 "minic.y"
    {
  // Implement
;}
    break;

  case 74:
#line 522 "minic.y"
    {
    (yyval.value)=(yyvsp[(1) - (1)].value);
;}
    break;

  case 75:
#line 526 "minic.y"
    {
  // Implement
;}
    break;

  case 76:
#line 530 "minic.y"
    {
  // Implement
;}
    break;

  case 77:
#line 536 "minic.y"
    {
  (yyval.value) = (yyvsp[(1) - (1)].value);
;}
    break;

  case 78:
#line 540 "minic.y"
    {
  // Implement
;}
    break;

  case 79:
#line 544 "minic.y"
    {
  // Implement
;}
    break;

  case 80:
#line 550 "minic.y"
    {
  (yyval.value) = (yyvsp[(1) - (1)].value);
;}
    break;

  case 81:
#line 554 "minic.y"
    {
  // Implement
;}
    break;

  case 82:
#line 558 "minic.y"
    {
  // Implement
;}
    break;

  case 83:
#line 562 "minic.y"
    {
  // Implement
;}
    break;

  case 84:
#line 568 "minic.y"
    { (yyval.value) = (yyvsp[(1) - (1)].value); ;}
    break;

  case 85:
#line 572 "minic.y"
    {
  int isArg=0;
  Value* val = symbol_find((yyvsp[(1) - (1)].id),&isArg);
  if (isArg)
    {
      // error
    }
  else
    (yyval.value) = val;
;}
    break;

  case 86:
#line 583 "minic.y"
    {
  int isArg=0;
  Value* val = symbol_find((yyvsp[(2) - (2)].id),&isArg);
  if (isArg)
    {
      // error
    }
  else
    (yyval.value) = Builder->CreateLoad(val);
;}
    break;

  case 87:
#line 596 "minic.y"
    {
  (yyval.value) = (yyvsp[(1) - (1)].value);
;}
    break;

  case 88:
#line 600 "minic.y"
    {
  // Implement
  (yyval.value) = (yyvsp[(2) - (2)].value);
;}
    break;

  case 89:
#line 605 "minic.y"
    {
  (yyval.value) = (yyvsp[(2) - (2)].value);
  // Implement
;}
    break;

  case 90:
#line 610 "minic.y"
    {
  // Implement
;}
    break;

  case 91:
#line 614 "minic.y"
    {
  (yyval.value) = (yyvsp[(2) - (2)].value);
;}
    break;

  case 92:
#line 618 "minic.y"
    {
  // Implement: Hint: xor
;}
    break;

  case 93:
#line 622 "minic.y"
    {
  // Implement
;}
    break;

  case 94:
#line 629 "minic.y"
    {
  (yyval.value) = (yyvsp[(1) - (1)].value);
;}
    break;

  case 95:
#line 635 "minic.y"
    { 
  int isArg=0;
  Value* val = symbol_find((yyvsp[(1) - (1)].id),&isArg);
  if (isArg)
    (yyval.value) = val;
  else
    (yyval.value) = Builder->CreateLoad(val);
;}
    break;

  case 96:
#line 644 "minic.y"
    {
  (yyval.value) = (yyvsp[(1) - (1)].value);
;}
    break;

  case 97:
#line 648 "minic.y"
    {
  (yyval.value) = (yyvsp[(2) - (3)].value)
;}
    break;

  case 98:
#line 654 "minic.y"
    { 
  // Implement
;}
    break;


/* Line 1267 of yacc.c.  */
#line 2357 "minic.y.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 659 "minic.y"


Value* BuildFunction(Type* RetType, const char *name, 
			   paramlist_t *params)
{
  int i;
  int size = paramlist_size(params);

  std::vector<Type*> v;
  std::vector<const char*> vname;

  v.resize(size,NULL);
  vname.resize(size,NULL);

  paramlist_t *tmp = params;
  i=size-1;
  while(tmp)
    {
      v[i]=(tmp->type);
      vname[i]=(tmp->name);
      tmp = next_param(tmp);
      i--;
    }
  ArrayRef<Type*> Params(v);

  FunctionType* FunType = FunctionType::get(RetType,Params,false);

  Fun = Function::Create(FunType,GlobalValue::ExternalLinkage,
			 name,M);
  Twine T("entry");
  BasicBlock *BB = BasicBlock::Create(getGlobalContext(),T,Fun);

  /* Create an Instruction Builder */
  Builder = new IRBuilder<>(getGlobalContext());
  Builder->SetInsertPoint(BB);

  Function::arg_iterator I = Fun->arg_begin();
  for(i=0; I!=Fun->arg_end();i++, I++)
    {
      // map args and create allocas!
      AllocaInst *AI = Builder->CreateAlloca(v[i]);
      Builder->CreateStore(&(*I),(Value*)AI);
      symbol_insert(vname[i],(Value*)AI,0);
    }


  return Fun;
}

extern int line_num;
extern char *infile[];
static int   infile_cnt=0;
extern FILE * yyin;

int parser_error(const char *msg)
{
  printf("%s (%d): Error -- %s\n",infile[infile_cnt-1],line_num,msg);
  return 1;
}

int internal_error(const char *msg)
{
  printf("%s (%d): Internal Error -- %s\n",infile[infile_cnt-1],line_num,msg);
  return 1;
}

int yywrap() {
  static FILE * currentFile = NULL;

  if ( (currentFile != 0) ) {
    fclose(yyin);
  }
  
  if(infile[infile_cnt]==NULL)
    return 1;

  currentFile = fopen(infile[infile_cnt],"r");
  if(currentFile!=NULL)
    yyin = currentFile;
  else
    printf("Could not open file: %s",infile[infile_cnt]);

  infile_cnt++;
  
  return (currentFile)?0:1;
}

int yyerror(const char* error)
{
  parser_error("Un-resolved syntax error.");
  return 1;
}

char * get_filename()
{
  return infile[infile_cnt-1];
}

int get_lineno()
{
  return line_num;
}


void minic_abort()
{
  parser_error("Too many errors to continue.");
  exit(1);
}

