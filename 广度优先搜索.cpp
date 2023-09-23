/**************************************************************
*
* �˴�����Visual Studio 2022�е���ͨ����
* ��Visual Studio 2022��װ��ͼ�ο�easyX��
* ���޴�ͼ�ο⣬�뽫�궨���е�graph��Ϊ0��
* (����ʹ��ͼ�ο�)
* 
* ���д����Զ������Թ������ҵ����·��/�Թ��޽�ʱ���������
* 
***************************************************************/

#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<graphics.h>
#include<Windows.h>

#define graph 1

#define cell_x_num 40  //�Թ��߶�
#define cell_y_num 60  //�Թ����
#define cell_len 10  //���Ƶ��������С
#define WinWidth (cell_y_num*cell_len+100)  //���ڿ��
#define	WinHeight (cell_x_num*cell_len+100)  //���ڸ߶�

#define in_x 1  //���x
#define in_y 0  //
#define out_x (cell_x_num - 2)
#define out_y (cell_y_num - 1)

typedef struct stack
{
	int x;  //��¼x����
	int y;  //��¼y����
	int stack_num;  //��ͷ��β������
	struct stack* pre;  //ָ����һ��Ԫ�ص�ָ��
}Stack;

void creat_maze(int cdnt[cell_x_num][cell_y_num]);
void init_frame(int cdnt[cell_x_num][cell_y_num], int x1, int y1, int x2, int y2);
void draw_cell(int i, int j, int color);
void init_window(int cdnt[cell_x_num][cell_y_num]);
void stack_init(Stack* p);

void calc_node_road(int cdnt[cell_x_num][cell_y_num], int* n, Stack* node, Stack temp[]);
Stack creat_node(Stack* p, int x, int y);

void output(int a[cell_x_num][cell_y_num]);



int main()
{
	int cell_cdnt[cell_x_num][cell_y_num] = { 0 };
	Stack path;
	Stack* queue = (Stack*)calloc(2,sizeof(Stack));
	queue[0].x = -1;
	queue[0].y = -1; //��ֹ����ָ��Ϊ��

	Stack* temp = (Stack*)calloc(2, sizeof(Stack));
	int queue_num = 1;  //������Ԫ�صĸ���
	int t_road_num = 1;  //�ݴ�ÿ�����֮����ߵķ�����
	Stack node_temp[4] = { 0 };  //�ݴ淵�صĽ�����ߵ����꣬�����
	Stack* temp_pre=(Stack*)calloc(cell_x_num*cell_y_num, sizeof(Stack));  //�ݴ����������Ľ��
	int n = 0;
	bool exitence = false;

	path.pre = NULL;
	path.x = in_x;
	path.y = in_y;
	path.stack_num = 1;

	temp = (Stack*)realloc(queue, (++queue_num) * sizeof(Stack));
	queue = temp;
	queue[queue_num - 1] = path;  //������������
	cell_cdnt[in_x][in_y] = 2;

	init_window(cell_cdnt);
	
	bool jd = false;
	int m = 0;
	int nm = 0;
	
	while (1)
	{
		n = queue_num;
		for (int i = 1; i < n; i++)
		{
			temp_pre[nm] = queue[1];  //�洢ÿһ����㣬���ں������

			if (queue[1].x == out_x && queue[1].y == out_y) {
				jd = true;
				break;
			}   //�ж��Ƿ񵽴��յ�
			else;

			//����queue[1]������һ·��������node_temp
			calc_node_road(cell_cdnt, &t_road_num, &queue[1], node_temp);

			for (int i = 1; i < queue_num-1; i++)
				queue[i] = queue[i + 1];
			temp = (Stack*)realloc(queue, (--queue_num) * sizeof(Stack));
			queue = temp;   //�������queue[1]����
			//����һ��������һ��

			for (int j = 0; j < t_road_num; j++)
			{
				queue_num++;
				temp = (Stack*)realloc(queue, (queue_num) * sizeof(Stack));
				queue = temp;
				queue[queue_num - 1] = node_temp[j];
				queue[queue_num - 1].pre = &temp_pre[nm];
				queue[queue_num - 1].stack_num = temp_pre[nm - 1].stack_num + 1;
				//���õ����½ڵ���ӣ�ͬʱ������ز�������ָ����һ�ڵ�temp_pre[nm]
			}

			//��temp_pre����һλ�����ռ䲻�������·���ռ�
			nm++;
			if (nm >= cell_x_num * cell_y_num)
			{
				m++;
				temp = (Stack*)realloc(temp_pre, (cell_x_num * cell_y_num + m) * sizeof(Stack));
				temp_pre = temp;
			}
		}

		if (jd)
			break;
		else;

		if (queue_num == 1)
		{
			printf("�޽�\n");
			exitence = true;
			break;
		}
		if (graph)
			Sleep(30);
	}


	//�������·�ߣ�����cell_cdnt��·�����긳Ϊ3
	Stack* mm = &queue[1];
	if (!exitence)
	{
		printf("���·������Ϊ��\n");
		while (1)
		{
			if (mm->x == path.x && mm->y == path.y)
				break;
			printf("%d\t%d\n", mm->x, mm->y);
			if(graph)
				draw_cell(mm->x, mm->y, RED);
			cell_cdnt[mm->x][mm->y] = 3;
			mm = mm->pre;
		}
		if(graph)
			draw_cell(in_x, in_y, RED);
	}
	printf("\n");
	output(cell_cdnt);

	system("pause");
	if (graph)
		closegraph();
	return 0;
}


