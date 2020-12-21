//time(&rawtime)������ȡ��ǰʱ���1970��1��1�յ����������������λ������rawtime ��

#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <stdio.h>
#define chang 640
#define kuan 480
#define ammomax 3//һ����Ļ�ڳ��ֵ���൯ҩ��
#define tankmax 4//��ʼ�з�̹�˵�����
#define mustkill 20//��Ϸʤ��Ŀ��
int sum=tankmax;//sum���ڼ�¼��ǰ��Ļ�ڻ�ʣ��ĵз�̹�˵�����
int kill=0;//��¼Ŀǰ�ɵ���̹����

struct TANK
{
	int x,y;
	int half;
	int dir;
	int step;
	int exist;
};

TANK stank;
void init_bianjie();							//��ʼ����ɫ���α߽�
void init_wenzi();								//��ʼ����Ϸ˵���ͼǷ���
void init_ammo(TANK &ammo);						//��ʼ����ҩ,��ʼ��ҩ������
void init_stank();								//��ʼ����̹�˴�С �ٶ� ����
void init_rtank(TANK &tank,int i);				//i����̹�˵ı��,��ֹ�ص�
void draw_stank(TANK tank);						//������̹��
void draw_rtank(TANK tank);						//�����з�̹��
void move_stank(char c,TANK *tank);				//�ƶ���̹��,���ж��ƶ����λ��
void move_rtank(TANK *tank,int s,TANK *ammo);	//�ƶ��з�̹��,���ж��ƶ����λ��
void stank_shoot(TANK *ammo,int s,TANK *tank);	//��̹�����,�жϵ�ǰ�ڿ��Ƿ����ӵ�δ���ü��ƶ�,���ж��ڿ�λ��
void rtank_shoot(TANK tank,TANK *ammo,int s);	//�з�̹��������ж��ڿ�λ��
void move_rammo(TANK &ammo);					//�ƶ���ɫ�ӵ�,���ж��ƶ����λ��
void exam_rammo(TANK &ammo);					//����ӵ���ǰλ���Ƿ����̹��,���ж��Ƿ�Խ��
void move_sammo(TANK *ammo,int s,TANK *tank);	//�ƶ���ɫ�ӵ�,���ж��ƶ����λ��
void exam_sammo(TANK *ammo,int s,TANK *tank);	//�����̹�˵��ӵ��ƶ���ĵ��Ƿ���˵з�̹���Լ�����ӵ��Ƿ񵽱߽�

void main()
{
	char c;
	int i,j;
	double t1,t2,t3;
	TANK rtank[tankmax],sammo[ammomax],rammo[tankmax][ammomax];
	srand((unsigned)time(NULL));
	initgraph(chang+20+200, kuan+20);//��ʼ�������С
	setorigin(10, 10);//Ϊ�����ñ߽�,��ԭ�������޸���
	init_bianjie();
	init_stank();//��ʼ����̹��
	init_wenzi();

	for(i=0; i<ammomax; i++)//��ʼ��̹�˻���ҩ
		init_ammo(sammo[i]);

	for(i=0; i<tankmax; i++)//��ʼ���з������̹��
	{
		init_rtank(rtank[i],i);
		for(j=0; j<ammomax; j++)//��ʼ�з�̹�˻���ҩ,ÿ���з�̹�˶����Է������maxammo�������ӵ�
			init_ammo(rammo[i][j]);
		draw_rtank(rtank[i]);
	}

	t3=t2=t1=time(NULL);//��ʼʱ��
	while(1)
	{
		if(kbhit())//����㰴�˼�,̹�˾Ͷ�Ӧ��������,������û����,�Զ�����
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
			t1+=0.5;//���������С��1ʱ�ͱ����ÿ���ƶ�(1/����)�Ĵ���!
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
		if(time(NULL)==int(t2+2))//�з�̹��ÿ�����ƶ�һ��
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
		if(time(NULL)==int(t3+3))//�з�̹��ÿ�������һ��
		{
			t3=t3+3;

			for(i=0; i<tankmax; i++)
				if(rtank[i].exist==1)//���ŵ�̹�����
				for(j=0; j<ammomax; j++)
					if(rammo[i][j].exist==0)//ѡ��һ����������ӵ�����,���,��֮,̹�˲����������
					{
						rtank_shoot(rtank[i],rammo[i],j);
						break;
					}
		}
	}
}

