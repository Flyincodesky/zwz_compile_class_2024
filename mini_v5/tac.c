#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "tac.h"

/* global var */
int scope_local, next_tmp, next_label;
SYM *sym_tab_global, *sym_tab_local;
TAC *tac_first, *tac_last;
FUNCINFO * func_list;
loop_record *  loop_list;
//
// int in_loop;
// loop_label_record* in_loop_labels;
// // help to judge whether in loop or not, initial in_loop = 0

// void in_loop_begin(){
// 	in_loop += 1;
// }

// void in_loop_end(){
// 	in_loop -= 1;
// }

// int get_in_loop(){
// 	printf("%d\n", in_loop);
// 	return in_loop;
// }

// void add_loop_label(TAC* label){
// 	// 检查 malloc 是否成功
//     loop_label_record* in_loop_label = (loop_label_record *)malloc(sizeof(loop_label_record));
//     if (in_loop_label == NULL) {
//         fprintf(stderr, "Memory allocation failed in add_loop_label\n");
//         exit(1); // 或者适当的错误处理
//     }

//     // 初始化新节点
//     in_loop_label->loop_label = label;
//     in_loop_label->next = NULL;
//     in_loop_label->prev = NULL; // 先初始化为 NULL

//     // 如果当前有循环标签，更新指针
//     if (in_loop_labels) {
//         in_loop_label->prev = in_loop_labels;
//         in_loop_labels->next = in_loop_label;
//     }

//     // 更新链表头指针
//     in_loop_labels = in_loop_label;
// }

// TAC *get_cur_loop_label(){
// 	return in_loop_labels->loop_label;
// }

// void back_loop_label(){
// 	loop_label_record* cur_loop_label = in_loop_labels;
//     if(cur_loop_label) {
//         in_loop_labels = in_loop_labels->prev;
//         if(in_loop_labels) {
//             in_loop_labels->next = NULL;
//         }
//         free(cur_loop_label);
//     }
// }

//
void tac_init()
{
	scope_local=0;
	sym_tab_global=NULL;
	sym_tab_local=NULL;	
	func_list=NULL;
	next_tmp=0;
	next_label=1;
	//
	// in_loop=0;
	// in_loop_labels=NULL;
	// add for break
}

void tac_complete()
{
	TAC *cur=NULL; 		/* Current TAC */
	TAC *prev=tac_last; 	/* Previous TAC */

	while(prev !=NULL)
	{
		prev->next=cur;
		cur=prev;
		prev=prev->prev;
	}

	tac_first = cur;
}

/*产生式：type : INT规约时 ，从INT中提取出类型，返回type of symbol宏*/
int get_var_type(int type){
	if(type==1) return SYM_VAR_INT;
	else if (type==2) return SYM_VAR_CHAR;

}

/*产生式： declaration : type variable_list ';' 规约时，将variable_list中变量的类型设置为type*/
void set_varlist_type(int type, TAC * var_list_last){
	/*type of symbol 
#define SYM_VAR_INT 1 variable int 
#define SYM_VAR_CHAR 6  variable char  */
	TAC * p = var_list_last;
	while(p){
		p->a->type=type;
		p=p->prev;
	}
}


SYM *lookup_sym(SYM *symtab, char *name)
{
	SYM *t=symtab;

	while(t !=NULL)
	{
		if(strcmp(t->name, name)==0) break; 
		else t=t->next;
	}
	
	return t; /* NULL if not found */
}

void insert_sym(SYM **symtab, SYM *sym)
{
	sym->next=*symtab; /* Insert at head */
	*symtab=sym;
}

SYM *mk_sym(void)
{
	SYM *t;
	t=(SYM *)malloc(sizeof(SYM));
	t->type = 0;
	t->store = 0; /* 0:global, 1:local */
	t->name = NULL;
	t->offset = 0;
	t->value = 0;
	t->label = 0;
	t->address = NULL; /* SYM_FUNC */	
	t->next = NULL;
	return t;
}