//(x1,y1)��ʾ������꣬(x2,y2)��ʾ��������
void init_frame(int cdnt[cell_x_num][cell_y_num], int x1, int y1, int x2, int y2)
{
	for (int i = 1; i < cell_y_num - 1; i++)
	{
		if (i != y1)
		{
			if (graph)
				draw_cell(0, i, DARKGRAY);
			cdnt[0][i] = 1;
		}
		if (i != y2)
		{
			if (graph)
				draw_cell(cell_x_num - 1, i, DARKGRAY);
			cdnt[cell_x_num - 1][i] = 1;
		}
	}
	for (int j = 0; j < cell_x_num; j++)
	{
		if (j != x1)
		{
			if (graph)
				draw_cell(j, 0, DARKGRAY);
			cdnt[j][0] = 1;
		}
		if (j != x2)
		{
			if (graph)
				draw_cell(j, cell_y_num - 1, DARKGRAY);
			cdnt[j][cell_y_num - 1] = 1;
		}
	}
}

//��������ϰ�����¼������cdnt��
void creat_maze(int cdnt[cell_x_num][cell_y_num])
{
	srand((unsigned)time(NULL));
	int r = rand() % 2;
	int a = cell_x_num;
	int b = cell_y_num;
	
	for (int i = 1; i < cell_x_num - 1; i++)
	{
		for (int j = 1; j < cell_y_num - 1; j++)
		{
			if (graph)
				cdnt[i][j] = (rand() % 3) / 2;
			else
				cdnt[i][j] = (rand() % 2);
			if (cdnt[i][j] == 1)
				if (graph)
					draw_cell(i, j, LIGHTGRAY);
			else;

			if ((i == 1 && (j == 1 || j == 2 || j == 3)) || ((i == 2 || i == 3) && j == 1))
			{
				if (cdnt[i][j] == 1)
				{
					cdnt[i][j] = 0;
					if (graph)
						draw_cell(i, j, WHITE);
				}
				else;
			}
			else;

			//�Ż����ܶ�ס�����
			if ((i == cell_x_num - 2 &&
				(j == cell_y_num - 2 || j == cell_y_num - 3)) ||
				(i == cell_x_num - 3 && j == cell_y_num - 2))
			{
				if (cdnt[i][j] == 1)
				{
					cdnt[i][j] = 0;
					if (graph)
						draw_cell(i, j, WHITE);
				}
				else;
			}
			else;
		}
	}
	
	if (graph)
	{
		if (a <= b)
		{
			for (int i = 1; i < (int)(2 * a / 3); i++)
			{
				if (r)
				{
					cdnt[i][(int)(b / 3)] = 1;
					if (graph)
						draw_cell(i, b / 3, LIGHTGRAY);

					cdnt[a - i - 1][(int)(2 * b / 3)] = 1;
					if (graph)
						draw_cell(a - i - 1, (int)(2 * b / 3), LIGHTGRAY);
				}
				else
				{
					cdnt[a - i - 1][(int)(b / 3)] = 1;
					if (graph)
						draw_cell(a - i - 1, (int)(b / 3), LIGHTGRAY);

					cdnt[i][(int)(2 * b / 3)] = 1;
					if (graph)
						draw_cell(i, (int)(2 * b / 3), LIGHTGRAY);
				}
			}
		}
	}
}

//����������ת��Ϊͼ�����겢��ʾ
void draw_cell(int i, int j, int color)
{
	int x = 50 - cell_len / 2 + i * cell_len;
	int y = 50 - cell_len / 2 + j * cell_len;
	setfillcolor(color);
	solidrectangle(y - cell_len / 2, x - cell_len / 2, y + cell_len / 2, x + cell_len / 2);
}

