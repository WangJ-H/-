/**************************************************************
*
* 此代码在Visual Studio 2022中调试通过，
* 该Visual Studio 2022安装了图形库easyX，
* (建议使用图形库)。
*
* 运行代码自动产生迷宫，当找到最短路径/迷宫无解时程序结束。
*
***************************************************************/


#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<graphics.h>
#include<Windows.h>


#define cell_x_num 40
#define cell_y_num 60
#define cell_len 10
#define WinWidth (cell_y_num*cell_len+100)
#define	WinHeight (cell_x_num*cell_len+100)

#define in_x 1
#define in_y 0
#define out_x (cell_x_num - 2)
#define out_y (cell_y_num - 1)

typedef struct stack
{
	int x;  //记录x坐标
	int y;  //记录y坐标
	int node_num;  //记录每一点的未走过的路口的个数
	int direction[4];  //记录可走的方向，为0不可走，为1：[0]右、[1]下、[2]左、[3]上
	int stack_num;  //栈内元素个数
	struct stack* next; //指向栈下一个元素的地址
}Stack;

void creat_maze(int cdnt[cell_x_num][cell_y_num]);
void init_frame(int cdnt[cell_x_num][cell_y_num], int x1, int y1, int x2, int y2);
void draw_cell(int i, int j, int color);
void init_window(int cdnt[cell_x_num][cell_y_num]);
void init_all(int cell_cdnt[cell_x_num][cell_y_num], Stack* path);

void stack_init(Stack* p);
void stack_enter(int cdnt[cell_x_num][cell_y_num],Stack* p, int x, int y);
void stack_out(int cdnt[cell_x_num][cell_y_num], Stack* p);
void calc_node_num(int cdnt[cell_x_num][cell_y_num], Stack* p);
void move(int cdnt[cell_x_num][cell_y_num], Stack* p);


void stack_output(Stack pa);
void output(int a[cell_x_num][cell_y_num]);

int main()
{
	int cell_cdnt[cell_x_num][cell_y_num] = { 0 };
	Stack path;

	init_all(cell_cdnt, &path);

	while (1)
	{
		//深度优先搜索DFS
		if (path.next->node_num != 0)
		{
			move(cell_cdnt, &path);
			draw_cell(path.next->x, path.next->y, RED);
		}
		else if (path.next->x != out_x)
		{
			draw_cell(path.next->x, path.next->y, getbkcolor());
			stack_out(cell_cdnt, &path);
			if (path.next->node_num != 0)
				calc_node_num(cell_cdnt, &path);
		}
		else if (path.next->y != out_y)
		{
			draw_cell(path.next->x, path.next->y, getbkcolor());
			stack_out(cell_cdnt, &path);
			if (path.next->node_num != 0)
				calc_node_num(cell_cdnt, &path);
		}
		else if (path.next->x == out_x && path.next->y == out_y)  //到达终点退出
			break;

		if (path.next->x == in_x && path.next->y == in_y)
			break;
		
		Sleep(30);
	}
	
	system("pause");

	closegraph();
	return 0;
}

void init_all(int cell_cdnt[cell_x_num][cell_y_num], Stack* path)
{
	init_window(cell_cdnt);
	stack_init(path);
	path->next = (Stack*)malloc(sizeof(Stack));
	path->next->stack_num = 0;

	stack_enter(cell_cdnt, path, in_x, in_y);
	draw_cell(in_x, in_y, RED);
	//将入口入栈
}

//(x1,y1)表示入口坐标，(x2,y2)表示出口坐标
void init_frame(int cdnt[cell_x_num][cell_y_num], int x1, int y1, int x2, int y2)
{
	for (int i = 1; i < cell_y_num-1; i++)
	{
		if (i != y1)
		{
			draw_cell(0, i, DARKGRAY);
			cdnt[0][i] = 1;
		}
		if (i != y2)
		{
			draw_cell(cell_x_num - 1, i, DARKGRAY);
			cdnt[cell_x_num - 1][i] = 1;
		}
	}
	for (int j = 0; j < cell_x_num ; j++)
	{
		if (j != x1)
		{
			draw_cell(j, 0, DARKGRAY);
			cdnt[j][0] = 1;
		}
		if (j != x2)
		{
			draw_cell(j, cell_y_num - 1, DARKGRAY);
			cdnt[j][cell_y_num - 1] = 1;
		}
	}
}

//随机产生障碍，记录到数组cdnt中
void creat_maze(int cdnt[cell_x_num][cell_y_num])
{
	srand((unsigned)time(NULL));
	for (int i = 1; i < cell_x_num-1; i++)
	{
		for (int j = 1; j < cell_y_num - 1; j++)
		{
			cdnt[i][j] = (rand() % 3) / 2;
			if (cdnt[i][j] == 1)
				draw_cell(i, j, LIGHTGRAY);
			else;

			if ((i == 1 && (j == 1 || j == 2 || j == 3)) || ((i == 2 || i == 3) && j == 1))
			{
				if (cdnt[i][j] == 1)
				{
					cdnt[i][j] = 0;
					draw_cell(i, j, WHITE);
				}
				else;
			}
			else;

			if ((i == cell_x_num - 2 &&
				(j == cell_y_num - 2 || j == cell_y_num - 3)) ||
				(i == cell_x_num - 3 && j == cell_y_num - 2))
			{
				if (cdnt[i][j] == 1)
				{
					cdnt[i][j] = 0;
					draw_cell(i, j, WHITE);
				}
				else;
			}
			else;
		}
		//需优化不能堵住出入口
	}
}

