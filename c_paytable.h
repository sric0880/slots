/**
*global variables
*/
#ifndef __C_PAYTABLE_H
#define __C_PAYTABLE_H
#include "c_global.h"
/**
*0-wild 1-scatter 2,3,4 for pictures and 5-a 6-k 7-q,8-j,9-10,10-9
*/
//enum symbol{wild,scatter,slarge,smid,ssmall,a,k,q,j,t,n};
const int paytable[symbol_size][reel_lenght]={
	{0,5,50,500,5000},
	{0,0,10,25,50},
	{0,2,40,200,1000},
	{0,2,30,150,500},
	{0,2,25,100,300},
	{0,0,20,75,200},
	{0,0,20,75,200},
	{0,0,20,75,200},
	{0,0,20,75,200},
	{0,0,10,25,50},
	{0,0,10,25,50}
}; 
#endif // !__C_PAYTABLE_H

