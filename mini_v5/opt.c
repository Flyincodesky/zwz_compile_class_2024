#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "opt.h"
#include "DFA.h"

#define MAX_ROUND 10

struct Optimizers{
    // all optimizers
    int (* simple_opt)(TAC * tac_first);
    int (* const_opt)(BB **BB_array, int BB_num, TAC * tac_first);
    int (* copy_propagaton_opt)(BB **BB_array, int BB_num, TAC * tac_first);
    int (* deadcode_opt)(BB **BB_array, int BB_num, TAC * tac_first);
    int (* deadvar_opt)(BB **BB_array, int BB_num, TAC * tac_first);
    int (* local_comsub_opt)(BB **BB_array, int BB_num);
    int (* global_comsub_opt)(BB **BB_array, int BB_num, TAC * tac_first);
};    

BB *BB_array[MAX_BB];/*存放所有基本块的指针*/// pointer array, representing the CFG
int bb_num;
iv_list *basic_iv, *final_iv;

/*###########################构建CFG部分############################*/

/*tac id大小排序函数 a->id >= b->id时返回1*/
int cmp (const void *a, const void *b){
    return ((*(TAC **)a)->id >= (*(TAC **)b)->id);
}
/*根据tac内容信息找到对应的tac地址*/
TAC * find_tac(int type, SYM *a, SYM *b, SYM *c){
    for(TAC *cur = tac_first; cur != NULL; cur = cur->next){
        if(cur->op == type && cur->a == a && cur->b == b && cur->c == c){
            // printf("success!\n");
            return cur;
        }
    }
    for(TAC *cur = tac_first; cur != NULL; cur = cur->next){
        tac_print(cur);
        printf("\n");
    }
    printf("%d\n", type);
    printf("%s\n", a->name);
    printf("Fail to find a correct TAC!\n");
    exit(-1);
}
/*根据tac的id找到对应的基本块id  */     // args: id is the target TAC id; num is the BB nums
// 有个奇怪的情况是一个BB会有两个Lable
int find_BB(int id, int num){
    for(int i = 0; i <= num; i++){
        if(id >= BB_array[i]->in->id && id <= BB_array[i]->out->id) return i;
    }
    printf("Fail to find the target BB!\n");
    exit(-1);
}
/*1111.创建程序的cfg，返回基本块数量，BB信息在BB_array[]中存储，无需返回。*/
//该cfg没有添加Entry和exit，而且没有显式区分多个过程的cfg
int build_CFG(TAC *tac_first){
    //print_alltac();
    /*为每个tac码依次赋值id */     // Set the id for all TAC, start from 0
    int tmp = 0;
    for(TAC *cur = tac_first; cur !=NULL; cur=cur->next){
        cur->id = tmp ++;
    }
    

    /*获得每个bb的leader*/
    // get all BBs for the first, assuming you are familiar with this algorithm
    TAC *headers[MAX_tac];
    int foo[MAX_tac] ={0}; //use for deduplication消除重复数据。 某个label类型tac可能是多个跳转的目的，用以消除重复
    int cur_headers = 0;
    //第一条tac the entry TAC
    headers[cur_headers ++] = tac_first;
    foo[0] = 1;
    //转移指令的目的地址和条件转移的下一条
    for(TAC *cur = tac_first->next; cur !=NULL; cur=cur->next){
        
        if(cur->op == TAC_GOTO){
           
            // need to find the GOTO target
            TAC *tmp_tac = find_tac(TAC_LABEL, cur->a, NULL, NULL);
             
            // add into headers array
            if(foo[tmp_tac->id] != 1){
                foo[tmp_tac->id] = 1;
                headers[cur_headers ++] = tmp_tac;
            }
        }
        else if(cur->op == TAC_IFZ || cur->op == TAC_IF || cur->op == TAC_GOTO){
          

            //SYM a is the LABLE, so we do the same
            TAC *tmp_tac = find_tac(TAC_LABEL, cur->a, NULL, NULL);
            if(foo[tmp_tac->id] != 1){
                foo[tmp_tac->id] = 1;
                headers[cur_headers ++] = tmp_tac;
            }

            // also , the next TAC is the header
            if(foo[cur->next->id] != 1){
                foo[cur->next->id] = 1;
                headers[cur_headers ++] = cur->next;
            }
        }
    }
    //对基本块header tac按照其id大小排序  find all the headers, now we should sort them and build the BB
    qsort(headers, cur_headers, sizeof(BB *), cmp);
   

    /*划分基本块，并存到 BB_array[] 中*/
    for(int i = 0; i < cur_headers - 1 ; i++){
        BB* tmp = (BB *)malloc(sizeof(BB));
        tmp->in = headers[i];
        tmp->out = headers[i + 1]->prev;
        tmp->id = i;
        tmp->suc = NULL;
        tmp->live = 1;
        BB_array[i] = tmp;
    }
    //最后一个基本块特殊处理，因为其out_tac是最后一条tac,不能通过下个基本块头的上一条实现 last BB need specially processing. Note that cur_headers equals to the number of headers and the number of BBs
    BB* tmp2 = (BB *)malloc(sizeof(BB));
    BB_array[cur_headers - 1] = tmp2;
    BB_array[cur_headers - 1]->in = headers[cur_headers - 1];
    BB_array[cur_headers - 1]->out = tac_last;
    BB_array[cur_headers - 1]->id = cur_headers - 1;
    BB_array[cur_headers - 1]->suc = NULL;
    BB_array[cur_headers - 1]->live = 1;


    /*添加cfg的边信息*/
    //通过基本块最后一条tac类型，把后继关系存储    build the CFG, note that the last BB have no successors
    for(int i = 0; i < cur_headers - 1; i++){
        if(BB_array[i]->out->op == TAC_GOTO){
            TAC *tmp = find_tac(TAC_LABEL, BB_array[i]->out->a, NULL, NULL);
            int bb_tmp = find_BB(tmp->id, cur_headers - 1);

            // Find the GOTO target BB, now we can create a adjust list node
            A_node *tmp1 = (A_node *)malloc(sizeof(A_node));
            tmp1->n = bb_tmp;
            tmp1->isjump_target=1;
            tmp1->next = NULL;

            BB_array[i]->suc = tmp1;
        }
        else if(BB_array[i]->out->op == TAC_IFZ || BB_array[i]->out->op == TAC_IF || BB_array[i]->out->op == TAC_GOTO){

            // we have 2 TAC to process
            TAC *tac_tar = find_tac(TAC_LABEL, BB_array[i]->out->a, NULL, NULL);
            TAC *tac_next = BB_array[i]->out->next;
            int bb_tmp = find_BB(tac_tar->id, cur_headers - 1);

            // suc1 is the IFZ or IF target
            A_node *tmp_tar = (A_node *)malloc(sizeof(A_node));
            tmp_tar->n = bb_tmp;
            tmp_tar->isjump_target=1;
            tmp_tar->next = (A_node *)malloc(sizeof(A_node));
            // suc2 is the next BB
            tmp_tar->next->n = i + 1;
            tmp_tar->next->isjump_target=0;
            tmp_tar->next->next = NULL;

            BB_array[i]->suc = tmp_tar;//第一个A_node是跳转的目的，第二个A_node才是正常后继
        }
        else{
            // BB caused by other jmp
            A_node *tmp1 = (A_node *)malloc(sizeof(A_node));
            tmp1->n = i + 1;
            tmp1->isjump_target=0;
            tmp1->next = NULL;
            BB_array[i]->suc = tmp1;
        }
    }
    //通过后继关系，将前驱关系链接好
    for(int i = 0; i < cur_headers ; i ++){
        A_node *tmp = BB_array[i]->suc;
        if(tmp != NULL){
            //第一条后继关系弄好对应前驱 there is a BB have a prev headers[i]
            A_node * new_prev = (A_node *)malloc(sizeof(A_node));
            new_prev->n = i;
            new_prev->isjump_target=tmp->isjump_target;
            new_prev->next = NULL;

            int target_BB = tmp->n;

            // we should find the tail of the prev list.
            if(BB_array[target_BB]->prev != NULL){
                A_node *t = BB_array[target_BB]->prev;
                while(t->next != NULL) t = t->next;
                // t is the nearest A_node
                t->next = new_prev;
            }else BB_array[target_BB]->prev = new_prev;

            //第二条后继弄好对应前驱 may have 2 successors
            if(tmp->next != NULL){
                tmp = tmp->next;
                
                new_prev = (A_node *)malloc(sizeof(A_node));
                new_prev->n = i;
                new_prev->isjump_target=tmp->isjump_target;

                new_prev->next = NULL; 
                // repeat!
                target_BB = tmp->n;
                if(BB_array[target_BB]->prev != NULL){
                    A_node *t = BB_array[target_BB]->prev;
                    while(t->next != NULL) t = t->next;
                    // t is the nearest A_node
                    t->next = new_prev;
                }else BB_array[target_BB]->prev = new_prev;
            }
        }
    }

    //返回基本块的数量。基本块信息在全局变量BB_array[]存储，无需返回。      return the number of BBs, for further DFA
    return cur_headers;
}   

/*###########################优化部分##############开始############*/

/*optimize 简单优化  :  删除"x=x"的无用赋值*/   //21 lines
int simple_opt(TAC * tac_first){
    int opt_count = 0;
    // iterate throurh all the TAC and delete x = x
    for(TAC *tmp = tac_first ; tmp != NULL ; ){
        if(tmp -> op == TAC_COPY){
            if((tmp->a->type == SYM_VAR_INT && tmp->b->type == SYM_VAR_INT)
            ||  (tmp->a->type == SYM_VAR_CHAR && tmp->b->type == SYM_VAR_CHAR)     ){
                if(strcmp(tmp->a->name, tmp->b->name) == 0){
                    // we not free the memory
                    tmp->prev->next = tmp->next;
                    tmp->next->prev = tmp->prev;
                    opt_count ++;
                }
            }
        }
        
        tmp = tmp->next;
    }

    return opt_count;
}



/*optimize 常量传播+常量折叠优化*/   // 66   + 284  =  350 lines
int const_opt(BB **BB_array, int BB_num, TAC * tac_first){
    
    R_node *res[BB_num];
    SYM *vars[100];
    int vars_index = 0;

    // Constant Propagation Dataflow Analysis
    Constant_Propagation(BB_array, BB_num, tac_first, res ,vars, &vars_index);
    
    int opt_count = 0;

    for(TAC * cur = tac_first; cur != NULL; cur = cur->next){
        
        if(cur->op >= 1 && cur->op <= 12){  //该tac是运算赋值类
            if(cur->op == TAC_COPY && cur->b->type == SYM_INT) 
                continue;
            // tac_print(cur);printf(",");tac_print(cur);printf("\n");

            SYM *tmp = cur->a;
            // tac_print(cur);
            // printf("\n");
            // for(int i = 0; i < vars_index ; i++){
            //     printf("%s ", vars[i]->name);
            // }
            // printf("\n");
            
            int opt_index = find_var(tmp, vars, vars_index);

            if(cur->Val_Status[opt_index] == DEF){

                if(cur->b->type == SYM_INT){
                    // 已经优化过了，等号右侧为常数
                    continue;
                }
                // Find a TAC that can be optimized.
                cur->op = TAC_COPY;
                SYM *new_b = (SYM *)malloc(sizeof(SYM));
                new_b -> type = SYM_INT;
                new_b -> value = cur->OUT_vector[opt_index];
                cur->b = new_b;

                opt_count ++;
            }
        }
        else if(cur->op == TAC_IFZ ||cur->op == TAC_IF){

            if(cur->b->type == SYM_INT) continue;
            
            SYM *tmp = cur->b;
            // tac_dump();
            int opt_index = find_var(tmp, vars, vars_index);

            if(cur->Val_Status[opt_index] == DEF){
                // Find a IFZ that can be optimized.
                SYM *new_b = (SYM *)malloc(sizeof(SYM));
                new_b -> type = SYM_INT;
                new_b -> value = cur->OUT_vector[opt_index];
                cur->b = new_b;

                opt_count ++;
            }

        }
        /*2024/05/09 add :补充了对函数调用的优化，优化实参 */
        else if(cur->op == TAC_ACTUAL){
            // printf("actual!\n");
            if(cur->a->type==SYM_INT)continue;
            SYM * tmp=cur->a;
            int opt_index=find_var(tmp,vars,vars_index);
            
            if(cur->Val_Status[opt_index]==DEF){
                SYM *new_a = (SYM *)malloc(sizeof(SYM));
                new_a -> type = SYM_INT;
                new_a -> value = cur->OUT_vector[opt_index];
                cur->a = new_a;
                // tac_print(cur);printf("\n");

                opt_count ++;
            }
        }

        
    
   }

   
    return opt_count;
}



