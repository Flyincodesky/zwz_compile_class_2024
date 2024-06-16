/* original parser id follows */
/* yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93" */
/* (use YYMAJOR/YYMINOR for ifdefs dependent on parser version) */

#define YYBYACC 1
#define YYMAJOR 2
#define YYMINOR 0
#define YYPATCH 20240109

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)
#define YYENOMEM       (-2)
#define YYEOF          0
#undef YYBTYACC
#define YYBTYACC 0
#define YYDEBUGSTR YYPREFIX "debug"
#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "mini.y"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tac.h"
#include "obj.h"
#include "opt.h"

int yylex();
void yyerror(char* msg);

#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#line 14 "mini.y"
typedef union YYSTYPE
{	
	int var_type ; /*non:-1 , default:0 , int:1 , char:2,  2024/04/25*/
	char character;
	char *string;
	SYM *sym;
	TAC *tac;
	EXP	*exp;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 52 "mini.y.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

#if !(defined(yylex) || defined(YYSTATE))
int YYLEX_DECL();
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define EQ 257
#define NE 258
#define LT 259
#define LE 260
#define GT 261
#define GE 262
#define UMINUS 263
#define IF 264
#define THEN 265
#define ELSE 266
#define FI 267
#define WHILE 268
#define FOR 269
#define DO 270
#define DONE 271
#define CONTINUE 272
#define FUNC 273
#define PRINT 274
#define RETURN 275
#define BREAK 276
#define INT 277
#define CHAR 278
#define INTEGER 279
#define IDENTIFIER 280
#define TEXT 281
#define CHARACTER 282
#define YYERRCODE 256
typedef int YYINT;
static const YYINT yylhs[] = {                           -1,
    0,    1,    1,    2,    2,   19,   27,   27,    5,    5,
    3,    3,   26,    4,    4,    4,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,   21,   17,   18,
   18,   20,   20,    7,   24,   24,   24,   24,   24,   24,
   24,   24,   24,   24,   24,   24,   24,   24,   24,   24,
   24,   22,   22,   23,   23,    8,    9,    9,   10,   10,
   11,   12,   13,   13,   14,   15,   16,   25,
};
static const YYINT yylen[] = {                            2,
    1,    1,    2,    1,    1,    3,    1,    1,    1,    3,
    5,    1,    1,    1,    3,    0,    2,    2,    2,    2,
    2,    1,    1,    1,    1,    2,    1,    1,    4,    0,
    2,    1,    2,    3,    3,    3,    3,    3,    2,    3,
    3,    3,    3,    3,    3,    3,    1,    1,    1,    1,
    1,    0,    1,    1,    3,    4,    1,    3,    1,    1,
    2,    1,    5,    7,    5,    9,    4,    4,
};
static const YYINT yydefred[] = {                         0,
   12,    7,    8,   13,    0,    0,    2,    4,    5,    0,
    0,    3,    0,    9,    0,   14,    0,    6,    0,    0,
    0,   10,   15,   30,   11,    0,   27,    0,    0,    0,
   62,    0,    0,   28,    0,   32,    0,    0,    0,    0,
   22,   23,   24,    0,   25,   31,    0,    0,    0,    0,
    0,    0,   51,   47,    0,   49,    0,    0,    0,   50,
    0,    0,   17,   20,   19,   21,   18,   29,   33,   26,
    0,    0,    0,    0,   60,    0,   57,    0,    0,   39,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   56,
    0,   46,    0,    0,    0,    0,    0,    0,    0,    0,
   37,   38,   67,    0,    0,   65,    0,   58,   68,    0,
    0,    0,   64,    0,    0,   66,
};
#if defined(YYDESTRUCT_CALL) || defined(YYSTYPE_TOSTRING)
static const YYINT yystos[] = {                           0,
  256,  277,  278,  280,  284,  285,  286,  287,  303,  310,
  311,  286,   40,  280,  289,  280,  288,   59,   44,   44,
   41,  280,  280,  123,  301,  302,  256,  264,  268,  269,
  272,  274,  275,  276,  280,  290,  291,  292,  295,  296,
  297,  298,  299,  300,  301,  303,  304,  305,   40,   40,
   40,   40,  256,  279,  280,  282,   45,   40,  308,  309,
   40,   61,   59,   59,   59,   59,   59,  125,  290,   59,
  308,  308,  280,  291,  281,  293,  294,  308,   40,  308,
  308,  257,  258,  259,  260,  261,  262,   43,   45,   42,
   47,  306,  307,  308,  308,   41,   41,   59,   44,   41,
  306,   41,  308,  308,  308,  308,  308,  308,  308,  308,
  308,  308,   41,   44,  301,  301,  308,  294,   41,  308,
  266,   59,  301,  291,   41,  301,
};
#endif /* YYDESTRUCT_CALL || YYSTYPE_TOSTRING */
static const YYINT yydgoto[] = {                          5,
    6,    7,    8,   17,   15,   36,   37,   38,   76,   77,
   39,   40,   41,   42,   43,   44,   45,   26,    9,   47,
   48,   92,   93,   78,   60,   10,   11,
};
static const YYINT yysindex[] = {                      -200,
    0,    0,    0,    0,    0, -200,    0,    0,    0,  -32,
 -251,    0, -246,    0,  -37,    0,   17,    0, -244, -234,
  -73,    0,    0,    0,    0, -122,    0,   12,   26,   31,
    0,   32,  -36,    0,  -34,    0,   28,   36,   46,   51,
    0,    0,    0,   54,    0,    0,  -99,   57,  -36,  -36,
 -195,  -40,    0,    0,   59,    0,  -36,  -36,   61,    0,
  -36,  -36,    0,    0,    0,    0,    0,    0,    0,    0,
  -24,  -10,   50,   62,    0,   18,    0,   61,  -36,    0,
   -3,  -36,  -36,  -36,  -36,  -36,  -36,  -36,  -36,  -36,
  -36,   82,   80,   61,   61,  -73,  -73,  -36,  -40,    0,
   84,    0,   75,   75,   75,   75,   75,   75,  -17,  -17,
    0,    0,    0,  -36, -140,    0,   55,    0,    0,   61,
  -73, -195,    0,   86,  -73,    0,
};
static const YYINT yyrindex[] = {                         0,
    0,    0,    0,    0,    0,  128,    0,    0,    0,    0,
    0,    0,   24,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  -31,    0,    0,    0,   70,    0,
   89,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   40,   89,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   90,   52,  -39,    0,    0,    0,    0,    0,
    0,    0,   16,   23,   29,   35,   42,   48,    4,   10,
    0,    0,    0,    0,  -82,    0,    0,    0,    0,   68,
    0,    0,    0,    0,    0,    0,
};
#if YYBTYACC
static const YYINT yycindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,
};
#endif
static const YYINT yygindex[] = {                         0,
    0,  126,    0,    0,    0,   88,  -48,    0,    0,   34,
    0,    0,    0,    0,    0,    0,   -6,    0,  110,    0,
    0,   58,    0,  278,    0,    0,    0,
};
#define YYTABLESIZE 392
static const YYINT yytable[] = {                         58,
   24,   34,   74,   58,   57,   61,   19,   13,   57,   48,
   48,   48,   48,   48,   25,   48,   96,   90,   88,   34,
   89,   18,   91,   24,   90,   68,   62,   48,   14,   91,
   97,   90,   88,   16,   89,   22,   91,  102,   90,   88,
   63,   89,   63,   91,   35,   23,   35,   35,   35,   24,
   36,   49,   36,   36,   36,    1,   40,   21,  100,   40,
   20,   99,   35,   41,   16,   50,   41,   16,   36,   42,
   51,   52,   42,  124,   40,   43,    2,    3,   43,    4,
   59,   41,   44,   59,   73,   44,   63,   42,   45,  115,
  116,   45,   54,   43,   64,   54,   90,   88,   79,   89,
   44,   91,   90,   88,   65,   89,   45,   91,   55,   66,
   62,   55,   67,  122,  123,   70,   90,   88,  126,   89,
   98,   91,  113,  114,  119,  121,  125,    1,   61,   52,
   53,   12,  118,   27,   69,   46,  101,    0,    0,    0,
    0,   28,    0,    0,    0,   29,   30,    0,    0,   31,
    0,   32,   33,   34,    2,    3,   27,   35,    0,    0,
    0,    0,    0,    0,   28,    0,    0,    0,   29,   30,
    0,    0,   31,   63,   32,   33,   34,    0,    0,    0,
   35,   63,    0,    0,    0,   63,   63,    0,    0,   63,
    0,   63,   63,   63,    0,    0,    0,   63,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   53,    0,    0,    0,   53,
    0,    0,    0,    0,    0,   48,   48,   48,   48,   48,
   48,    0,   82,   83,   84,   85,   86,   87,   54,   55,
   75,   56,   54,   55,    0,   56,   82,   83,   84,   85,
   86,   87,    0,   82,   83,   84,   85,   86,   87,    0,
   35,   35,   35,   35,   35,   35,   36,   36,   36,   36,
   36,   36,   40,   40,   40,   40,   40,   40,    0,   41,
   41,   41,   41,   41,   41,   42,   42,   42,   42,   42,
   42,   43,   43,   43,   43,   43,   43,    0,   44,   44,
   44,   44,   44,   44,   45,   45,   45,   45,   45,   45,
   59,   82,   83,   84,   85,   86,   87,   82,   83,   84,
   85,   86,   87,    0,    0,    0,   71,   72,    0,    0,
    0,    0,    0,    0,   80,   81,    0,    0,   94,   95,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   94,    0,    0,  103,
  104,  105,  106,  107,  108,  109,  110,  111,  112,    0,
    0,    0,    0,    0,    0,  117,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  120,
};
static const YYINT yycheck[] = {                         40,
  123,   41,   51,   40,   45,   40,   44,   40,   45,   41,
   42,   43,   44,   45,   21,   47,   41,   42,   43,   59,
   45,   59,   47,  123,   42,  125,   61,   59,  280,   47,
   41,   42,   43,  280,   45,  280,   47,   41,   42,   43,
  123,   45,  125,   47,   41,  280,   43,   44,   45,  123,
   41,   40,   43,   44,   45,  256,   41,   41,   41,   44,
   44,   44,   59,   41,   41,   40,   44,   44,   59,   41,
   40,   40,   44,  122,   59,   41,  277,  278,   44,  280,
   41,   59,   41,   44,  280,   44,   59,   59,   41,   96,
   97,   44,   41,   59,   59,   44,   42,   43,   40,   45,
   59,   47,   42,   43,   59,   45,   59,   47,   41,   59,
   61,   44,   59,   59,  121,   59,   42,   43,  125,   45,
   59,   47,   41,   44,   41,  266,   41,    0,   59,   41,
   41,    6,   99,  256,   47,   26,   79,   -1,   -1,   -1,
   -1,  264,   -1,   -1,   -1,  268,  269,   -1,   -1,  272,
   -1,  274,  275,  276,  277,  278,  256,  280,   -1,   -1,
   -1,   -1,   -1,   -1,  264,   -1,   -1,   -1,  268,  269,
   -1,   -1,  272,  256,  274,  275,  276,   -1,   -1,   -1,
  280,  264,   -1,   -1,   -1,  268,  269,   -1,   -1,  272,
   -1,  274,  275,  276,   -1,   -1,   -1,  280,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  256,   -1,   -1,   -1,  256,
   -1,   -1,   -1,   -1,   -1,  257,  258,  259,  260,  261,
  262,   -1,  257,  258,  259,  260,  261,  262,  279,  280,
  281,  282,  279,  280,   -1,  282,  257,  258,  259,  260,
  261,  262,   -1,  257,  258,  259,  260,  261,  262,   -1,
  257,  258,  259,  260,  261,  262,  257,  258,  259,  260,
  261,  262,  257,  258,  259,  260,  261,  262,   -1,  257,
  258,  259,  260,  261,  262,  257,  258,  259,  260,  261,
  262,  257,  258,  259,  260,  261,  262,   -1,  257,  258,
  259,  260,  261,  262,  257,  258,  259,  260,  261,  262,
   33,  257,  258,  259,  260,  261,  262,  257,  258,  259,
  260,  261,  262,   -1,   -1,   -1,   49,   50,   -1,   -1,
   -1,   -1,   -1,   -1,   57,   58,   -1,   -1,   61,   62,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   79,   -1,   -1,   82,
   83,   84,   85,   86,   87,   88,   89,   90,   91,   -1,
   -1,   -1,   -1,   -1,   -1,   98,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  114,
};
#if YYBTYACC
static const YYINT yyctable[] = {                        -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,
};
#endif
#define YYFINAL 5
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 282
#define YYUNDFTOKEN 312
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? YYUNDFTOKEN : (a))
#if YYDEBUG
static const char *const yyname[] = {

"$end",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,"'('","')'","'*'","'+'","','","'-'",0,"'/'",0,0,0,0,0,0,0,0,0,0,0,"';'",0,
"'='",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'{'",0,"'}'",0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,"error","EQ","NE","LT","LE","GT","GE","UMINUS","IF","THEN","ELSE","FI",
"WHILE","FOR","DO","DONE","CONTINUE","FUNC","PRINT","RETURN","BREAK","INT",
"CHAR","INTEGER","IDENTIFIER","TEXT","CHARACTER","$accept","program",
"function_declaration_list","function_declaration","function","parameter_list",
"variable_list","statement","assignment_statement","print_statement",
"print_list","print_item","return_statement","null_statement","if_statement",
"while_statement","for_statement","call_statement","block","declaration_list",
"declaration","statement_list","break_statement","argument_list",
"expression_list","expression","call_expression","function_head","type",
"illegal-symbol",
};
static const char *const yyrule[] = {
"$accept : program",
"program : function_declaration_list",
"function_declaration_list : function_declaration",
"function_declaration_list : function_declaration_list function_declaration",
"function_declaration : function",
"function_declaration : declaration",
"declaration : type variable_list ';'",
"type : INT",
"type : CHAR",
"variable_list : IDENTIFIER",
"variable_list : variable_list ',' IDENTIFIER",
"function : function_head '(' parameter_list ')' block",
"function : error",
"function_head : IDENTIFIER",
"parameter_list : IDENTIFIER",
"parameter_list : parameter_list ',' IDENTIFIER",
"parameter_list :",
"statement : assignment_statement ';'",
"statement : call_statement ';'",
"statement : return_statement ';'",
"statement : print_statement ';'",
"statement : null_statement ';'",
"statement : if_statement",
"statement : while_statement",
"statement : for_statement",
"statement : block",
"statement : break_statement ';'",
"statement : error",
"break_statement : BREAK",
"block : '{' declaration_list statement_list '}'",
"declaration_list :",
"declaration_list : declaration_list declaration",
"statement_list : statement",
"statement_list : statement_list statement",
"assignment_statement : IDENTIFIER '=' expression",
"expression : expression '+' expression",
"expression : expression '-' expression",
"expression : expression '*' expression",
"expression : expression '/' expression",
"expression : '-' expression",
"expression : expression EQ expression",
"expression : expression NE expression",
"expression : expression LT expression",
"expression : expression LE expression",
"expression : expression GT expression",
"expression : expression GE expression",
"expression : '(' expression ')'",
"expression : INTEGER",
"expression : IDENTIFIER",
"expression : CHARACTER",
"expression : call_expression",
"expression : error",
"argument_list :",
"argument_list : expression_list",
"expression_list : expression",
"expression_list : expression_list ',' expression",
"print_statement : PRINT '(' print_list ')'",
"print_list : print_item",
"print_list : print_list ',' print_item",
"print_item : expression",
"print_item : TEXT",
"return_statement : RETURN expression",
"null_statement : CONTINUE",
"if_statement : IF '(' expression ')' block",
"if_statement : IF '(' expression ')' block ELSE block",
"while_statement : WHILE '(' expression ')' block",
"for_statement : FOR '(' assignment_statement ';' expression ';' assignment_statement ')' block",
"call_statement : IDENTIFIER '(' argument_list ')'",
"call_expression : IDENTIFIER '(' argument_list ')'",

};
#endif