SYM *mk_var(char *name)
{
	SYM *sym=NULL;

	if(scope_local)  
		sym=lookup_sym(sym_tab_local,name);
	else
		sym=lookup_sym(sym_tab_global,name);

	/* var already declared */
	if(sym!=NULL)
	{
		error("variable already declared");
		return NULL;
	}

	/* var unseen before, set up a new symbol table node, insert_sym it into the symbol table. */
	sym=mk_sym();
	sym->type=SYM_VAR_INT;
	sym->name=name; /* ysj */
	sym->offset=-1; /* Unset address */

	if(scope_local)  
		insert_sym(&sym_tab_local,sym);
	else
		insert_sym(&sym_tab_global,sym);

	return sym;
}

SYM *mk_var2(char *name)
{
	SYM *sym=NULL;

	if(scope_local)  
		sym=lookup_sym(sym_tab_local,name);
	else
		sym=lookup_sym(sym_tab_global,name);

	/* var already declared */
	if(sym!=NULL)
	{
		return sym;
		error("variable already declared");
	}

	/* var unseen before, set up a new symbol table node, insert_sym it into the symbol table. */
	sym=mk_sym();
	sym->type=SYM_VAR_INT;
	sym->name=name; /* ysj */
	sym->offset=-1; /* Unset address */

	if(scope_local)  
		insert_sym(&sym_tab_local,sym);
	else
		insert_sym(&sym_tab_global,sym);

	return sym;
}

TAC *join_tac(TAC *c1, TAC *c2)
{
	TAC *t;

	if(c1==NULL) return c2;
	if(c2==NULL) return c1;

	/* Run down c2, until we get to the beginning and then add c1 */
	t=c2;
	while(t->prev !=NULL) 
		t=t->prev;

	t->prev=c1;
	return c2;
}

TAC *declare_var(char *name)
{
	return mk_tac(TAC_VAR,mk_var(name),NULL,NULL);
}

TAC *mk_tac(int op, SYM *a, SYM *b, SYM *c)
{
	TAC *t=(TAC *)malloc(sizeof(TAC));

	t->next=NULL; /* Set these for safety */
	t->prev=NULL;
	t->op=op;
	t->a=a;
	t->b=b;
	t->c=c;

	return t;
}  

SYM *mk_label(char *name)
{
	SYM *t=mk_sym();

	t->type=SYM_LABEL;
	t->name=strdup(name);

	return t;
}  

TAC *do_func(SYM *func, TAC *args, TAC *code)
{
	TAC *tlist; /* The backpatch list */

	TAC *tlab; /* Label at start of function */
	TAC *tbegin; /* BEGINFUNC marker */
	TAC *tend; /* ENDFUNC marker */

	tlab=mk_tac(TAC_LABEL, mk_label(func->name), NULL, NULL);
	tbegin=mk_tac(TAC_BEGINFUNC, NULL, NULL, NULL);
	tend=mk_tac(TAC_ENDFUNC,   NULL, NULL, NULL);

	func->address=tlab;

	tbegin->prev=tlab;
	code=join_tac(args, code);
	tend->prev=join_tac(tbegin, code);

	/*保存局部符号表*/
	FUNCINFO * func_tmp=(FUNCINFO * )malloc(sizeof(FUNCINFO));
	func_tmp->func_name=func->name;
	func_tmp->symtab_local=sym_tab_local;
	func_tmp->tac_first=tlab;

	func_tmp->next=func_list;
	func_list=func_tmp;


	return tend;
}

SYM *mk_tmp(void)
{
	SYM *sym;
	char *name;

	name=malloc(12);
	sprintf(name, "t%d", next_tmp++); /* Set up text */
	return mk_var(name);
}

TAC *declare_para(char *name)
{
	return mk_tac(TAC_FORMAL,mk_var(name),NULL,NULL);
}

SYM *declare_func(char *name)
{
	SYM *sym=NULL;

	sym=lookup_sym(sym_tab_global,name);

	/* name used before declared */
	if(sym!=NULL)
	{
		if(sym->type==SYM_FUNC)
		{
			error("func already declared");
			return NULL;
		}

		if(sym->type !=SYM_UNDEF)
		{
			error("func name already used");
			return NULL;
		}

		return sym;
	}
	
	
	sym=mk_sym();
	sym->type=SYM_FUNC;
	sym->name=name;
	sym->address=NULL;

	insert_sym(&sym_tab_global,sym);
	return sym;
}

