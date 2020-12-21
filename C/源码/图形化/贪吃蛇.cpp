//额外插件可以设置加速 改变time之类的东西

#include <graphics.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>

#define Chang 20
#define Kuan 20
#define Gc 20
#define Time 500

int length=1;
char direction;
typedef struct
{
	int model;//0和1,再加一个2表示奖励道具
	char direction;//最后一个蛇身方块经过该方块时的方向
}Block;

typedef struct
{
	int gc;	  //将贪吃蛇的整个界面划分为小格的话,每一格的格长就是gc
	int chang;//整个大屏的横向长度为chang*gc
	int kuan; //整个大屏的纵向长度为kuan*gc
	Block **array;
}Graph;

typedef struct
{
	int x;
	int y;
}Special;

void init_graph(Graph &graph);//将Graph中的元素都赋予初值并用init_array()函数对array申请空间对边界置0的操作
void init_array(Block ** &array);

void init_special(Graph &graph, Special &head, Special &tail, Special &ball);//给贪吃蛇的头随机生成一个位置
void draw_block(int x, int y);//画出数组的序列号对应的图中的方块
void clear_block(int x, int y);////清除数组的序列号对应的图中的方块
void draw_ball(int x, int y);
void move_snake(Graph &graph, Special &head, Special &tail, Special &ball);
void improve(Graph &graph, Special &tail, Special &ball);

void main()
{
	Graph graph;
	Special head,tail,ball;

	init_graph(graph); 	initgraph(graph.chang*graph.gc,graph.kuan*graph.gc);

	init_special(graph,head,tail,ball);

	direction=getch();
	while(1)
	{
		Sleep(Time);
		move_snake(graph,head,tail,ball);
	}

	closegraph();
}

void improve(Graph &graph, Special &tail, Special &ball)
{
	length++;//延伸蛇身 这个用来记分的

	do{
		ball.x=rand()%Chang+1;
		ball.y=rand()%Kuan+1;
	}while(graph.array[ball.y][ball.x].model==0);

	graph.array[ball.y][ball.x].model=2;

	draw_ball(ball.x,ball.y);

	switch(graph.array[tail.y][tail.x].direction)
	{
		case 'w': tail.y++; graph.array[tail.y][tail.x].direction='w'; break;
		case 's': tail.y--; graph.array[tail.y][tail.x].direction='s'; break;
		case 'a': tail.x++; graph.array[tail.y][tail.x].direction='a'; break;
		case 'd': tail.x--; graph.array[tail.y][tail.x].direction='d'; 
	}
	draw_block(tail.x,tail.y);
	graph.array[tail.y][tail.x].model=0;
}

void move_snake(Graph &graph, Special &head, Special &tail, Special &ball)
{
	char c;
	int x,y;
	x=head.x; y=head.y;
	if(kbhit())
	{
		c=getch();
		switch(direction)
		{
			case 'w':
			case 's':
				if(c=='a'||c=='d')
				{
					graph.array[head.y][head.x].direction=c;
					direction=c;
					switch(c)
					{
						case 'a': x--; break;
						case 'd': x++; 
					}
				}
				break;
			case 'a':
			case 'd':
				if(c=='w'||c=='s')
				{
					graph.array[head.y][head.x].direction=c;
					direction=c;
					switch(c)
					{
						case 'w': y--; break;
						case 's': y++; 
					}
				}
		}
	}
	if(x==head.x&&y==head.y)
	{
		graph.array[head.y][head.x].direction=direction;
		switch(direction)
		{
			case 'w': y--; break;
			case 's': y++; break;
			case 'a': x--; break;
			case 'd': x++;
		}
	}

	if(graph.array[y][x].model==2)
	{
		improve(graph,tail,ball);
	}
	else if(graph.array[y][x].model==0)
	{
		printf("Game Over!\n");
		exit(-1);
	}

	head.x=x; head.y=y;
	//printf("头: x:%d y:%d ",head.x,head.y);
	//printf("球: x:%d y:%d\n",ball.x,ball.y);
	draw_block(head.x,head.y);

	graph.array[head.y][head.x].model=0;

	//printf("尾: x:%d y:%d\n",tail.x,tail.y);
	clear_block(tail.x,tail.y);
	graph.array[tail.y][tail.x].model=1;
	switch(graph.array[tail.y][tail.x].direction)
	{
		case 'w': tail.y--; break;
		case 's': tail.y++; break;
		case 'a': tail.x--; break;
		case 'd': tail.x++;
	}
}

void init_special(Graph &graph, Special &head, Special &tail, Special &ball)
{
	srand((unsigned)time(NULL));

	head.x=rand()%Chang+1;
	head.y=rand()%Kuan+1;
	graph.array[head.y][head.x].model=0;

	do{
		ball.x=rand()%Chang+1;
		ball.y=rand()%Kuan+1;
	}while(graph.array[ball.y][ball.x].model==0);
	graph.array[ball.y][ball.x].model=2;
	draw_ball(ball.x,ball.y);

	tail=head;
	draw_block(head.x,head.y);
}

void init_graph(Graph &graph)
{
	graph.gc=Gc;
	graph.chang=Chang;
	graph.kuan=Kuan;
	init_array(graph.array);
}

void init_array(Block ** &array)
{
	int i;
	array=(Block **)malloc((Kuan+2)*sizeof(Block *));//每一行的数组头

	for(i=0; i<=Kuan+1; i++)
	{
		array[i]=(Block *)malloc((Chang+2)*sizeof(Block));//给每个数组头分配空间

		array[i][0].model=0;		//这个组的第一个元素置为0表示不可过
		array[i][Chang+1].model=0;//这个组的最后一个元素置为0表示不可过 总:界面的左右边界
	}

	for(i=1; i<=Chang; i++)	//界面的上下边界
	{
		array[0][i].model=0;
		array[Kuan+1][i].model=0;
	}

	for(int j=1; j<=Kuan; j++)//界面的中心空白
		for(i=1; i<=Chang; i++)
			array[j][i].model=1;
}

void draw_block(int x, int y)
{
	setfillcolor(BLUE);
	solidrectangle((x-1)*Gc+2,(y-1)*Gc+2,x*Gc-2,y*Gc-2);
}

void clear_block(int x, int y)
{
	clearrectangle((x-1)*Gc,(y-1)*Gc,x*Gc,y*Gc);
}

void draw_ball(int x,int y)
{
	setfillcolor(RED);
	solidcircle(x*Gc-Gc/2,y*Gc-Gc/2,5);
}