void rtank_shoot(TANK tank,TANK *ammo,int s)
{
	ammo[s].exist=1;//�����ӵ�,�ӵ�����,����ÿ���ƶ�ʱҪ�ƶ���
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
	if(ammo.x<0||ammo.x>chang||ammo.y<0||ammo.y>kuan)//����ӵ��Ƿ񵽱߽�
		ammo.exist=0;
}

void stank_shoot(TANK *ammo,int s,TANK *tank)
{
	int i;
	ammo[s].exist=1;//�����ӵ�,�ӵ�����,����ÿ���ƶ�ʱҪ�ƶ���
	ammo[s].dir=stank.dir;	
	switch(ammo[s].dir)
	{
		case 0: ammo[s].x=stank.x; ammo[s].y=stank.y-15; break;
		case 1: ammo[s].x=stank.x; ammo[s].y=stank.y+15; break;
		case 2:	ammo[s].x=stank.x-15; ammo[s].y=stank.y; break;
		case 3:	ammo[s].x=stank.x+15; ammo[s].y=stank.y;
	}
	for(i=0; i<s; i++)//������ʹ��ո񰴵Ĺ����ӵ�Ҳ�����ص�
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
			z=1;//����õ�ҩ�ͱ�ĵ�ҩ�ص���,������ƶ��Ļ�,�ͻ��ԭλ�õĵ�ҩҲ�����
			break;
		}
	}

	if(z==0) clearcircle(ammo[s].x,ammo[s].y,ammo[s].half+1);//ԭ��û�������ӵ��Ļ���ɾ��ԭλ�õ��ӵ�ͼ��
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
		clearrectangle(chang+110,210,chang+130,230);//����ұ���Ϣ������з�̹�˵ĸ���,�����´�ӡ����
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
	if(ammo[s].x<0||ammo[s].x>chang||ammo[s].y<0||ammo[s].y>kuan)//����ӵ��Ƿ񵽱߽�
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
	stank.y=460;//��ʼ����̹��
	draw_stank(stank);
}

