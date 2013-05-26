#include "c_spin.h"
#include "c_lines.h"
#include "c_paytable.h"
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//#include <wtypes.h>  
//#include <stdarg.h>  
//#include <tchar.h>
int cur_winrate;//��ǰ��ʤ��
int cur_chpt_main=0;//��ǰ�ؿ�intint
int winrate;//�̶���ʤ��
int counter_play;//��ǰ�Ѿ�����Ĵ���
int counter_win;//��ǰӮ�ô���
const int history_n=20;//��¼���n�ε�Ӯ�ĸ��� nΪ10ʱ��ӽ�ʤ�ʣ�ԽСʤ���½�
int history[history_n];//ǰn��Ӯ�����
int threshold=1;//��ǰʤ�ʺ���ʷʤ�ʵĲ�ֵ����ֵ
const float lamda=0.8f;//ָ���ֲ��Ĳ���
int winratio[]={70,60,50,45,40,35};//ÿ���ؿ���ʤ��
inline const bool random(int percentage);//����Ӯ�ø�����percentage֮��
inline const int expDist(int lines);//��������ۻ�ָ�����ʷֲ�
inline int getSymbol();//���һ��symbol��ÿһ�����ָ��ʲ�һ��
//void LogTrace(LPCTSTR pszFormat, ...);//��debugger���
//void Log(bool iswin, int results[],int numOfhittedLines, int hittedLines[], int firstHittedPics[], int state,int gains);
int spin(int backgd, int chpt_main, int chpt_sec, int lines, int coins, int results[],
		 int &numOfhittedLines, int *&hittedLines, int *&firstHittedPics, int &state,
		 int boosters){
	int multiple_winnings=coins*boosters;
//	int bet=coins*lines;
	int gains=0;//��õĽ�����
	winrate=winratio[chpt_main-1];
	//ȫ�ֳ�ʼ��
	if(cur_chpt_main!=chpt_main){
		srand((unsigned)time(NULL));//�������
		cur_winrate=winrate;
		counter_play=0;
		counter_win=0;
		cur_chpt_main = chpt_main;
	}
	++counter_play;
	//���� Ӯ or ��
	bool iswin=random(winrate);
	if(counter_play>history_n)
	{
		int last_n_win=0;
		int average=history_n*winrate/100;
		for(int i=0; i<history_n;++i)
		{
			if(history[i]==1)
				++last_n_win;
		}
		if((last_n_win-average)>threshold)//ǰn��ʤ��ԶԶ���ڹ̶�ʤ�ʣ�����
		{
			iswin=false;
		}
		else if((average-last_n_win)>threshold)//ǰn��ʤ��ԶԶ���ڹ̶�ʤ�ʣ���Ӯ
		{
			iswin=true;
		}
		if(iswin)
			history[(counter_play-1)%history_n]=1;
		else
			history[(counter_play-1)%history_n]=0;
	}
	else
	{
		if(iswin)
			history[counter_play-1]=1;
		else
			history[counter_play-1]=0;
	}
	if(iswin)
	{
		++counter_win;
	}
	cur_winrate=counter_win*100/counter_play;
	//���ɲ�����Ӯ������
	if(!iswin)
	{
		state=0;
		numOfhittedLines=0;
		hittedLines=NULL;
		firstHittedPics=NULL;
		bool f = true;//���н���״̬
		while(f)
		{
			for(int i=0;i<5;++i)//�������5�е�����
			{
				//ÿһ��û���ظ�������
				results[i]=getSymbol();
				results[5+i]=getSymbol();
				while(results[i]==results[5+i])
				{
					results[5+i]=getSymbol();
				}
				results[10+i]=getSymbol();
				while(results[10+i]==results[i]||results[10+i]==results[5+i])
				{
					results[10+i]=getSymbol();
				}
			}
			//����ǰ3�е����� ��֤û���н�
			f=false;
			for(int i=0;i<lines;++i)
			{
				int a = results[5*line[i][0]];
				int b = results[5*line[i][1]+1];
				int c = results[5*line[i][2]+2];
				if(a==b)
				{
					if(a==0||a==2||a==3||a==4)
					{
						f=true;
						break;
					}
					else
					{
						if(c==b)
						{
							f=true;
							break;
						}
						else if(c==0&&a!=1)
						{
							f=true;
							break;
						}
						else
							continue;
					}
				}
				else
				{
					if((a==0&&b==1)||(a==1&&b==0))
						continue;
					if(a!=0&&b!=0)
						continue;
					if((a==0&&c!=b)||(a!=c&&b==0))
						continue;
					else
					{
						f=true;
						break;
					}
				}
			}
		}
	}
	else
	{
		state=0;
		//�������е�����(����ָ���ֲ�)
		numOfhittedLines=expDist(lines);//�������0~lines֮�������
		//Ȼ�����ѡ��lines
		hittedLines=new int[numOfhittedLines];
		firstHittedPics=new int[numOfhittedLines];
		//�Դ�
		int flag=-1;
		while(flag==-1)
		{
			int potential_hitted = 0;
			//ά��һ�������н����ߵ�����
			//�ǵ��ͷ��ڴ�
			int *potentialLines = new int[lines];
			//for(int i=0;i<numOfhittedLines;++i)
			//{
			//	potentialLines[hittedLines[i]-1]=0;
			//}
			while(potential_hitted<numOfhittedLines)
			{
				potential_hitted=0;
				for(int i=0;i<lines;++i)
				{
					potentialLines[i]=0;
				}
				for(int i=0;i<2;++i)//�������2�е�����
				{
					//ÿһ��û���ظ�������
					results[i]=getSymbol();
					results[5+i]=getSymbol();
					while(results[i]==results[5+i])
					{
						results[5+i]=getSymbol();
					}
					results[10+i]=getSymbol();
					while(results[10+i]==results[i]||results[10+i]==results[5+i])
					{
						results[10+i]=getSymbol();
					}
				}
				//����ǰ���� �������н�������
				for(int i=0;i<lines;++i)
				{
					int row1 = line[i][0];
					int row2 = line[i][1];
					int a = results[row1*5];
					int b = results[row2*5+1];
					if(a!=b)
					{
						if((a==0&&b==1)||(a==1&&b==0))
							continue;
						else if(a!=0&&b!=0)
							continue;
						else
						{
							potentialLines[i]=1;
							++potential_hitted;
						}
					}
					else
					{
						potentialLines[i]=1;
						++potential_hitted;
					}
				}
			}
			//����990��
			int count = 0;
			bool isbreak = false;
			flag=symbol_size-1;
			//����һ�����ƫ����(1-10) ��֤�����������
			int offset=10*rand()/(RAND_MAX+1.0f)+1;
			for(;flag>=0;--flag)
			{
				for(int j=symbol_size-1;j>=0&&j!=flag;--j)
				{
					for(int k=symbol_size-1;k>=0&&k!=j&&k!=flag;--k)
					{
						results[2]=flag-offset;
						results[7]=j-offset;
						results[12]=k-offset;
						if(results[2]<0)results[2]+=symbol_size;
						if(results[7]<0)results[7]+=symbol_size;
						if(results[12]<0)results[12]+=symbol_size;
						//����ǰ3�� ���н������� �������numOfhittedLines
						isbreak=false;
						count = 0;
						for(int i=0;i<lines;++i)
						{
							if(potentialLines[i]==1)
							{
								//�п��ܳ�Ϊ
								int row1 = line[i][0];
								int row2 = line[i][1];
								int row3 = line[i][2];
								int a = results[row1*5];
								int b = results[row2*5+1];
								int c = results[row3*5+2];
								//������Ĵ���һ��
								if(a==b)
								{
									if(a==0&&c==1)
										continue;
									if(a==0||a==2||a==3||a==4)
									{
										if(count>=numOfhittedLines)
										{
											isbreak =true;
											break;
										}
										hittedLines[count]=i+1;
										++count;
									}
									else
									{
										if(c==b)
										{
											if(count>=numOfhittedLines)
											{	
												isbreak =true;
												break;
											}
											hittedLines[count]=i+1;
											++count;
										}
										else if(c==0&&a!=1)
										{
											if(count>=numOfhittedLines)
											{	
												isbreak =true;
												break;
											}
											hittedLines[count]=i+1;
											++count;
										}
										else
											continue;
									}
								}
								else
								{
									if((a==0&&b==1)||(a==1&&b==0))
										continue;
									if(a!=0&&b!=0)
										continue;
									if((a==0&&c!=b)||(a!=c&&b==0))
										continue;
									else
									{
										if(count>=numOfhittedLines)
										{	
											isbreak =true;
											break;
										}
										hittedLines[count]=i+1;
										++count;
									}
								}
							}
						}
						if(count==numOfhittedLines&&!isbreak)
							break;
					}
					if(count==numOfhittedLines&&!isbreak)
						break;
				}
				if(count==numOfhittedLines&&!isbreak)
					break;
			}
			delete potentialLines;
		}
		//ȷ���������
		for(int i=3;i<5;++i)//����������2�е�����
		{
			//ÿһ��û���ظ�������
			results[i]=getSymbol();
			results[5+i]=getSymbol();
			while(results[i]==results[5+i])
			{
				results[5+i]=getSymbol();
			}
			results[10+i]=getSymbol();
			while(results[10+i]==results[i]||results[10+i]==results[5+i])
			{
				results[10+i]=getSymbol();
			}
		}
		//���Ҽ���firstHittedPics��ֵ
		//�����ܽ������
		int bigwin_zimu=0;
		int bigwin_shuzi=0;
		int bigwin_pic=0;
		for(int i=0;i<numOfhittedLines;++i)
		{
			int row1 = line[hittedLines[i]-1][0];
			int row2 = line[hittedLines[i]-1][1];
			int row3 = line[hittedLines[i]-1][2];
			int row4 = line[hittedLines[i]-1][3];
			int row5 = line[hittedLines[i]-1][4];
			int a = results[row1*5];
			int b = results[row2*5+1];
			int c = results[row3*5+2];
			int d = results[row4*5+3];
			int e = results[row5*5+4];
			int coin=0;
			if(a==0)
			{
				if(b==0)
				{
					coin=paytable[0][1];
					if(c==0)
					{
						coin=paytable[0][2];
						if(d==0)
						{
							coin=paytable[0][3];
							if(e==0)
							{
								coin=paytable[0][4];
								firstHittedPics[i]=5;
								state = 2;
							}
							else a=b=c=d=e;
						}
						else a=b=c=d;
					}
					else a=b=c;
				}
				else a=b;
			}
			gains+=coin;
			if(b==0) b=a;
			if(c==0) c=b;
			if(d==0) d=c;
			if(e==0) e=d;
			if(c!=b) firstHittedPics[i]=2;
			else if(d!=c) firstHittedPics[i]=3;
			else if(e!=d) firstHittedPics[i]=4;
			else firstHittedPics[i]=5;
			if(a>4&&a<9&&firstHittedPics[i]>=3)//��ĸ
				++bigwin_zimu;
			else if(a>8&&a<11&&firstHittedPics[i]>=4)//����
			{
				if(firstHittedPics[i]==5)
					state=2;
				++bigwin_shuzi;
			}
			else if(a>1&&a<5&&firstHittedPics[i]>=3)//ͼƬ
				++bigwin_pic;
			if(a==1&&firstHittedPics[i]>=3) //������ bonus game
				state=1;
			gains+=paytable[a][firstHittedPics[i]-1];
		}
		//�ж��Ƿ�Ϊbig win
		if(lines>=9&&lines<14)
		{
			if(numOfhittedLines>lines/2)
				state=2;
		}
		else if(lines>=14)
		{
			if(numOfhittedLines>=7)
				state=2;
		}
		if(chpt_main<3)
		{
			if(bigwin_zimu>=2||bigwin_shuzi>=1||bigwin_pic>=1)
				state=2;
		}
		else
		{
			if(bigwin_zimu>=3||bigwin_pic>=2)
				state=2;
		}
	}
	//Log(iswin,results,numOfhittedLines,hittedLines,firstHittedPics,state,gains*multiple_winnings);
	if(!iswin)
		return 0;
	else
		return gains*multiple_winnings;
}

