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
int cur_winrate;//当前的胜率
int cur_chpt_main=0;//当前关卡intint
int winrate;//固定的胜率
int counter_play;//当前已经玩过的次数
int counter_win;//当前赢得次数
const int history_n=20;//记录最近n次的赢的概率 n为10时最接近胜率，越小胜率下降
int history[history_n];//前n次赢输情况
int threshold=1;//当前胜率和历史胜率的差值的阈值
const float lamda=0.8f;//指数分布的参数
int winratio[]={70,60,50,45,40,35};//每个关卡的胜率
inline const bool random(int percentage);//控制赢得概率在percentage之下
inline const int expDist(int lines);//反向求解累积指数概率分布
inline int getSymbol();//随机一个symbol，每一个数字概率不一样
//void LogTrace(LPCTSTR pszFormat, ...);//向debugger输出
//void Log(bool iswin, int results[],int numOfhittedLines, int hittedLines[], int firstHittedPics[], int state,int gains);
int spin(int backgd, int chpt_main, int chpt_sec, int lines, int coins, int results[],
		 int &numOfhittedLines, int *&hittedLines, int *&firstHittedPics, int &state,
		 int boosters){
	int multiple_winnings=coins*boosters;
//	int bet=coins*lines;
	int gains=0;//获得的奖金数
	winrate=winratio[chpt_main-1];
	//全局初始化
	if(cur_chpt_main!=chpt_main){
		srand((unsigned)time(NULL));//随机种子
		cur_winrate=winrate;
		counter_play=0;
		counter_win=0;
		cur_chpt_main = chpt_main;
	}
	++counter_play;
	//计算 赢 or 输
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
		if((last_n_win-average)>threshold)//前n次胜率远远高于固定胜率，必输
		{
			iswin=false;
		}
		else if((average-last_n_win)>threshold)//前n次胜率远远低于固定胜率，必赢
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
	//生成不可能赢的数组
	if(!iswin)
	{
		state=0;
		numOfhittedLines=0;
		hittedLines=NULL;
		firstHittedPics=NULL;
		bool f = true;//是中奖的状态
		while(f)
		{
			for(int i=0;i<5;++i)//随机生成5列的数字
			{
				//每一列没有重复的数字
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
			//计算前3列的数字 保证没有中奖
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
		//计算命中的条数(满足指数分布)
		numOfhittedLines=expDist(lines);//随机返回0~lines之间的输数
		//然后随机选择lines
		hittedLines=new int[numOfhittedLines];
		firstHittedPics=new int[numOfhittedLines];
		//试错法
		int flag=-1;
		while(flag==-1)
		{
			int potential_hitted = 0;
			//维持一个可能中奖的线的数组
			//记得释放内存
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
				for(int i=0;i<2;++i)//随机生成2列的数字
				{
					//每一列没有重复的数字
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
				//计算前两列 看可能中奖的条数
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
			//遍历990次
			int count = 0;
			bool isbreak = false;
			flag=symbol_size-1;
			//设置一个随机偏移量(1-10) 保证遍历的随机性
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
						//计算前3列 看中奖的条数 必须等于numOfhittedLines
						isbreak=false;
						count = 0;
						for(int i=0;i<lines;++i)
						{
							if(potentialLines[i]==1)
							{
								//有可能成为
								int row1 = line[i][0];
								int row2 = line[i][1];
								int row3 = line[i][2];
								int a = results[row1*5];
								int b = results[row2*5+1];
								int c = results[row3*5+2];
								//和上面的代码一样
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
		//确定最后两列
		for(int i=3;i<5;++i)//随机生成最后2列的数字
		{
			//每一列没有重复的数字
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
		//并且计算firstHittedPics的值
		//计算总金币数量
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
			if(a>4&&a<9&&firstHittedPics[i]>=3)//字母
				++bigwin_zimu;
			else if(a>8&&a<11&&firstHittedPics[i]>=4)//数字
			{
				if(firstHittedPics[i]==5)
					state=2;
				++bigwin_shuzi;
			}
			else if(a>1&&a<5&&firstHittedPics[i]>=3)//图片
				++bigwin_pic;
			if(a==1&&firstHittedPics[i]>=3) //或者是 bonus game
				state=1;
			gains+=paytable[a][firstHittedPics[i]-1];
		}
		//判断是否为big win
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
	//产生一个0-100的随机数
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
//	LogTrace("命中条数： %d.\r\n",numOfhittedLines);
//	for(int i=0;i<numOfhittedLines;++i)
//	{
//		LogTrace("%d-%d ",hittedLines[i],firstHittedPics[i]);
//	}
//	LogTrace("\r\nstate： %d.\r\n",state);
//	for(int i=0;i<3;++i)
//	{
//		for(int j=0;j<5;++j)
//		{
//			LogTrace("%d ",results[i*5+j]);
//		}
//		LogTrace("\r\n");
//	}
//	LogTrace("获得金币： %d.\r\n",gains);
//}
