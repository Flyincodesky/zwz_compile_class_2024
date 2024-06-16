%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tac.h"
#include "obj.h"
#include "opt.h"

int yylex();
void yyerror(char* msg);

%}

%union
{	
	int var_type ; /*non:-1 , default:0 , int:1 , char:2,  2024/04/25*/
	char character;
	char *string;
	SYM *sym;
	TAC *tac;
	EXP	*exp;
}

%token  EQ NE LT LE GT GE UMINUS IF THEN ELSE FI WHILE FOR DO DONE CONTINUE FUNC PRINT RETURN BREAK
%token <var_type> INT  CHAR
%token <string> INTEGER IDENTIFIER TEXT    

%token <character> CHARACTER

%left EQ NE LT LE GT GE
%left '+' '-'
%left '*' '/'
%right UMINUS

%type <tac> program function_declaration_list function_declaration function parameter_list variable_list statement assignment_statement print_statement print_list print_item return_statement null_statement if_statement while_statement for_statement call_statement block declaration_list declaration statement_list break_statement
%type <exp> argument_list expression_list expression call_expression
%type <sym> function_head
%type <var_type> type

%%

program : function_declaration_list
{
	tac_last=$1;
	tac_complete();
}
;

function_declaration_list : function_declaration
| function_declaration_list function_declaration
{
	$$=join_tac($1, $2);
}
;

function_declaration : function
| declaration
;

declaration : type variable_list ';'
{
	set_varlist_type($1,$2);
	$$=$2;
}
;

type : INT { 
	$$=get_var_type($1);
}
| CHAR {  
	$$=get_var_type($1); 
}
;

variable_list : IDENTIFIER
{
	$$=declare_var($1);
}               
| variable_list ',' IDENTIFIER
{
	$$=join_tac($1, declare_var($3));
}               
;

function : function_head '(' parameter_list ')' block
{
	$$=do_func($1, $3, $5);
	scope_local=0; /* Leave local scope. */
	sym_tab_local=NULL; /* Clear local symbol table. */
}
| error
{
	error("Bad function syntax");
	$$=NULL;
}
;

function_head : IDENTIFIER
{
	$$=declare_func($1);
	scope_local=1; /* Enter local scope. */
	sym_tab_local=NULL; /* Init local symbol table. */
}
;

parameter_list : IDENTIFIER
{
	$$=declare_para($1);
}               
| parameter_list ',' IDENTIFIER
{
	$$=join_tac($1, declare_para($3));
}               
|
{
	$$=NULL;
}
;

statement : assignment_statement ';'
| call_statement ';'
| return_statement ';'
| print_statement ';'
| null_statement ';'
| if_statement
| while_statement
| for_statement
| block
| break_statement ';'
| error
{
	error("Bad statement syntax");
	$$=NULL;
}
;

break_statement : BREAK
{
	$$ = do_break();
}
;

block : '{' declaration_list statement_list '}'
{
	$$=join_tac($2, $3);
}               
;

declaration_list        :
{
	$$=NULL;
}
| declaration_list declaration
{
	$$=join_tac($1, $2);
}
;

statement_list : statement
| statement_list statement
{
	$$=join_tac($1, $2);
}               
;

assignment_statement : IDENTIFIER '=' expression
{
	$$=do_assign(get_var($1), $3);
}
;

expression : expression '+' expression
{
	$$=do_bin(TAC_ADD, $1, $3);
}
| expression '-' expression
{
	$$=do_bin(TAC_SUB, $1, $3);
}
| expression '*' expression
{
	$$=do_bin(TAC_MUL, $1, $3);
}
| expression '/' expression
{
	$$=do_bin(TAC_DIV, $1, $3);
}
| '-' expression  %prec UMINUS
{
	$$=do_un(TAC_NEG, $2);
}
| expression EQ expression
{
	$$=do_cmp(TAC_EQ, $1, $3);
}
| expression NE expression
{
	$$=do_cmp(TAC_NE, $1, $3);
}
| expression LT expression
{
	$$=do_cmp(TAC_LT, $1, $3);
}
| expression LE expression
{
	$$=do_cmp(TAC_LE, $1, $3);
}
| expression GT expression
{
	$$=do_cmp(TAC_GT, $1, $3);
}
| expression GE expression
{
	$$=do_cmp(TAC_GE, $1, $3);
}
| '(' expression ')'
{
	$$=$2;
}               
| INTEGER
{
	$$=mk_exp(NULL, mk_const(atoi($1)), NULL);
}
| IDENTIFIER
{
	$$=mk_exp(NULL, get_var($1), NULL);
}
|  CHARACTER  
{
	$$=mk_exp(NULL, mk_char($1), NULL);
}
| call_expression
{
	$$=$1;
}               
| error
{
	error("Bad expression syntax");
	$$=mk_exp(NULL, NULL, NULL);
}
;

argument_list           :
{
	$$=NULL;
}
| expression_list
;

expression_list : expression
|  expression_list ',' expression
{
	/*上面的expression_list : expression没有语义子程序，是因为规约产生expression时，yyval会赋值该expression的值
	而这两个规约是紧靠进行的，当上面expression_list : expression再次规约时，yyval值无需改变，
	因此无需语义子程序。后需的yacc代码会把yyval旧值压栈yyvsa中。
	argument_list : expression_list此处没有语义子程序是相同道理。expression_list规约时对yycal赋值，后续无需语义子程序改值*/
	$3->next=$1;
	$$=$3;
}
;

print_statement : PRINT '(' print_list ')'
{
	$$=$3;
}               
;

print_list : print_item
| print_list ',' print_item
{
	$$=join_tac($1, $3);
}               
;

print_item : expression
{
	$$=join_tac($1->tac,
	do_lib("PRINTN", $1->ret));
}
| TEXT
{
	$$=do_lib("PRINTS", mk_text($1));
}
;

return_statement : RETURN expression
{
	TAC *t=mk_tac(TAC_RETURN, $2->ret, NULL, NULL);
	t->prev=$2->tac;
	$$=t;
}               
;

null_statement : CONTINUE
{
	$$=NULL;
}               
;

if_statement : IF '(' expression ')' block
{
	$$=do_if($3, $5);
}
| IF '(' expression ')' block ELSE block
{
	$$=do_test($3, $5, $7);
}
;

while_statement : WHILE '(' expression ')' block
{
	$$=do_while_pro($3, $5);
}               
;

for_statement : FOR '('  assignment_statement ';' expression ';' assignment_statement  ')'  block
{
	$$=do_for_pro($3,$5,$7,$9);
}
;

call_statement : IDENTIFIER '(' argument_list ')'
{
	$$=do_call($1, $3);
}
;

call_expression : IDENTIFIER '(' argument_list ')'
{
	$$=do_call_ret($1, $3);
}
;

%%

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
