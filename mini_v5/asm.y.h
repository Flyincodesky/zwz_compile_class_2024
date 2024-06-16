#ifndef _yy_defines_h_
#define _yy_defines_h_

#define ADD 257
#define SUB 258
#define MUL 259
#define DIV 260
#define TST 261
#define STO 262
#define LOD 263
#define JMP 264
#define JEZ 265
#define JLZ 266
#define JGZ 267
#define DBN 268
#define DBS 269
#define OUT 270
#define NOP 271
#define END 272
#define INTEGER 273
#define REG 274
#define LABEL 275
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union YYSTYPE
{
	int number;
	char *string;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;

#endif /* _yy_defines_h_ */