//  50 + 168 = 218 lines
int find_tac_in_deftac(TAC *cur, TAC ** def_tac, int index){
    for(int i = 0; i < index ; i++){
        if(cur == def_tac[i]) return i;
    }
    printf("can not find a correct TAC! \n");
    exit(-1);
}
/*optimize 全局和局部复制传播优化*/
int copy_opt(BB **BB_array, int BB_num, TAC * tac_first){
    // 全局复制传播优化，这里需要把Reaching Definition的meet改为and
    R_node *res[BB_num];
    TAC *def_tac[100];
    int def_index = 0;

    int opt_cnt = 0;

    // Reaching Definition Dataflow Analysis
    Reaching_definition(BB_array, BB_num, tac_first, res, def_tac, &def_index, 0);
    
    for(TAC *cur = tac_first; cur != NULL; cur=cur->next){
        // tac_print(cur);printf(",");tac_print(cur);printf("\n");

        // find x = y
        if(cur->op == TAC_COPY && (cur->b->type == SYM_VAR_INT||cur->b->type == SYM_VAR_CHAR)){
            //找出在def_tac中代表 当前tac码指针cur的assign 是否可达的元素下标
            int opt_index = find_tac_in_deftac(cur, def_tac, def_index);

            SYM *tmp = cur->a;
            // find all TAC that use tmp 
            for(TAC * tac_tmp = cur; tac_tmp != NULL; tac_tmp = tac_tmp->next){

                if(tac_tmp->op >= 1 && tac_tmp->op <= 12){

                    if(tac_tmp->OUT_vector[opt_index] == 1){
                        //cur的assign可达当前tac_tmp
                        // check the Reaching Definition Results
                        if(tac_tmp->b == tmp) {
                            tac_tmp->b = cur->b;
                            opt_cnt ++;
                        }
                        if(tac_tmp->c == tmp) {
                            tac_tmp->c = cur->b;
                            opt_cnt ++;
                        }
                        
                    }
                }
            }
        }
        // find x = const
        if(cur->op == TAC_COPY && (cur->b->type == SYM_INT||cur->b->type == SYM_CHAR)){
            //找出在def_tac中代表 当前tac码指针cur的assign 是否可达的元素下标
            int opt_index = find_tac_in_deftac(cur, def_tac, def_index);

            SYM *tmp = cur->a;
            // find all TAC that use tmp 
            for(TAC * tac_tmp = cur; tac_tmp != NULL; tac_tmp = tac_tmp->next){

                if(tac_tmp->op >= 1 && tac_tmp->op <= 12){

                    if(tac_tmp->OUT_vector[opt_index] == 1){

                        //cur的assign可达当前tac_tmp
                        // check the Reaching Definition Results
                        if(tac_tmp->b == tmp) {
                            tac_tmp->b = cur->b;
                            opt_cnt ++;
                        }
                        if(tac_tmp->c == tmp) {
                            tac_tmp->c = cur->b;
                            opt_cnt ++;
                        }
                        
                    }
                }
            }
        }
    }

    return opt_cnt;
}




// ldf写的惊为天人的11层嵌套      // 136 lines
void dead_brunch(BB **BB_array, int BB_num){

    for(int i=0;i<BB_num;i++){
        if(BB_array[i]->live == 1){
 
            if(BB_array[i]->out->op==TAC_IFZ ||BB_array[i]->out->op==TAC_IF){
                
                TAC* cur = BB_array[i]->out;
                 if(cur->b->type==SYM_INT){
                    
                    //该基本块最后一条条件跳转tac的条件为常数
                    if(BB_array[i]->suc!=NULL){ 
                        if(BB_array[i]->suc->next!=NULL){
                            
                            //有两个后继的情况下才能裁剪
                            int label_num;
                            A_node* suc ;
                            for(suc= BB_array[i]->suc; suc!=NULL; suc=suc->next){
                                if(BB_array[suc->n]->in->op==TAC_LABEL && strcmp(BB_array[suc->n]->in->a->name,cur->a->name)==0){
                                    label_num = suc->n;
                                }
                            }

                            int jump_flag=0;
                            if(BB_array[i]->out->op==TAC_IFZ )jump_flag=0;
                            else if(BB_array[i]->out->op==TAC_IF)jump_flag=1;
                            if(cur->b->value==jump_flag){
                                //要跳转，kill掉除了label_num的另一个
                                if(BB_array[i]->suc->n==label_num) BB_array[i]->suc->next=NULL;
                                else BB_array[i]->suc = BB_array[i]->suc->next;
                                
                                if(jump_flag==0){
                                //删除ifz goto 这条tac
                                    BB_array[i]->out->prev->next=BB_array[i]->out->next;
                                    BB_array[i]->out->next->prev=BB_array[i]->out->prev;
                                    BB_array[i]->out=BB_array[i]->out->prev;
                                }

                            }
                            else{
                                //不会跳转  ， kill掉label_num
                                if(BB_array[i]->suc->n==label_num) BB_array[i]->suc = BB_array[i]->suc->next;
                                else BB_array[i]->suc->next=NULL;

                                if(jump_flag==0){
                                //删除ifz goto 这条tac
                                    BB_array[i]->out->prev->next=BB_array[i]->out->next;
                                    BB_array[i]->out->next->prev=BB_array[i]->out->prev;
                                    BB_array[i]->out=BB_array[i]->out->prev;
                                }
                            }
                            
                        }
                    }
                }
                
            } 
            
        
        }
    }

}
void dfs(BB **BB_array, int BB, int *reached_BB){
    reached_BB[BB] = 1;
    A_node* suc;
    for(suc = BB_array[BB]->suc; suc!=NULL;suc=suc->next){
        if(!reached_BB[suc->n]){
            dfs(BB_array,suc->n,reached_BB);
        }
    }
}
void delete_from_suc(BB **BB_array, int suc, int delete_num){
    A_node * tmp;
    if(BB_array[suc]->prev->n == delete_num && BB_array[suc]->prev->next == NULL){
        //只剩一个prev,这个prev就是要删除的节点
        BB_array[suc]->prev = NULL;
    }
    // else if(BB_array[suc]->prev->n != delete_num && BB_array[suc]->prev->next == NULL){

    // }
    else if(BB_array[suc]->prev->n == delete_num && BB_array[suc]->prev->next != NULL){
        //剩两个prev,这个prev在第一个
        BB_array[suc]->prev = BB_array[suc]->prev->next;
    }
    else if(BB_array[suc]->prev->next->n == delete_num){
        //剩两个prev，这个prev在第二个
        BB_array[suc]->prev->next = NULL;
    }
}
void delete_from_prev(BB **BB_array, int prev, int delete_num){
    A_node * tmp;
    if(BB_array[prev]->suc == NULL){//前一个block已经被删了

    }
    else if(BB_array[prev]->suc->n == delete_num && BB_array[prev]->suc->next == NULL){
        //只剩一个suc,这个suc就是要删除的节点
        BB_array[prev]->suc = NULL;
    }
    else if(BB_array[prev]->suc->n != delete_num && BB_array[prev]->suc->next == NULL){
    }
    else if(BB_array[prev]->suc->n == delete_num && BB_array[prev]->suc->next != NULL){
        //剩两个suc,这个suc在第一个
        BB_array[prev]->suc = BB_array[prev]->suc->next;
    }
    else if(BB_array[prev]->suc->next->n == delete_num){
        //剩两个suc，这个suc在第二个
        BB_array[prev]->suc->next = NULL;
    }
}
int kill_BB(BB **BB_array, int BB_num, TAC * tac_first ,int delete_num){
    if(BB_array[delete_num]->live == 1){
        //change live sign
        BB_array[delete_num]->live = 0;
        //delete TAC
        if(delete_num == 0){
            tac_first = BB_array[delete_num]->out->next;
        }
        else if(delete_num == delete_num-1){
            BB_array[delete_num]->in->prev->next = NULL;
        }
        else{
            BB_array[delete_num]->in->prev->next = BB_array[delete_num]->out->next;
            BB_array[delete_num]->out->next->prev = BB_array[delete_num]->in->prev;
        }
        //delete its information from its suc and prev
        A_node* tmp;
        if(BB_array[delete_num]->suc!=NULL){
            for(tmp = BB_array[delete_num]->suc; tmp!=NULL; tmp=tmp->next){
                delete_from_suc(BB_array,tmp->n,delete_num);
            }
            BB_array[delete_num]->suc=NULL;
        }
        if(BB_array[delete_num]->prev!=NULL){
            for(tmp = BB_array[delete_num]->prev; tmp!=NULL; tmp=tmp->next){
                delete_from_prev(BB_array,tmp->n,delete_num);
            }
            BB_array[delete_num]->prev=NULL;
        }
        return 1;
    }
    else{
        return 0;
    }
}
/*optimize 死代码消除*/
int deadcode_opt(BB **BB_array, int BB_num, TAC * tac_first){
    //int reached_BB[BB_num] = {0};
    int opt_count = 0;
    int * reached_BB = (int*)malloc(BB_num * sizeof(int));
    for(int i=0;i<BB_num;i++){
        reached_BB[i] = 0;
    }
   
    
    //detect dead branch 检测死分支
    dead_brunch(BB_array,BB_num);
     
    //dfs尝试遍历所有基本块。访问不到的基本块即为要删除的基本块（不会到达的分支路径在前面已经被删除）
    dfs(BB_array,0,reached_BB); 

    // for(int i=0;i<BB_num;i++){
    //     printf("%d ",reached_BB[i]);
    // }
    // printf("\n");
    for (int i =0; i<BB_num;i++){
        if(reached_BB[i] == 0){
            opt_count+=kill_BB(BB_array,BB_num,tac_first,i);
        }
    }

 
    return opt_count;
}




/*optimize eadassign消除*/    //39 + 206=245  lines
int deadvar_opt(BB **BB_array, int BB_num, TAC * tac_first){
    int opt_count = 0;
    R_node *res[BB_num];
    SYM *def_vars[MAX_var];
    int res_def_index;

    
    Live_Var(BB_array, BB_num, tac_first, res, def_vars, &res_def_index);
    TAC * cur;
    //根据live var analysis分析
    for(cur=tac_first;cur!=NULL;cur=cur->next){
        if(cur->op >= 1 && cur->op <= 12){
            for(int i =0;i<res_def_index;i++){
                if(strcmp(def_vars[i]->name,cur->a->name)==0 && cur->OUT_vector[i]!=1){
                    //it's a dead assign
                    cur->prev->next = cur->next;
                    cur->next->prev = cur->prev;
                    for(int j=0;j<BB_num;j++){
                        if(cur == BB_array[j]->in) BB_array[j]->in = cur->next;
                        else if (cur == BB_array[j]->out) BB_array[j]->out = cur->prev;
                    }
                    opt_count++;
                }
            }
        }
        else if(cur->op == TAC_VAR){//TAC_VAR要为0的时候才能删除
            for(int i =0;i<res_def_index;i++){
                if(strcmp(def_vars[i]->name,cur->a->name)==0 && cur->OUT_vector[i]==0){//it's a dead assign
                    cur->prev->next = cur->next;
                    cur->next->prev = cur->prev;
                    for(int j=0;j<BB_num;j++){
                        if(cur == BB_array[j]->in) BB_array[j]->in = cur->next;
                        else if (cur == BB_array[j]->out) BB_array[j]->out = cur->prev;
                    }
                    opt_count++;
                }
            }
        }
    }  


    return opt_count;
}





