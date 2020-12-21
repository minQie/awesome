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
	int hg;    //�Թ�ÿһ���ж��ٸ�����
	int zg;    //�Թ�ÿһ���ж��ٸ�����
	int gc;    //ÿһ������Ϊ������,�����������εı߳�
	int **base;//�����Թ�ÿһ��λ���ж��Ƿ�Ϊǽ�ڵĶ�ά����
}MAZE;

void init_maze(MAZE &maze);//��maze��base�����ά����Ŀռ�,�߽���0,������������Ϊ1
void init_jiemian(MAZE maze);//���� ����draw����
void draw(MAZE maze,int color);//���������Թ���ǽ��
void change_color(MAZE maze,int i,int j,int color);//�ı��ά�����Ӧ�������ɫ
void mousesetting(MAZE &maze,DIAN &start,DIAN &end);//�����������㡢�յ����Ϣ
void trave_maze(MAZE maze,DIAN start,DIAN end);
void draw_man(DIAN man,int gc);//��MAN�ṹ���x��y��������,����ʵ�������е�С��

void main()
{
	DIAN start,end;//man��Ϊ���,end��Ϊ�յ�
	MAZE maze;
	init_maze(maze);

	initgraph(500,300);

	settextcolor(YELLOW);
	char s0[]="�� ��: �� �� ��";
	outtextxy(20,10,s0);

	settextcolor(WHITE);
	char s1[]="�� �� �� �� �� �� �� �� �� �� �� :";
	outtextxy(10,50,s1);
	char s2[]="\"������\" : �� �� ǽ ��";
	outtextxy(10,100,s2);
	char s3[]="\"Ctrl + ������\" : �� �� ǽ �� (ע:��ʼ����ɫǽ���޷�����)";
	outtextxy(10,100,s3);
	char s4[]="\"Shift + ������\" : ������ʼ�㡢�յ� (ע:����ֻ��һ��,����ѡ��)";
	outtextxy(10,150,s4);
	char s5[]="\"����Ҽ�\" : �� �� �� ��,�� �� �� �� �� �� �� ̽ �� �� ��!";
	outtextxy(10,200,s5);
	char s6[]="�� �� �� �� �� ��,�� �� �� �� �� ʼ!";
	outtextxy(10,250,s6);
	getche();
	closegraph();

	initgraph((maze.hg+2)*maze.gc,(maze.zg+2)*maze.gc);//��ʼ������,������꽨�ڹ���
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

	DIAN man1=start,man2=man1;//man2����̽���Թ�,������е�man2��ǰ·������,����man1�ƶ�����
	draw_man(man1,maze.gc);

	maze.base[start.y][start.x]=0;
	while(man1.x!=end.x||man1.y!=end.y)//�Թ�����ֱ��man1�ҵ����ڽ���
	{
		Sleep(500);
		for(dir=DOWN; dir<=LEFT; dir++)//ÿ���ҵ�һ��man1���ߵķ���,����������һ��λ��
		{
			switch(dir)
			{
				case DOWN:  man2.y++; break;
				case RIGHT: man2.x++; break;
				case UP:	man2.y--; break;
				case LEFT : man2.x--;
			}
			if(maze.base[man2.y][man2.x]!=0)//����ǽ�ڶ�����
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
			char lost[]="�� �� �� �� ʲ ô ��! �� �� �� �� �� ��. . .";
			outtextxy(10,10,lost);
			getche();
			return;
		}
	}
	draw_man(man2,maze.gc);
	char win[]="�� �� �� �� �� ��! �� �� �� �� �� ��. . .";
	outtextxy(10,10,win);
	getche();
}

void draw_man(DIAN man,int gc)
{
	man.x=man.x*gc+int(0.5*gc);
	man.y=man.y*gc+int(0.5*gc);
	setlinecolor(BLUE);
	circle(man.x,man.y-5,5);//�Դ�
	fillcircle(man.x+2,man.y-6,2);//����
	fillcircle(man.x-2,man.y-6,2);//����
	line(man.x,man.y,man.x,man.y+8);//����
	line(man.x,man.y+2,man.x+5,man.y+5);//����
	line(man.x,man.y+2,man.x-5,man.y+5);//����
	line(man.x,man.y+8,man.x+8,man.y+12);//����
	line(man.x,man.y+8,man.x-8,man.y+12);//����
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
				if(m.mkCtrl&&(maze.base[y][x]==0||maze.base[y][x]==1))//����ǽ��
				{
					maze.base[y][x]=1;//�����־Ϊ1
					change_color(maze,y,x,BLACK);//��λ:��ɫ
				}
				else if(m.mkShift&&z1&&maze.base[y][x]!=0)//ֻ����һ����ֻ�������ٿ�λ
				{
					maze.base[y][x]=2;//�����־Ϊ2
					change_color(maze,y,x,YELLOW);//���:��ɫ
					start.x=x,start.y=y;
					z1=0;
				}
				else if(m.mkShift&&z2&&maze.base[y][x]!=0&&maze.base[y][x]!=2)//ֻ����һ����ֻ�������ٿ�λ
				{
					maze.base[y][x]=3;//�����־Ϊ3
					change_color(maze,y,x,RED);//�յ�:��ɫ
					end.x=x,end.y=y;
					z2=0;
				}
				else if(maze.base[y][x]==1)//ǽ��ֻ�ܽ��ڿ�λ��
				{
					maze.base[y][x]=0;//�����־Ϊ0
					change_color(maze,y,x,BLUE);//ǽ��:��ɫ
				}
				break;
			}
			case WM_RBUTTONUP:
				if(z1==0&&z2==0)
				{
					z=0;//����Ҽ� ���ý���
				}
		}
	}
}

void init_maze(MAZE &maze)
{
	int i,j;
	printf("��ʼ���Թ���С...\n");
	printf("�������Թ��ĺ��������������� (��Ĭ��Ϊ30):\n");
	scanf("%d %d",&maze.hg,&maze.zg);
	maze.gc=30;

	maze.base=(int **)malloc((maze.zg+2)*sizeof(int *));
	for(i=0; i<maze.zg+2; i++)
		maze.base[i]=(int *)malloc((maze.hg+2)*sizeof(int));

	for(i=1; i<=maze.zg; i++)//�����ÿ�
	{
		for(j=1; j<=maze.hg; j++)
		{
			maze.base[i][j]=1;
		}
	}
	for(i=0; i<maze.zg+2; i++)//���±߽�
	{
		maze.base[i][0]=0;
		maze.base[i][maze.hg+1]=0;
	}
	for(i=0; i<maze.hg+2; i++)//���ұ߽�
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
		line(i*maze.gc,0,i*maze.gc,(maze.zg+2)*maze.gc);//����

	for(i=0; i<maze.zg+2; i++)
		line(0,i*maze.gc,(maze.hg+2)*maze.gc,i*maze.gc);//����

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