TAC *do_assign(SYM *var, EXP *exp)
{
	TAC *code;

	if(var->type !=SYM_VAR_INT && var->type !=SYM_VAR_CHAR) error("assignment to non-variable");


	code=mk_tac(TAC_COPY, var, exp->ret, NULL);
	code->prev=exp->tac;

	return code;
}

EXP *do_bin( int binop, EXP *exp1, EXP *exp2)
{
	TAC *temp; /* TAC code for temp symbol */
	TAC *ret; /* TAC code for result */

	if( (exp1->ret->type==SYM_INT || exp1->ret->type==SYM_CHAR) &&
	    (exp2->ret->type==SYM_INT || exp2->ret->type==SYM_CHAR) ){

		int newval; /* The result of constant folding */

		switch(binop) /* Chose the operator */
		{
			case TAC_ADD:
			newval=exp1->ret->value + exp2->ret->value;
			break;

			case TAC_SUB:
			newval=exp1->ret->value - exp2->ret->value;
			break;

			case TAC_MUL:
			newval=exp1->ret->value * exp2->ret->value;
			break;

			case TAC_DIV:
			newval=exp1->ret->value / exp2->ret->value;
			break;
		}

		exp1->ret=mk_const(newval); /* New space for result */

		return exp1; /* The new expression */
	}

	temp=mk_tac(TAC_VAR, mk_tmp(), NULL, NULL);
	temp->prev=join_tac(exp1->tac, exp2->tac);
	ret=mk_tac(binop, temp->a, exp1->ret, exp2->ret);
	ret->prev=temp;

	exp1->ret=temp->a;
	exp1->tac=ret;

	return exp1;  
}   

EXP *do_cmp( int binop, EXP *exp1, EXP *exp2)
{
	TAC *temp; /* TAC code for temp symbol */
	TAC *ret; /* TAC code for result */

	if( (exp1->ret->type==SYM_INT || exp1->ret->type==SYM_CHAR) &&
	    (exp2->ret->type==SYM_INT || exp2->ret->type==SYM_CHAR) ){

		int newval; /* The result of constant folding */

		switch(binop) /* Chose the operator */
		{
			case TAC_EQ:
			newval=(exp1->ret->value==exp2->ret->value);
			break;
			
			case TAC_NE:
			newval=(exp1->ret->value !=exp2->ret->value);
			break;
			
			case TAC_LT:
			newval=(exp1->ret->value < exp2->ret->value);
			break;
			
			case TAC_LE:
			newval=(exp1->ret->value <=exp2->ret->value);
			break;
			
			case TAC_GT:
			newval=(exp1->ret->value > exp2->ret->value);
			break;
			
			case TAC_GE:
			newval=(exp1->ret->value >=exp2->ret->value);
			break;
		}

		exp1->ret=mk_const(newval); /* New space for result */
		return exp1; /* The new expression */
	}

	temp=mk_tac(TAC_VAR, mk_tmp(), NULL, NULL);
	temp->prev=join_tac(exp1->tac, exp2->tac);
	ret=mk_tac(binop, temp->a, exp1->ret, exp2->ret);
	ret->prev=temp;

	exp1->ret=temp->a;
	exp1->tac=ret;

	return exp1;  
}   

EXP *do_un( int unop, EXP *exp) 
{
	TAC *temp; /* TAC code for temp symbol */
	TAC *ret; /* TAC code for result */

	/* Do constant folding if possible. Calculate the constant into exp */
	if(exp->ret->type==SYM_INT)
	{
		switch(unop) /* Chose the operator */
		{
			case TAC_NEG:
			exp->ret->value=- exp->ret->value;
			break;
		}

		return exp; /* The new expression */
	}

	if(exp->ret->type==SYM_CHAR || exp->ret->type==SYM_VAR_CHAR)
	{
		 //error
		error("Character type variables or constants do not allow negation operations");
		return NULL;
	}

	temp=mk_tac(TAC_VAR, mk_tmp(), NULL, NULL);
	temp->prev=exp->tac;
	ret=mk_tac(unop, temp->a, exp->ret, NULL);
	ret->prev=temp;

	exp->ret=temp->a;
	exp->tac=ret;

	return exp;   
}