#if YYDEBUG
int      yydebug;
#endif

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;
int      yynerrs;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
YYLTYPE  yyloc; /* position returned by actions */
YYLTYPE  yylloc; /* position from the lexer */
#endif

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(loc, rhs, n) \
do \
{ \
    if (n == 0) \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 0).last_line; \
        (loc).first_column = YYRHSLOC(rhs, 0).last_column; \
        (loc).last_line    = YYRHSLOC(rhs, 0).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, 0).last_column; \
    } \
    else \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 1).first_line; \
        (loc).first_column = YYRHSLOC(rhs, 1).first_column; \
        (loc).last_line    = YYRHSLOC(rhs, n).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, n).last_column; \
    } \
} while (0)
#endif /* YYLLOC_DEFAULT */
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#if YYBTYACC

#ifndef YYLVQUEUEGROWTH
#define YYLVQUEUEGROWTH 32
#endif
#endif /* YYBTYACC */

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#ifndef YYINITSTACKSIZE
#define YYINITSTACKSIZE 200
#endif

typedef struct {
    unsigned stacksize;
    YYINT    *s_base;
    YYINT    *s_mark;
    YYINT    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  *p_base;
    YYLTYPE  *p_mark;
#endif
} YYSTACKDATA;
#if YYBTYACC

