#include <graphics.h>
#include <time.h>
#include <conio.h>
#define kuan 480
#define chang 640
#define R 20
#define Sum 5
int sum =Sum;
int r=R;

struct STANK
{
	int	x;
	int	y;
	int	step;
};
struct RTANK
{
	int	x;
	int	y;
	int	eat;
};

void Init_stank(STANK &tank);
void Init_rtank(RTANK &tank);
void Move_stank(STANK &tank,char c);
void Move_rand(RTANK &tank);

void main()
{
	int i,z=1,s=0;
	char c;
	STANK stank;
	RTANK tank[Sum];

	srand((unsigned)time(NULL));	// 随机种子
	initgraph(chang, kuan);			// 创建绘图窗口
	Init_stank(stank);

	for(i = 0; i < sum; i++) Init_rtank(tank[i]);

	while(z)
	{
		z=1;
		c=getche();
		if(c=='w'||c=='s'||c=='a'||c=='d')
			Move_stank(stank,c);
		else z=0;
		Sleep(200);
		if(s==sum) 	
		{
			for(i = 0; i < sum; i++) Init_rtank(tank[i]);
			s=0;
		}
		for(i = 0; i < sum; i++)
		{
			if(tank[i].eat==0&&tank[i].x>stank.x-r&&tank[i].x<stank.x+r&&tank[i].y>stank.y-r&&tank[i].y<stank.y+r)
			{
				setfillcolor(BLUE);
				fillcircle(tank[i].x, tank[i].y, 4);
				tank[i].eat=1;
				r+=5;
				s++;
			}
			if(tank[i].eat==0) Move_rand(tank[i]);
			if(tank[i].eat==0&&tank[i].x>stank.x-r&&tank[i].x<stank.x+r&&tank[i].y>stank.y-r&&tank[i].y<stank.y+r)
			{
				Sleep(200);
				setfillcolor(BLUE);
				fillcircle(tank[i].x, tank[i].y, 4);
				tank[i].eat=1;
				r+=5;
				s++;
			}
		}
	}

	closegraph();
}

void Init_stank(STANK &tank)
{
	tank.x = (int)(rand()/10)*10% chang;
	tank.y = (int)(rand()/10)*10% kuan;
	tank.step = 40;
	setfillcolor(RED);
	fillcircle(tank.x, tank.y, r);//主初始为红心圆点
}

void Init_rtank(RTANK &tank)
{
	tank.x = (int)(rand()/10)*10% chang;
	tank.y = (int)(rand()/10)*10% kuan;
	tank.eat=0;
	setfillcolor(GREEN);
	fillcircle(tank.x, tank.y, 3);
}

void Move_stank(STANK &tank,char c)
{
	clearcircle(tank.x, tank.y, r+1);

	switch(c)
		{
			case 'w':tank.y -= tank.step;
					if (tank.y < 0)	tank.y=kuan; break;

			case 's':tank.y += tank.step;
					if (tank.y > kuan)	tank.y-=kuan; break;

			case 'a':tank.x -= tank.step;
					if (tank.x < 0)	tank.x=chang; break;

			case 'd':tank.x += tank.step;
					if (tank.x > chang)	tank.x-=chang; break;
		}
		setfillcolor(BLUE);
		fillcircle(tank.x, tank.y, r); 
}

void Move_rand(RTANK &tank)
{	
	int i;

	clearcircle(tank.x, tank.y, 4);

	i= rand()%4;
	if(i==0) tank.x -= 20;
	else if(i==1) tank.x += 20;
	else if(i==2) tank.y -= 20;
	else tank.y += 20;

	if (tank.x > chang)	tank.x -=chang;
	else if(tank.x < 0) tank.x +=chang;
	if (tank.y > kuan)	tank.y -=kuan;
	else if(tank.y < 0) tank.y +=kuan;

	setfillcolor(GREEN);
	fillcircle(tank.x, tank.y, 3);
}
