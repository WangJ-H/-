/**************************************************************
*
* 此代码在Visual Studio 2022中调试通过，
* 该Visual Studio 2022安装了图形库easyX，
* 若无此图形库，请将宏定义中的graph改为0。
* (建议使用图形库)
* 
* 运行代码自动产生迷宫，当找到最短路径/迷宫无解时程序结束。
* 
***************************************************************/

#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<graphics.h>
#include<Windows.h>

#define graph 1

#define cell_x_num 40  //迷宫高度
#define cell_y_num 60  //迷宫宽度
#define cell_len 10  //绘制单个方格大小
#define WinWidth (cell_y_num*cell_len+100)  //窗口宽度
#define	WinHeight (cell_x_num*cell_len+100)  //窗口高度

#define in_x 1  //入口x
#define in_y 0  //
#define out_x (cell_x_num - 2)
#define out_y (cell_y_num - 1)

typedef struct stack
{
	int x;  //记录x坐标
	int y;  //记录y坐标
	int stack_num;  //从头到尾的数量
	struct stack* pre;  //指向上一个元素的指针
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
	queue[0].y = -1; //防止队列指针为空

	Stack* temp = (Stack*)calloc(2, sizeof(Stack));
	int queue_num = 1;  //队列中元素的个数
	int t_road_num = 1;  //暂存每个结点之后可走的方向数
	Stack node_temp[4] = { 0 };  //暂存返回的结点后可走的坐标，并入队
	Stack* temp_pre=(Stack*)calloc(cell_x_num*cell_y_num, sizeof(Stack));  //暂存已搜索过的结点
	int n = 0;
	bool exitence = false;

	path.pre = NULL;
	path.x = in_x;
	path.y = in_y;
	path.stack_num = 1;

	temp = (Stack*)realloc(queue, (++queue_num) * sizeof(Stack));
	queue = temp;
	queue[queue_num - 1] = path;  //将起点坐标入队
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
			temp_pre[nm] = queue[1];  //存储每一个结点，用于后面回溯

			if (queue[1].x == out_x && queue[1].y == out_y) {
				jd = true;
				break;
			}   //判断是否到达终点
			else;

			//计算queue[1]结点的下一路径，存入node_temp
			calc_node_road(cell_cdnt, &t_road_num, &queue[1], node_temp);

			for (int i = 1; i < queue_num-1; i++)
				queue[i] = queue[i + 1];
			temp = (Stack*)realloc(queue, (--queue_num) * sizeof(Stack));
			queue = temp;   //将上面的queue[1]出队
			//计算一个，出队一个

			for (int j = 0; j < t_road_num; j++)
			{
				queue_num++;
				temp = (Stack*)realloc(queue, (queue_num) * sizeof(Stack));
				queue = temp;
				queue[queue_num - 1] = node_temp[j];
				queue[queue_num - 1].pre = &temp_pre[nm];
				queue[queue_num - 1].stack_num = temp_pre[nm - 1].stack_num + 1;
				//将得到的新节点入队，同时计算相关参数，并指向上一节点temp_pre[nm]
			}

			//将temp_pre后移一位，若空间不足则重新分配空间
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
			printf("无解\n");
			exitence = true;
			break;
		}
		if (graph)
			Sleep(30);
	}


	//画出最短路线，数组cell_cdnt中路线坐标赋为3
	Stack* mm = &queue[1];
	if (!exitence)
	{
		printf("最短路径坐标为：\n");
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


//(x1,y1)表示入口坐标，(x2,y2)表示出口坐标
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

//随机产生障碍，记录到数组cdnt中
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

			//优化不能堵住出入口
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

//将数组坐标转化为图形坐标并显示
void draw_cell(int i, int j, int color)
{
	int x = 50 - cell_len / 2 + i * cell_len;
	int y = 50 - cell_len / 2 + j * cell_len;
	setfillcolor(color);
	solidrectangle(y - cell_len / 2, x - cell_len / 2, y + cell_len / 2, x + cell_len / 2);
}

//初始化窗口
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

//初始化栈头
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

//计算结点node处的可走路径，并以Stack数组形式返回，n为可走路径数
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
			temp[num - 1] = creat_node(node, x - 1, y);//上
			if (graph)
				draw_cell(x - 1, y, YELLOW);
		}
		if (cdnt[x + 1][y] == 0) {
			cdnt[x + 1][y] = 2;
			num++;
			temp[num - 1] = creat_node(node, x + 1, y);//下
			if (graph)
				draw_cell(x + 1, y, YELLOW);
		}
		if (cdnt[x][y - 1] == 0) {
			cdnt[x][y - 1] = 2;
			num++;
			temp[num - 1] = creat_node(node, x, y - 1);//左
			if (graph)
				draw_cell(x, y-1, YELLOW);
		}
		if (cdnt[x][y + 1] == 0) {
			cdnt[x][y + 1] = 2;
			num++;
			//temp = (Stack*)realloc(temp, (num) * sizeof(Stack));
			temp[num - 1] = creat_node(node, x, y + 1);//右
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
			temp[num - 1] = creat_node(node, x + 1, y);//下
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
			temp[num - 1] = creat_node(node, x - 1, y);//上
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
			temp[num - 1] = creat_node(node, x, y + 1);//右
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
			temp[num - 1] = creat_node(node, x, y - 1);//左
			if (graph)
				draw_cell(x, y-1, YELLOW);
		}
	}
	else;

	*n = num;
}

//产生一个坐标为(x,y)的结点，指向p，并返回
Stack creat_node(Stack* p, int x, int y)
{
	Stack temp = { 0 };
	temp.x = x;
	temp.y = y;
	temp.pre = p;
	temp.stack_num = p->stack_num + 1;
	return temp;
}

//输出二维数组
void output(int a[cell_x_num][cell_y_num])
{
	for (int i = 0; i < cell_x_num; i++)
	{
		for (int j = 0; j < cell_y_num; j++)
			printf("%d ", a[i][j]);
		printf("\n");
	}
}