//time(&rawtime)函数获取当前时间距1970年1月1日的秒数，以秒计数单位，存于rawtime 中

#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <stdio.h>
#define chang 640
#define kuan 480
#define ammomax 3//一个屏幕内出现的最多弹药数
#define tankmax 4//初始敌方坦克的数量
#define mustkill 20//游戏胜利目标
int sum=tankmax;//sum用于记录当前屏幕内还剩余的敌方坦克的数量
int kill=0;//记录目前干掉的坦克数

struct TANK
{
	int x,y;
	int half;
	int dir;
	int step;
	int exist;
};

TANK stank;
void init_bianjie();							//初始化紫色矩形边界
void init_wenzi();								//初始化游戏说明和记分屏
void init_ammo(TANK &ammo);						//初始化弹药,初始弹药不存在
void init_stank();								//初始化主坦克大小 速度 存在
void init_rtank(TANK &tank,int i);				//i代表坦克的标号,防止重叠
void draw_stank(TANK tank);						//画出主坦克
void draw_rtank(TANK tank);						//画出敌方坦克
void move_stank(char c,TANK *tank);				//移动主坦克,并判断移动后的位置
void move_rtank(TANK *tank,int s,TANK *ammo);	//移动敌方坦克,并判断移动后的位置
void stank_shoot(TANK *ammo,int s,TANK *tank);	//主坦克射击,判断当前炮口是否有子弹未来得及移动,并判断炮口位置
void rtank_shoot(TANK tank,TANK *ammo,int s);	//敌方坦克射击并判断炮口位置
void move_rammo(TANK &ammo);					//移动蓝色子弹,并判断移动后的位置
void exam_rammo(TANK &ammo);					//检查子弹当前位置是否打到主坦克,并判断是否越界
void move_sammo(TANK *ammo,int s,TANK *tank);	//移动红色子弹,并判断移动后的位置
void exam_sammo(TANK *ammo,int s,TANK *tank);	//检查主坦克的子弹移动后的点是否打到了敌方坦克以及检查子弹是否到边界

void main()
{
	char c;
	int i,j;
	double t1,t2,t3;
	TANK rtank[tankmax],sammo[ammomax],rammo[tankmax][ammomax];
	srand((unsigned)time(NULL));
	initgraph(chang+20+200, kuan+20);//初始化界面大小
	setorigin(10, 10);//为了设置边界,将原点坐标修改下
	init_bianjie();
	init_stank();//初始化主坦克
	init_wenzi();

	for(i=0; i<ammomax; i++)//初始主坦克化弹药
		init_ammo(sammo[i]);

	for(i=0; i<tankmax; i++)//初始化敌方的随机坦克
	{
		init_rtank(rtank[i],i);
		for(j=0; j<ammomax; j++)//初始敌方坦克化弹药,每个敌方坦克都可以发射最多maxammo数量的子弹
			init_ammo(rammo[i][j]);
		draw_rtank(rtank[i]);
	}

	t3=t2=t1=time(NULL);//初始时间
	while(1)
	{
		if(kbhit())//如果你按了键,坦克就对应着做动作,否则你没按键,自动跳过
		{
			c=getch();
			if(c=='w'||c=='s'||c=='a'||c=='d'||c=='W'||c=='S'||c=='A'||c=='D')
				move_stank(c,rtank);
			else if(c==' ')
			{
				for(i=0; i<ammomax; i++)
					if(sammo[i].exist==0)
					{
						stank_shoot(sammo,i,rtank);
						break;
					}
			}
		}
		if(time(NULL)==int(t1+0.5))
		{
			t1+=0.5;//当这里的数小于1时就变成了每秒移动(1/该数)的次数!
			for(i=0; i<ammomax; i++)
			{
				if(sammo[i].exist)
					move_sammo(sammo,i,rtank);
			}
			for(i=0; i<tankmax; i++)
			{
				for(j=0; j<ammomax; j++)
				if(rammo[i][j].exist)
					move_rammo(rammo[i][j]);
			}
			if(kill==mustkill)
			{
				settextcolor(YELLOW);
				char s[] = "YOU WIN !";
				outtextxy(180, kuan/2, s);
				getch();
				closegraph();
				exit(0);
			}
			if(sum==0)
			{
				for(i=0; i<tankmax; i++)
				{
					init_rtank(rtank[i],i);
					draw_rtank(rtank[i]);
				}
				sum=tankmax;
			}
		}
		if(time(NULL)==int(t2+2))//敌方坦克每两秒移动一次
		{
			t2+=2;
			for(i=0; i<tankmax; i++)
				if(rtank[i].exist) move_rtank(rtank,i,sammo);
			if(kill==mustkill)
			{
				settextcolor(YELLOW);
				char s[] = "YOU WIN !";
				outtextxy(180, kuan/2, s);
				getch();
				closegraph();
				exit(0);
			}
			if(sum==0)
			{
				for(i=0; i<tankmax; i++)
				{
					init_rtank(rtank[i],i);
					draw_rtank(rtank[i]);
				}
				sum=tankmax;
			}
		}
		if(time(NULL)==int(t3+3))//敌方坦克每三秒射击一次
		{
			t3=t3+3;

			for(i=0; i<tankmax; i++)
				if(rtank[i].exist==1)//活着的坦克射击
				for(j=0; j<ammomax; j++)
					if(rammo[i][j].exist==0)//选择一个能射击的子弹变量,射击,反之,坦克不做射击动作
					{
						rtank_shoot(rtank[i],rammo[i],j);
						break;
					}
		}
	}
}

