#ifndef OPTH
#define OPTH

#include "tac.h"
#include "obj.h"
#include "DFA.h"
#include <stdio.h>

#define MAX_tac 1000
#define MAX_BB 1000
#define MAX_var 100

typedef struct Adjacency_list_node{
    // n is the successor BB's id  
    int n;
    int isjump_target;  //0 or 1 
    struct Adjacency_list_node* next;
}A_node;

typedef struct basic_block{
    // in and out TAC pointer of a BB
    TAC *in;
    TAC *out;
    // ID start from 0
    int id;
    // use Adjacency List to store the CFG, this is the pointer to successors
    A_node * suc; //后继
    // prev BBs
    A_node * prev;//前驱


    //live sign
    int live;    /*用于数据流分析 ，初始均为1.    未知功能*/
}BB;

/* 归纳变量列表 */
typedef struct induction_var_list{
	//y=k*x+con 
	char* y;	//左值
	char* x;	//基础归纳变量
	int k;	//乘积常量
	int con; //式子的第二个常量
	int change;	//每次循环的变化量
	//int first;  //该基础归纳变量第一次循环后的值，初值=first-change
	//用于确定基础归纳变量的作用域
	TAC *begin_tac;//往前一条是var t
	TAC *end_tac;//只有基础归纳变量里用了这个，所以也只有基础归纳变量处理了这个
	//相同x向下连接，不同x向后连接
	struct induction_var_list * next;//头接法，顺序不重要
	struct induction_var_list * prev;//便于删除
	struct induction_var_list * samex_next;//尾接法，要注意前后顺序
}iv_list;

void print_tac(TAC *tac, char *str , int flag);
void print_BB(BB * bb);
void print_cfg();
void print_iv(iv_list *iv_cur);
void print_ivlist(iv_list *begin);
void print_alltac();
void print_alltac_fromprev();
int tac_optimizer();

void BB_dump();

#endif