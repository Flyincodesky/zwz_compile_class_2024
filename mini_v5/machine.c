#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "opcode.h"

#define REGMAX 16
#define MEMMAX (256 * 256)
#define R_FLAG 0
#define R_IP 1
#define FLAG_EZ 0
#define FLAG_LZ 1
#define FLAG_GZ 2

int reg[REGMAX]; /* registers */
unsigned char mem[MEMMAX]; /* memory */
int op, rx, ry, constant; /* opcode, register1, register2, immediate constant */
int total_circles;//程序执行的所有指令的总周期数
int cost_circles[256];//存储每条指令的周期数

void circles_set()
{
    cost_circles[0x00] = 1;   // I_END
    cost_circles[0x01] = 1;   // I_NOP
    cost_circles[0x02] = 2;   // I_OUT
    cost_circles[0x10] = 1;   // I_LOD_0
    cost_circles[0x11] = 1;   // I_LOD_1
    cost_circles[0x12] = 2;   // I_LOD_2
    cost_circles[0x13] = 4;   // I_LOD_3
    cost_circles[0x14] = 4;   // I_LOD_4
    cost_circles[0x15] = 5;   // I_LOD_5
    cost_circles[0x20] = 4;   // I_STO_0
    cost_circles[0x21] = 4;   // I_STO_1
    cost_circles[0x22] = 5;   // I_STO_2
    cost_circles[0x23] = 5;   // I_STO_3
    cost_circles[0x30] = 1;   // I_ADD_0
    cost_circles[0x31] = 1;   // I_ADD_1
    cost_circles[0x40] = 1;   // I_SUB_0
    cost_circles[0x41] = 1;   // I_SUB_1
    cost_circles[0x50] = 5;   // I_MUL_0
    cost_circles[0x51] = 5;   // I_MUL_1
    cost_circles[0x60] = 3;   // I_DIV_0
    cost_circles[0x61] = 3;   // I_DIV_1
    cost_circles[0x70] = 1;   // I_TST_0
    cost_circles[0x80] = 1;   // I_JMP_0
    cost_circles[0x81] = 1;   // I_JMP_1
    cost_circles[0x82] = 1;   // I_JEZ_0
    cost_circles[0x83] = 1;   // I_JEZ_1
    cost_circles[0x84] = 1;   // I_JLZ_0
    cost_circles[0x85] = 1;   // I_JLZ_1
    cost_circles[0x86] = 1;   // I_JGZ_0
    cost_circles[0x87] = 1;   // I_JGZ_1
	cost_circles[0xFF] = 1;   // I_MAX
}

void instruction(int ip)
{
	op=mem[ip];
	op=(op << 8) | mem[ip+1];
	rx=mem[ip+2];
	ry=mem[ip+3];
	constant=mem[ip+4];
	constant=(constant << 8) | mem[ip+5];
	constant=(constant << 8) | mem[ip+6];
	constant=(constant << 8) | mem[ip+7];	
}

