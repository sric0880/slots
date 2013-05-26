#ifndef __C_SPIN_H
#define __C_SPIN_H
/**
spin接口详细定义
*参数：
	backgd：场景（从1开始编号）
	chpt_main：checkpoints 主关卡(从1开始编号1 2 3 4 5 6)
	chpt_sec：二级关卡 （从1开始编号）
	lines：赌线条数
	coins：倍数
	results：3*5的数组 一行5个图片 3行
		不同的图片以数字表示，如下：
			0:wild
			1:scatter
			2:pic01
			3:pic02
			4:pic03
			5-a 6-k 7-q,8-j,9-10,10-9
	numOfhittedLines：命中赌线的条数
		即hittedLines和firstHittedPics数组的个数
		如果numOfhittedLines为0，hittedLines和firstHittedPics为NULL
	hittedLines：命中的赌线（25条赌线，从1开始编号）
	firstHittedPics：对应每一条赌线中，前x（x>=2）个连续的pics，x的数组
	state：引用
		0: 正常
		1：bonus game
		2：big win
	boosters：加倍道具，默认为1
返回值:赢得的金币数量
	0 表示没有赢（不返回赌注的数额）
	>0 表示赢得金币数量
*/
int spin(int backgd, int chpt_main, int chpt_sec, int lines, int coins, int results[],
		 int &numOfhittedLines, int *&hittedLines, int *&firstHittedPics, int &state,
		 int boosters=1);
#endif // !__C_SPIN_H