void rtank_shoot(TANK tank,TANK *ammo,int s)
{
	ammo[s].exist=1;//生成子弹,子弹存在,代表每次移动时要移动它
	ammo[s].dir=tank.dir;	
	switch(ammo[s].dir)
	{
		case 0: ammo[s].x=tank.x; ammo[s].y=tank.y-15; break;
		case 1: ammo[s].x=tank.x; ammo[s].y=tank.y+15; break;
		case 2:	ammo[s].x=tank.x-15; ammo[s].y=tank.y; break;
		case 3:	ammo[s].x=tank.x+15; ammo[s].y=tank.y;
	}
	exam_rammo(ammo[s]);
	if(ammo[s].exist==1)
	{
		setfillcolor(LIGHTBLUE);
		solidcircle(ammo[s].x,ammo[s].y,ammo[s].half);
	}
}

void move_rammo(TANK &ammo)
{
	clearcircle(ammo.x,ammo.y,ammo.half+1);
	switch(ammo.dir)
	{
		case 0: ammo.y-=ammo.step; break;
		case 1: ammo.y+=ammo.step; break;
		case 2:	ammo.x-=ammo.step; break;
		case 3:	ammo.x+=ammo.step;
	}
	exam_rammo(ammo);
	if(ammo.exist)
	{
		setfillcolor(LIGHTBLUE);
		solidcircle(ammo.x,ammo.y,ammo.half);
	}
}

void exam_rammo(TANK &ammo)
{
	if(ammo.x<stank.x+stank.half+4&&ammo.x>stank.x-stank.half-4&&ammo.y<stank.y+stank.half+4&&ammo.y>stank.y-stank.half-4)
	{
		clearrectangle(stank.x-stank.half-4,stank.y-stank.half-4,stank.x+stank.half+4,stank.y+stank.half+4);
		settextcolor(RED);
		char s[] = "YOU LOST !";
		outtextxy(180, kuan/2, s);
		getch();
		closegraph();
		exit(0);
	}
	if(ammo.x<0||ammo.x>chang||ammo.y<0||ammo.y>kuan)//检查子弹是否到边界
		ammo.exist=0;
}