struct YYParseState_s
{
    struct YYParseState_s *save;    /* Previously saved parser state */
    YYSTACKDATA            yystack; /* saved parser stack */
    int                    state;   /* saved parser state */
    int                    errflag; /* saved error recovery status */
    int                    lexeme;  /* saved index of the conflict lexeme in the lexical queue */
    YYINT                  ctry;    /* saved index in yyctable[] for this conflict */
};
typedef struct YYParseState_s YYParseState;
#endif /* YYBTYACC */
/* variables for the parser stack */
static YYSTACKDATA yystack;
#if YYBTYACC

/* Current parser state */
static YYParseState *yyps = 0;

/* yypath != NULL: do the full parse, starting at *yypath parser state. */
static YYParseState *yypath = 0;

/* Base of the lexical value queue */
static YYSTYPE *yylvals = 0;

/* Current position at lexical value queue */
static YYSTYPE *yylvp = 0;

/* End position of lexical value queue */
static YYSTYPE *yylve = 0;

/* The last allocated position at the lexical value queue */
static YYSTYPE *yylvlim = 0;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
/* Base of the lexical position queue */
static YYLTYPE *yylpsns = 0;

/* Current position at lexical position queue */
static YYLTYPE *yylpp = 0;

/* End position of lexical position queue */
static YYLTYPE *yylpe = 0;

