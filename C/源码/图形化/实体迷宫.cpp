#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>

enum Direction{DOWN=0,RIGHT,UP,LEFT};
typedef struct
{
	int x;
	int y;
}DIAN;

typedef struct
{
	DIAN *base;
	DIAN *top;
}STACK;

typedef struct
{
	int hg;    //迷宫每一行有多少个格子
	int zg;    //迷宫每一列有多少个格子
	int gc;    //每一个格子为正方形,这里是正方形的边长
	int **base;//这是迷宫每一个位置判断是否为墙壁的二维数组
}MAZE;

void init_maze(MAZE &maze);//给maze的base申请二维数组的空间,边界置0,中心区格子置为1
void init_jiemian(MAZE maze);//建线 内置draw函数
void draw(MAZE maze,int color);//画出整个迷宫的墙壁
void change_color(MAZE maze,int i,int j,int color);//改变二维数组对应方格的颜色
void mousesetting(MAZE &maze,DIAN &start,DIAN &end);//用鼠标设置起点、终点等信息
void trave_maze(MAZE maze,DIAN start,DIAN end);
void draw_man(DIAN man,int gc);//以MAN结构体的x和y数组坐标,画出实际坐标中的小人

void main()
{
	DIAN start,end;//man作为起点,end作为终点
	MAZE maze;
	init_maze(maze);

	initgraph(500,300);

	settextcolor(YELLOW);
	char s0[]="作 者: 王 敏 聪";
	outtextxy(20,10,s0);

	settextcolor(WHITE);
	char s1[]="接 下 来 将 进 行 自 主 建 迷 宫 :";
	outtextxy(10,50,s1);
	char s2[]="\"鼠标左键\" : 建 立 墙 壁";
	outtextxy(10,100,s2);
	char s3[]="\"Ctrl + 鼠标左键\" : 擦 除 墙 壁 (注:初始的蓝色墙壁无法擦除)";
	outtextxy(10,100,s3);
	char s4[]="\"Shift + 鼠标左键\" : 建立起始点、终点 (注:设置只有一次,谨慎选择)";
	outtextxy(10,150,s4);
	char s5[]="\"鼠标右键\" : 建 立 完 成,进 入 观 看 迷 宫 的 探 索 过 程!";
	outtextxy(10,200,s5);
	char s6[]="如 果 看 明 白 了,按 任 意 键 开 始!";
	outtextxy(10,250,s6);
	getche();
	closegraph();

	initgraph((maze.hg+2)*maze.gc,(maze.zg+2)*maze.gc);//初始化格子,进入鼠标建壁过程
	init_jiemian(maze);
	mousesetting(maze,start,end);

	trave_maze(maze,start,end);
	closegraph();
}

void trave_maze(MAZE maze,DIAN start,DIAN end)
{
	int dir;
	STACK stack;
	stack.base=(DIAN *)malloc(100*sizeof(DIAN));
	stack.top=stack.base-1;

	DIAN man1=start,man2=man1;//man2用于探索迷宫,如果先行的man2当前路径可行,就让man1移动过来
	draw_man(man1,maze.gc);

	maze.base[start.y][start.x]=0;
	while(man1.x!=end.x||man1.y!=end.y)//迷宫主体直到man1找到出口结束
	{
		Sleep(500);
		for(dir=DOWN; dir<=LEFT; dir++)//每次找到一个man1能走的方向,否则跳回上一个位置
		{
			switch(dir)
			{
				case DOWN:  man2.y++; break;
				case RIGHT: man2.x++; break;
				case UP:	man2.y--; break;
				case LEFT : man2.x--;
			}
			if(maze.base[man2.y][man2.x]!=0)//不是墙壁都能走
			{
				*++stack.top=man1;
				change_color(maze,man1.y,man1.x,BLACK);
				maze.base[man2.y][man2.x]=0;
				draw_man(man2,maze.gc);
				man1=man2;
				break;
			}
			else man2=man1;
		}
		if(dir>LEFT&&stack.top>=stack.base)
		{
			change_color(maze,man1.y,man1.x,BLACK);
			draw_man(*stack.top,maze.gc);
			man2=man1=*stack.top--;
		}
		else if(stack.top<stack.base)
		{
			char lost[]="你 这 迷 宫 什 么 鬼! 按 任 意 键 退 出. . .";
			outtextxy(10,10,lost);
			getche();
			return;
		}
	}
	draw_man(man2,maze.gc);
	char win[]="成 功 找 到 出 口! 按 任 意 键 退 出. . .";
	outtextxy(10,10,win);
	getche();
}