int main(int argc, char *argv[])
{
	circles_set();
	total_circles=0;
	if(argc!=2) {
		fprintf(stderr, "usage: %s filename\n", argv[0]);
		exit(0);		
	}

	FILE * input=fopen( argv[1], "rb" );
	if( input ==  NULL )
	{
		fprintf(stderr, "error: open %s failed\n", argv[1] );
		exit(0);
	}

	int i, ch, t, t1, t2;

	/* init reg */
	for( i=0; i < REGMAX; i++ ) 
		reg[i]=0;

	/* init mem */
	for( i=0; (ch=fgetc(input)) != EOF; i++ ) 
		mem[i]=(char)ch;
	for( ; i < MEMMAX; i++ ) 
		mem[i]=0;
	
	/* run machine */
	for(;;)
	{
		instruction(reg[R_IP]);
		total_circles+=cost_circles[op];//增加周期数
		switch(op)
		{
			case I_END:
			printf("\n程序总消耗%d个周期数\n",total_circles);
			exit(0);

			case I_NOP:
			break;

			case I_OUT:
			printf( "%c", reg[15] ); /* Print out reg[15] in ASCII */
			break;

			case I_ADD_0:
			reg[rx]=reg[rx] + constant;
			break;

			case I_ADD_1:
			reg[rx]=reg[rx] + reg[ry];
			break;

			case I_SUB_0:
			reg[rx]=reg[rx] - constant;
			break;

			case I_SUB_1:
			reg[rx]=reg[rx] - reg[ry];
			break;

			case I_MUL_0:
			reg[rx]=reg[rx] * constant;
			break;

			case I_MUL_1:
			reg[rx]=reg[rx] * reg[ry];
			break;

			case I_DIV_0:
			if( constant == 0 ) 
			{
				fprintf(stderr, "error: divide by zero\n");
				exit(0);
			} 
			else 
			{
				reg[rx]=reg[rx] / constant;
			}			
			break;

			case I_DIV_1:
			if( reg[ry] == 0 ) 
			{
				fprintf(stderr, "error: divide by zero\n");
				exit(0);
			} 
			else 
			{
				reg[rx]=reg[rx] / reg[ry];
			}			
			break;

			case I_LOD_0:
			reg[rx]=constant;
			break;

			case I_LOD_1://1 circle
			reg[rx]=reg[ry];
			break;

			case I_LOD_2:
			reg[rx]=reg[ry] + constant;
			break;

			case I_LOD_3:
			t1=constant;
			t2=mem[t1     ];
			t2=(t2 << 8) + mem[t1  + 1];
			t2=(t2 << 8) + mem[t1  + 2];
			t2=(t2 << 8) + mem[t1  + 3];
			reg[rx]=t2;
			break;

			case I_LOD_4:
			t=mem[reg[ry]    ];
			t=(t << 8) + mem[reg[ry] + 1];
			t=(t << 8) + mem[reg[ry] + 2];
			t=(t << 8) + mem[reg[ry] + 3];
			reg[rx]=t;
			break;

			case I_LOD_5:
			t1=reg[ry] + constant;
			t2=mem[t1     ];
			t2=(t2 << 8) + mem[t1  + 1];
			t2=(t2 << 8) + mem[t1  + 2];
			t2=(t2 << 8) + mem[t1  + 3];
			reg[rx]=t2;
			break;

			case I_STO_0:
			mem[reg[rx]]=constant >> 24;
			mem[reg[rx] + 1]=constant >> 16 & 0xff;
			mem[reg[rx] + 2]=constant >>  8 & 0xff;
			mem[reg[rx] + 3]=constant       & 0xff;
			break;

			case I_STO_1:
			mem[reg[rx]]=reg[ry] >> 24;
			mem[reg[rx] + 1]=reg[ry] >> 16 & 0xff;
			mem[reg[rx] + 2]=reg[ry] >>  8 & 0xff;
			mem[reg[rx] + 3]=reg[ry]       & 0xff;
			break;

			case I_STO_2:
			t=reg[ry]+constant;
			mem[reg[rx]]=t >> 24;
			mem[reg[rx] + 1]=t >> 16 & 0xff;
			mem[reg[rx] + 2]=t >>  8 & 0xff;
			mem[reg[rx] + 3]=t       & 0xff;
			break;

			case I_STO_3:
			t=reg[rx] + constant;
			mem[t]=reg[ry] >> 24;
			mem[t + 1]=reg[ry] >> 16 & 0xff;
			mem[t + 2]=reg[ry] >>  8 & 0xff;
			mem[t + 3]=reg[ry]       & 0xff;
			break;

			case I_TST_0:
			t=reg[rx];
			if(t==0) reg[R_FLAG]=FLAG_EZ;
			else if(t<0) reg[R_FLAG]=FLAG_LZ;
			else if(t>0) reg[R_FLAG]=FLAG_GZ;
			break;

			case I_JMP_0:
			reg[R_IP]=constant;
			continue;

			case I_JMP_1:
			reg[R_IP]=reg[rx];
			continue;

			case I_JEZ_0:
			if(reg[R_FLAG]==FLAG_EZ) { reg[R_IP]=constant; continue; }
			else break;
			
			case I_JEZ_1:
			if(reg[R_FLAG]==FLAG_EZ) { reg[R_IP]=reg[rx]; continue; }
			else break;

			case I_JLZ_0:
			if(reg[R_FLAG]==FLAG_LZ) { reg[R_IP]=constant; continue; }
			else break;

			case I_JLZ_1:
			if(reg[R_FLAG]==FLAG_LZ) { reg[R_IP]=reg[rx]; continue; }
			else break;

			case I_JGZ_0:
			if(reg[R_FLAG]==FLAG_GZ) { reg[R_IP]=constant; continue; }
			else break;

			case I_JGZ_1:
			if(reg[R_FLAG]==FLAG_GZ) { reg[R_IP]=reg[rx]; continue; }
			else break;

			default:
			fprintf(stderr, "error: invalid opcode %02x\n", op);
			exit(0);
		}
		
		reg[R_IP]=reg[R_IP]+8; /* next instruction */
	}
	
	return 0;
}


