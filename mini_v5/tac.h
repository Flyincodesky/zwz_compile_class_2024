#ifndef TACH
#define TACH

#include <ctype.h>
#include <stdio.h>

/* type of symbol */
#define SYM_UNDEF 0
#define SYM_VAR_INT 1  /* variable int */
#define SYM_FUNC 2
#define SYM_TEXT 3 
#define SYM_INT 4   /* const int */
#define SYM_LABEL 5

#define SYM_VAR_CHAR 6  /* variable char */
#define SYM_CHAR 7     /* const char */

/* type of tac */ 
#define TAC_UNDEF 0 /* undefine */
#define TAC_ADD 1 /* a=b+c */
#define TAC_SUB 2 /* a=b-c */
#define TAC_MUL 3 /* a=b*c */
#define TAC_DIV 4 /* a=b/c */
#define TAC_EQ 5 /* a=(b==c) */
#define TAC_NE 6 /* a=(b!=c) */
#define TAC_LT 7 /* a=(b<c) */
#define TAC_LE 8 /* a=(b<=c) */
#define TAC_GT 9 /* a=(b>c) */
#define TAC_GE 10 /* a=(b>=c) */
#define TAC_NEG 11 /* a=-b */
#define TAC_COPY 12 /* a=b */

#define TAC_GOTO 13 /* goto a */
#define TAC_IFZ 14 /* ifz b goto a */
#define TAC_IF  23 /*if b goto a*/

#define TAC_BEGINFUNC 15 /* function begin */
#define TAC_ENDFUNC 16 /* function end */

#define TAC_LABEL 17 /* label a */
#define TAC_VAR 18 /* int a */
#define TAC_FORMAL 19 /* formal a */
#define TAC_ACTUAL 20 /* actual a */

#define TAC_CALL 21 /* a=call b */
#define TAC_RETURN 22 /* return a */
#define TAC_BREAK 40 /* break

/* struct */
typedef struct sym
{
	/*	
		****type:SYM_VAR_INT name:abc value:98 offset:-1
		****type:SYM_VAR_INT name:bcd value:99 offset:4
		type:SYM_LABEL name:L1/max			
		type:SYM_INT value:1			
		****type:SYM_FUNC name:max address:1234		
		****type:SYM_TEXT name:"hello" lable:10
		new: 20240425
		type:SYM_VAR_CHAR name:abc value:'a' offset:-1
		type:SYM_VAR_CHAR name:bcd value:'b' offset:4
		****type:SYM_CHAR value: 97(ASCALL,'a')  			
	*/
	int type;
	int store; /* 0:global, 1:local */
	char *name;
	int offset;
	int value;
	int label;
	struct tac *address; /* SYM_FUNC */	
	struct sym *next;
} SYM;

typedef struct tac /* TAC instruction node */
{
	struct tac  *next; /* Next instruction */
	struct tac  *prev; /* Previous instruction */
	int op; /* TAC instruction */
	SYM *a;
	SYM *b;
	SYM *c;

	/* ---------------- For Optimizer Phase ---------------- */
	int id; 
	// For Data-Flow-Analysis, we need to know the OUT of each TAC sometimes.
	__int8_t * OUT_vector;
	// __int8_t * IN_vector;
	// This is used ONLY for Constant Propagation Data-flow-analysis!
	int * Val_Status; //状态值，常量传播中是那个def、undef、nac
} TAC;

typedef struct exp /* Parser expression */
{
	struct exp *next; /* For argument lists */
	TAC *tac; /* The code */
	SYM *ret; /* Where the result is */
} EXP;

/*用于局部符号表表头存储*/
typedef struct func_info{
	char *func_name;
	SYM * symtab_local;
	TAC * tac_first;
	struct func_info * next;
} FUNCINFO ;

/* label for loop */
typedef struct loop_record{
	int flag;  /*1：while   2：for     区分for和while，因为其结构有区别*/
	TAC *begin_label;
	TAC *end_label;

	struct loop_record * next;
}loop_record;

// typedef struct loop_label_record{
// 	TAC *loop_label;
// 	struct loop_label_record * prev;
// 	struct loop_label_record * next;
// }loop_label_record;


/* global var */
extern int yylineno, scope_local, next_tmp, next_label;
extern SYM *sym_tab_global, *sym_tab_local;
extern TAC *tac_first, *tac_last;
extern FUNCINFO * func_list;
extern loop_record *  loop_list;
/* function */
//
// void in_loop_begin();
// void in_loop_end();
// int get_in_loop();
// void add_loop_label(TAC* label);
// TAC *get_cur_loop_label();
// void back_loop_label();
// break /*2024/06/14*/
void tac_init();
void tac_complete();
void tac_dump();
void tac_print(TAC *i);
void tac_file_print(TAC *i, FILE* file);
void tac_sprintf(TAC *i ,char * tac_code);
int  get_var_type(int type);        /*2024/04/25*/
void set_varlist_type(int type, TAC * var_list_last );    /*2024/04/25*/
SYM *mk_var(char *name);/*2024/05/13*/
SYM *mk_var2(char *name);/*2024/05/13*/
SYM *mk_tmp(void);/*2024/05/13*/
SYM *mk_const(int n);
SYM *mk_char(char a);   /*2024/04/25*/
SYM *mk_text(char *text);
TAC *mk_tac(int op, SYM *a, SYM *b, SYM *c);
EXP *mk_exp(EXP *next, SYM *ret, TAC *code);
TAC *join_tac(TAC *c1, TAC *c2);
SYM *get_var(char *name); 
SYM *declare_func(char *name);
TAC *declare_var(char *name);
TAC *declare_para(char *name);
TAC *do_func(SYM *name,    TAC *args, TAC *code);
TAC *do_assign(SYM *var, EXP *exp);
TAC *do_call(char *name, EXP *arglist);
TAC *do_lib(char *name, SYM *arg);
TAC *do_if(EXP *exp, TAC *stmt);
TAC *tac_copy(TAC * x);
TAC *do_test(EXP *exp, TAC *stmt1, TAC *stmt2);
TAC *do_while(EXP *exp, TAC *stmt);
TAC *do_for(TAC *stmt1, EXP *exp ,TAC *stmt2, TAC *stmt3);       /*2024/04/26*/
TAC *do_while_pro(EXP *exp, TAC *stmt);							 /*2024/05/08*/
TAC *do_for_pro(TAC *stmt1, EXP *exp ,TAC *stmt2, TAC *stmt3);       /*2024/05/08*/
TAC *do_break(); /*2024/06/14*/
EXP *do_bin( int binop, EXP *exp1, EXP *exp2);
EXP *do_cmp( int binop, EXP *exp1, EXP *exp2);
EXP *do_un( int unop, EXP *exp);
EXP *do_call_ret(char *name, EXP *arglist);
void error(char *str);
void break2goto();
void break2goto_pro();
#endif