//��ʼ������
void init_window(int cdnt[cell_x_num][cell_y_num])
{
	if (graph)
	{
		initgraph(WinWidth, WinHeight);// , EX_SHOWCONSOLE);
		setbkcolor(WHITE);
		cleardevice();
	}
	creat_maze(cdnt);
	init_frame(cdnt, in_x, in_y, out_x, out_y);
}

//��ʼ��ջͷ
void stack_init(Stack* p)
{
	p= (Stack*)malloc(sizeof(Stack));
	p->pre = NULL;
	p->x = in_x;
	p->y = in_y;
	p->stack_num = 1;

	//p->next[0] = (Stack*)malloc(sizeof(Stack));
	//p->next[0]->pre = p;
}

//������node���Ŀ���·��������Stack������ʽ���أ�nΪ����·����
void calc_node_road(int cdnt[cell_x_num][cell_y_num], int* n, Stack* node,Stack temp[])
{
	int x = node->x;
	int y = node->y;
	//Stack temp[4] = { 0 };
	int num = 0;
	if (x >= 1 && x <= cell_x_num - 1 && y >= 1 && y <= cell_y_num - 1)
	{
		if (cdnt[x - 1][y] == 0) {
			cdnt[x - 1][y] = 2;
			num++;
			temp[num - 1] = creat_node(node, x - 1, y);//��
			if (graph)
				draw_cell(x - 1, y, YELLOW);
		}
		if (cdnt[x + 1][y] == 0) {
			cdnt[x + 1][y] = 2;
			num++;
			temp[num - 1] = creat_node(node, x + 1, y);//��
			if (graph)
				draw_cell(x + 1, y, YELLOW);
		}
		if (cdnt[x][y - 1] == 0) {
			cdnt[x][y - 1] = 2;
			num++;
			temp[num - 1] = creat_node(node, x, y - 1);//��
			if (graph)
				draw_cell(x, y-1, YELLOW);
		}
		if (cdnt[x][y + 1] == 0) {
			cdnt[x][y + 1] = 2;
			num++;
			//temp = (Stack*)realloc(temp, (num) * sizeof(Stack));
			temp[num - 1] = creat_node(node, x, y + 1);//��
			if (graph)
				draw_cell(x , y+1, YELLOW);
		}
	}
	else if (x == 0 && y >= 1 && y <= cell_y_num - 1)
	{
		if (cdnt[x + 1][y] == 0) {
			cdnt[x + 1][y] = 2;
			num++;
			//temp = (Stack*)realloc(temp, (num) * sizeof(Stack));
			temp[num - 1] = creat_node(node, x + 1, y);//��
			if (graph)
				draw_cell(x + 1, y, YELLOW);
		}
	}
	else if (x == cell_x_num - 1 && y >= 1 && y <= cell_y_num - 1)
	{
		if (cdnt[x - 1][y] == 0) {
			cdnt[x - 1][y] = 2;
			num++;
			//temp = (Stack*)realloc(temp, (num) * sizeof(Stack));
			temp[num - 1] = creat_node(node, x - 1, y);//��
			if (graph)
				draw_cell(x - 1, y, YELLOW);
		}

	}
	else if (y == 0 && x >= 1 && x <= cell_x_num - 1)
	{
		if (cdnt[x][y + 1] == 0) {
			cdnt[x][y + 1] = 2;
			num++;
			//temp = (Stack*)realloc(temp, (num) * sizeof(Stack));
			temp[num - 1] = creat_node(node, x, y + 1);//��
			if (graph)
				draw_cell(x, y+1, YELLOW);
		}
	}
	else if (y == cell_y_num - 1 && x >= 1 && x <= cell_x_num - 1)
	{
		if (cdnt[x][y - 1] == 0) {
			cdnt[x][y - 1] = 2;
			num++;
			//temp = (Stack*)realloc(temp, (num) * sizeof(Stack));
			temp[num - 1] = creat_node(node, x, y - 1);//��
			if (graph)
				draw_cell(x, y-1, YELLOW);
		}
	}
	else;

	*n = num;
}

//����һ������Ϊ(x,y)�Ľ�㣬ָ��p��������
Stack creat_node(Stack* p, int x, int y)
{
	Stack temp = { 0 };
	temp.x = x;
	temp.y = y;
	temp.pre = p;
	temp.stack_num = p->stack_num + 1;
	return temp;
}

//�����ά����
void output(int a[cell_x_num][cell_y_num])
{
	for (int i = 0; i < cell_x_num; i++)
	{
		for (int j = 0; j < cell_y_num; j++)
			printf("%d ", a[i][j]);
		printf("\n");
	}
}