void stank_shoot(TANK *ammo,int s,TANK *tank)
{
	int i;
	ammo[s].exist=1;//生成子弹,子弹存在,代表每次移动时要移动它
	ammo[s].dir=stank.dir;	
	switch(ammo[s].dir)
	{
		case 0: ammo[s].x=stank.x; ammo[s].y=stank.y-15; break;
		case 1: ammo[s].x=stank.x; ammo[s].y=stank.y+15; break;
		case 2:	ammo[s].x=stank.x-15; ammo[s].y=stank.y; break;
		case 3:	ammo[s].x=stank.x+15; ammo[s].y=stank.y;
	}
	for(i=0; i<s; i++)//这样即使你空格按的过快子弹也不会重叠
	{
		if(ammo[i].x==ammo[s].x&&ammo[i].y==ammo[s].y)
		{
			ammo[s].exist=0;
			break;
		}
	}
	exam_sammo(ammo,s,tank);
	if(ammo[s].exist==1)
	{
		setfillcolor(LIGHTRED);
		solidcircle(ammo[s].x,ammo[s].y,ammo[s].half);
	}
}

void move_sammo(TANK *ammo,int s,TANK *tank)
{
	int i,z=0;
	for(i=0; i<ammomax; i++)
	{
		if(i==s) continue;
		if(ammo[i].exist==1)
		if(ammo[s].x==ammo[i].x&&ammo[s].y==ammo[i].y)
		{
			z=1;//如果该弹药和别的弹药重叠了,如果它移动的话,就会把原位置的弹药也清除了
			break;
		}
	}

	if(z==0) clearcircle(ammo[s].x,ammo[s].y,ammo[s].half+1);//原来没有其他子弹的话就删除原位置的子弹图像
	switch(ammo[s].dir)
	{
		case 0: ammo[s].y-=ammo[s].step; break;
		case 1: ammo[s].y+=ammo[s].step; break;
		case 2:	ammo[s].x-=ammo[s].step; break;
		case 3:	ammo[s].x+=ammo[s].step;
	}
	exam_sammo(ammo,s,tank);
	if(ammo[s].exist)
	{
		setfillcolor(LIGHTRED);
		solidcircle(ammo[s].x,ammo[s].y,ammo[s].half);
	}
}

void exam_sammo(TANK *ammo,int s,TANK *tank)
{
	int i;
	for(i=0; i<tankmax; i++)
	if(tank[i].exist==1)
	if(ammo[s].x<tank[i].x+tank[i].half+4&&ammo[s].x>tank[i].x-tank[i].half-4&&ammo[s].y<tank[i].y+tank[i].half+4&&ammo[s].y>tank[i].y-tank[i].half-4)
	{
		sum--;
		kill++;

		char b[3];
		clearrectangle(chang+110,210,chang+130,230);//清除右边信息框清除敌方坦克的个数,再重新打印出来
;
		b[0]=kill/10+48;
		b[1]=kill%10+48;
		b[2]=0;
		outtextxy(chang+110, 210, b);

		ammo[s].exist=0;
		tank[i].exist=0;
		clearrectangle(tank[i].x-tank[i].half-4,tank[i].y-tank[i].half-4,tank[i].x+tank[i].half+4,tank[i].y+tank[i].half+4);
		break;
	}
	if(ammo[s].x<0||ammo[s].x>chang||ammo[s].y<0||ammo[s].y>kuan)//检查子弹是否到边界
		ammo[s].exist=0;
}

void init_ammo(TANK &ammo)
{
	ammo.half=3;
	ammo.step=30;
	ammo.exist=0;
}

void init_rtank(TANK &tank,int i)
{
	tank.half=10;
	tank.step=20;
	tank.exist=1;
	tank.dir=i%4;
	tank.x=128+tank.dir*128;
	tank.y=10;
}

void init_stank()
{
	stank.half=10;
	stank.step=4;
	stank.exist=3;
	stank.dir=0;
	stank.x=rand()%(chang-2*stank.half)+10;
	stank.y=460;//初始化主坦克
	draw_stank(stank);
}