TAC *do_call(char *name, EXP *arglist)
{
	EXP  *alt; /* For counting args */
	TAC *code; /* Resulting code */
	TAC *temp; /* Temporary for building code */

	code=NULL;
	for(alt=arglist; alt !=NULL; alt=alt->next) code=join_tac(code, alt->tac);

	while(arglist !=NULL) /* Generate ARG instructions */
	{
		temp=mk_tac(TAC_ACTUAL, arglist->ret, NULL, NULL);
		temp->prev=code;
		code=temp;

		alt=arglist->next;
		arglist=alt;
	};

	temp=mk_tac(TAC_CALL, NULL, (SYM *)strdup(name), NULL);
	temp->prev=code;
	code=temp;

	return code;
}

EXP *do_call_ret(char *name, EXP *arglist)
{
	EXP  *alt; /* For counting args */
	SYM *ret; /* Where function result will go */
	TAC *code; /* Resulting code */
	TAC *temp; /* Temporary for building code */

	ret=mk_tmp(); /* For the result */
	code=mk_tac(TAC_VAR, ret, NULL, NULL);

	for(alt=arglist; alt !=NULL; alt=alt->next) code=join_tac(code, alt->tac);

	while(arglist !=NULL) /* Generate ARG instructions */
	{
		temp=mk_tac(TAC_ACTUAL, arglist->ret, NULL, NULL);
		temp->prev=code;
		code=temp;

		alt=arglist->next;
		arglist=alt;
	};

	temp=mk_tac(TAC_CALL, ret, (SYM *)strdup(name), NULL);
	temp->prev=code;
	code=temp;

	return mk_exp(NULL, ret, code);
}

TAC *do_lib(char *name, SYM *arg)
{
        TAC *a=mk_tac(TAC_ACTUAL, arg, NULL, NULL);
        TAC *c=mk_tac(TAC_CALL, NULL, (SYM *)strdup(name), NULL);
        c->prev=a;
        return c;
}

char *mk_lstr(int i)
{
	char lstr[10]="L";
	sprintf(lstr,"L%d",i);
	return(strdup(lstr));	
}

TAC *do_if(EXP *exp, TAC *stmt)
{
	TAC *label=mk_tac(TAC_LABEL, mk_label(mk_lstr(next_label++)), NULL, NULL);
	TAC *code=mk_tac(TAC_IFZ, label->a, exp->ret, NULL);

	code->prev=exp->tac;
	code=join_tac(code, stmt);
	label->prev=code;

	return label;
}

TAC *tac_copy(TAC * x){
	TAC *t=(TAC *)malloc(sizeof(TAC));
	t->next=NULL; /* Set these for safety */
	t->prev=NULL;
	t->op=x->op;
	t->a=x->a;
	t->b=x->b;
	t->c=x->c;
	return t;
}

TAC *do_test(EXP *exp, TAC *stmt1, TAC *stmt2)
{
	TAC *label1=mk_tac(TAC_LABEL, mk_label(mk_lstr(next_label++)), NULL, NULL);
	TAC *label2=mk_tac(TAC_LABEL, mk_label(mk_lstr(next_label++)), NULL, NULL);
	TAC *code1=mk_tac(TAC_IFZ, label1->a, exp->ret, NULL);
	TAC *code2=mk_tac(TAC_GOTO, label2->a, NULL, NULL);

	code1->prev=exp->tac; /* Join the code */
	code1=join_tac(code1, stmt1);
	code2->prev=code1;
	label1->prev=code2;
	label1=join_tac(label1, stmt2);
	label2->prev=label1;
	
	return label2;
}

