#include "c_spin.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(int argv, char * args[]){
	
//	int totalCoins=1000;
	int result[15];
	int numOfhittedLines;
	int *hittedLines;
	int *firstHittedPics;
	int state;
	for(int i=0;i<100;++i){
		int coins = spin(1, 1, 2, 25, 6, result,
			numOfhittedLines, hittedLines, firstHittedPics, state,1);
		if(coins!=0)
			printf("%d:win\t: %d.\r\n",i,coins);
		else
			printf("%d:lose\r\n",i);
		printf("hitted lines: %d.\r\n",numOfhittedLines);
		for(int i=0;i<numOfhittedLines;++i)
		{
			printf("%d-%d ",hittedLines[i],firstHittedPics[i]);
		}
		printf("\r\nstate: %d.\r\n",state);
		for(int i=0;i<3;++i)
		{
			for(int j=0;j<5;++j)
			{
				printf("%d ",result[i*5+j]);
			}
			printf("\r\n");
		}
	}
	/*float lamda=0.8f;
	const int num = 14;*/
	/*
	float sum=0.0f;
	
	float p[num];
	for(int i=0;i<num;++i){
		p[i]=lamda*exp(-lamda*i);
		sum+=p[i];
		
	}
	float summ=0.0f;
	for(int i=0;i<num;++i){
		float pp=p[i]/sum;
		summ+=pp;
		printf("%f\r\n", pp);
	}
	printf("sum: %f\r\n", summ);*/
	//srand((unsigned)time(NULL));
	/*char a='\0';
	scanf("%c",a);*/
	return 0;
}