void move_rtank(TANK *tank,int s,TANK *ammo)
{
	int i,t,z=1;
	clearrectangle(tank[s].x-tank[s].half-4,tank[s].y-tank[s].half-4,tank[s].x+tank[s].half+4,tank[s].y+tank[s].half+4);
	tank[s].dir=rand()%4;
	switch(tank[s].dir)
	{
		case 0://坦克方向为上
			{
				tank[s].y-=tank[s].step;
				if (tank[s].y - tank[s].half < 0) tank[s].y=tank[s].half+2;//边界问题
				for(i=0; i<tankmax; i++)//是否有别的坦克
				{
					if(i==s) continue;//排除自己
					t=tank[s].x-tank[i].x;//控制横坐标
					if(t<0) t=tank[i].x-tank[s].x;
					if(tank[i].exist==1&&tank[s].y-tank[i].y>0)//只找在tank[s]上方存在的坦克
					if(tank[s].y-tank[i].y<tank[s].half+tank[i].half+2&&t<tank[s].half+tank[i].half+4)//如果有重叠
					tank[s].y=tank[i].y+tank[s].half+tank[i].half+8;
				}
				t=tank[s].x-stank.x;//控制横坐标
					if(t<0) t=stank.x-tank[s].x;
				if(tank[s].y-stank.y>0)//是否有主坦克
					if(tank[s].y-stank.y<tank[s].half+stank.half+2&&t<tank[s].half+stank.half+4)//如果有重叠
					tank[s].y=stank.y+tank[s].half+stank.half+8;
				break;
			}
		case 1://坦克方向为下
			{
				tank[s].y+=tank[s].step;
				if (tank[s].y + tank[s].half > kuan) tank[s].y=kuan-tank[s].half-2;
				for(i=0; i<tankmax; i++)//是否有别的坦克
				{
					if(i==s) continue;
					t=tank[s].x-tank[i].x;//控制横坐标
					if(t<0)
						t=tank[i].x-tank[s].x;
					if(tank[i].exist==1&&tank[i].y-tank[s].y>0)//只找在tank[s]下方存在的坦克
					if(tank[i].y-tank[s].y<tank[s].half+tank[i].half+2&&t<tank[s].half+tank[i].half+4)//如果有重叠
					tank[s].y=tank[i].y-tank[s].half-tank[i].half-8;
				}
				t=tank[s].x-stank.x;//控制横坐标
					if(t<0) t=stank.x-tank[s].x;
				if(stank.y-tank[s].y>0)//是否有主坦克
					if(stank.y-tank[s].y<tank[s].half+stank.half+2&&t<tank[s].half+stank.half+4)//如果有重叠
					tank[s].y=stank.y-tank[s].half-stank.half-8;
				break;
			}
		case 2://坦克方向为左
			{
				tank[s].x-=tank[s].step;
				if(tank[s].x - tank[s].half < 0)
					tank[s].x=tank[s].half+2;
				for(i=0; i<tankmax; i++)//是否有别的坦克
				{
					if(i==s) continue;
					t=tank[s].y-tank[i].y;//控制纵坐标
					if(t<0)
						t=tank[i].y-tank[s].y;
					if(tank[i].exist==1&&tank[s].x-tank[i].x>0)//只找在tank[s]左方存在的坦克
					if(tank[s].x-tank[i].x<tank[s].half+tank[i].half+2&&t<tank[s].half+tank[i].half+4)//如果有重叠
					tank[s].x=tank[i].x+tank[s].half+tank[i].half+8;
				}
				t=tank[s].y-stank.y;//控制纵坐标
					if(t<0) t=stank.y-tank[s].y;
				if(tank[s].x-stank.x>0)//是否有主坦克
					if(tank[s].x-stank.x<tank[s].half+stank.half+2&&t<tank[s].half+stank.half+4)//如果有重叠
					tank[s].x=stank.x+tank[s].half+stank.half+8;
				break;
			}
		case 3://坦克方向为右
			{
				tank[s].x+=tank[s].step;
				if (tank[s].x + tank[s].half > chang)
					tank[s].x=chang-tank[s].half-2;
				for(i=0; i<tankmax; i++)//是否有别的坦克
				{
					if(i==s) continue;
					t=tank[s].y-tank[i].y;//控制纵坐标
					if(t<0)
						t=tank[i].y-tank[s].y;
					if(tank[i].exist==1&&tank[i].x-tank[s].x>0)//只找在tank[s]右方存在的坦克
					if(tank[i].x-tank[s].x<tank[s].half+tank[i].half+2&&t<tank[s].half+tank[i].half+4)//如果有重叠
					tank[s].x=tank[i].x-tank[s].half-tank[i].half-8;
				}
				t=tank[s].y-stank.y;//控制纵坐标
					if(t<0) t=stank.y-tank[s].y;
				if(stank.x-tank[s].x>0)//是否有主坦克
					if(stank.x-tank[s].x<tank[s].half+stank.half+2&&t<tank[s].half+stank.half+4)//如果有重叠
					tank[s].x=stank.x-tank[s].half-stank.half-8;
			}
	}
	for(i=0; i<ammomax; i++)//如果坦克移动过程中碰到了子弹
		if(ammo[i].exist==1)
		if(ammo[i].x<tank[s].x+tank[s].half+4&&ammo[i].x>tank[s].x-tank[s].half-4&&ammo[i].y<tank[s].y+tank[s].half+4&&ammo[i].y>tank[s].y-tank[s].half-4)
		{
			sum--;
			kill++;
			char b[3];

			clearrectangle(chang+110,210,chang+130,230);
			b[0]=kill/10+48;
			b[1]=kill%10+48;
			b[2]=0;
			outtextxy(chang+110, 210, b);

			ammo[i].exist=0;
			tank[s].exist=0;
			clearcircle(ammo[i].x,ammo[i].y,ammo[i].half+1);
			z=0;
			break;
		}
		if(z) draw_rtank(tank[s]);
}