void draw_man(DIAN man,int gc)
{
	man.x=man.x*gc+int(0.5*gc);
	man.y=man.y*gc+int(0.5*gc);
	setlinecolor(BLUE);
	circle(man.x,man.y-5,5);//脑袋
	fillcircle(man.x+2,man.y-6,2);//右眼
	fillcircle(man.x-2,man.y-6,2);//左眼
	line(man.x,man.y,man.x,man.y+8);//躯干
	line(man.x,man.y+2,man.x+5,man.y+5);//右手
	line(man.x,man.y+2,man.x-5,man.y+5);//左手
	line(man.x,man.y+8,man.x+8,man.y+12);//右腿
	line(man.x,man.y+8,man.x-8,man.y+12);//左腿
}

void mousesetting(MAZE &maze,DIAN &start,DIAN &end)
{
	int z=1,z1=1,z2=1;
	int x,y;

	MOUSEMSG m;

	while(z)
	{
		m = GetMouseMsg();
		y=m.y/maze.gc,x=m.x/maze.gc;
		if(( x>0 && x<maze.hg+1) && ( y>0 && y<maze.zg+1 ))
		switch(m.uMsg)
		{
			case WM_LBUTTONDOWN:
			{
				if(m.mkCtrl&&(maze.base[y][x]==0||maze.base[y][x]==1))//消除墙壁
				{
					maze.base[y][x]=1;//数组标志为1
					change_color(maze,y,x,BLACK);//空位:黑色
				}
				else if(m.mkShift&&z1&&maze.base[y][x]!=0)//只设置一次且只能设置再空位
				{
					maze.base[y][x]=2;//数组标志为2
					change_color(maze,y,x,YELLOW);//起点:黄色
					start.x=x,start.y=y;
					z1=0;
				}
				else if(m.mkShift&&z2&&maze.base[y][x]!=0&&maze.base[y][x]!=2)//只设置一次且只能设置再空位
				{
					maze.base[y][x]=3;//数组标志为3
					change_color(maze,y,x,RED);//终点:红色
					end.x=x,end.y=y;
					z2=0;
				}
				else if(maze.base[y][x]==1)//墙壁只能建在空位上
				{
					maze.base[y][x]=0;//数组标志为0
					change_color(maze,y,x,BLUE);//墙壁:蓝色
				}
				break;
			}
			case WM_RBUTTONUP:
				if(z1==0&&z2==0)
				{
					z=0;//鼠标右键 设置结束
				}
		}
	}
}

void init_maze(MAZE &maze)
{
	int i,j;
	printf("初始化迷宫大小...\n");
	printf("请输入迷宫的横向格数和纵向格数 (格长默认为30):\n");
	scanf("%d %d",&maze.hg,&maze.zg);
	maze.gc=30;

	maze.base=(int **)malloc((maze.zg+2)*sizeof(int *));
	for(i=0; i<maze.zg+2; i++)
		maze.base[i]=(int *)malloc((maze.hg+2)*sizeof(int));

	for(i=1; i<=maze.zg; i++)//中心置空
	{
		for(j=1; j<=maze.hg; j++)
		{
			maze.base[i][j]=1;
		}
	}
	for(i=0; i<maze.zg+2; i++)//上下边界
	{
		maze.base[i][0]=0;
		maze.base[i][maze.hg+1]=0;
	}
	for(i=0; i<maze.hg+2; i++)//左右边界
	{
		maze.base[0][i]=0;
		maze.base[maze.zg+1][i]=0;
	}
}

void init_jiemian(MAZE maze)
{
	int i;

	setlinecolor(MAGENTA);
	for(i=0; i<maze.hg+2; i++)
		line(i*maze.gc,0,i*maze.gc,(maze.zg+2)*maze.gc);//纵线

	for(i=0; i<maze.zg+2; i++)
		line(0,i*maze.gc,(maze.hg+2)*maze.gc,i*maze.gc);//横线

	draw(maze,BLUE);
}

void draw(MAZE maze,int color)
{
	int i,j;
	setfillcolor(color);
	for(i=0; i<maze.zg+2; i++)
	{
		for(j=0; j<maze.hg+2; j++)
			if(maze.base[i][j]==0)
			{
				solidrectangle(j*maze.gc,i*maze.gc,(j+1)*maze.gc,(i+1)*maze.gc);
			}
	}
}

void change_color(MAZE maze,int i,int j,int color)
{
	setfillcolor(color);
	solidrectangle(j*maze.gc+1,i*maze.gc+1,(j+1)*maze.gc-1,(i+1)*maze.gc-1);
}