void move_rtank(TANK *tank,int s,TANK *ammo)
{
	int i,t,z=1;
	clearrectangle(tank[s].x-tank[s].half-4,tank[s].y-tank[s].half-4,tank[s].x+tank[s].half+4,tank[s].y+tank[s].half+4);
	tank[s].dir=rand()%4;
	switch(tank[s].dir)
	{
		case 0://̹�˷���Ϊ��
			{
				tank[s].y-=tank[s].step;
				if (tank[s].y - tank[s].half < 0) tank[s].y=tank[s].half+2;//�߽�����
				for(i=0; i<tankmax; i++)//�Ƿ��б��̹��
				{
					if(i==s) continue;//�ų��Լ�
					t=tank[s].x-tank[i].x;//���ƺ�����
					if(t<0) t=tank[i].x-tank[s].x;
					if(tank[i].exist==1&&tank[s].y-tank[i].y>0)//ֻ����tank[s]�Ϸ����ڵ�̹��
					if(tank[s].y-tank[i].y<tank[s].half+tank[i].half+2&&t<tank[s].half+tank[i].half+4)//������ص�
					tank[s].y=tank[i].y+tank[s].half+tank[i].half+8;
				}
				t=tank[s].x-stank.x;//���ƺ�����
					if(t<0) t=stank.x-tank[s].x;
				if(tank[s].y-stank.y>0)//�Ƿ�����̹��
					if(tank[s].y-stank.y<tank[s].half+stank.half+2&&t<tank[s].half+stank.half+4)//������ص�
					tank[s].y=stank.y+tank[s].half+stank.half+8;
				break;
			}
		case 1://̹�˷���Ϊ��
			{
				tank[s].y+=tank[s].step;
				if (tank[s].y + tank[s].half > kuan) tank[s].y=kuan-tank[s].half-2;
				for(i=0; i<tankmax; i++)//�Ƿ��б��̹��
				{
					if(i==s) continue;
					t=tank[s].x-tank[i].x;//���ƺ�����
					if(t<0)
						t=tank[i].x-tank[s].x;
					if(tank[i].exist==1&&tank[i].y-tank[s].y>0)//ֻ����tank[s]�·����ڵ�̹��
					if(tank[i].y-tank[s].y<tank[s].half+tank[i].half+2&&t<tank[s].half+tank[i].half+4)//������ص�
					tank[s].y=tank[i].y-tank[s].half-tank[i].half-8;
				}
				t=tank[s].x-stank.x;//���ƺ�����
					if(t<0) t=stank.x-tank[s].x;
				if(stank.y-tank[s].y>0)//�Ƿ�����̹��
					if(stank.y-tank[s].y<tank[s].half+stank.half+2&&t<tank[s].half+stank.half+4)//������ص�
					tank[s].y=stank.y-tank[s].half-stank.half-8;
				break;
			}
		case 2://̹�˷���Ϊ��
			{
				tank[s].x-=tank[s].step;
				if(tank[s].x - tank[s].half < 0)
					tank[s].x=tank[s].half+2;
				for(i=0; i<tankmax; i++)//�Ƿ��б��̹��
				{
					if(i==s) continue;
					t=tank[s].y-tank[i].y;//����������
					if(t<0)
						t=tank[i].y-tank[s].y;
					if(tank[i].exist==1&&tank[s].x-tank[i].x>0)//ֻ����tank[s]�󷽴��ڵ�̹��
					if(tank[s].x-tank[i].x<tank[s].half+tank[i].half+2&&t<tank[s].half+tank[i].half+4)//������ص�
					tank[s].x=tank[i].x+tank[s].half+tank[i].half+8;
				}
				t=tank[s].y-stank.y;//����������
					if(t<0) t=stank.y-tank[s].y;
				if(tank[s].x-stank.x>0)//�Ƿ�����̹��
					if(tank[s].x-stank.x<tank[s].half+stank.half+2&&t<tank[s].half+stank.half+4)//������ص�
					tank[s].x=stank.x+tank[s].half+stank.half+8;
				break;
			}
		case 3://̹�˷���Ϊ��
			{
				tank[s].x+=tank[s].step;
				if (tank[s].x + tank[s].half > chang)
					tank[s].x=chang-tank[s].half-2;
				for(i=0; i<tankmax; i++)//�Ƿ��б��̹��
				{
					if(i==s) continue;
					t=tank[s].y-tank[i].y;//����������
					if(t<0)
						t=tank[i].y-tank[s].y;
					if(tank[i].exist==1&&tank[i].x-tank[s].x>0)//ֻ����tank[s]�ҷ����ڵ�̹��
					if(tank[i].x-tank[s].x<tank[s].half+tank[i].half+2&&t<tank[s].half+tank[i].half+4)//������ص�
					tank[s].x=tank[i].x-tank[s].half-tank[i].half-8;
				}
				t=tank[s].y-stank.y;//����������
					if(t<0) t=stank.y-tank[s].y;
				if(stank.x-tank[s].x>0)//�Ƿ�����̹��
					if(stank.x-tank[s].x<tank[s].half+stank.half+2&&t<tank[s].half+stank.half+4)//������ص�
					tank[s].x=stank.x-tank[s].half-stank.half-8;
			}
	}
	for(i=0; i<ammomax; i++)//���̹���ƶ��������������ӵ�
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
	clearrectangle(stank.x-stank.half-4,stank.y-stank.half-4,stank.x+stank.half+4,stank.y+stank.half+4);//���֮ǰ��̹��
	switch(c)
		{
			case 'w':
			case 'W':
				{
					stank.dir=0;
					stank.y -= stank.step;
					if (stank.y - stank.half < 0)
						stank.y=stank.half+2;//�жϱ߽�����
					for(i=0; i<tankmax; i++)//�Ƿ��б��̹��
					{
						t=stank.x-rtank[i].x;//���ƺ�����
						if(t<0) t=rtank[i].x-stank.x;
						if(rtank[i].exist==1&&stank.y-rtank[i].y>0)//ֻ��stank�Ϸ����ڵ�̹��
						if(stank.y-rtank[i].y<stank.half+rtank[i].half+2&&t<stank.half+rtank[i].half+4)//������ص�
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
					for(i=0; i<tankmax; i++)//�Ƿ��б��̹��
					{
						t=stank.x-rtank[i].x;//���ƺ�����
						if(t<0) t=rtank[i].x-stank.x;
						if(rtank[i].exist==1&&rtank[i].y-stank.y>0)//ֻ��stank�·����ڵ�̹��
						if(rtank[i].y-stank.y<stank.half+rtank[i].half+2&&t<stank.half+rtank[i].half+4)//������ص�
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
					for(i=0; i<tankmax; i++)//�Ƿ��б��̹��
					{
						t=stank.y-rtank[i].y;//����������
						if(t<0)
							t=rtank[i].y-stank.y;
						if(rtank[i].exist==1&&stank.x-rtank[i].x>0)//ֻ��stank�󷽴��ڵ�̹��
						if(stank.x-rtank[i].x<stank.half+rtank[i].half+2&&t<stank.half+rtank[i].half+4)//������ص�
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
					for(i=0; i<tankmax; i++)//�Ƿ��б��̹��
					{
						t=stank.y-rtank[i].y;//����������
						if(t<0)
							t=rtank[i].y-stank.y;
						if(rtank[i].exist==1&&rtank[i].x-stank.x>0)//ֻ��stank�ҷ����ڵ�̹��
						if(rtank[i].x-stank.x<stank.half+rtank[i].half+2&&t<stank.half+rtank[i].half+4)//������ص�
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
	fillroundrect(tank.x-tank.half,tank.y-tank.half,tank.x+tank.half,tank.y+tank.half,tank.half,tank.half);//�����
	fillcircle(tank.x,tank.y,5);//�ڿڻ���
	switch(tank.dir)
	{
		case 0:
		{
			fillrectangle(tank.x-1,tank.y-12,tank.x+1,tank.y+1);//�ڿ�

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
			fillrectangle(tank.x-1,tank.y-1,tank.x+1,tank.y+12);//�ڿ�

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
			fillrectangle(tank.x-12,tank.y-1,tank.x+1,tank.y+1);//�ڿ�

			fillcircle(tank.x,tank.y-tank.half,2);//����������
			fillcircle(tank.x-tank.half*1/5*2,tank.y-tank.half,2);
			fillcircle(tank.x-tank.half*2/5*2,tank.y-tank.half,2);
			fillcircle(tank.x+tank.half*1/5*2,tank.y-tank.half,2);
			fillcircle(tank.x+tank.half*2/5*2,tank.y-tank.half,2);

			fillcircle(tank.x,tank.y+tank.half,2);//�ұ��������
			fillcircle(tank.x-tank.half*1/5*2,tank.y+tank.half,2);
			fillcircle(tank.x-tank.half*2/5*2,tank.y+tank.half,2);
			fillcircle(tank.x+tank.half*1/5*2,tank.y+tank.half,2);
			fillcircle(tank.x+tank.half*2/5*2,tank.y+tank.half,2);
			break;
		}
		case 3:
		{
			fillrectangle(tank.x-1,tank.y-1,tank.x+12,tank.y+1);//�ڿ�

			fillcircle(tank.x,tank.y-tank.half,2);//����������
			fillcircle(tank.x-tank.half*1/5*2,tank.y-tank.half,2);
			fillcircle(tank.x-tank.half*2/5*2,tank.y-tank.half,2);
			fillcircle(tank.x+tank.half*1/5*2,tank.y-tank.half,2);
			fillcircle(tank.x+tank.half*2/5*2,tank.y-tank.half,2);

			fillcircle(tank.x,tank.y+tank.half,2);//�ұ��������
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
	fillcircle(tank.x,tank.y,tank.half);//�����
	fillroundrect(tank.x-2,tank.y-2,tank.x+2,tank.y+2,2,2);//�ڿڻ���
	switch(tank.dir)
	{
		case 0:
		{
			fillrectangle(tank.x-1,tank.y-12,tank.x+1,tank.y+1);//�ڿ�

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
			fillrectangle(tank.x-1,tank.y-1,tank.x+1,tank.y+12);//�ڿ�

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
			fillrectangle(tank.x-12,tank.y-1,tank.x+1,tank.y+1);//�ڿ�

			fillcircle(tank.x,tank.y-tank.half,2);//����������
			fillcircle(tank.x-tank.half*1/5*2,tank.y-tank.half,2);
			fillcircle(tank.x-tank.half*2/5*2,tank.y-tank.half,2);
			fillcircle(tank.x+tank.half*1/5*2,tank.y-tank.half,2);
			fillcircle(tank.x+tank.half*2/5*2,tank.y-tank.half,2);

			fillcircle(tank.x,tank.y+tank.half,2);//�ұ��������
			fillcircle(tank.x-tank.half*1/5*2,tank.y+tank.half,2);
			fillcircle(tank.x-tank.half*2/5*2,tank.y+tank.half,2);
			fillcircle(tank.x+tank.half*1/5*2,tank.y+tank.half,2);
			fillcircle(tank.x+tank.half*2/5*2,tank.y+tank.half,2);
			break;
		}
		case 3:
		{
			fillrectangle(tank.x-1,tank.y-1,tank.x+12,tank.y+1);//�ڿ�

			fillcircle(tank.x,tank.y-tank.half,2);//����������
			fillcircle(tank.x-tank.half*1/5*2,tank.y-tank.half,2);
			fillcircle(tank.x-tank.half*2/5*2,tank.y-tank.half,2);
			fillcircle(tank.x+tank.half*1/5*2,tank.y-tank.half,2);
			fillcircle(tank.x+tank.half*2/5*2,tank.y-tank.half,2);

			fillcircle(tank.x,tank.y+tank.half,2);//�ұ��������
			fillcircle(tank.x-tank.half*1/5*2,tank.y+tank.half,2);
			fillcircle(tank.x-tank.half*2/5*2,tank.y+tank.half,2);
			fillcircle(tank.x+tank.half*1/5*2,tank.y+tank.half,2);
			fillcircle(tank.x+tank.half*2/5*2,tank.y+tank.half,2);		
		}
	}
}

void init_bianjie()
{
	setfillcolor(MAGENTA);//��������
	solidrectangle(-10,-10,chang+210,-5);		//�ϱ߿�(����������)
	solidrectangle(-10,kuan+5,chang+210,kuan+10);//�±߿�(����������)
	solidrectangle(-10,-10,-5,kuan+5);			//��߿�
	solidrectangle(chang+5,-10,chang+10,kuan+5);//�ұ߿�

	solidrectangle(chang+205,-10,chang+210,kuan+5);
}

void init_wenzi()
{
	settextcolor(YELLOW);
	char zj[] = "����: ������";
	char yd0[]="̹�˿��Ʒ�ʽ:",yd1[]="w: �����ƶ�",yd2[]="A: �����ƶ�",yd3[]="S: �����ƶ�",yd4[]="D: �����ƶ�";
	outtextxy(chang+20, 0, zj);
	settextcolor(WHITE);
	outtextxy(chang+20, 40, yd0);
	outtextxy(chang+20, 60, yd1);
	outtextxy(chang+20, 80, yd2);
	outtextxy(chang+20, 100, yd3);
	outtextxy(chang+20, 120, yd4);

	char zd[]="�ո�: �����ӵ�";
	outtextxy(chang+20, 150, zd);

	settextcolor(LIGHTGREEN);
	char win1[]="ʤ��: ����";
	outtextxy(chang+20, 190, win1);

	char a[3];
	a[0]=mustkill/10+48;
	a[1]=mustkill%10+48;
	a[2]=0;
	outtextxy(chang+93, 190, a);

	char win2[]="���з�̹��";
	outtextxy(chang+110, 190, win2);

	settextcolor(RED);
	char k[]="��������:";
	outtextxy(chang+20, 210, k);

	char b[3];
	b[0]=kill/10+48;
	b[1]=kill%10+48;
	b[2]=0;
	outtextxy(chang+100, 210, b);
}