void move_stank(char c,TANK *rtank)
{
	int i,t;
	clearrectangle(stank.x-stank.half-4,stank.y-stank.half-4,stank.x+stank.half+4,stank.y+stank.half+4);//清除之前的坦克
	switch(c)
		{
			case 'w':
			case 'W':
				{
					stank.dir=0;
					stank.y -= stank.step;
					if (stank.y - stank.half < 0)
						stank.y=stank.half+2;//判断边界问题
					for(i=0; i<tankmax; i++)//是否有别的坦克
					{
						t=stank.x-rtank[i].x;//控制横坐标
						if(t<0) t=rtank[i].x-stank.x;
						if(rtank[i].exist==1&&stank.y-rtank[i].y>0)//只找stank上方存在的坦克
						if(stank.y-rtank[i].y<stank.half+rtank[i].half+2&&t<stank.half+rtank[i].half+4)//如果有重叠
						stank.y=rtank[i].y+stank.half+rtank[i].half+8;
					}

					break;
				}
			case 's':
			case 'S':
				{
					stank.dir=1;
					stank.y += stank.step;
					if (stank.y + stank.half > kuan)
						stank.y=kuan-stank.half-2;
					for(i=0; i<tankmax; i++)//是否有别的坦克
					{
						t=stank.x-rtank[i].x;//控制横坐标
						if(t<0) t=rtank[i].x-stank.x;
						if(rtank[i].exist==1&&rtank[i].y-stank.y>0)//只找stank下方存在的坦克
						if(rtank[i].y-stank.y<stank.half+rtank[i].half+2&&t<stank.half+rtank[i].half+4)//如果有重叠
						stank.y=rtank[i].y-stank.half-rtank[i].half-8;
					}
					break;
				}

			case 'a':
			case 'A':
				{
					stank.dir=2;
					stank.x -= stank.step;
					if (stank.x - stank.half < 0)
						stank.x=stank.half+2;
					for(i=0; i<tankmax; i++)//是否有别的坦克
					{
						t=stank.y-rtank[i].y;//控制纵坐标
						if(t<0)
							t=rtank[i].y-stank.y;
						if(rtank[i].exist==1&&stank.x-rtank[i].x>0)//只找stank左方存在的坦克
						if(stank.x-rtank[i].x<stank.half+rtank[i].half+2&&t<stank.half+rtank[i].half+4)//如果有重叠
						stank.x=rtank[i].x+stank.half+rtank[i].half+8;
					}
					break;
				}

			case 'd':
			case 'D':
				{
					stank.dir=3;
					stank.x += stank.step;
					if (stank.x + stank.half > chang)
						stank.x=chang-stank.half-2;
					for(i=0; i<tankmax; i++)//是否有别的坦克
					{
						t=stank.y-rtank[i].y;//控制纵坐标
						if(t<0)
							t=rtank[i].y-stank.y;
						if(rtank[i].exist==1&&rtank[i].x-stank.x>0)//只找stank右方存在的坦克
						if(rtank[i].x-stank.x<stank.half+rtank[i].half+2&&t<stank.half+rtank[i].half+4)//如果有重叠
						stank.x=rtank[i].x-stank.half-rtank[i].half-8;
					}
					break;
				}
		}
		draw_stank(stank);
}