//判断两条表达式型TAC的表达式是否相同.相同返回1；否则返回0    //  70 lines
int is_same_exp(TAC *ta, TAC *tb){
    if(ta->b->type == 1 && ta->c->type == 1){
        int sb,sc,so;
        sb = same_var(ta->b, tb->b);
        sc = same_var(ta->c, tb->c);
        so = (ta->op == tb->op) ? 1 : 0;
        if(sb == 1 && sc == 1 && so == 1){
            return 1;
        }
        else{
            return 0;
        }
    }
    if(ta->b->type == 1 && ta->c->type == 4){
        int sb,sc,so;
        sb = same_var(ta->b, tb->b);
        sc = same_cons(ta->c, tb->c);
        so = (ta->op == tb->op) ? 1 : 0;
        if(sb == 1 && sc == 1 && so == 1){
            return 1;
        }
        else{
            return 0;
        }
    }
    if(ta->b->type == 4 && ta->c->type == 1){
        int sb,sc,so;
        sb = same_cons(ta->b, tb->b);
        sc = same_var(ta->c, tb->c);
        so = (ta->op == tb->op) ? 1 : 0;
        if(sb == 1 && sc == 1 && so == 1){
            return 1;
        }
        else{
            return 0;
        }
    }
}
//如果源操作数被重定义了，返回1；没有则返回0
int is_redef(TAC *sch, TAC *curr){
    for(TAC * between = sch->next; (between != NULL) && (between != curr); between = between->next){
        if(between->op == 12){
            if(same_var(between->a, curr->b) || same_var(between->a, curr->c)){
                return 1;
            }
        }
    }
    return 0;
}
/*optimize 局部公共子表达式消除*/
int part_comsub_opt(BB **BB_array, int BB_num){
    int opt_count = 0;
    //遍历每个BB
    for(int i = 0;i < BB_num;i ++){
        //遍历BB中每一条表达式型TAC（tn = x op y)
        for(TAC * cur = BB_array[i]->in; (cur != NULL) && (cur != BB_array[i]->out->next); cur = cur->next){
            if(cur->op >= 1 && cur->op <= 10){
                //遍历cur的前驱（从cur自底向上遍历）
                for(TAC * search_tac = cur->prev; (search_tac != NULL) && (search_tac != BB_array[i]->in->prev); search_tac = search_tac->prev){
                    if(search_tac->op >= 1 && search_tac->op <= 10){
                        int same_flag, redef_flag;
                        same_flag = is_same_exp(cur, search_tac);
                        if(same_flag == 1){
                            redef_flag = is_redef(search_tac, cur);
                            if(redef_flag == 0){
                                strcpy(cur->next->b->name, search_tac->a->name);
                                cur->prev->prev->next = cur->next;
                                cur->next->prev = cur->prev->prev;
                                opt_count += 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return opt_count;
}




//遍历当前BB的所有前驱BB中的所有TAC，找是否有跟cur表达式一样的TAC  //54 + 234 =288 lines
void bl_prebb(int bb_i, BB **BB_array, TAC *cur){
    //遍历当前BB的所有前驱BB
    for(A_node *tmp = BB_array[bb_i]->prev; tmp != NULL; tmp = tmp->next){
        //flag，指示当前前驱是否有TAC与cur表达式相同，0为没有，1为有
        int f = 0;
        //自底向上遍历前驱BB的每条表达式型TAC
        for(TAC *prev_cur = BB_array[tmp->n]->out; (prev_cur != NULL) && (prev_cur != BB_array[tmp->n]->in->prev); prev_cur = prev_cur->prev){
            if((prev_cur->op >=1) && (prev_cur->op <= 10)){
                int same_flag;
                same_flag = is_same_exp(cur, prev_cur);
                if(same_flag == 1){
                    strcpy(cur->next->b->name, prev_cur->a->name);
                    cur->prev->prev->next = cur->next;
                    cur->next->prev = cur->prev->prev;
                    // cur->prev->next = cur->next;
                    // cur->next->prev = cur->prev;
                    f = 1;
                }
            }
        }
        //若遍历完前驱BB的所有TAC后f仍为0则遍历该前驱的前驱
        if(f == 0){
            bl_prebb(tmp->n, BB_array, cur);
        }
    }
}
/*optimize 全局公共子表达式消除*/
int global_comsub_opt(BB **BB_array, int BB_num, TAC * tac_first){
    int exp_num;
    int opt_cnt;
    R_node * res[BB_num];
    My_exp * exp_list[1000];
    exp_num = Available_expression(BB_array, BB_num, tac_first, res, exp_list);
    for(int i = 0; i < BB_num; i ++){
        for(TAC *cur = BB_array[i]->in; (cur != NULL) && (cur != BB_array[i]->out->next); cur = cur->next){
            //判断当前TAC是否为表达式型
            if(cur->op >= 1 && cur->op <= 11){
                //如果是，则找到当前表达式在exp_list中的下标j
                int j = 0;
                j = find_exp(cur, exp_num, exp_list);
                //如果当前BB的R_node的in_vector有此表达式，则说明该表达式可用
                if((j != -1) && (res[i]->in_vector[j] == 1)){
                    //检查cur的两个源操作数是否在之前被定义过
                    int redef_fl;
                    redef_fl = is_redef(BB_array[i]->in, cur);
                    if(redef_fl == 0){
                        bl_prebb(i, BB_array, cur);
                        opt_cnt += 1;
                    }
                }
            }  
        }
    }
    return opt_cnt;
}

// void bl_prebb(int bb_i, BB **BB_array, TAC *cur) {
//     // 遍历当前BB的所有前驱BB
//     for (A_node *tmp = BB_array[bb_i]->prev; tmp != NULL; tmp = tmp->next) {
//         // flag，指示当前前驱是否有TAC与cur表达式相同，0为没有，1为有
//         int f = 0;
//         // 自底向上遍历前驱BB的每条表达式型TAC
//         for (TAC *prev_cur = BB_array[tmp->n]->out; (prev_cur != NULL) && (prev_cur != BB_array[tmp->n]->in->prev); prev_cur = prev_cur->prev) {
//             if ((prev_cur->op >= 1) && (prev_cur->op <= 10)) {
//                 int same_flag = is_same_exp(cur, prev_cur);
//                 if (same_flag == 1) {
//                     if (cur->next != NULL && prev_cur->a != NULL) {
//                         // 确保 cur 和 prev_cur 指针不为空
//                         strcpy(cur->next->b->name, prev_cur->a->name);
//                     } else {
//                         printf("Null pointer encountered when trying to copy names.\n");
//                         continue; // 避免段错误，跳过这个节点
//                     }
//                     // 检查 cur->prev 和 cur->next 是否不为 NULL
//                     if (cur->prev != NULL && cur->next != NULL) {
//                         printf("Deleting TAC ID: %d, OP: %d\n", cur->id, cur->op); // 调试信息

//                         if (cur->prev->next != NULL) {
//                             cur->prev->next = cur->next;
//                         }
//                         if (cur->next->prev != NULL) {
//                             cur->next->prev = cur->prev;
//                         }
//                     } else {
//                         printf("Null pointer encountered in TAC manipulation.\n");
//                     }
                    
//                     // cur->op = TAC_COPY;
//                     // cur->b = prev_cur->a;
//                     // cur->c = NULL;
//                     f = 1;
//                     //
//                     break; // 找到相同表达式并删除后可以退出循环
//                 }
//             }
//         }
//         // 若遍历完前驱BB的所有TAC后f仍为0则遍历该前驱的前驱
//         if (f == 0) {
//             bl_prebb(tmp->n, BB_array, cur);
//         }
//     }
// }

// int global_comsub_opt(BB **BB_array, int BB_num, TAC * tac_first){
//     int exp_num;
//     int opt_cnt = 0; // 初始化 opt_cnt
//     R_node *res[BB_num];
//     My_exp *exp_list[1000];
    
//     exp_num = Available_expression(BB_array, BB_num, tac_first, res, exp_list);

//     for(int i = 0; i < BB_num; i++) {
//         for(TAC *cur = BB_array[i]->in; (cur != NULL) && (cur != BB_array[i]->out->next); cur = cur->next) {
//             // 判断当前TAC是否为表达式型
//             if(cur->op >= 1 && cur->op <= 11) {
//                 // 找到当前表达式在exp_list中的下标j
//                 int j = find_exp(cur, exp_num, exp_list);
//                 // 如果当前BB的R_node的in_vector有此表达式，则说明该表达式可用
//                 if((j != -1) && (res[i]->in_vector[j] == 1)) {
//                     // 检查cur的两个源操作数是否在之前被定义过
//                     int redef_fl = is_redef(BB_array[i]->in, cur);
//                     if(redef_fl == 0) {
//                         // 调试信息
//                         printf("Optimizing BB %d, TAC ID %d\n", i, cur->id);
//                         // 调用bl_prebb进行优化
//                         bl_prebb(i, BB_array, cur);
//                         opt_cnt += 1;
//                     }
//                 }
//             }
//         }
//     }
//     return opt_cnt;
// }

/*###########################优化部分##############结束############*/


/*
此外，还更改了数据结构A_node,增加了成员变量int isjump_target;  //0 or 1   */

typedef struct loop_invar{
    SYM * symbol;
    TAC * tac_ptr;
    int live;
    struct loop_invar * next;
}loop_invar;


loop_invar * mk_loop_invar(SYM * sym,TAC * tac,int live){
    loop_invar * tmp=(loop_invar * )malloc(sizeof(loop_invar));
    tmp->symbol=sym;
    tmp->tac_ptr=tac;
    tmp->live=live;
    tmp->next=NULL;
}
loop_invar * lookup_sym_loop_invar(SYM * sym , loop_invar * lp_invar){
    loop_invar * cur=lp_invar;
    while(cur){
        if(sym==cur->symbol&&cur->live==1)return cur;
        cur=cur->next;
    }
    return NULL;
}
loop_invar * lookup_tac_loop_invar(TAC * tac , loop_invar * lp_invar){
    loop_invar * cur=lp_invar;
    while(cur){
        if( cur->tac_ptr==tac)return cur;
        cur=cur->next;
    }
    return NULL;
}
loop_invar * find_loop_invariant(TAC * begin , TAC * end){
    TAC * cur;
    loop_invar *invariant_sym, *invariant,* temp;
    invariant_sym=NULL;
    invariant=NULL;

    //第一遍迭代，将赋值的tac以及定值的符号插入list1:invariant_sym
    for(cur=begin;cur!=end;cur=cur->next){
        if(cur->op>=1&&cur->op<=12){
            temp=mk_loop_invar(cur->a,cur,1);
            temp->next=invariant_sym;
            invariant_sym=temp;
        }
    }

    //第二遍迭代，找出循环不变式
    for(cur=begin;cur!=end;cur=cur->next){
        
        if(cur->op>=1&&cur->op<=10){  //三操作数
            if(lookup_sym_loop_invar(cur->b,invariant_sym)==NULL && 
            lookup_sym_loop_invar(cur->c,invariant_sym)==NULL){
                loop_invar * tmp=lookup_tac_loop_invar(cur,invariant);
                if(tmp==NULL){
                    //查循环不变式表，没找到，则插入a
                    tmp=mk_loop_invar(cur->a,cur,1);
                    tmp->next=invariant;
                    invariant=tmp;
                    //将list1:invariant_sym中左值符号的live置0
                    loop_invar *sym_tmp=lookup_sym_loop_invar(cur->a,invariant_sym);
                    if(sym_tmp!=NULL)sym_tmp->live=0;

                }else{
                    //循环不定式表里有该表达式，该表达式不是循环不定式
                    tmp->live=0;
                    //将list1:invariant_sym中左值符号的live置0
                    loop_invar *sym_tmp=lookup_sym_loop_invar(cur->a,invariant_sym);
                    if(sym_tmp!=NULL)sym_tmp->live=1;

                }
            }
        }
        else if(cur->op==TAC_NEG ||cur->op==TAC_COPY){ //二操作数
            if(lookup_sym_loop_invar(cur->b,invariant_sym)==NULL){ 
                loop_invar * tmp=lookup_tac_loop_invar(cur,invariant);
                if(tmp==NULL){
                    //查循环不变式表，没找到，则插入该表达式tac
                    tmp=mk_loop_invar(cur->a,cur,1);
                    tmp->next=invariant;
                    invariant=tmp;
                    //将list1:invariant_sym中左值符号的live置0
                    loop_invar *sym_tmp=lookup_sym_loop_invar(cur->a,invariant_sym);
                    if(sym_tmp!=NULL)sym_tmp->live=0;

                }else{
                    //循环不定式表里有该表达式，该表达式不是循环不定式
                    tmp->live=0;
                    //将list1:invariant_sym中左值符号的live置0
                    loop_invar *sym_tmp=lookup_sym_loop_invar(cur->a,invariant_sym);
                    if(sym_tmp!=NULL)sym_tmp->live=1;

                }
            }
        } 


    }

    // //释放invariant_sym
    // for(temp=invariant_sym->next;temp!=NULL;temp=temp->next){
    //     free(invariant_sym);
    //     invariant_sym=temp;
    //     if(temp->next==NULL)free(invariant_sym);
    // }
    
    return invariant;

}

//定位某个tac所在的BB的id
int locat_BB(TAC * tac){
    TAC * cur;
    for(int i =0 ;i<bb_num;i++){
        // if(BB_array[i]->in==tac)return i;
        for(cur=BB_array[i]->in;cur!=BB_array[i]->out->next;cur=cur->next){
            if(cur==tac)return i;
        }
    }
    return -1;
}
//del_bb后对cfg进行深度优先遍历，reached_BB记录可达信息，判断del_bb是否为关键节点
void dfs_reach(BB **BB_array, int begin_bb,int del_bb, int *reached_BB){
    reached_BB[begin_bb] = 1;
    A_node* suc;
    for(suc = BB_array[begin_bb]->suc; suc!=NULL;suc=suc->next){

        if(suc->n==del_bb)break;

        if(!reached_BB[suc->n]){
            dfs_reach(BB_array,suc->n,del_bb,reached_BB);
        }
    }
}
/*判断tac所在的基本块是否是流程图的关键节点*/
int inKeyPath(TAC * tac,int begin_bb,int end_bb){
    /*该tac是否在begin_bb和end_bb之间的关键路径上，1：是   ,   0：不是*/
    int bb_index=locat_BB(tac);
    // tac_print(tac);  printf("\n");
    // printf("%d\n",bb_index);
    // printf("%d,%d\n",begin_bb,end_bb);
    if(begin_bb==end_bb)return 1;   
    else if(bb_index==begin_bb || bb_index==end_bb)return 1;
    else{
        // (BB_array[bb_index-1]->out->op!=TAC_IFZ && BB_array[bb_index-1]->out->op!=TAC_IF  )return 1;
        int reach_bb[bb_num] ;
        memset(reach_bb, 0, sizeof(reach_bb));
        dfs_reach(BB_array,begin_bb,bb_index,reach_bb);
        if(reach_bb[end_bb]==1)return 0;  //删除当前bb仍然可以到达end_bb说明当前路径不是关键的
        else return 1;

    }
    return 0;
}
//
//
//
//将cur插入later_bb前面非跳转
void insert_BB(BB *cur ,int later_bb){
    A_node * tmp=BB_array[later_bb]->prev;
    while (tmp){
        if(tmp->isjump_target==0)break;
        tmp=tmp->next;
    }
    int prev_bb=0;
  
    if(tmp)prev_bb=tmp->n;
    //在prev_bb和later_bb之间插入cur

    tmp=BB_array[prev_bb]->suc;
    while (tmp){
        if(tmp->n==later_bb){
            tmp->n=cur->id;
            break;
        }
        tmp=tmp->next;
    }
    tmp=BB_array[later_bb]->prev;
    while (tmp){
        if(tmp->n==prev_bb){
            tmp->n=cur->id;
            break;
        }
        tmp=tmp->next;
    }
    
    cur->suc=(A_node *)malloc(sizeof(A_node)); //后继
    cur->suc->isjump_target=0;
    cur->suc->n=later_bb;
    cur->suc->next=NULL;
    cur->prev=(A_node *)malloc(sizeof(A_node)); //前驱
    cur->prev->n=prev_bb;
    cur->prev->isjump_target=0;
    cur->prev->next=NULL;

    //tac插入
    BB_array[prev_bb]->out->next=cur->in;
    cur->in->prev=BB_array[prev_bb]->out;

    BB_array[later_bb]->in->prev=cur->out;
    cur->out->next=BB_array[later_bb]->in;
}

/*optimizer循环不变量外提优化*/
void loop_invariant(){
    //loop_list
    for(loop_record * loop =loop_list;loop!=NULL;loop=loop->next){
        // tac_print(loop->begin_label);printf("\n");
        // tac_print(loop->end_label->prev);printf("\n");

        loop_invar *invariant_list ,* tmp;
        invariant_list=find_loop_invariant(loop->begin_label,loop->end_label);
        
        TAC * preheader, *cur_tac;
        preheader=NULL;
        cur_tac=NULL;
        for(tmp=invariant_list;tmp!=NULL;tmp=tmp->next){
            int flag_keypath=0;
            if(tmp->live){
                flag_keypath=inKeyPath(tmp->tac_ptr,locat_BB(loop->begin_label),locat_BB(loop->end_label->prev));
                if(flag_keypath){  //循环不变式在关键路径上，可以外提
                    cur_tac=tmp->tac_ptr;
                    cur_tac->prev->next=cur_tac->next;
                    cur_tac->next->prev=cur_tac->prev;

                    cur_tac->next=preheader;
                    preheader=cur_tac;
                }
            }
        }
        if(preheader!=NULL){
            //有需要外提的代码

            //根据后继关系 链接‘外提tac链’的前驱关系
            TAC * header ,*cur;
            cur=NULL;
            for(header=preheader;header!=NULL;header=header->next){
                header->prev=cur;
                cur=header;
            }//至此，循环外提部分tac代码以 preheader头，cur尾 提取出来
            //新建bb块  代表外提代码
            BB * pre_bb=(BB*)malloc(sizeof(BB));
            pre_bb->id=bb_num;
            pre_bb->in=preheader;
            pre_bb->out=cur;
            pre_bb->live=1;
            BB_array[bb_num++]=pre_bb;
            printf("bb_num:%d\n",bb_num);
            //将pre_bb插入begin_bb前面
            int begin_bb=locat_BB(loop->begin_label);
            insert_BB(pre_bb,begin_bb);

        }
        
    }

}



// void insert_BB(BB *cur, int later_bb) {
//     if (later_bb < 0 || later_bb >= bb_num) {
//         printf("Invalid later_bb index: %d\n", later_bb);
//         return;
//     }
//     A_node *tmp = BB_array[later_bb]->prev;
//     int prev_bb = -1;

//     // 找到非跳转目标的前驱块
//     while (tmp) {
//         if (tmp->isjump_target == 0) {
//             prev_bb = tmp->n;
//             break;
//         }
//         tmp = tmp->next;
//     }

//     if (prev_bb == -1) {
//         printf("No valid previous basic block found for later_bb %d\n", later_bb);
//         return;
//     }

//     // 在 prev_bb 和 later_bb 之间插入 cur
//     tmp = BB_array[prev_bb]->suc;
//     while (tmp) {
//         if (tmp->n == later_bb) {
//             tmp->n = cur->id;
//             break;
//         }
//         tmp = tmp->next;
//     }

//     tmp = BB_array[later_bb]->prev;
//     while (tmp) {
//         if (tmp->n == prev_bb) {
//             tmp->n = cur->id;
//             break;
//         }
//         tmp = tmp->next;
//     }

//     // 创建 cur 的后继和前驱节点
//     cur->suc = (A_node *)malloc(sizeof(A_node));
//     if (cur->suc == NULL) {
//         printf("Memory allocation failed for cur->suc.\n");
//         exit(EXIT_FAILURE);
//     }
//     cur->suc->isjump_target = 0;
//     cur->suc->n = later_bb;
//     cur->suc->next = NULL;

//     cur->prev = (A_node *)malloc(sizeof(A_node));
//     if (cur->prev == NULL) {
//         printf("Memory allocation failed for cur->prev.\n");
//         exit(EXIT_FAILURE);
//     }
//     cur->prev->n = prev_bb;
//     cur->prev->isjump_target = 0;
//     cur->prev->next = NULL;

//     // 将 cur 插入基本块链中
//     if (BB_array[prev_bb]->out != NULL) {
//         BB_array[prev_bb]->out->next = cur->in;
//     }
//     if (cur->in != NULL) {
//         cur->in->prev = BB_array[prev_bb]->out;
//     }

//     if (BB_array[later_bb]->in != NULL) {
//         BB_array[later_bb]->in->prev = cur->out;
//     }
//     if (cur->out != NULL) {
//         cur->out->next = BB_array[later_bb]->in;
//     }
// }
// /*optimizer循环不变量外提优化*/
// void loop_invariant(){
//     //loop_list
//     for(loop_record * loop =loop_list;loop!=NULL;loop=loop->next){
//         // tac_print(loop->begin_label);printf("\n");
//         // tac_print(loop->end_label->prev);printf("\n");
//         loop_invar *invariant_list ,* tmp;
//         if(loop->begin_label!=NULL && loop->end_label!=NULL){
//             invariant_list=find_loop_invariant(loop->begin_label, loop->end_label);
//         }else{
//             printf("Missing Loop Label\n");
//             exit(1);
//         }
//         TAC * preheader, *cur_tac;
//         preheader=NULL;
//         cur_tac=NULL;
//         for(tmp=invariant_list;tmp!=NULL;tmp=tmp->next){
//             int flag_keypath=0;
//             if(tmp->live){
//                 flag_keypath=inKeyPath(tmp->tac_ptr,locat_BB(loop->begin_label),locat_BB(loop->end_label->prev));
//                 if(flag_keypath){  //循环不变式在关键路径上，可以外提
//                     cur_tac=tmp->tac_ptr;
                    
//                     // 检查指针是否为 NULL
//                     if (cur_tac->prev != NULL && cur_tac->next != NULL) {
//                         cur_tac->prev->next = cur_tac->next;
//                         cur_tac->next->prev = cur_tac->prev;
//                         cur_tac->next = preheader;
//                         preheader = cur_tac;
//                     } else {
//                         printf("Null pointer encountered in TAC manipulation.\n");
//                         continue; // 避免段错误，跳过这个节点
//                     }
//                 }
//             }
//         }
//         if(preheader!=NULL){
//             //有需要外提的代码

//             //根据后继关系 链接‘外提tac链’的前驱关系
//             TAC * header ,*cur;
//             cur=NULL;
//             for(header=preheader;header!=NULL;header=header->next){
//                 header->prev=cur;
//                 cur=header;
//             }//至此，循环外提部分tac代码以 preheader头，cur尾 提取出来
//             //新建bb块  代表外提代码
//             BB * pre_bb=(BB*)malloc(sizeof(BB));
//             if (pre_bb == NULL) {
//                 printf("Memory allocation failed for pre_bb.\n");
//                 exit(EXIT_FAILURE);
//             }
//             pre_bb->id=bb_num;
//             pre_bb->in=preheader;
//             pre_bb->out=cur;
//             pre_bb->live=1;
//             BB_array[bb_num++]=pre_bb;
//             printf("bb_num:%d\n",bb_num);
//             //将pre_bb插入begin_bb前面
//             int begin_bb=locat_BB(loop->begin_label);
//             insert_BB(pre_bb, begin_bb);
//         }
//     }
// }

/* 识别归纳变量部分*///1043-1750
//basci_iv用来寻找基础归纳变量，final_iv用来存最终的归纳变量列表

//如果是循环中的常量则返回1，否则返回0
int loop_const(SYM* a,TAC * tac_cur)
{
    if(a->type==SYM_CHAR||a->type==SYM_INT)
    {
        return 1;
    }
    R_node *res[bb_num];
    SYM *vars[100];
    int vars_index = 0;

    // Constant Propagation Dataflow Analysis 这部分可以提出去，不如每次调用都要重新计算
    Constant_Propagation(BB_array, bb_num, tac_first, res ,vars, &vars_index);

    int a_index=find_var(a,vars,vars_index);

    if(tac_cur->Val_Status[a_index]==DEF)
    {
        return 1;
    }
    return 0;
}

//如果iv_list中有以x为基类的归纳变量则返回该类的最后一个iv_list，否则返回null；可以找final_iv/basic_iv
iv_list* find_samex(iv_list* iv_target,iv_list *begin)
{
    for(iv_list * curx=begin;curx!=NULL;curx=curx->next)
    {
        for(iv_list * iv_cur=curx;iv_cur!=NULL;iv_cur=iv_cur->samex_next)
        {
            if(!strcmp(iv_target->x,iv_cur->x)&&iv_cur->samex_next==NULL)
            return iv_cur;
        }
    }
    return NULL;
}

//如果变量a是归纳变量则返回以a为左值的iv_list，否则返回null；只找final_iv，每条向下连接的列表的第一个是基础归纳
iv_list* in_final_iv(char* a)
{
    for(iv_list * curx=final_iv;curx!=NULL;curx=curx->next)
    {
        for(iv_list * iv_cur=curx;iv_cur!=NULL;iv_cur=iv_cur->samex_next)
        {
            if(!strcmp(a,curx->y))
            return iv_cur;
        }
    }
    return NULL;
}

iv_list* in_basic_iv(char* a)
{
    for(iv_list * curx=basic_iv;curx!=NULL;curx=curx->next)
    {
        if(!strcmp(a,curx->y))
        return curx;
    }
    return NULL;
}
//如果两个操作数中第一个是变量，第二个是循环不变量则返回1，否则返回0
int two_isconst(SYM*op1,SYM*op2,TAC* tac_cur)
{
    int op1_isconst,op2_isconst;
    op1_isconst=loop_const(op1,tac_cur);
    op2_isconst=loop_const(op2,tac_cur);
    if (!op1_isconst&&op2_isconst)
    {
        return 1;
    }
    else return 0;
}

//处理基础归纳变量
void add_iv_basic(TAC* tac_cur,SYM*op1,SYM*op2)
{
    if (two_isconst(op1,op2,tac_cur))
    {
        //i、t对应相等  //字符数组的比较还需优化
        if(!strcmp(tac_cur->next->a->name,op1->name)&&!strcmp(tac_cur->a->name,tac_cur->next->b->name))
        {
            //print_iv(basic_iv);
            iv_list* new=(iv_list*)malloc(sizeof(iv_list));
            new->x=malloc(12);
            new->y=malloc(12);
            strcpy(new->x,tac_cur->next->a->name);
            strcpy(new->y,tac_cur->next->a->name);
            new->k=1;
            new->con=op2->value;
            new->change=op2->value;//若有同类要累加con并分为多个基础归纳变量
            new->begin_tac=tac_cur;
            //如果没有出现samex的情况
            iv_list* new_samex=find_samex(new,basic_iv);//同类的最后一个
            if (!new_samex)
            {
                new->next=basic_iv;
                basic_iv=new; 
            }
            else
            {
                new_samex->end_tac=new->begin_tac->prev;
                new_samex->samex_next=new;
            }
            //print_iv(basic_iv);
        }
    }             
}

//遍历当前循环内的tac，如果遇到基础归纳变量赋值为常数或乘除了非1的数，则不再是基础归纳变量
void basic_iv_delete(loop_record* loop_cur)
{
    for(TAC* tac_cur=loop_cur->begin_label;tac_cur!=loop_cur->end_label;tac_cur=tac_cur->next)
    {
        if(tac_cur->op==12)//假设i=i*1 i=i/1这种已经被优化了，op3-12左值若为基础归纳变量，那么基础归纳变量要被删除
        {
            iv_list* iv_cur=in_basic_iv(tac_cur->a->name);
            if(iv_cur)
            {
                //如果前一句不是t=i+/-c，则有问题  //这一句是i=t
                if((tac_cur->prev->op==1||tac_cur->prev->op==2)&&!strcmp(tac_cur->prev->a->name,tac_cur->b->name))
                {
                    if((loop_const(tac_cur->prev->b,tac_cur->prev)&&!strcmp(tac_cur->prev->c->name,tac_cur->a->name))||(loop_const(tac_cur->prev->c,tac_cur->prev)&&!strcmp(tac_cur->prev->b->name,tac_cur->a->name)))
                    {
                        continue;
                    }
                }
                else
                {
                    if(iv_cur->next)
                    {
                        iv_cur->next->prev=iv_cur->prev;
                        iv_cur->prev->next=iv_cur->next;
                    }
                    else//如果是最后一个
                    {
                        if(iv_cur->prev)
                        {
                            iv_cur->prev->next=NULL;
                        }
                        else//如果是唯一一个
                        {
                            basic_iv=NULL;
                        }
                    }
                }
            }
        }
        if(tac_cur->op==18)
        {
            iv_list* iv_cur=in_basic_iv(tac_cur->a->name);
            if(iv_cur)
            if(iv_cur->next)
            {
                iv_cur->next->prev=iv_cur->prev;
                iv_cur->prev->next=iv_cur->next;
            }
            else//如果是最后一个
            {
                if(iv_cur->prev)
                {
                    iv_cur->prev->next=NULL;
                }
                else//如果是唯一一个
                {
                    basic_iv=NULL;
                }
            }
        }
    }
}

// void basic_iv_delete(loop_record* loop_cur) {
//     printf("Starting basic_iv_delete\n");
    
//     for (TAC* tac_cur = loop_cur->begin_label; tac_cur != loop_cur->end_label; tac_cur = tac_cur->next) {
//         printf("Processing TAC: %p, op: %d\n", tac_cur, tac_cur->op);

//         if (tac_cur->op == 12) { // 假设 i=i*1, i=i/1 这种已经被优化了，op3-12 左值若为基础归纳变量，那么基础归纳变量要被删除
//             iv_list* iv_cur = in_basic_iv(tac_cur->a->name);
//             if (iv_cur) {
//                 printf("Found basic induction variable: %s\n", iv_cur->y);

//                 // 如果前一句不是 t=i+/-c，则有问题  // 这一句是 i=t
//                 if (tac_cur->prev && (tac_cur->prev->op == 1 || tac_cur->prev->op == 2) && strcmp(tac_cur->prev->a->name, tac_cur->b->name) == 0) {
//                     printf("Previous TAC is an assignment with + or - operation.\n");
//                     if ((loop_const(tac_cur->prev->b, tac_cur->prev) && strcmp(tac_cur->prev->c->name, tac_cur->a->name) == 0) ||
//                         (loop_const(tac_cur->prev->c, tac_cur->prev) && strcmp(tac_cur->prev->b->name, tac_cur->a->name) == 0)) {
//                         printf("The previous TAC has valid loop constants.\n");
//                         continue;
//                     }
//                 } else {
//                     printf("Deleting induction variable: %s\n", iv_cur->y);
//                     if (iv_cur->next) {
//                         iv_cur->next->prev = iv_cur->prev;
//                         if (iv_cur->prev) {
//                             iv_cur->prev->next = iv_cur->next;
//                         }
//                     } else { // 如果是最后一个
//                         if (iv_cur->prev) {
//                             iv_cur->prev->next = NULL;
//                         } else { // 如果是唯一一个
//                             basic_iv = NULL;
//                         }
//                     }
//                 }
//             }
//         }
//         if (tac_cur->op == 18) {
//             iv_list* iv_cur = in_basic_iv(tac_cur->a->name);
//             if (iv_cur) {
//                 printf("Found variable declaration: %s\n", iv_cur->y);
//                 if (iv_cur->next) {
//                     iv_cur->next->prev = iv_cur->prev;
//                     if (iv_cur->prev) {
//                         iv_cur->prev->next = iv_cur->next;
//                     }
//                 } else { // 如果是最后一个
//                     if (iv_cur->prev) {
//                         iv_cur->prev->next = NULL;
//                     } else { // 如果是唯一一个
//                         basic_iv = NULL;
//                     }
//                 }
//             }
//         }
//     }
//     printf("Finished basic_iv_delete\n");
// }


//basic_iv中需要重命名、修改tac信息、con求和
void basic_iv_fix(loop_record* loop_cur)
{
    for(iv_list * curx=basic_iv;curx!=NULL;curx=curx->next)
        {
            int cnt=0;//计算一共用多少个以该x为基的基本归纳变量
            int total_change=0;
            if(curx->samex_next!=NULL)
            {
                for(iv_list * iv_cur=curx;iv_cur!=NULL;iv_cur=iv_cur->samex_next)
                {
                    total_change+=iv_cur->con;
                    iv_cur->con=total_change;
                }
                for(iv_list * iv_cur=curx;iv_cur!=NULL;iv_cur=iv_cur->samex_next)
                {
                    //iv_cur的重命名
                    iv_cur->change=total_change;
                    cnt+=1;
                    char *oldx;
                    oldx=malloc(12);
                    strcpy(oldx,iv_cur->x);
                    char *newx;
                    newx=malloc(12);
                    sprintf(newx, "%s%d", iv_cur->x,cnt); /* Set up text */
                    iv_cur->x=newx;
                    iv_cur->y=newx;
                    SYM *irename =mk_var(newx);
                    //修改tac码   t=i+c,i=t ->t=in+c,in=t
                    TAC * tac_cur=iv_cur->begin_tac;

                    // printf("增加初始化前：\n");
                    // print_alltac();
                    //增加初始化的tac var t ,t=i+con-change,in=t
                    SYM *tmp = mk_tmp(); // 创建临时变量还要有一个var t的tac
                    SYM *oldx_sym=mk_var2(oldx);
                    //printf("oldx:%s\n",oldx);
                    SYM *con_c=mk_const(abs(iv_cur->con-iv_cur->change));//构造一个常数SYM
                    TAC *code0 = mk_tac(TAC_VAR, irename, NULL, NULL);//生成一个var newx
                    if(iv_cur->con-iv_cur->change==0)
                    {
                        TAC *code1 = mk_tac(TAC_COPY, irename,oldx_sym, NULL); // 生成 newx = oldx 的 TAC 代码
                        code0->next=code1;
                        code1->prev=code0;
                        code0->prev=loop_cur->begin_label->prev;
                        loop_cur->begin_label->prev->next=code0;
                        code1->next=loop_cur->begin_label;
                        loop_cur->begin_label->prev=code1; 
                    }
                    else
                    {
                        TAC *code1 = mk_tac(TAC_VAR, tmp, NULL, NULL); // 生成 var t 的 TAC 代码
                        TAC *code2;
                        if(iv_cur->con-iv_cur->change<0)
                        {
                            code2 = mk_tac(TAC_SUB, tmp, oldx_sym, con_c);// 生成 t = i-con 的 TAC 代码
                        }
                        if(iv_cur->con-iv_cur->change>0)
                        {
                            code2 = mk_tac(TAC_ADD, tmp, oldx_sym, con_c); // 生成 t = i+con 的 TAC 代码
                        }
                        TAC *code3 = mk_tac(TAC_COPY, irename,tmp, NULL); // 生成 in = t 的 TAC 代码
                        code0->next=code1;
                        code1->prev=code0;                        
                        code1->next=code2;
                        code2->prev=code1;
                        code2->next=code3;
                        code3->prev=code2;
                        code0->prev=loop_cur->begin_label->prev;
                        loop_cur->begin_label->prev->next=code0;
                        code3->next=loop_cur->begin_label;
                        loop_cur->begin_label->prev=code3;
                    }
                   if (two_isconst(tac_cur->b,tac_cur->c,tac_cur))  //t=i+c  i=t
                   {
                    //指针改的是地址，所以所有a都被改成a1了,这里的改名可以放在后面做
                    // tac_cur->b=irename;
                    // tac_cur->next->a=irename;
                        tac_cur->c->value=total_change;
                   }
                    if (two_isconst(tac_cur->c,tac_cur->b,tac_cur)) //t=c+i
                    {
                        tac_cur->b->value=total_change;
                    }
                    //printf("增加初始化后：\n");
                    // print_alltac();

                    iv_cur->con=iv_cur->change;

                    //修改原本i的作用域中的其它tac，将对应的i改为in
                    if(iv_cur->end_tac==NULL)//每一类的最后一个在add_iv_basic中不好修改，就在这里处理一下
                    {
                        // c = a2
                        // if t1 goto L1
                        // label L2   
                        iv_cur->end_tac=loop_cur->end_label->prev->prev;
                    }
                        // printf("begin_end：\n");
                        // print_tac(iv_cur->begin_tac,NULL,1);
                        // print_tac(iv_cur->end_tac,NULL,1);
                    for(TAC * tac_mid=iv_cur->begin_tac;tac_mid!=iv_cur->end_tac->next;tac_mid=tac_mid->next)
                    {
                        if(tac_mid->a)
                        {   
                            if(tac_mid->a->name)
                            {
                                if(!strcmp(tac_mid->a->name,oldx))
                                {
                                    tac_mid->a=irename;
                                }
                            }
                        }
                        if(tac_mid->b)
                        {   
                            if(tac_mid->b->name)
                            {
                                if(!strcmp(tac_mid->b->name,oldx))
                                {
                                    tac_mid->b=irename;
                                }
                            }
                        }
                        if(tac_mid->c)
                        {   
                            if(tac_mid->c->name)
                            {
                                if(!strcmp(tac_mid->c->name,oldx))
                                {
                                    tac_mid->c=irename;
                                }
                            }
                        }                   
                    }
                        // printf("修改中间tac后：\n");
                        // print_alltac();
                    //最后一步是修改basic_iv的节点之间连接的方式，基础归纳变量最后应该每个节点的samex_next为null
                    //后面发现这一步没必要

                    //增加a=ai_last的赋值，保证循环外的一致性  或者也可以判断后面是否出现过a来决定这一步要不要
                    if(iv_cur->samex_next==NULL)
                    {
                        TAC *code_last = mk_tac(TAC_COPY,oldx_sym,irename, NULL);
                        code_last->next=iv_cur->end_tac;
                        code_last->prev=iv_cur->end_tac->prev;
                        iv_cur->end_tac->prev=code_last;
                        code_last->prev->next=code_last;
                    }
                    //print_alltac(tac_first);

                }
            }
        }
}

//将basic_iv中信息增加到final_iv中
void basic_to_final()
{
    for(iv_list * curx=basic_iv;curx!=NULL;curx=curx->next)
    {
        for(iv_list * iv_cur=curx;iv_cur!=NULL;iv_cur=iv_cur->samex_next)
        {
            iv_list* new=(iv_list*)malloc(sizeof(iv_list));
            new->x=malloc(12);
            new->y=malloc(12);
            strcpy(new->x,iv_cur->x);
            strcpy(new->y,iv_cur->y);
            new->k=iv_cur->k;
            new->con=iv_cur->con;
            new->change=iv_cur->change;//若有同类要累加con并分为多个基础归纳变量
            new->next=final_iv;
            final_iv=new;
        }
    }
}

//t=i+c;j=t 处理加/减型依赖归纳变量
void add_iv(TAC* tac_cur,SYM*op1,SYM*op2)
{
    if (two_isconst(op1,op2,tac_cur))
    {
        //i、t对应相等  
        //修订：这里应该先判断tac_cur->next的左值是不是基础归纳变量再决定要不要加上，排除t=i+1,i=t重复入列情况
        iv_list* cur_basic=in_final_iv(tac_cur->next->a->name);
        if(cur_basic)
        {
            if(!strcmp(cur_basic->y,cur_basic->x))//说明是基础归纳变量，不能重复入列
            {
                return;
            }
        }
        //再判断t=i+1,j=t中的i是否为归纳变量
         cur_basic=in_final_iv(op1->name);
        if (cur_basic)
        {
            if(!strcmp(tac_cur->next->a->name,op1->name)&&!strcmp(tac_cur->a->name,tac_cur->next->b->name))
            {
                //t=i+c;j=t
                iv_list* new=(iv_list*)malloc(sizeof(iv_list));
                new->x=malloc(12);
                new->y=malloc(12);
                strcpy(new->x,cur_basic->x);
                strcpy(new->y,tac_cur->next->a->name);
                new->con=tac_cur->c->value;
                new->k=cur_basic->k;
                new->change=cur_basic->change;
                new->begin_tac=tac_cur;
                //如果没有出现samex的情况
                iv_list* new_samex=find_samex(new,final_iv);
                if (!new_samex)//实际上不可能，因为他一定连接某一归纳变量的samex_next后
                {
                    new->next=final_iv;
                    final_iv=new; 
                }
                else
                {
                    new_samex->samex_next=new;
                }
            }
        }
    }     
}

//处理乘/除型依赖归纳变量
void mul_iv(TAC* tac_cur,SYM*op1,SYM*op2)
{
    //t=i*c
    if (two_isconst(op1,op2,tac_cur))
    {
        //t=i*c  j=t
        if (tac_cur->next->op==12)
        {
            if(!strcmp(tac_cur->a->name,tac_cur->next->b->name))
            {
                iv_list* cur_basic=in_final_iv(op1->name);
                if(cur_basic)
                {
                    iv_list* new=(iv_list*)malloc(sizeof(iv_list));
                    new->x=malloc(12);
                    new->y=malloc(12);
                    strcpy(new->y,tac_cur->next->a->name);
                    new->k=op2->value*cur_basic->k;
                    strcpy(new->x,cur_basic->x);
                    new->con=0;
                    //new->first=cur_basic->first*op2->value;
                    new->change=cur_basic->change*op2->value;
                    new->begin_tac=tac_cur;
                    //如果没有出现samex的情况
                    iv_list* new_samex=find_samex(new,final_iv);
                    if (!new_samex)
                    {
                        new->next=final_iv;
                        final_iv=new; 
                    }
                    else
                    {
                        new_samex->samex_next=new;
                    }
                }
            }


        }
        //t=i*c ; var t2 ; t2=t+c2 ;j=t2
        if (tac_cur->next->op==18&&(tac_cur->next->next->op==1||tac_cur->next->next->op==2)&&tac_cur->next->next->next->op==12)
        {
            //first  t=i*c
            TAC* second=tac_cur->next;//var t2 ;
            TAC* third=second->next;//t2=t+c2 ;
            TAC* fourth=third->next;//j=t2 ;
            if(!strcmp(tac_cur->a->name,third->b->name)&&!strcmp(third->a->name,fourth->b->name)&&!strcmp(second->a->name,third->a->name))
            {
                iv_list* cur_basic=in_final_iv(op1->name);
                if(cur_basic)
                {
                    iv_list* new=(iv_list*)malloc(sizeof(iv_list));
                    new->x=malloc(12);
                    new->y=malloc(12);
                    strcpy(new->y,fourth->a->name);
                    new->k=op2->value*cur_basic->k;
                    strcpy(new->x,cur_basic->x);
                    new->change=op2->value*cur_basic->change;
                    new->con=third->c->value;
                    new->begin_tac=tac_cur;
                    //如果没有出现samex的情况
                    iv_list* new_samex=find_samex(new,final_iv);
                    if (!new_samex)
                    {
                        new->next=final_iv;
                        final_iv=new; 
                    }
                    else
                    {
                        new_samex->samex_next=new;
                    }
                }
            }
        }
    }
}

//遍历每个循环，分别构建basic_iv和final_iv
void find_iv(loop_record* loop_cur)
{
    for(TAC *tac_cur=loop_cur->begin_label; tac_cur && tac_cur != loop_cur->end_label;tac_cur=tac_cur->next)
    {
        //t=i+/-c  i=t
        if(tac_cur && (tac_cur->op==1 || tac_cur->op==2))//+/-
        {
            if(tac_cur->next && tac_cur->next->op==12)// =
            {
                add_iv_basic(tac_cur,tac_cur->b,tac_cur->c);
                add_iv_basic(tac_cur,tac_cur->c,tac_cur->b);
            }
        }
    }
    if(basic_iv==NULL)
    {
        return;
    }
    for(iv_list* cur=basic_iv;cur->next!=NULL;cur=cur->next)
    {
        cur->next->prev=cur;
    }
    printf("first basic_iv\n");
    // print_ivlist(basic_iv);

    basic_iv_delete(loop_cur);
    printf("delete basic_iv\n");
    // print_ivlist(basic_iv);

    //basic_iv中需要重命名、修改tac信息（前提是没有类似i=2*i，i=c的赋值,否则要删去）、diff求和
    basic_iv_fix(loop_cur);
    printf("rename basic_iv\n");
    //print_ivlist(basic_iv);

    //此时basic_iv中记录好了基本归纳变量的信息,将basic_iv中信息增加到final_iv中
    //这里不能简单复制，要写一个函数,转换完后可以free掉basic_iv，但还没写
    basic_to_final();

    //现在寻找依赖归纳变量来填充final_iv，应该先找*再找+？
    for(TAC *tac_cur=loop_cur->begin_label;tac_cur!=loop_cur->end_label;tac_cur=tac_cur->next)
    {
        if(tac_cur->op==3||tac_cur->op==4)// * or /
        {
            mul_iv(tac_cur,tac_cur->b,tac_cur->c);
            mul_iv(tac_cur,tac_cur->c,tac_cur->b);
        }
        if(tac_cur->op==1||tac_cur->op==2)// +/-
        {
            if(tac_cur->next->op==12)// =
            {
                add_iv(tac_cur,tac_cur->b,tac_cur->c);
                add_iv(tac_cur,tac_cur->c,tac_cur->b);
            }
        }
    }
    //此处final_iv已经建好，可进行强度削弱
}

/* 强度削弱部分 */
void strenth_reduce()
{
    for(loop_record* loop_cur=loop_list;loop_cur!=NULL;loop_cur=loop_cur->next)
    {
        basic_iv=NULL;
        final_iv=NULL;
        // printf("initial basic_iv:\n");
        // print_ivlist(basic_iv);
        //调用find_iv构建好final_iv
        find_iv(loop_cur);
        if(basic_iv==NULL)
        {
            printf("there is no basic indution var in this loop\n");
            return;
        }
        // printf("after find_iv,final_iv:\n");
        // print_ivlist(final_iv);
        //遍历final_iv，找到k不为1的induction_var，将其初始化语句增加在ifz和label间的循环初始化部分（loopbegin是label吧）
        for(iv_list * curx=final_iv;curx!=NULL;curx=curx->next)
        {
            for(iv_list * iv_cur=curx;iv_cur!=NULL;iv_cur=iv_cur->samex_next)
            {
                if(iv_cur->k!=1)//i=0 ; i=i+1 j=2*i+2  --->i=0 j=2;   j=j+2  
                {
                    iv_list* cur_basic=in_final_iv(iv_cur->x);

                    //tac码处理部分  修改原本tac以及加上初始化
                    if(iv_cur->con==0)// t=i*c  j=t
                    {
                        //初始化部分把j=i*c放到循环前
                        SYM *tmp = mk_tmp(); // 创建临时变量还要有一个var t的tac
                        SYM *conc=mk_const( iv_cur->k);
                        SYM *vary=mk_var2(iv_cur->y);//j
                        SYM *varx=mk_var2(iv_cur->x);//i                        
                        TAC *code1 = mk_tac(TAC_VAR, tmp, NULL, NULL); // 生成 var t 的 TAC 代码
                        TAC *code2 = mk_tac(TAC_MUL, tmp, varx, conc); // 生成 t = i * c 的 TAC 代码
                        TAC *code3 = mk_tac(TAC_COPY, vary,tmp, NULL); // 生成 j = t 的 TAC 代码
                        code1->next=code2;
                        code2->prev=code1;
                        code2->next=code3;
                        code3->prev=code2;
                        code1->prev=loop_cur->begin_label->prev;
                        loop_cur->begin_label->prev->next=code1;
                        code3->next=loop_cur->begin_label;
                        loop_cur->begin_label->prev=code3;

                        //修改t=i*c  j=t
                        TAC* tac_cur=iv_cur->begin_tac;
                        //print_tac(tac_cur,NULL,1);//处理前 t=i*c  j=t
                        tac_cur->b=tac_cur->next->a;
                        tac_cur->op=TAC_ADD;
                        tac_cur->c->value=iv_cur->change;
                        tac_cur->c->type=SYM_INT;
                        //print_tac(tac_cur,NULL,1);//处理后 t=j+c  j=t
                    }
                    if(iv_cur->con!=0)
                    {
                        //初始化部分把j=i*c+c2放到循环前,即var t t=i*c ; var t2 ; t2=t+c2 ;j=t2   5条
                        SYM *tmp1 = mk_tmp(); 
                        SYM *tmp2 = mk_tmp();
                        SYM *c1=mk_const( iv_cur->k);
                        SYM *c2=mk_const( iv_cur->con); 
                        SYM *vary=mk_var2(iv_cur->y);
                        SYM *varx=mk_var2(iv_cur->x);
                        TAC *code1 = mk_tac(TAC_VAR, tmp1, NULL, NULL); //  var t 
                        TAC *code2 = mk_tac(TAC_MUL, tmp1, varx, c1); //  t = i * c 
                        TAC *code3 = mk_tac(TAC_VAR, tmp2, NULL, NULL); //  var t2  
                        TAC *code4 = mk_tac(TAC_ADD, tmp2, tmp1, c2); //  t2 = t+c2
                        TAC *code5 = mk_tac(TAC_COPY, vary,tmp2, NULL); //  j = t2  
                        code1->next=code2;
                        code2->prev=code1;
                        code2->next=code3;
                        code3->prev=code2;
                        code3->next=code4;
                        code4->prev=code3;
                        code4->next=code5;
                        code5->prev=code4;
                        code1->prev=loop_cur->begin_label->prev;
                        loop_cur->begin_label->prev->next=code1;
                        code5->next=loop_cur->begin_label;
                        loop_cur->begin_label->prev=code5;        

                        //修改（var t） t=i*c ; var t2 ; t2=t+c2 ;j=t2
                        TAC* tac_cur=iv_cur->begin_tac;
                        TAC* tac_end=iv_cur->begin_tac->next->next->next;
                        //print_tac(tac_cur,NULL,1);
                        SYM * a,*b,*c;
                        tac_end->b=tac_cur->a;
                        tac_cur->b=tac_end->a;
                        tac_cur->op=TAC_ADD;
                        tac_cur->c->value=iv_cur->change;
                        tac_cur->c->type=SYM_INT;
                        //print_tac(tac_cur,NULL,1);//（var t） t=j+c ; var t2 ; t2=j+c2 ;j=t(中间两个tac应该会被优化掉)
                    }
                    iv_cur->x=iv_cur->y;
                    iv_cur->con=iv_cur->change;
                    iv_cur->k=1;
                }
                /*
                if(iv_cur->k==1&&strcmp(iv_cur->x,iv_cur->y))//strcmp相等返回0 不对，加法型的依赖变量不需要强度削弱
                {
                    //t=i+c j=t
                    iv_list* cur_basic=in_final_iv(iv_cur->x);
                    //初始化部分把j=i*c放到循环前
                    SYM *tmp = mk_tmp(); // 创建临时变量还要有一个var t的tac
                    TAC *code1 = mk_tac(TAC_VAR, tmp, NULL, NULL); // 生成 var t 的 TAC 代码
                    TAC *code2 = mk_tac(TAC_ADD, tmp, iv_cur->x, iv_cur->con); // 生成 t = i+c 的 TAC 代码
                    TAC *code3 = mk_tac(TAC_COPY, iv_cur->y,tmp, NULL); // 生成 j = t 的 TAC 代码
                    code1->next=code2;
                    code2->prev=code1;
                    code2->next=code3;
                    code3->prev=code2;
                    code1->prev=loop_cur->begin_label->prev;
                    loop_cur->begin_label->prev->next=code1;
                    code3->next=loop_cur->begin_label;
                    loop_cur->begin_label->prev=code3;

                    //修改t=i+c  j=t
                    TAC* tac_cur=iv_cur->begin_tac;
                    print_tac(tac_cur,NULL,1);//处理前 t=i+c  j=t
                    SYM * a,*b,*c;
                    tac_cur->b->name=tac_cur->next->a->name;
                    tac_cur->op=TAC_ADD;
                    tac_cur->c->value=iv_cur->change;
                    tac_cur->c->type=SYM_INT;
                    print_tac(tac_cur,NULL,1);//处理后 t=j+c  j=t 
                }
                */
            }
        }
    }
}

// void strenth_reduce() {
//     for (loop_record* loop_cur = loop_list; loop_cur != NULL; loop_cur = loop_cur->next) {
//         basic_iv = NULL;
//         final_iv = NULL;
//         // 调用find_iv构建好final_iv
//         find_iv(loop_cur);
//         if (basic_iv == NULL) {
//             printf("There is no basic induction variable in this loop\n");
//             continue;
//         }
//         // 遍历final_iv，找到k不为1的induction_var，将其初始化语句增加在ifz和label间的循环初始化部分
//         for (iv_list *curx = final_iv; curx != NULL; curx = curx->next) {
//             for (iv_list *iv_cur = curx; iv_cur != NULL; iv_cur = iv_cur->samex_next) {
//                 if (iv_cur->k != 1) { // i=0 ; i=i+1 j=2*i+2  ---> i=0 j=2; j=j+2
//                     iv_list* cur_basic = in_final_iv(iv_cur->x);
//                     if (!cur_basic) {
//                         printf("No final induction variable found for %s\n", iv_cur->x);
//                         continue;
//                     }

//                     // TAC 代码处理部分，修改原本TAC以及加上初始化
//                     if (iv_cur->con == 0) { // t=i*c  j=t
//                         // 初始化部分把j=i*c放到循环前
//                         SYM *tmp = mk_tmp(); // 创建临时变量
//                         SYM *conc = mk_const(iv_cur->k);
//                         SYM *vary = mk_var2(iv_cur->y); // j
//                         SYM *varx = mk_var2(iv_cur->x); // i
                        
//                         TAC *code1 = mk_tac(TAC_VAR, tmp, NULL, NULL); // 生成 var t 的 TAC 代码
//                         TAC *code2 = mk_tac(TAC_MUL, tmp, varx, conc); // 生成 t = i * c 的 TAC 代码
//                         TAC *code3 = mk_tac(TAC_COPY, vary, tmp, NULL); // 生成 j = t 的 TAC 代码

//                         // 调试信息
//                         printf("Inserting initialization code for %s\n", iv_cur->y);

//                         // 插入TAC代码
//                         code1->next = code2;
//                         code2->prev = code1;
//                         code2->next = code3;
//                         code3->prev = code2;
//                         code1->prev = loop_cur->begin_label->prev;
//                         loop_cur->begin_label->prev->next = code1;
//                         code3->next = loop_cur->begin_label;
//                         loop_cur->begin_label->prev = code3;

//                         // 修改t=i*c j=t
//                         TAC* tac_cur = iv_cur->begin_tac;
//                         if (tac_cur->next) {
//                             tac_cur->b = tac_cur->next->a;
//                             tac_cur->op = TAC_ADD;
//                             tac_cur->c->value = iv_cur->change;
//                             tac_cur->c->type = SYM_INT;
//                         } else {
//                             printf("Null pointer encountered in TAC manipulation for %s\n", iv_cur->y);
//                         }
//                     } else {
//                         // 初始化部分把j=i*c+c2放到循环前
//                         SYM *tmp1 = mk_tmp();
//                         SYM *tmp2 = mk_tmp();
//                         SYM *c1 = mk_const(iv_cur->k);
//                         SYM *c2 = mk_const(iv_cur->con);
//                         SYM *vary = mk_var2(iv_cur->y);
//                         SYM *varx = mk_var2(iv_cur->x);

//                         TAC *code1 = mk_tac(TAC_VAR, tmp1, NULL, NULL); // var t
//                         TAC *code2 = mk_tac(TAC_MUL, tmp1, varx, c1); // t = i * c
//                         TAC *code3 = mk_tac(TAC_VAR, tmp2, NULL, NULL); // var t2
//                         TAC *code4 = mk_tac(TAC_ADD, tmp2, tmp1, c2); // t2 = t + c2
//                         TAC *code5 = mk_tac(TAC_COPY, vary, tmp2, NULL); // j = t2

//                         // 调试信息
//                         printf("Inserting initialization code for %s with constant\n", iv_cur->y);

//                         // 插入TAC代码
//                         code1->next = code2;
//                         code2->prev = code1;
//                         code2->next = code3;
//                         code3->prev = code2;
//                         code3->next = code4;
//                         code4->prev = code3;
//                         code4->next = code5;
//                         code5->prev = code4;
//                         code1->prev = loop_cur->begin_label->prev;
//                         loop_cur->begin_label->prev->next = code1;
//                         code5->next = loop_cur->begin_label;
//                         loop_cur->begin_label->prev = code5;

//                         // 修改（var t） t=i*c ; var t2 ; t2=t+c2 ;j=t2
//                         TAC* tac_cur = iv_cur->begin_tac;
//                         if (tac_cur->next && tac_cur->next->next && tac_cur->next->next->next) {
//                             TAC* tac_end = tac_cur->next->next->next;
//                             tac_end->b = tac_cur->a;
//                             tac_cur->b = tac_end->a;
//                             tac_cur->op = TAC_ADD;
//                             tac_cur->c->value = iv_cur->change;
//                             tac_cur->c->type = SYM_INT;
//                         } else {
//                             printf("Null pointer encountered in TAC manipulation for %s\n", iv_cur->y);
//                         }
//                     }
//                     iv_cur->x = iv_cur->y;
//                     iv_cur->con = iv_cur->change;
//                     iv_cur->k = 1;
//                 }
//             }
//         }
//     }
// }

// 删除归纳变量
void induction_variable_elimation(){
    // printf("basic iv\n");
    // for(iv_list* cur = basic_iv; cur!=NULL; cur=cur->next){
        // printf("%s=%d*%s+%d\n", cur->y, cur->k, cur->x, cur->con);
    // }
    // printf("final iv\n");
    // for(iv_list* cur = final_iv; cur!=NULL; cur=cur->next){
        // printf("%s=%d*%s+%d\n", cur->y, cur->k, cur->x, cur->con);
    // }
    loop_record* loop_records = loop_list;
    for(loop_records=loop_list;loop_records!=NULL;loop_records=loop_records->next){
        int loop2BB_id = find_BB(loop_records->end_label->id, bb_num);
        // printf("loop records: %d\n", loop2BB_id);
        R_node *res[bb_num];
        SYM *def_vars[MAX_var];
        int res_def_index;
        Live_Var(BB_array, bb_num, tac_first, res, def_vars, &res_def_index);
        printf("BB %d Live Varibles: ", loop2BB_id);
        for (int i = 0; i < res_def_index; i++) {
            if (res[loop2BB_id]->in_vector[i] == 1) {
                printf("%s ", def_vars[i]->name);
            }
        }
        // find_BB 是 查询TAC所在的Basic Block
        // Live_Var 是 查询TAC所在的
        //仅支持单循环
        typedef struct iv_valid{
            char* name;
            int valid;
            iv_list* iv;
            int init;
            struct iv_valid* next;
            struct iv_valid* prev;
        }iv_valid;
        iv_valid* iv_valid_lists = NULL;
        int iv_valid_lists_cnt = 0;
        iv_valid* target_iv=NULL;
        for(iv_list* cur = final_iv; cur!=NULL; cur=cur->next){
            iv_valid_lists_cnt += 1;
            iv_valid* new_iv_valid = (iv_valid*)malloc(sizeof(iv_valid));
            new_iv_valid->name = cur->y;
            new_iv_valid->iv = cur;
            new_iv_valid->next = NULL;
            new_iv_valid->prev = NULL;
            new_iv_valid->valid = 0;
            for (int i = 0; i < res_def_index; i++) {
                if (strcmp(new_iv_valid->name, def_vars[i]->name) == 0) {
                    printf("%s\n", def_vars[i]->name);
                    new_iv_valid->valid = res[loop2BB_id]->in_vector[i];
                    if(new_iv_valid->valid){
                        target_iv = new_iv_valid;
                    }
                }
            }
            //
            if(iv_valid_lists == NULL){
                iv_valid_lists = new_iv_valid;
            }else{
                iv_valid_lists->next = new_iv_valid;
                new_iv_valid->prev = iv_valid_lists;
                iv_valid_lists = new_iv_valid;
            }
            //延生链表
        }
        if(target_iv==NULL){
            printf("No replace choice\n");
            continue;
        }
        //判定iv是否为活跃变量
        for(TAC* cur=loop_records->begin_label->prev; cur!=NULL;cur=cur->prev){
            if(iv_valid_lists_cnt = 0){
                break;
            }
            for(iv_valid* cur_iv_valid=iv_valid_lists;cur_iv_valid!=NULL;cur_iv_valid=cur_iv_valid->prev){
                if(cur->op == TAC_COPY && strcmp(cur_iv_valid->name, cur->a->name) == 0){
                    cur_iv_valid->init = cur->b->value;
                    iv_valid_lists_cnt -= 1;
                }
            }
        }
        //找到iv的初值
        //
        //优化循环
        char* target_name=loop_records->end_label->prev->prev->b->name; // 判定条件
        printf("target name %s\n",target_name);
        iv_valid* source_iv=NULL;
        int opt_finished=0;
        int if_find=0;
        for(iv_valid* cur_iv_valid=iv_valid_lists;cur_iv_valid!=NULL;cur_iv_valid=cur_iv_valid->prev){
            if(strcmp(cur_iv_valid->name, target_name) == 0){
                if_find=1;
                source_iv=cur_iv_valid;
                if(cur_iv_valid->valid){
                    opt_finished=1;
                }
                break;
            }
        }
        if(opt_finished){
            continue;
        }
        if(if_find==0){
            for(TAC* cur=loop_records->end_label->prev;cur!=NULL&&cur!=loop_records->begin_label;cur=cur->prev){
                if(cur->op == TAC_COPY){
                    if(strcmp(cur->b->name, target_name) == 0){
                        target_name=cur->a->name;
                        break;
                    }
                }    
            }
        }
        for(iv_valid* cur_iv_valid=iv_valid_lists;cur_iv_valid!=NULL;cur_iv_valid=cur_iv_valid->prev){
            if(strcmp(cur_iv_valid->name, target_name) == 0){
                if_find=1;
                source_iv=cur_iv_valid;
                if(cur_iv_valid->valid){
                    opt_finished=1;
                }
                break;
            }
        }
        if(if_find==0){
            printf("error\n");
            break;
        }
        if(opt_finished){
            continue;
        }//结束优化
        else{
        //开始优化
            int target_value=loop_records->end_label->prev->prev->c->value;
            target_value-=source_iv->init;
            int tmp_k=target_iv->iv->con/source_iv->iv->con;
            target_value*=tmp_k;
            target_value+=target_iv->init;
            loop_records->end_label->prev->prev->c->value=target_value;
            SYM* target_expression;
            for(TAC* cur=loop_records->begin_label;cur!=NULL&&cur!=loop_records->end_label;cur=cur->next){
                if(strcmp(target_iv->name, cur->a->name) == 0){
                    target_expression=cur->a;
                }
            }
            loop_records->end_label->prev->prev->b=target_expression;
        }
        for(TAC* cur=loop_records->begin_label;cur!=NULL&&cur!=loop_records->end_label;cur=cur->next){
            if (cur->a && strcmp(cur->a->name, source_iv->name) == 0){

                // 检查 cur->next 和 cur->prev 是否为空
                if (cur->prev) {
                    cur->prev->next = cur->next;
                }
                if (cur->next) {
                    cur->next->prev = cur->prev;
                }
            }
        }
        //优化结束
    }
}

/*###########################总优化接口部分############################*/

static const struct Optimizers do_opt = {
    .simple_opt = simple_opt,    //简单优化
    .const_opt = const_opt,      //常量传播+常量折叠优化   (局部)
    .copy_propagaton_opt = copy_opt,  //全局和局部复制传播优化
    .deadcode_opt = deadcode_opt,    // 死代码消除    （全局）
    .deadvar_opt = deadvar_opt,     // deadassign消除:消除无用赋值
    .local_comsub_opt = part_comsub_opt,  // 公共子表达式消除(局部)
    .global_comsub_opt = global_comsub_opt,  //全局公共子表达式消除
};

int tac_optimizer(){
    /*调试文件*/
    FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        // 如果文件打开失败，打印错误消息并退出程序
        perror("Unable to open file");
        return 1;
    }

    int BB_num=0;
    // All things done, now we begin to do optimize!
    int opt_cnt;
    int round = 1;
    do{ 
        BB_num = build_CFG(tac_first);
        bb_num = BB_num;
        fprintf(file, "bb_num:%d,round:%d\n",bb_num,round);
        printf("bb_num:%d,round:%d\n",bb_num,round);

        opt_cnt = 1;
        
        //简单优化 X = X
        opt_cnt += do_opt.simple_opt(tac_first);
        fprintf(file, "\t simple opt done!\n");

        for(TAC *cur = tac_first; cur != NULL; cur = cur->next){
        tac_file_print(cur, file);
        fprintf(file, "\n");
        }

        strenth_reduce();
        BB_num = build_CFG(tac_first);
        bb_num=BB_num;
        fprintf(file, "\t strength reduce done!\n");

        for(TAC *cur = tac_first; cur != NULL; cur = cur->next){
        tac_file_print(cur, file);
        fprintf(file, "\n");
        }

        //  // 常量传播+常量折叠优化
        opt_cnt += do_opt.const_opt(BB_array, BB_num, tac_first);
        fprintf(file, "\t const_opt done!\n");

        for(TAC *cur = tac_first; cur != NULL; cur = cur->next){
        tac_file_print(cur, file);
        fprintf(file, "\n");
        }

        // 全局和局部复制传播优化
        opt_cnt += do_opt.copy_propagaton_opt(BB_array, BB_num, tac_first);
        fprintf(file, "\t copy_propagaton_opt done!\n");

        for(TAC *cur = tac_first; cur != NULL; cur = cur->next){
        tac_file_print(cur, file);
        fprintf(file, "\n");
        }

        // // 死代码消除
        opt_cnt += do_opt.deadcode_opt(BB_array,BB_num,tac_first);
        fprintf(file, "\t deadcode_opt done!\n");

        // // deadassign消除
        opt_cnt += do_opt.deadvar_opt(BB_array,BB_num,tac_first);
        fprintf(file, "\t deadvar_opt done!\n");

        for(TAC *cur = tac_first; cur != NULL; cur = cur->next){
        tac_file_print(cur, file);
        fprintf(file, "\n");
        }

        // 公共子表达式消除
        opt_cnt += do_opt.local_comsub_opt(BB_array, BB_num);
        fprintf(file, "\t local_comsub_opt done!\n");

        for(TAC *cur = tac_first; cur != NULL; cur = cur->next){
        tac_file_print(cur, file);
        fprintf(file, "\n");
        }

        opt_cnt += do_opt.global_comsub_opt(BB_array,BB_num,tac_first);
        fprintf(file, "\t global_comsub_opt done!\n");

        for(TAC *cur = tac_first; cur != NULL; cur = cur->next){
        tac_file_print(cur, file);
        fprintf(file, "\n");
        }


        loop_invariant();
        // fprintf(file, "\t loop_invariant opt done!\n"); 

        // for(TAC *cur = tac_first; cur != NULL; cur = cur->next){
        // tac_file_print(cur, file);
        // fprintf(file, "\n");
        // }

        // induction_variable_elimation();

        fprintf(file, "\t induction variable elimation done!\n"); 

        for(TAC *cur = tac_first; cur != NULL; cur = cur->next){
        tac_file_print(cur, file);
        fprintf(file, "\n");
        }
        round ++;
        fprintf(file, "***************************************************************");
        fprintf(file, "\n");
        for(int i=0;i<bb_num;i++){
            A_node * del_tmp;
            del_tmp=BB_array[i]->prev;
            while(del_tmp){
                del_tmp=del_tmp->next;
                free(BB_array[i]->prev);
                BB_array[i]->prev=del_tmp;
            }
            del_tmp=BB_array[i]->suc;
            while(del_tmp){
                del_tmp=del_tmp->next;
                free(BB_array[i]->suc);
                BB_array[i]->suc=del_tmp;
            }
            free(BB_array[i]);
        }
    }while(opt_cnt > 0 && round <= MAX_ROUND);
   fprintf(file, "opt done!\n");
   printf("opt done!\n");

}

void print_tac(TAC *tac, char *str , int flag){ /*居中打印tac,if tac==null,则打印str  例： |     tac      |          */
    
    int llen=47;
    char * tmp=(char *)malloc(sizeof(char)*(llen+1) );
    for(int i =0;i<llen;i++){
        if(i==0 &&flag==1)tmp[i]='|';
        else if(i==llen-1 && flag==1)tmp[llen-1]='|';
        else if(i==llen  )tmp[llen ]='\0';
        else tmp[i]=' ';
    }

    char * tac_code;
    if(tac!=NULL){
        tac_code=(char *)malloc(sizeof(char)*(llen+1) );
        tac_sprintf(tac , tac_code);
    }else  tac_code=str;
    
    int slen=strlen(tac_code);
    int pos=(llen-slen)/2;

    strcpy(tmp+pos,tac_code);
    tmp[pos+slen]=     (pos+slen==llen-1 && flag==1) ? '|' : ' '     ;
    printf("%s\n",tmp );
    free(tmp);
     if(tac!=NULL)free(tac_code);

}
void print_BB(BB * bb){
    int slen=47;
    char str[slen];
    /*上边界  __*/
    for(int i=0;i<slen-1;i++)str[i]=i==slen-2?'\0': '_';
    print_tac(NULL,str,0);
   /*id*/
    sprintf(str,"id=%d                                    ",bb->id);
    print_tac(NULL,str,1);
    /*tac码部分*/
    
    // print_tac(NULL,"                 ",1);
    print_tac(bb->in,NULL,1);
    print_tac(NULL,"......",1);
    print_tac(bb->out,NULL,1);
    
    /*入口前驱和出口后继*/
    int num=0;
    if(bb->prev==NULL)num=0;
    else if(bb->prev->next==NULL)num=1;
    else if(bb->prev->next->next==NULL)num=2;
    if(num==0)      sprintf(str,"innum=%d: ; ",num);
    else if(num==1) sprintf(str,"innum=%d:%d ; ",num,bb->prev->n);
    else if(num==2) sprintf(str,"innum=%d:%d,%d ; ",num,bb->prev->n,bb->prev->next->n);
    // print_tac(NULL,str,1);
    char str2[slen];
    if(bb->suc==NULL)num=0;
    else if(bb->suc->next==NULL)num=1;
    else if(bb->suc->next->next==NULL)num=2;
    if(num==0)      sprintf(str2,"outnum=%d: ;",num);
    else if(num==1) sprintf(str2,"outnum=%d:%d ;",num,bb->suc->n);
    else if(num==2) sprintf(str2,"outnum=%d:%d,%d ;",num,bb->suc->n,bb->suc->next->n);
    strcat(str,str2);
    print_tac(NULL,str,1);
 

    /*下边界  ___*/
    for(int i=0;i<slen-1;i++)str[i]=i==slen-2?'\0': '_';
    print_tac(NULL,str,1);

}
void print_alltac()
{
    for(TAC* cur=tac_first;cur!=NULL;cur=cur->next)
    {
        tac_print(cur);
        printf("\n");
    }
}
void print_alltac_fromprev()
{
    for(TAC* cur=tac_last;cur!=NULL;cur=cur->prev)
    {
        tac_print(cur);
        printf("\n");
    }
}
void print_cfg( ){
    for(int i=0;i<bb_num;i++){
        print_BB(BB_array[i]);
    }
}

void print_iv(iv_list *iv_cur)
{
    if(iv_cur==NULL)
    {
        printf("this iv is null");
    }
    else
    {
    printf("y:%s\n",iv_cur->y);
    printf("k:%d\n",iv_cur->k);
    printf("x:%s\n",iv_cur->x);
    printf("con:%d\n",iv_cur->con);
    printf("change:%d\n",iv_cur->change);
    }
}

void print_ivlist(iv_list *begin)
{
    if(begin==NULL)
    {
        printf("there is no iv in\n");
    }
    else
    {
    for(iv_list * curx=begin;curx!=NULL;curx=curx->next)
    {
        for(iv_list * iv_cur=curx;iv_cur!=NULL;iv_cur=iv_cur->samex_next)
        {
            print_iv(iv_cur);
        }
    }
    }
}