//将数组坐标转化为图形坐标并显示
void draw_cell(int i, int j, int color)
{
	int x = 50 - cell_len / 2 + i * cell_len;
	int y = 50 - cell_len / 2 + j * cell_len;
	setfillcolor(color);
	solidrectangle(y - cell_len / 2,x-cell_len/2, y + cell_len / 2, x + cell_len / 2);
}

//初始化窗口
void init_window(int cdnt[cell_x_num][cell_y_num])
{
	initgraph(WinWidth, WinHeight);// , EX_SHOWCONSOLE);
	setbkcolor(WHITE);
	cleardevice();
	creat_maze(cdnt);
	init_frame(cdnt, in_x, in_y, out_x, out_y);
}

//初始化栈
void stack_init(Stack* p)
{
	p = (Stack*)malloc(sizeof(Stack));
	if (p == NULL)
		exit(111);
}

//将一个坐标（x，y）入栈
void stack_enter(int cdnt[cell_x_num][cell_y_num],Stack* p, int x, int y)
{
	Stack* temp;
	temp = (Stack*)malloc(sizeof(Stack));
	temp->x = x;
	temp->y = y;
	for (int i = 0; i < 4; i++)
		temp->direction[i] = 0;
	temp->stack_num = p->next->stack_num + 1;
	temp->next = p->next;
	p->next = temp;
	cdnt[x][y] = 2;    //2表示在栈内的坐标
	calc_node_num(cdnt, p);
}

//将栈顶元素出栈
void stack_out(int cdnt[cell_x_num][cell_y_num],Stack* p)
{
	Stack* temp = p->next;
	cdnt[temp->x][temp->y] = 3;  //3表示走不通的路线
	p->next = p->next->next;
	free(temp);
}

//计算每一个坐标可以走的方向
void calc_node_num(int cdnt[cell_x_num][cell_y_num], Stack* p)
{
	int count = 0;
	int x = p->next->x;
	int y = p->next->y;
	if (x >= 1 && x <= cell_x_num - 1 && y >= 1 && y <= cell_y_num - 1)
	{
		if (cdnt[x - 1][y] == 0) {
			count++; p->next->direction[3] = 1;//上
		}
		if (cdnt[x + 1][y] == 0) {
			count++; p->next->direction[1] = 1;//下
		}
		if (cdnt[x][y - 1] == 0) {
			count++; p->next->direction[2] = 1;//左
		}
		if (cdnt[x][y + 1] == 0) {
			count++; p->next->direction[0] = 1;//右
		}
	}
	else if (x == 0 && y >= 1 && y <= cell_y_num - 1)
	{
		if (cdnt[x + 1][y] == 0) {
			count++; p->next->direction[1] = 1;//下
		}
	}
	else if (x == cell_x_num - 1 && y >= 1 && y <= cell_y_num - 1)
	{
		if (cdnt[x - 1][y] == 0) {
			count++; p->next->direction[3] = 1;//上
		}
		
	}
	else if (y == 0 && x >= 1 && x <= cell_x_num - 1)
	{
		if (cdnt[x][y + 1] == 0) {
			count++; p->next->direction[0] = 1;//右
		}
	}
	else if (y == cell_y_num - 1 && x >= 1 && x <= cell_x_num - 1)
	{
		if (cdnt[x][y - 1] == 0) {
			count++; p->next->direction[2] = 1;//左
		}
	}
	else;
	p->next->node_num = count;
}

//随机向未探索方向走一步，并将新位置入栈
void move(int cdnt[cell_x_num][cell_y_num], Stack* p)
{
	int direc = 0;
	int next_x = 0;
	int next_y = 0;
	for (int i = 0; i < 4; i++)
	{
		if (p->next->direction[i] == 1)
		{
			direc = i;
			p->next->direction[i] = 0;
			break;
		}
	}//确定方向

	switch (direc)
	{
	case(3):
		next_x = p->next->x - 1;
		next_y = p->next->y;
		break;//上
	case(2):
		next_x = p->next->x;
		next_y = p->next->y-1;
		break;//左
	case(1):
		next_x = p->next->x + 1;
		next_y = p->next->y;
		break;//下
	case(0):
		next_x = p->next->x;
		next_y = p->next->y + 1;
		break;//右
	default:
		break;
	}

	stack_enter(cdnt, p, next_x, next_y);
}













void stack_output(Stack pa)
{
	Stack* p = &pa;
	p = p->next;
	int n = p->stack_num;
	for (int i = 0; i < n; i++)
	{
		//draw_cell(p->x, p->y, RED);
		printf("%d,%d\t%d\n", p->x, p->y, p->stack_num);
		p = p->next;
	}
}

void output(int a[cell_x_num][cell_y_num])
{
	for (int i = 0; i < cell_x_num; i++)
	{
		for (int j = 0; j < cell_y_num; j++)
		{
			printf("%d ", a[i][j]);
		}printf("\n");
	}
}