void draw_stank(TANK tank)
{
	setfillcolor(LIGHTMAGENTA);
	fillroundrect(tank.x-tank.half,tank.y-tank.half,tank.x+tank.half,tank.y+tank.half,tank.half,tank.half);//大核心
	fillcircle(tank.x,tank.y,5);//炮口基座
	switch(tank.dir)
	{
		case 0:
		{
			fillrectangle(tank.x-1,tank.y-12,tank.x+1,tank.y+1);//炮口

			fillcircle(tank.x-tank.half,tank.y,2);
			fillcircle(tank.x-tank.half,tank.y-tank.half*1/5*2,2);
			fillcircle(tank.x-tank.half,tank.y-tank.half*2/5*2,2);
			fillcircle(tank.x-tank.half,tank.y+tank.half*1/5*2,2);
			fillcircle(tank.x-tank.half,tank.y+tank.half*2/5*2,2);

			fillcircle(tank.x+tank.half,tank.y,2);
			fillcircle(tank.x+tank.half,tank.y-tank.half*1/5*2,2);
			fillcircle(tank.x+tank.half,tank.y-tank.half*2/5*2,2);
			fillcircle(tank.x+tank.half,tank.y+tank.half*1/5*2,2);
			fillcircle(tank.x+tank.half,tank.y+tank.half*2/5*2,2);
			break;
		}
		case 1:
		{
			fillrectangle(tank.x-1,tank.y-1,tank.x+1,tank.y+12);//炮口

			fillcircle(tank.x-tank.half,tank.y,2);
			fillcircle(tank.x-tank.half,tank.y-tank.half*1/5*2,2);
			fillcircle(tank.x-tank.half,tank.y-tank.half*2/5*2,2);
			fillcircle(tank.x-tank.half,tank.y+tank.half*1/5*2,2);
			fillcircle(tank.x-tank.half,tank.y+tank.half*2/5*2,2);

			fillcircle(tank.x+tank.half,tank.y,2);
			fillcircle(tank.x+tank.half,tank.y-tank.half*1/5*2,2);
			fillcircle(tank.x+tank.half,tank.y-tank.half*2/5*2,2);
			fillcircle(tank.x+tank.half,tank.y+tank.half*1/5*2,2);
			fillcircle(tank.x+tank.half,tank.y+tank.half*2/5*2,2);
			break;
		}

		case 2:
		{
			fillrectangle(tank.x-12,tank.y-1,tank.x+1,tank.y+1);//炮口

			fillcircle(tank.x,tank.y-tank.half,2);//左边五个轮子
			fillcircle(tank.x-tank.half*1/5*2,tank.y-tank.half,2);
			fillcircle(tank.x-tank.half*2/5*2,tank.y-tank.half,2);
			fillcircle(tank.x+tank.half*1/5*2,tank.y-tank.half,2);
			fillcircle(tank.x+tank.half*2/5*2,tank.y-tank.half,2);

			fillcircle(tank.x,tank.y+tank.half,2);//右边五个轮子
			fillcircle(tank.x-tank.half*1/5*2,tank.y+tank.half,2);
			fillcircle(tank.x-tank.half*2/5*2,tank.y+tank.half,2);
			fillcircle(tank.x+tank.half*1/5*2,tank.y+tank.half,2);
			fillcircle(tank.x+tank.half*2/5*2,tank.y+tank.half,2);
			break;
		}
		case 3:
		{
			fillrectangle(tank.x-1,tank.y-1,tank.x+12,tank.y+1);//炮口

			fillcircle(tank.x,tank.y-tank.half,2);//左边五个轮子
			fillcircle(tank.x-tank.half*1/5*2,tank.y-tank.half,2);
			fillcircle(tank.x-tank.half*2/5*2,tank.y-tank.half,2);
			fillcircle(tank.x+tank.half*1/5*2,tank.y-tank.half,2);
			fillcircle(tank.x+tank.half*2/5*2,tank.y-tank.half,2);

			fillcircle(tank.x,tank.y+tank.half,2);//右边五个轮子
			fillcircle(tank.x-tank.half*1/5*2,tank.y+tank.half,2);
			fillcircle(tank.x-tank.half*2/5*2,tank.y+tank.half,2);
			fillcircle(tank.x+tank.half*1/5*2,tank.y+tank.half,2);
			fillcircle(tank.x+tank.half*2/5*2,tank.y+tank.half,2);		
		}
	}
}
void draw_rtank(TANK tank)
{
	setfillcolor(BLUE);
	fillcircle(tank.x,tank.y,tank.half);//大核心
	fillroundrect(tank.x-2,tank.y-2,tank.x+2,tank.y+2,2,2);//炮口基座
	switch(tank.dir)
	{
		case 0:
		{
			fillrectangle(tank.x-1,tank.y-12,tank.x+1,tank.y+1);//炮口

			fillcircle(tank.x-tank.half,tank.y,2);
			fillcircle(tank.x-tank.half,tank.y-tank.half*1/5*2,2);
			fillcircle(tank.x-tank.half,tank.y-tank.half*2/5*2,2);
			fillcircle(tank.x-tank.half,tank.y+tank.half*1/5*2,2);
			fillcircle(tank.x-tank.half,tank.y+tank.half*2/5*2,2);

			fillcircle(tank.x+tank.half,tank.y,2);
			fillcircle(tank.x+tank.half,tank.y-tank.half*1/5*2,2);
			fillcircle(tank.x+tank.half,tank.y-tank.half*2/5*2,2);
			fillcircle(tank.x+tank.half,tank.y+tank.half*1/5*2,2);
			fillcircle(tank.x+tank.half,tank.y+tank.half*2/5*2,2);
			break;
		}
		case 1:
		{
			fillrectangle(tank.x-1,tank.y-1,tank.x+1,tank.y+12);//炮口

			fillcircle(tank.x-tank.half,tank.y,2);
			fillcircle(tank.x-tank.half,tank.y-tank.half*1/5*2,2);
			fillcircle(tank.x-tank.half,tank.y-tank.half*2/5*2,2);
			fillcircle(tank.x-tank.half,tank.y+tank.half*1/5*2,2);
			fillcircle(tank.x-tank.half,tank.y+tank.half*2/5*2,2);

			fillcircle(tank.x+tank.half,tank.y,2);
			fillcircle(tank.x+tank.half,tank.y-tank.half*1/5*2,2);
			fillcircle(tank.x+tank.half,tank.y-tank.half*2/5*2,2);
			fillcircle(tank.x+tank.half,tank.y+tank.half*1/5*2,2);
			fillcircle(tank.x+tank.half,tank.y+tank.half*2/5*2,2);
			break;
		}

		case 2:
		{
			fillrectangle(tank.x-12,tank.y-1,tank.x+1,tank.y+1);//炮口

			fillcircle(tank.x,tank.y-tank.half,2);//左边五个轮子
			fillcircle(tank.x-tank.half*1/5*2,tank.y-tank.half,2);
			fillcircle(tank.x-tank.half*2/5*2,tank.y-tank.half,2);
			fillcircle(tank.x+tank.half*1/5*2,tank.y-tank.half,2);
			fillcircle(tank.x+tank.half*2/5*2,tank.y-tank.half,2);

			fillcircle(tank.x,tank.y+tank.half,2);//右边五个轮子
			fillcircle(tank.x-tank.half*1/5*2,tank.y+tank.half,2);
			fillcircle(tank.x-tank.half*2/5*2,tank.y+tank.half,2);
			fillcircle(tank.x+tank.half*1/5*2,tank.y+tank.half,2);
			fillcircle(tank.x+tank.half*2/5*2,tank.y+tank.half,2);
			break;
		}
		case 3:
		{
			fillrectangle(tank.x-1,tank.y-1,tank.x+12,tank.y+1);//炮口

			fillcircle(tank.x,tank.y-tank.half,2);//左边五个轮子
			fillcircle(tank.x-tank.half*1/5*2,tank.y-tank.half,2);
			fillcircle(tank.x-tank.half*2/5*2,tank.y-tank.half,2);
			fillcircle(tank.x+tank.half*1/5*2,tank.y-tank.half,2);
			fillcircle(tank.x+tank.half*2/5*2,tank.y-tank.half,2);

			fillcircle(tank.x,tank.y+tank.half,2);//右边五个轮子
			fillcircle(tank.x-tank.half*1/5*2,tank.y+tank.half,2);
			fillcircle(tank.x-tank.half*2/5*2,tank.y+tank.half,2);
			fillcircle(tank.x+tank.half*1/5*2,tank.y+tank.half,2);
			fillcircle(tank.x+tank.half*2/5*2,tank.y+tank.half,2);		
		}
	}
}

