#ifndef __C_SPIN_H
#define __C_SPIN_H
/**
spin�ӿ���ϸ����
*������
	backgd����������1��ʼ��ţ�
	chpt_main��checkpoints ���ؿ�(��1��ʼ���1 2 3 4 5 6)
	chpt_sec�������ؿ� ����1��ʼ��ţ�
	lines����������
	coins������
	results��3*5������ һ��5��ͼƬ 3��
		��ͬ��ͼƬ�����ֱ�ʾ�����£�
			0:wild
			1:scatter
			2:pic01
			3:pic02
			4:pic03
			5-a 6-k 7-q,8-j,9-10,10-9
	numOfhittedLines�����ж��ߵ�����
		��hittedLines��firstHittedPics����ĸ���
		���numOfhittedLinesΪ0��hittedLines��firstHittedPicsΪNULL
	hittedLines�����еĶ��ߣ�25�����ߣ���1��ʼ��ţ�
	firstHittedPics����Ӧÿһ�������У�ǰx��x>=2����������pics��x������
	state������
		0: ����
		1��bonus game
		2��big win
	boosters���ӱ����ߣ�Ĭ��Ϊ1
����ֵ:Ӯ�õĽ������
	0 ��ʾû��Ӯ�������ض�ע�����
	>0 ��ʾӮ�ý������
*/
int spin(int backgd, int chpt_main, int chpt_sec, int lines, int coins, int results[],
		 int &numOfhittedLines, int *&hittedLines, int *&firstHittedPics, int &state,
		 int boosters=1);
#endif // !__C_SPIN_H