TAC *do_while(EXP *exp, TAC *stmt) 
{
	TAC *label=mk_tac(TAC_LABEL, mk_label(mk_lstr(next_label++)), NULL, NULL);
	TAC *code=mk_tac(TAC_GOTO, label->a, NULL, NULL);

	code->prev=stmt; /* Bolt on the goto */
	TAC *if_stmt = do_if(exp, code);


	loop_record * tmp=(loop_record *)malloc(sizeof(loop_record));
	tmp->flag=1;
	tmp->begin_label=label;
	tmp->end_label=if_stmt;

	tmp->next=loop_list;
	loop_list=tmp;

	return join_tac(label, if_stmt);
}

TAC *do_while_pro(EXP *exp, TAC *stmt) 
{
	TAC *label=mk_tac(TAC_LABEL, mk_label(mk_lstr(next_label++)), NULL, NULL);
	TAC *code=mk_tac(TAC_IF, label->a, exp->ret, NULL);

	code = join_tac(tac_copy(exp->tac),code);
	// printf("begin\n");
	// tac_print(code->prev);
	// printf("\nend\n");
	code =join_tac(join_tac(label,stmt),code); /* Bolt on the goto */
	TAC *if_stmt = do_if(exp, code);


	loop_record * tmp=(loop_record *)malloc(sizeof(loop_record));
	tmp->flag=1;
	tmp->begin_label=label;
	tmp->end_label=if_stmt;

	tmp->next=loop_list;
	loop_list=tmp;

	return if_stmt;
}



/*new一个L1，stmt2(for里的第三式)前驱指向block,goto L1前驱指向stmt2后，然后在label L1后链接do_if（exp,stmt+goto L1） (L2定义及goto在do_if中)，再链接在stmt1(init)*/
TAC *do_for(TAC *init, EXP *exp ,TAC *stmt2, TAC *block){
	TAC *label=mk_tac(TAC_LABEL, mk_label(mk_lstr(next_label++)), NULL, NULL); /*new一个 Lable L1 的tac码*/
	TAC *code=mk_tac(TAC_GOTO, label->a, NULL, NULL);  /*new一个 goto L1的TAC码*/

	code-> prev=join_tac(block,stmt2);
	TAC *if_stmt = do_if(exp, code);

	loop_record * tmp=(loop_record *)malloc(sizeof(loop_record));
	tmp->flag=2;
	tmp->begin_label=label;
	tmp->end_label=if_stmt;

	tmp->next=loop_list;
	loop_list=tmp;
	return join_tac(init ,join_tac(label,if_stmt)  );
}

TAC *do_for_pro(TAC *init, EXP *exp ,TAC *stmt2, TAC *block){
	TAC *label=mk_tac(TAC_LABEL, mk_label(mk_lstr(next_label++)), NULL, NULL); /*new一个 Lable L1 的tac码*/
	TAC *code=mk_tac(TAC_IF, label->a, exp->ret, NULL);  /*new一个 if exp goto L1的TAC码*/
	
	code = join_tac(tac_copy(exp->tac),code);
	code-> prev=join_tac(join_tac(label,block),stmt2);
	TAC *if_stmt = do_if(exp, code);

	loop_record * tmp=(loop_record *)malloc(sizeof(loop_record));
	tmp->flag=2;
	tmp->begin_label=label;
	tmp->end_label=if_stmt;

	tmp->next=loop_list;
	loop_list=tmp;

	return join_tac(init , if_stmt  );
}


TAC *do_break(){
	TAC *code=mk_tac(TAC_BREAK, NULL, NULL, NULL);
	// 为break添加指令
	// goto到循环外的label
	return code;
}


SYM *get_var(char *name)
{
	SYM *sym=NULL; /* Pointer to looked up symbol */

	if(scope_local) sym=lookup_sym(sym_tab_local,name);

	if(sym==NULL) sym=lookup_sym(sym_tab_global,name);

	if(sym==NULL)
	{
		error("name not declared as local/global variable");
		return NULL;
	}

	if(sym->type!=SYM_VAR_INT && sym->type!=SYM_VAR_CHAR)
	{
		error("not a variable");
		return NULL;
	}

	return sym;
} 

EXP *mk_exp(EXP *next, SYM *ret, TAC *code)
{
	EXP *exp=(EXP *)malloc(sizeof(EXP));

	exp->next=next;
	exp->ret=ret;
	exp->tac=code;

	return exp;
}

