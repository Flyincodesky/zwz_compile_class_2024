#ifndef _yy_defines_h_
#define _yy_defines_h_

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
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
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
extern YYSTYPE yylval;

#endif /* _yy_defines_h_ */