inline const bool random(int percentage)
{
	//����һ��0-100�������
	int r=(int)(100*rand()/(RAND_MAX+1.0f));
	if(percentage>r)
		return true;
	else
		return false;
}
inline const int expDist(int lines)
{
	int x = log(1-(float)rand()/(RAND_MAX+1.0f))/(-lamda)+1;
	if(x>lines) x=lines;
	return x;
}
inline int getSymbol()
{
	float x = (float)rand()/(RAND_MAX+1.0f);
	if(x<0.05) return 0;
	else if(x<0.12) return 1;
	else if(x<0.18) return 2;
	else if(x<0.26) return 3;
	else if(x<0.36) return 4;
	else if(x<0.46) return 5;
	else if(x<0.56) return 6;
	else if(x<0.66) return 7;
	else if(x<0.76) return 8;
	else if(x<0.88) return 9;
	else return 10;
}
//void LogTrace(LPCTSTR pszFormat, ...)
//{  
//    va_list pArgs;
//
//    char szMessageBuffer[16380]={0};
//    va_start( pArgs, pszFormat );
//    _vsntprintf( szMessageBuffer, 16380, pszFormat, pArgs );
//    va_end( pArgs );
//
//    OutputDebugString(szMessageBuffer);
//}

//void Log(bool iswin, int results[],int numOfhittedLines, int hittedLines[], int firstHittedPics[], int state,int gains)
//{
//	if(iswin)
//		LogTrace("%d:win\t",counter_play);
//	else
//		LogTrace("%d:lose\t",counter_play);
//	LogTrace("%d\r\n",cur_winrate);
//	LogTrace("���������� %d.\r\n",numOfhittedLines);
//	for(int i=0;i<numOfhittedLines;++i)
//	{
//		LogTrace("%d-%d ",hittedLines[i],firstHittedPics[i]);
//	}
//	LogTrace("\r\nstate�� %d.\r\n",state);
//	for(int i=0;i<3;++i)
//	{
//		for(int j=0;j<5;++j)
//		{
//			LogTrace("%d ",results[i*5+j]);
//		}
//		LogTrace("\r\n");
//	}
//	LogTrace("��ý�ң� %d.\r\n",gains);
//}