void init_bianjie()
{
	setfillcolor(MAGENTA);//左上右下
	solidrectangle(-10,-10,chang+210,-5);		//上边框(包括文字区)
	solidrectangle(-10,kuan+5,chang+210,kuan+10);//下边框(包括文字区)
	solidrectangle(-10,-10,-5,kuan+5);			//左边框
	solidrectangle(chang+5,-10,chang+10,kuan+5);//右边框

	solidrectangle(chang+205,-10,chang+210,kuan+5);
}

void init_wenzi()
{
	settextcolor(YELLOW);
	char zj[] = "作者: 王敏聪";
	char yd0[]="坦克控制方式:",yd1[]="w: 向上移动",yd2[]="A: 向左移动",yd3[]="S: 向下移动",yd4[]="D: 向右移动";
	outtextxy(chang+20, 0, zj);
	settextcolor(WHITE);
	outtextxy(chang+20, 40, yd0);
	outtextxy(chang+20, 60, yd1);
	outtextxy(chang+20, 80, yd2);
	outtextxy(chang+20, 100, yd3);
	outtextxy(chang+20, 120, yd4);

	char zd[]="空格: 发射子弹";
	outtextxy(chang+20, 150, zd);

	settextcolor(LIGHTGREEN);
	char win1[]="胜利: 消灭";
	outtextxy(chang+20, 190, win1);

	char a[3];
	a[0]=mustkill/10+48;
	a[1]=mustkill%10+48;
	a[2]=0;
	outtextxy(chang+93, 190, a);

	char win2[]="个敌方坦克";
	outtextxy(chang+110, 190, win2);

	settextcolor(RED);
	char k[]="消灭数量:";
	outtextxy(chang+20, 210, k);

	char b[3];
	b[0]=kill/10+48;
	b[1]=kill%10+48;
	b[2]=0;
	outtextxy(chang+100, 210, b);
}