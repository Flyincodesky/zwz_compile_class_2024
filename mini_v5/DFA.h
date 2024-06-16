#ifndef DFA
#define DFA

#include "tac.h"
#include "obj.h"
#include "opt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 100

//Constant_Propagation  用于常量传播数据流分析
#define DEF 1   //定义、常量
#define NAC 2   //非常量
#define UNDEF 0


typedef struct worklist{
    int data[MAX_SIZE];
    int front;
    int rear;
}WL;//模拟队列/  front指队列头前一个空间的位置。rear是对列尾的位置

// output of DFA: each node representing the IN and OUT state of a BB
// one node <--> one BB
typedef struct res_node
{
    // n index the BB
    int n;
    // number of elems in vector
    int count;
    // IN and OUT vector for DFA
    __int8_t * in_vector;
    __int8_t * out_vector;

    // 这只用于常量传播！ this is used ONLY for constant propagation!
    int * constant_status;   // DEF,UNDEF,NAC
    int * status_in;

}R_node;

typedef struct my_expression
{
    SYM *b;
    SYM *c;
    int op;
}My_exp;


 


WL * init_WL(void);
int is_empty(WL *wl);
int is_full(WL *wl);
int get_BB_from_WL(WL *wl);
int insert_BB_to_WL(WL *wl, int BB);

typedef struct basic_block BB;

int same_cons(SYM *a, SYM *b);
int same_var(SYM *a, SYM *b);
int find_exp(TAC *cur_tac,int exp_index,My_exp **exp_list);
void Live_Var(BB **BB_array, int BB_num,TAC * tac_first,R_node ** res,SYM **def_vars,int * res_def_index);
void Reaching_definition(BB **BB_array, int BB_num, TAC *tac_first, R_node ** res, TAC ** def_tac, int *def_tac_index, int control_bit);
int Available_expression(BB **BB_array, int BB_num, TAC *tac_first, R_node ** res, My_exp ** exp_list);
void Constant_Union_operation(__int8_t * arr1, __int8_t * arr2, int n, int * status1, int * status2);
int find_var(SYM * sym, SYM ** def_vars, int index);
void Constant_Propagation(BB **BB_array, int BB_num, TAC *tac_first, R_node ** res, SYM **def_vars, int *var_index);

#endif