SYM *mk_text(char *text)
{
	SYM *sym=NULL; /* Pointer to looked up symbol */

	sym=lookup_sym(sym_tab_global,text);

	/* text already used */
	if(sym!=NULL)
	{
		return sym;
	}

	/* text unseen before, set up a new symbol table node, insert_sym it into the symbol table. */
	sym=mk_sym();
	sym->type=SYM_TEXT;
	sym->name=text; /* ysj */
	sym->label=next_label++; /* ysj */

	insert_sym(&sym_tab_global,sym);
	return sym;
}

SYM *mk_const(int n)
{
	SYM *c=mk_sym(); /* Create a new node */

	c->type=SYM_INT;
	c->value=n; /* ysj */
	return c;
}     

/*new sym_node -> ( Const char type )*/
SYM *mk_char(char a){
	SYM *c=mk_sym(); /* Create a new node */

	c->type=SYM_CHAR;
	c->value=a; /* zyc */
	return c;
}

char *to_str(SYM *s, char *str) 
{
	/* Check we haven't been given NULL */
	if(s==NULL)	return "NULL";

	/* Identify the type */
	switch(s->type)
	{
		case SYM_FUNC:
		case SYM_VAR_INT:
		/* Just return the name */
		return s->name; /* ysj */

		case SYM_VAR_CHAR:
		/* Just return the name */
		return s->name; /* zyc 2024/04/25 */

		case SYM_TEXT:
		/* Put the address of the text */
		sprintf(str, "L%d", s->label);
		return str;

		case SYM_INT:
		/* Convert the number to string */
		sprintf(str, "%d", s->value);
		return str;

		case SYM_CHAR:
		// Convert the char to string 
		sprintf(str, "%d", s->value);

		default:
		/* Unknown arg type */
		error("unknown TAC arg type");
		return "?";
	}
} 