/* The last allocated position at the lexical position queue */
static YYLTYPE *yylplim = 0;
#endif

/* Current position at lexical token queue */
static YYINT  *yylexp = 0;

static YYINT  *yylexemes = 0;
#endif /* YYBTYACC */
#line 335 "mini.y"

void yyerror(char* msg) 
{
	fprintf(stderr, "%s: line %d\n", msg, yylineno);
	exit(0);
}

int main(int argc,   char *argv[])
{

	if(argc != 2 && argc != 3)
	{
		printf("Usage: %s filename\n", argv[0]);
		exit(0);
	}
	
 

	char *input, *output;
	input = argv[1];
	if(freopen(input, "r", stdin)==NULL)
	{
		printf("error: open %s failed\n", input);
		return 0;
	}

	tac_init();

	yyparse();
	break2goto_pro();

	output=(char *)malloc(strlen(input)+ 10);
	strcpy(output,input);
	strcat(output,".t");
	if(freopen(output, "w", stdout)==NULL){
		printf("error: open %s failed\n", output);
		return 0;
	}
	tac_dump();



	stdout = fopen("/dev/tty", "w");

	
	tac_optimizer();
	/* printf(" --------------- After Optimization: ---------------\n");  */
	/* tac_dump(); */
	

	// tac_obj();
	output=(char *)malloc(strlen(input)+ 10);
	strcpy(output,input);
	strcat(output,"_opt.t");
	if(freopen(output, "w", stdout)==NULL){
		printf("error: open %s failed\n", output);
		return 0;
	}
	tac_dump();


	output=(char *)malloc(strlen(input)+ 10);
	strcpy(output,input);
	strcat(output,".s");
	if(freopen(output, "w", stdout)==NULL){
		printf("error: open %s failed\n", output);
		return 0;
	}
	tac_obj();

	return 0;
}
#line 655 "mini.y.c"

/* For use in generated program */
#define yydepth (int)(yystack.s_mark - yystack.s_base)
#if YYBTYACC
#define yytrial (yyps->save)
#endif /* YYBTYACC */

#if YYDEBUG
#include <stdio.h>	/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    YYINT *newss;
    YYSTYPE *newvs;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE *newps;
#endif

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return YYENOMEM;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (YYINT *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return YYENOMEM;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return YYENOMEM;

    data->l_base = newvs;
    data->l_mark = newvs + i;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    newps = (YYLTYPE *)realloc(data->p_base, newsize * sizeof(*newps));
    if (newps == 0)
        return YYENOMEM;

    data->p_base = newps;
    data->p_mark = newps + i;
#endif

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;

#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%sdebug: stack size increased to %d\n", YYPREFIX, newsize);
#endif
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    free(data->p_base);
#endif
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif /* YYPURE || defined(YY_NO_LEAKS) */
#if YYBTYACC

static YYParseState *
yyNewState(unsigned size)
{
    YYParseState *p = (YYParseState *) malloc(sizeof(YYParseState));
    if (p == NULL) return NULL;

    p->yystack.stacksize = size;
    if (size == 0)
    {
        p->yystack.s_base = NULL;
        p->yystack.l_base = NULL;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        p->yystack.p_base = NULL;
#endif
        return p;
    }
    p->yystack.s_base    = (YYINT *) malloc(size * sizeof(YYINT));
    if (p->yystack.s_base == NULL) return NULL;
    p->yystack.l_base    = (YYSTYPE *) malloc(size * sizeof(YYSTYPE));
    if (p->yystack.l_base == NULL) return NULL;
    memset(p->yystack.l_base, 0, size * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    p->yystack.p_base    = (YYLTYPE *) malloc(size * sizeof(YYLTYPE));
    if (p->yystack.p_base == NULL) return NULL;
    memset(p->yystack.p_base, 0, size * sizeof(YYLTYPE));
#endif

    return p;
}

static void
yyFreeState(YYParseState *p)
{
    yyfreestack(&p->yystack);
    free(p);
}
#endif /* YYBTYACC */

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab
#if YYBTYACC
#define YYVALID        do { if (yyps->save)            goto yyvalid; } while(0)
#define YYVALID_NESTED do { if (yyps->save && \
                                yyps->save->save == 0) goto yyvalid; } while(0)
#endif /* YYBTYACC */

int
YYPARSE_DECL()
{
    int yym, yyn, yystate, yyresult;
#if YYBTYACC
    int yynewerrflag;
    YYParseState *yyerrctx = NULL;
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  yyerror_loc_range[3]; /* position of error start/end (0 unused) */
#endif
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
    if (yydebug)
        fprintf(stderr, "%sdebug[<# of symbols on state stack>]\n", YYPREFIX);
#endif
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    memset(yyerror_loc_range, 0, sizeof(yyerror_loc_range));
#endif

#if YYBTYACC
    yyps = yyNewState(0); if (yyps == 0) goto yyenomem;
    yyps->save = 0;
#endif /* YYBTYACC */
    yym = 0;
    /* yyn is set below */
    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base;
#endif
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
#if YYBTYACC
        do {
        if (yylvp < yylve)
        {
            /* we're currently re-reading tokens */
            yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc = *yylpp++;
#endif
            yychar = *yylexp++;
            break;
        }
        if (yyps->save)
        {
            /* in trial mode; save scanner results for future parse attempts */
            if (yylvp == yylvlim)
            {   /* Enlarge lexical value queue */
                size_t p = (size_t) (yylvp - yylvals);
                size_t s = (size_t) (yylvlim - yylvals);

                s += YYLVQUEUEGROWTH;
                if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL) goto yyenomem;
                if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL) goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL) goto yyenomem;
#endif
                yylvp   = yylve = yylvals + p;
                yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp   = yylpe = yylpsns + p;
                yylplim = yylpsns + s;
#endif
                yylexp  = yylexemes + p;
            }
            *yylexp = (YYINT) YYLEX;
            *yylvp++ = yylval;
            yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *yylpp++ = yylloc;
            yylpe++;
