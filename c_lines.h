/**
*global variables
*/
//lines defination
#ifndef __C_LINES_H
#define __C_LINES_H
#include "c_global.h"
const int max_lines_num=25;
const int line_1[5]={1,1,1,1,1};
const int line_2[5]={0,0,0,0,0};
const int line_3[5]={2,2,2,2,2};
const int line_4[5]={0,1,2,1,0};
const int line_5[5]={2,1,0,1,2};
const int line_6[5]={1,0,0,0,1};
const int line_7[5]={1,2,2,2,1};
const int line_8[5]={0,0,1,2,2};
const int line_9[5]={2,2,1,0,0};
const int line_10[5]={1,0,1,2,1};
const int line_11[5]={1,2,1,0,1};
const int line_12[5]={0,1,1,1,0};
const int line_13[5]={2,1,1,1,2};
const int line_14[5]={0,1,0,1,0};
const int line_15[5]={2,1,2,1,2};
const int line_16[5]={1,1,0,1,1};
const int line_17[5]={1,1,2,1,1};
const int line_18[5]={0,0,2,0,0};
const int line_19[5]={2,2,0,2,2};
const int line_20[5]={0,2,2,2,0};
const int line_21[5]={2,0,0,0,2};
const int line_22[5]={1,2,0,2,1};
const int line_23[5]={1,0,2,0,1};
const int line_24[5]={0,2,0,2,0};
const int line_25[5]={2,0,2,0,2};

//sort by 1,2,...
const int *line[max_lines_num]={
	line_1,
	line_2,
	line_3,
	line_4,
	line_5,
	line_6,
	line_7,
	line_8,
	line_9,
	line_10,
	line_11,
	line_12,
	line_13,
	line_14,
	line_15,
	line_16,
	line_17,
	line_18,
	line_19,
	line_20,
	line_21,
	line_22,
	line_23,
	line_24,
	line_25
};

//convert from line defination to reel-strip index
void convert(const int * line, int* des, const int reel_lenght){
	for(int i=0;i<reel_lenght;++i){
		des[i]=line[i]*reel_lenght+i;
	}
}
#endif // !__C_LINES_H