void tac_print(TAC *i)
{
	char sa[12]; /* For text of TAC args */
	char sb[12];
	char sc[12];

	switch(i->op)
	{
		case TAC_UNDEF:
		printf("undef");
		break;

		case TAC_ADD:
		printf("%s = %s + %s", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_SUB:
		printf("%s = %s - %s", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_MUL:
		printf("%s = %s * %s", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_DIV:
		printf("%s = %s / %s", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_EQ:
		printf("%s = (%s == %s)", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_NE:
		printf("%s = (%s != %s)", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_LT:
		printf("%s = (%s < %s)", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_LE:
		printf("%s = (%s <= %s)", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_GT:
		printf("%s = (%s > %s)", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_GE:
		printf("%s = (%s >= %s)", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_NEG:
		printf("%s = - %s", to_str(i->a, sa), to_str(i->b, sb));
		break;

		case TAC_COPY:
		printf("%s = %s", to_str(i->a, sa), to_str(i->b, sb));
		break;

		case TAC_GOTO:
		printf("goto %s", i->a->name);
		break;

		case TAC_IFZ:
		printf("ifz %s goto %s", to_str(i->b, sb), i->a->name);
		break;

		case TAC_IF:
		printf("if %s goto %s", to_str(i->b, sb), i->a->name);
		break;

		case TAC_ACTUAL:
		printf("actual %s", to_str(i->a, sa));
		break;

		case TAC_FORMAL:
		printf("formal %s", to_str(i->a, sa));
		break;

		case TAC_CALL:
		if(i->a==NULL) printf("call %s", (char *)i->b);
		else printf("%s = call %s", to_str(i->a, sa), (char *)i->b);
		break;

		case TAC_RETURN:
		printf("return %s", to_str(i->a, sa));
		break;

		case TAC_LABEL:
		printf("label %s", i->a->name);
		break;

		case TAC_VAR:
		printf("var %s", to_str(i->a, sa));
		break;

		case TAC_BEGINFUNC:
		printf("begin");
		break;

		case TAC_ENDFUNC:
		printf("end");
		break;

		default:
		error("unknown TAC opcode");
		break;
	}

	fflush(stdout);

}

void tac_file_print(TAC *i, FILE* file)
{
	char sa[12]; /* For text of TAC args */
	char sb[12];
	char sc[12];

	switch(i->op)
	{
		case TAC_UNDEF:
		fprintf(file, "undef");
		break;

		case TAC_ADD:
		fprintf(file, "%s = %s + %s", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_SUB:
		fprintf(file, "%s = %s - %s", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_MUL:
		fprintf(file, "%s = %s * %s", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_DIV:
		fprintf(file, "%s = %s / %s", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_EQ:
		fprintf(file, "%s = (%s == %s)", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_NE:
		fprintf(file, "%s = (%s != %s)", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_LT:
		fprintf(file, "%s = (%s < %s)", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_LE:
		fprintf(file, "%s = (%s <= %s)", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_GT:
		fprintf(file, "%s = (%s > %s)", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_GE:
		fprintf(file, "%s = (%s >= %s)", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_NEG:
		fprintf(file, "%s = - %s", to_str(i->a, sa), to_str(i->b, sb));
		break;

		case TAC_COPY:
		fprintf(file, "%s = %s", to_str(i->a, sa), to_str(i->b, sb));
		break;

		case TAC_GOTO:
		fprintf(file, "goto %s", i->a->name);
		break;

		case TAC_IFZ:
		fprintf(file, "ifz %s goto %s", to_str(i->b, sb), i->a->name);
		break;

		case TAC_IF:
		fprintf(file, "if %s goto %s", to_str(i->b, sb), i->a->name);
		break;

		case TAC_ACTUAL:
		fprintf(file, "actual %s", to_str(i->a, sa));
		break;

		case TAC_FORMAL:
		fprintf(file, "formal %s", to_str(i->a, sa));
		break;

		case TAC_CALL:
		if(i->a==NULL) fprintf(file, "call %s", (char *)i->b);
		else fprintf(file, "%s = call %s", to_str(i->a, sa), (char *)i->b);
		break;

		case TAC_RETURN:
		fprintf(file, "return %s", to_str(i->a, sa));
		break;

		case TAC_LABEL:
		fprintf(file, "label %s", i->a->name);
		break;

		case TAC_VAR:
		fprintf(file, "var %s", to_str(i->a, sa));
		break;

		case TAC_BEGINFUNC:
		fprintf(file, "begin");
		break;

		case TAC_ENDFUNC:
		fprintf(file, "end");
		break;

		default:
		error("unknown TAC opcode");
		break;
	}
}


void tac_sprintf(TAC *i ,char * tac_code){
	char sa[12]; /* For text of TAC args */
	char sb[12];
	char sc[12];

	switch(i->op)
	{
		case TAC_UNDEF:
		sprintf(tac_code,"undef");
		break;

		case TAC_ADD:
		sprintf(tac_code,"%s = %s + %s", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_SUB:
		sprintf(tac_code,"%s = %s - %s", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_MUL:
		sprintf(tac_code,"%s = %s * %s", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_DIV:
		sprintf(tac_code,"%s = %s / %s", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_EQ:
		sprintf(tac_code,"%s = (%s == %s)", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_NE:
		sprintf(tac_code,"%s = (%s != %s)", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_LT:
		sprintf(tac_code,"%s = (%s < %s)", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_LE:
		sprintf(tac_code,"%s = (%s <= %s)", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_GT:
		sprintf(tac_code,"%s = (%s > %s)", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_GE:
		sprintf(tac_code,"%s = (%s >= %s)", to_str(i->a, sa), to_str(i->b, sb), to_str(i->c, sc));
		break;

		case TAC_NEG:
		sprintf(tac_code,"%s = - %s", to_str(i->a, sa), to_str(i->b, sb));
		break;

		case TAC_COPY:
		sprintf(tac_code,"%s = %s", to_str(i->a, sa), to_str(i->b, sb));
		break;

		case TAC_GOTO:
		sprintf(tac_code,"goto %s", i->a->name);
		break;

		case TAC_IFZ:
		sprintf(tac_code,"ifz %s goto %s", to_str(i->b, sb), i->a->name);
		break;

		case TAC_IF:
		sprintf(tac_code,"if %s goto %s", to_str(i->b, sb), i->a->name);
		break;

		case TAC_ACTUAL:
		sprintf(tac_code,"actual %s", to_str(i->a, sa));
		break;

		case TAC_FORMAL:
		sprintf(tac_code,"formal %s", to_str(i->a, sa));
		break;

		case TAC_CALL:
		if(i->a==NULL) sprintf(tac_code,"call %s", (char *)i->b);
		else sprintf(tac_code,"%s = call %s", to_str(i->a, sa), (char *)i->b);
		break;

		case TAC_RETURN:
		sprintf(tac_code,"return %s", to_str(i->a, sa));
		break;

		case TAC_LABEL:
		sprintf(tac_code,"label %s", i->a->name);
		break;

		case TAC_VAR:
		sprintf(tac_code,"var %s", to_str(i->a, sa));
		break;

		case TAC_BEGINFUNC:
		sprintf(tac_code,"begin");
		break;

		case TAC_ENDFUNC:
		sprintf(tac_code,"end");
		break;

		default:
		error("unknown TAC opcode");
		break;
	}

}

void tac_dump()
{
	TAC * cur;
	for(cur = tac_first; cur !=NULL; cur=cur->next)
	{
		tac_print(cur);
		printf("\n");
	}
}

void error(char *str)
{
	fprintf(stderr, "error: %s\n", str);
	exit(0);
} 

void break2goto(){
    loop_record* loop_records = loop_list;

    // Ensure the loop list is not empty
    if (!loop_records) {
        printf("Loop list is empty\n");
        return;
    }

    while (loop_records) {
        // Ensure begin_label and end_label are not null
        if (!loop_records->begin_label || !loop_records->end_label) {
            printf("Invalid loop record\n");
            loop_records = loop_records->next;
            continue;
        }

        for (TAC *tac_cur = loop_records->begin_label; tac_cur != loop_records->end_label; tac_cur = tac_cur->next) {
            if (!tac_cur) {
                printf("Invalid TAC node\n");
                break;
            }

            if (tac_cur->op == TAC_BREAK) {
                tac_cur->op = TAC_GOTO;
                tac_cur->a = loop_records->end_label->a;
                printf("break\n");
            }
        }
        loop_records = loop_records->next;
    }
    // Break check
    for (TAC *tac_cur = tac_first; tac_cur != tac_last; tac_cur = tac_cur->next) {
        if (!tac_cur) {
            printf("Invalid TAC node in main list\n");
            break;
        }

        if (tac_cur->op == TAC_BREAK) {
            printf("break\n");
            error("Break not in Loop");
        }
    }
}

void break2goto_pro(){
    loop_record* loop_records = loop_list;
	int loop_cnt = 0;
	typedef struct LoopLabel{
		TAC* loop_out_label;
		struct LoopLabel* prev;
	}LoopLabel;
	LoopLabel* looplabels = (LoopLabel*)malloc(sizeof(LoopLabel));
	looplabels->loop_out_label = NULL;
	looplabels->prev = NULL;
	for (TAC *tac_cur = tac_first; tac_cur != tac_last; tac_cur = tac_cur->next){
		for (loop_records = loop_list; loop_records != NULL; loop_records = loop_records->next){
			if (tac_cur == loop_records->begin_label){
				LoopLabel* new_looplabel = (LoopLabel*)malloc(sizeof(LoopLabel));
				new_looplabel->loop_out_label = loop_records->end_label;
				new_looplabel->prev = looplabels;
				looplabels = new_looplabel;
				loop_cnt += 1;
				break;
			}
			else if(tac_cur == loop_records->end_label){
				LoopLabel* cur_looplabel = looplabels;
				looplabels = looplabels->prev;
				free(cur_looplabel);
				loop_cnt -= 1;
				break;
			}
		}
		if (tac_cur->op == TAC_BREAK){
			if(loop_cnt <= 0){
				error("Break not in Loop");
			}else{
				tac_cur->op = TAC_GOTO;
				tac_cur->a = looplabels->loop_out_label->a;
			}
		}
	}
}