#endif
            yychar = *yylexp++;
            break;
        }
        /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
        yychar = YYLEX;
#if YYBTYACC
        } while (0);
#endif /* YYBTYACC */
        if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, " <%s>", YYSTYPE_TOSTRING(yychar, yylval));
#endif
            fputc('\n', stderr);
        }
#endif
    }
#if YYBTYACC

    /* Do we have a conflict? */
    if (((yyn = yycindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
        yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        YYINT ctry;

        if (yypath)
        {
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: CONFLICT in state %d: following successful trial parse\n",
                                YYDEBUGSTR, yydepth, yystate);
#endif
            /* Switch to the next conflict context */
            save = yypath;
            yypath = save->save;
            save->save = NULL;
            ctry = save->ctry;
            if (save->state != yystate) YYABORT;
            yyFreeState(save);

        }
        else
        {

            /* Unresolved conflict - start/continue trial parse */
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
            {
                fprintf(stderr, "%s[%d]: CONFLICT in state %d. ", YYDEBUGSTR, yydepth, yystate);
                if (yyps->save)
                    fputs("ALREADY in conflict, continuing trial parse.\n", stderr);
                else
                    fputs("Starting trial parse.\n", stderr);
            }
#endif
            save                  = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (save == NULL) goto yyenomem;
            save->save            = yyps->save;
            save->state           = yystate;
            save->errflag         = yyerrflag;
            save->yystack.s_mark  = save->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (save->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            save->yystack.l_mark  = save->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (save->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            save->yystack.p_mark  = save->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (save->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            ctry                  = yytable[yyn];
            if (yyctable[ctry] == -1)
            {
#if YYDEBUG
                if (yydebug && yychar >= YYEOF)
                    fprintf(stderr, "%s[%d]: backtracking 1 token\n", YYDEBUGSTR, yydepth);
#endif
                ctry++;
            }
            save->ctry = ctry;
            if (yyps->save == NULL)
            {
                /* If this is a first conflict in the stack, start saving lexemes */
                if (!yylexemes)
                {
                    yylexemes = (YYINT *) malloc((YYLVQUEUEGROWTH) * sizeof(YYINT));
                    if (yylexemes == NULL) goto yyenomem;
                    yylvals   = (YYSTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYSTYPE));
                    if (yylvals == NULL) goto yyenomem;
                    yylvlim   = yylvals + YYLVQUEUEGROWTH;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpsns   = (YYLTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYLTYPE));
                    if (yylpsns == NULL) goto yyenomem;
                    yylplim   = yylpsns + YYLVQUEUEGROWTH;
#endif
                }
                if (yylvp == yylve)
                {
                    yylvp  = yylve = yylvals;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp  = yylpe = yylpsns;
#endif
                    yylexp = yylexemes;
                    if (yychar >= YYEOF)
                    {
                        *yylve++ = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                        *yylpe++ = yylloc;
#endif
                        *yylexp  = (YYINT) yychar;
                        yychar   = YYEMPTY;
                    }
                }
            }
            if (yychar >= YYEOF)
            {
                yylvp--;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp--;
#endif
                yylexp--;
                yychar = YYEMPTY;
            }
            save->lexeme = (int) (yylvp - yylvals);
            yyps->save   = save;
        }
        if (yytable[yyn] == ctry)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                                YYDEBUGSTR, yydepth, yystate, yyctable[ctry]);
#endif
            if (yychar < 0)
            {
                yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp++;
#endif
                yylexp++;
            }
            if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
                goto yyoverflow;
            yystate = yyctable[ctry];
            *++yystack.s_mark = (YYINT) yystate;
            *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *++yystack.p_mark = yylloc;
#endif
            yychar  = YYEMPTY;
            if (yyerrflag > 0) --yyerrflag;
            goto yyloop;
        }
        else
        {
            yyn = yyctable[ctry];
            goto yyreduce;
        }
    } /* End of code dealing with conflicts */
#endif /* YYBTYACC */
    if (((yyn = yysindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                            YYDEBUGSTR, yydepth, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yylloc;
#endif
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if (((yyn = yyrindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag != 0) goto yyinrecovery;
#if YYBTYACC

    yynewerrflag = 1;
    goto yyerrhandler;
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */

yyerrlab:
    /* explicit YYERROR from an action -- pop the rhs of the rule reduced
     * before looking for error recovery */
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif

    yynewerrflag = 0;
yyerrhandler:
    while (yyps->save)
    {
        int ctry;
        YYParseState *save = yyps->save;
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: ERROR in state %d, CONFLICT BACKTRACKING to state %d, %d tokens\n",
                            YYDEBUGSTR, yydepth, yystate, yyps->save->state,
                    (int)(yylvp - yylvals - yyps->save->lexeme));
#endif
        /* Memorize most forward-looking error state in case it's really an error. */
        if (yyerrctx == NULL || yyerrctx->lexeme < yylvp - yylvals)
        {
            /* Free old saved error context state */
            if (yyerrctx) yyFreeState(yyerrctx);
            /* Create and fill out new saved error context state */
            yyerrctx                 = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (yyerrctx == NULL) goto yyenomem;
            yyerrctx->save           = yyps->save;
            yyerrctx->state          = yystate;
            yyerrctx->errflag        = yyerrflag;
            yyerrctx->yystack.s_mark = yyerrctx->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (yyerrctx->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yyerrctx->yystack.l_mark = yyerrctx->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (yyerrctx->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yyerrctx->yystack.p_mark = yyerrctx->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (yyerrctx->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yyerrctx->lexeme         = (int) (yylvp - yylvals);
        }
        yylvp          = yylvals   + save->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yylpp          = yylpsns   + save->lexeme;
#endif
        yylexp         = yylexemes + save->lexeme;
        yychar         = YYEMPTY;
        yystack.s_mark = yystack.s_base + (save->yystack.s_mark - save->yystack.s_base);
        memcpy (yystack.s_base, save->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
        yystack.l_mark = yystack.l_base + (save->yystack.l_mark - save->yystack.l_base);
        memcpy (yystack.l_base, save->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yystack.p_mark = yystack.p_base + (save->yystack.p_mark - save->yystack.p_base);
        memcpy (yystack.p_base, save->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
        ctry           = ++save->ctry;
        yystate        = save->state;
        /* We tried shift, try reduce now */
        if ((yyn = yyctable[ctry]) >= 0) goto yyreduce;
        yyps->save     = save->save;
        save->save     = NULL;
        yyFreeState(save);

        /* Nothing left on the stack -- error */
        if (!yyps->save)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%sdebug[%d,trial]: trial parse FAILED, entering ERROR mode\n",
                                YYPREFIX, yydepth);
#endif
            /* Restore state as it was in the most forward-advanced error */
            yylvp          = yylvals   + yyerrctx->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylpp          = yylpsns   + yyerrctx->lexeme;
#endif
            yylexp         = yylexemes + yyerrctx->lexeme;
            yychar         = yylexp[-1];
            yylval         = yylvp[-1];
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc         = yylpp[-1];
#endif
            yystack.s_mark = yystack.s_base + (yyerrctx->yystack.s_mark - yyerrctx->yystack.s_base);
            memcpy (yystack.s_base, yyerrctx->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yystack.l_mark = yystack.l_base + (yyerrctx->yystack.l_mark - yyerrctx->yystack.l_base);
            memcpy (yystack.l_base, yyerrctx->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yystack.p_mark = yystack.p_base + (yyerrctx->yystack.p_mark - yyerrctx->yystack.p_base);
            memcpy (yystack.p_base, yyerrctx->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yystate        = yyerrctx->state;
            yyFreeState(yyerrctx);
            yyerrctx       = NULL;
        }
        yynewerrflag = 1;
    }
    if (yynewerrflag == 0) goto yyinrecovery;
#endif /* YYBTYACC */

    YYERROR_CALL("syntax error");
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yyerror_loc_range[1] = yylloc; /* lookahead position is error start position */
#endif

#if !YYBTYACC
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */
yyerrlab:
#endif
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if (((yyn = yysindex[*yystack.s_mark]) != 0) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: state %d, error recovery shifting to state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* lookahead position is error end position */
                yyerror_loc_range[2] = yylloc;
                YYLLOC_DEFAULT(yyloc, yyerror_loc_range, 2); /* position of error span */
                *++yystack.p_mark = yyloc;
#endif
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: error recovery discarding state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* the current TOS position is the error start position */
                yyerror_loc_range[1] = *yystack.p_mark;
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
                if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark, yystack.p_mark);
#else
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
                --yystack.s_mark;
                --yystack.l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                --yystack.p_mark;
#endif
            }
        }
    }
    else
    {
        if (yychar == YYEOF) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, error recovery discarding token %d (%s)\n",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
        }
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval, &yylloc);
#else
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
    yym = yylen[yyn];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: state %d, reducing by rule %d (%s)",
                        YYDEBUGSTR, yydepth, yystate, yyn, yyrule[yyn]);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            if (yym > 0)
            {
                int i;
                fputc('<', stderr);
                for (i = yym; i > 0; i--)
                {
                    if (i != yym) fputs(", ", stderr);
                    fputs(YYSTYPE_TOSTRING(yystos[yystack.s_mark[1-i]],
                                           yystack.l_mark[1-i]), stderr);
                }
                fputc('>', stderr);
            }
#endif
        fputc('\n', stderr);
    }
#endif
    if (yym > 0)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)

    /* Perform position reduction */
    memset(&yyloc, 0, sizeof(yyloc));
#if YYBTYACC
    if (!yytrial)
#endif /* YYBTYACC */
    {
        YYLLOC_DEFAULT(yyloc, &yystack.p_mark[-yym], yym);
        /* just in case YYERROR is invoked within the action, save
           the start of the rhs as the error start position */
        yyerror_loc_range[1] = yystack.p_mark[1-yym];
    }
#endif

    switch (yyn)
    {
case 1:
#line 43 "mini.y"
	{
	tac_last=yystack.l_mark[0].tac;
	tac_complete();
}
#line 1331 "mini.y.c"
break;
case 3:
#line 51 "mini.y"
	{
	yyval.tac=join_tac(yystack.l_mark[-1].tac, yystack.l_mark[0].tac);
}
#line 1338 "mini.y.c"
break;
case 6:
#line 61 "mini.y"
	{
	set_varlist_type(yystack.l_mark[-2].var_type,yystack.l_mark[-1].tac);
	yyval.tac=yystack.l_mark[-1].tac;
}
#line 1346 "mini.y.c"
break;
case 7:
#line 67 "mini.y"
	{ 
	yyval.var_type=get_var_type(yystack.l_mark[0].var_type);
}
#line 1353 "mini.y.c"
break;
case 8:
#line 70 "mini.y"
	{  
	yyval.var_type=get_var_type(yystack.l_mark[0].var_type); 
}
#line 1360 "mini.y.c"
break;
case 9:
#line 76 "mini.y"
	{
	yyval.tac=declare_var(yystack.l_mark[0].string);
}
#line 1367 "mini.y.c"
break;
case 10:
#line 80 "mini.y"
	{
	yyval.tac=join_tac(yystack.l_mark[-2].tac, declare_var(yystack.l_mark[0].string));
}
#line 1374 "mini.y.c"
break;
case 11:
#line 86 "mini.y"
	{
	yyval.tac=do_func(yystack.l_mark[-4].sym, yystack.l_mark[-2].tac, yystack.l_mark[0].tac);
	scope_local=0; /* Leave local scope. */
	sym_tab_local=NULL; /* Clear local symbol table. */
}
#line 1383 "mini.y.c"
break;
case 12:
#line 92 "mini.y"
	{
	error("Bad function syntax");
	yyval.tac=NULL;
}
#line 1391 "mini.y.c"
break;
case 13:
#line 99 "mini.y"
	{
	yyval.sym=declare_func(yystack.l_mark[0].string);
	scope_local=1; /* Enter local scope. */
	sym_tab_local=NULL; /* Init local symbol table. */
}
#line 1400 "mini.y.c"
break;
case 14:
#line 107 "mini.y"
	{
	yyval.tac=declare_para(yystack.l_mark[0].string);
}
#line 1407 "mini.y.c"
break;
case 15:
#line 111 "mini.y"
	{
	yyval.tac=join_tac(yystack.l_mark[-2].tac, declare_para(yystack.l_mark[0].string));
}
#line 1414 "mini.y.c"
break;
case 16:
#line 115 "mini.y"
	{
	yyval.tac=NULL;
}
#line 1421 "mini.y.c"
break;
case 27:
#line 131 "mini.y"
	{
	error("Bad statement syntax");
	yyval.tac=NULL;
}
#line 1429 "mini.y.c"
break;
case 28:
#line 138 "mini.y"
	{
	yyval.tac = do_break();
}
#line 1436 "mini.y.c"
break;
case 29:
#line 144 "mini.y"
	{
	yyval.tac=join_tac(yystack.l_mark[-2].tac, yystack.l_mark[-1].tac);
}
#line 1443 "mini.y.c"
break;
case 30:
#line 150 "mini.y"
	{
	yyval.tac=NULL;
}
#line 1450 "mini.y.c"
break;
case 31:
#line 154 "mini.y"
	{
	yyval.tac=join_tac(yystack.l_mark[-1].tac, yystack.l_mark[0].tac);
}
#line 1457 "mini.y.c"
break;
case 33:
#line 161 "mini.y"
	{
	yyval.tac=join_tac(yystack.l_mark[-1].tac, yystack.l_mark[0].tac);
}
#line 1464 "mini.y.c"
break;
case 34:
#line 167 "mini.y"
	{
	yyval.tac=do_assign(get_var(yystack.l_mark[-2].string), yystack.l_mark[0].exp);
}
#line 1471 "mini.y.c"
break;
case 35:
#line 173 "mini.y"
	{
	yyval.exp=do_bin(TAC_ADD, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);
}
#line 1478 "mini.y.c"
break;
case 36:
#line 177 "mini.y"
	{
	yyval.exp=do_bin(TAC_SUB, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);
}
#line 1485 "mini.y.c"
break;
case 37:
#line 181 "mini.y"
	{
	yyval.exp=do_bin(TAC_MUL, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);
}
#line 1492 "mini.y.c"
break;
case 38:
#line 185 "mini.y"
	{
	yyval.exp=do_bin(TAC_DIV, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);
}
#line 1499 "mini.y.c"
break;
case 39:
#line 189 "mini.y"
	{
	yyval.exp=do_un(TAC_NEG, yystack.l_mark[0].exp);
}
#line 1506 "mini.y.c"
break;
case 40:
#line 193 "mini.y"
	{
	yyval.exp=do_cmp(TAC_EQ, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);
}
#line 1513 "mini.y.c"
break;
case 41:
#line 197 "mini.y"
	{
	yyval.exp=do_cmp(TAC_NE, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);
}
#line 1520 "mini.y.c"
break;
case 42:
#line 201 "mini.y"
	{
	yyval.exp=do_cmp(TAC_LT, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);
}
#line 1527 "mini.y.c"
break;
case 43:
#line 205 "mini.y"
	{
	yyval.exp=do_cmp(TAC_LE, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);
}
#line 1534 "mini.y.c"
break;
case 44:
#line 209 "mini.y"
	{
	yyval.exp=do_cmp(TAC_GT, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);
}
#line 1541 "mini.y.c"
break;
case 45:
#line 213 "mini.y"
	{
	yyval.exp=do_cmp(TAC_GE, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);
}
#line 1548 "mini.y.c"
break;
case 46:
#line 217 "mini.y"
	{
	yyval.exp=yystack.l_mark[-1].exp;
}
#line 1555 "mini.y.c"
break;
case 47:
#line 221 "mini.y"
	{
	yyval.exp=mk_exp(NULL, mk_const(atoi(yystack.l_mark[0].string)), NULL);
}
#line 1562 "mini.y.c"
break;
case 48:
#line 225 "mini.y"
	{
	yyval.exp=mk_exp(NULL, get_var(yystack.l_mark[0].string), NULL);
}
#line 1569 "mini.y.c"
break;
case 49:
#line 229 "mini.y"
	{
	yyval.exp=mk_exp(NULL, mk_char(yystack.l_mark[0].character), NULL);
}
#line 1576 "mini.y.c"
break;
case 50:
#line 233 "mini.y"
	{
	yyval.exp=yystack.l_mark[0].exp;
}
#line 1583 "mini.y.c"
break;
case 51:
#line 237 "mini.y"
	{
	error("Bad expression syntax");
	yyval.exp=mk_exp(NULL, NULL, NULL);
}
#line 1591 "mini.y.c"
break;
case 52:
#line 244 "mini.y"
	{
	yyval.exp=NULL;
}
#line 1598 "mini.y.c"
break;
case 55:
#line 252 "mini.y"
	{
	/*上面的expression_list : expression没有语义子程序，是因为规约产生expression时，yyval会赋值该expression的值
	而这两个规约是紧靠进行的，当上面expression_list : expression再次规约时，yyval值无需改变，
	因此无需语义子程序。后需的yacc代码会把yyval旧值压栈yyvsa中。
	argument_list : expression_list此处没有语义子程序是相同道理。expression_list规约时对yycal赋值，后续无需语义子程序改值*/
	yystack.l_mark[0].exp->next=yystack.l_mark[-2].exp;
	yyval.exp=yystack.l_mark[0].exp;
}
#line 1610 "mini.y.c"
break;
case 56:
#line 263 "mini.y"
	{
	yyval.tac=yystack.l_mark[-1].tac;
}
#line 1617 "mini.y.c"
break;
case 58:
#line 270 "mini.y"
	{
	yyval.tac=join_tac(yystack.l_mark[-2].tac, yystack.l_mark[0].tac);
}
#line 1624 "mini.y.c"
break;
case 59:
#line 276 "mini.y"
	{
	yyval.tac=join_tac(yystack.l_mark[0].exp->tac,
	do_lib("PRINTN", yystack.l_mark[0].exp->ret));
}
#line 1632 "mini.y.c"
break;
case 60:
#line 281 "mini.y"
	{
	yyval.tac=do_lib("PRINTS", mk_text(yystack.l_mark[0].string));
}
#line 1639 "mini.y.c"
break;
case 61:
#line 287 "mini.y"
	{
	TAC *t=mk_tac(TAC_RETURN, yystack.l_mark[0].exp->ret, NULL, NULL);
	t->prev=yystack.l_mark[0].exp->tac;
	yyval.tac=t;
}
#line 1648 "mini.y.c"
break;
case 62:
#line 295 "mini.y"
	{
	yyval.tac=NULL;
}
#line 1655 "mini.y.c"
break;
case 63:
#line 301 "mini.y"
	{
	yyval.tac=do_if(yystack.l_mark[-2].exp, yystack.l_mark[0].tac);
}
#line 1662 "mini.y.c"
break;
case 64:
#line 305 "mini.y"
	{
	yyval.tac=do_test(yystack.l_mark[-4].exp, yystack.l_mark[-2].tac, yystack.l_mark[0].tac);
}
#line 1669 "mini.y.c"
break;
case 65:
#line 311 "mini.y"
	{
	yyval.tac=do_while_pro(yystack.l_mark[-2].exp, yystack.l_mark[0].tac);
}
#line 1676 "mini.y.c"
break;
case 66:
#line 317 "mini.y"
	{
	yyval.tac=do_for_pro(yystack.l_mark[-6].tac,yystack.l_mark[-4].exp,yystack.l_mark[-2].tac,yystack.l_mark[0].tac);
}
#line 1683 "mini.y.c"
break;
case 67:
#line 323 "mini.y"
	{
	yyval.tac=do_call(yystack.l_mark[-3].string, yystack.l_mark[-1].exp);
}
#line 1690 "mini.y.c"
break;
case 68:
#line 329 "mini.y"
	{
	yyval.exp=do_call_ret(yystack.l_mark[-3].string, yystack.l_mark[-1].exp);
}
#line 1697 "mini.y.c"
break;
#line 1699 "mini.y.c"
    default:
        break;
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
        {
            fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[YYFINAL], yyval));
#endif
            fprintf(stderr, "shifting from state 0 to final state %d\n", YYFINAL);
        }
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yyloc;
#endif
        if (yychar < 0)
        {
#if YYBTYACC
            do {
            if (yylvp < yylve)
            {
                /* we're currently re-reading tokens */
                yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylloc = *yylpp++;
#endif
                yychar = *yylexp++;
                break;
            }
            if (yyps->save)
            {
                /* in trial mode; save scanner results for future parse attempts */
                if (yylvp == yylvlim)
                {   /* Enlarge lexical value queue */
                    size_t p = (size_t) (yylvp - yylvals);
                    size_t s = (size_t) (yylvlim - yylvals);

                    s += YYLVQUEUEGROWTH;
                    if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL)
                        goto yyenomem;
                    if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL)
                        goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL)
                        goto yyenomem;
#endif
                    yylvp   = yylve = yylvals + p;
                    yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp   = yylpe = yylpsns + p;
                    yylplim = yylpsns + s;
#endif
                    yylexp  = yylexemes + p;
                }
                *yylexp = (YYINT) YYLEX;
                *yylvp++ = yylval;
                yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                *yylpp++ = yylloc;
                yylpe++;
#endif
                yychar = *yylexp++;
                break;
            }
            /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
            yychar = YYLEX;
#if YYBTYACC
            } while (0);
#endif /* YYBTYACC */
            if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
            if (yydebug)
            {
                if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
                fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)\n",
                                YYDEBUGSTR, yydepth, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == YYEOF) goto yyaccept;
        goto yyloop;
    }
    if (((yyn = yygindex[yym]) != 0) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[yystate], yyval));
#endif
        fprintf(stderr, "shifting from state %d to state %d\n", *yystack.s_mark, yystate);
    }
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    *++yystack.s_mark = (YYINT) yystate;
    *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    *++yystack.p_mark = yyloc;
#endif
    goto yyloop;
#if YYBTYACC

    /* Reduction declares that this path is valid. Set yypath and do a full parse */
yyvalid:
    if (yypath) YYABORT;
    while (yyps->save)
    {
        YYParseState *save = yyps->save;
        yyps->save = save->save;
        save->save = yypath;
        yypath = save;
    }
#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%s[%d]: state %d, CONFLICT trial successful, backtracking to state %d, %d tokens\n",
                        YYDEBUGSTR, yydepth, yystate, yypath->state, (int)(yylvp - yylvals - yypath->lexeme));
#endif
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    yylvp          = yylvals + yypath->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yylpp          = yylpsns + yypath->lexeme;
#endif
    yylexp         = yylexemes + yypath->lexeme;
    yychar         = YYEMPTY;
    yystack.s_mark = yystack.s_base + (yypath->yystack.s_mark - yypath->yystack.s_base);
    memcpy (yystack.s_base, yypath->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
    yystack.l_mark = yystack.l_base + (yypath->yystack.l_mark - yypath->yystack.l_base);
    memcpy (yystack.l_base, yypath->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base + (yypath->yystack.p_mark - yypath->yystack.p_base);
    memcpy (yystack.p_base, yypath->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
    yystate        = yypath->state;
    goto yyloop;
#endif /* YYBTYACC */

yyoverflow:
    YYERROR_CALL("yacc stack overflow");
#if YYBTYACC
    goto yyabort_nomem;
yyenomem:
    YYERROR_CALL("memory exhausted");
yyabort_nomem:
#endif /* YYBTYACC */
    yyresult = 2;
    goto yyreturn;

yyabort:
    yyresult = 1;
    goto yyreturn;

yyaccept:
#if YYBTYACC
    if (yyps->save) goto yyvalid;
#endif /* YYBTYACC */
    yyresult = 0;

yyreturn:
#if defined(YYDESTRUCT_CALL)
    if (yychar != YYEOF && yychar != YYEMPTY)
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval, &yylloc);
#else
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */

    {
        YYSTYPE *pv;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYLTYPE *pp;

        for (pv = yystack.l_base, pp = yystack.p_base; pv <= yystack.l_mark; ++pv, ++pp)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv, pp);
#else
        for (pv = yystack.l_base; pv <= yystack.l_mark; ++pv)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
    }
#endif /* defined(YYDESTRUCT_CALL) */

#if YYBTYACC
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    while (yyps)
    {
        YYParseState *save = yyps;
        yyps = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
    while (yypath)
    {
        YYParseState *save = yypath;
        yypath = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
#endif /* YYBTYACC */
    yyfreestack(&yystack);
    return (yyresult);
}
