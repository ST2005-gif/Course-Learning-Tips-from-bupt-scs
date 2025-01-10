#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:6031)
#include<graphics.h>
#include<conio.h>
#include<stdio.h>	
#include<stdlib.h>
#include<string.h>
#define BAR_WIDTH 300
#define BAR_HEIGHT 40
#define SLIDER_WIDTH 20
typedef struct Food
{
	char foodname[60];
	int foodtime;
	int foodmax;
	int remain;
	int ordernum;
	int orderfinnum;
	int timer;
} food;
typedef struct Set
{
	char setname[60];
	int foodid[5];
	int size;
} set;
typedef struct Order
{
	int ordertime;
	int foodid[5];
	int foodcon[5];
	int size;
	int finishtime;
	int con;
	int showtime;
} order;
typedef struct {
	int x, y;
	int value;
} ProgressBar;
food* foods = NULL;
int foodsize = 0;
set* sets = NULL;
int setsize = 0;
order orders[100];
int ordersize = 0;
int w1;
int w2;
int ordernum = 0; //每秒正在制作的订单数
int ordercon = 2; //系统状态，2代表关闭，1代表开放
int porder = -1;  //最早的正在制作的订单号
int TIME = 0;
int time;
int check;
int ac = 0;       //加速倍数
int foodmax = 1;//全局变量
int shine;
char timept[10];
ExMessage a;
IMAGE img;

int findfood(char a[])
{
	int b;
	for (b = 0; b < foodsize; b++)
		if (strcmp(a, foods[b].foodname) == 0)
			return b;
	return -1;
}
int findset(char a[])
{
	int b;
	for (b = 0; b < setsize; b++)
		if (strcmp(a, sets[b].setname) == 0)
			return b;
	return -1;
}
int judgeorder(int a)
{
	int b;
	for (b = 0; b < orders[a].size; b++)
		if (orders[a].foodcon[b] == 0)
			return 1;
	return 2;
}
int checkorder(int a)
{
	if (ordernum == w1 && ordercon == 2 && a == 1)
	{
		ordercon = 1;
		return 1;
	}
	else if (ordernum < w2 && ordercon == 1)
	{
		ordercon = 2;
		return 0;
	}
	else if (ordernum < w1 && ordercon == 2)
	{
		ordercon = 2;
		return 1;
	}
	else
		return 0;
}
int findorder(int b)
{
	int a;
	if (b == -1)
		b = 0;
	for (a = b; a < ordersize; a++)
		if (orders[a].ordertime == time)
			return a;
	return -1;
}
void initializefoodset()
{
	int a, c, d;
	char b[60];
	FILE* fptr;
	fptr = fopen("dict.dic", "r");
	fscanf(fptr, "%d %d", &foodsize, &setsize);
	foods = (food*)malloc(foodsize * sizeof(food));
	for (a = 0; a < foodsize; a++)
	{
		fscanf(fptr, "%s", foods[a].foodname);
	}
	for (a = 0; a < foodsize; a++)
	{
		fscanf(fptr, "%d", &foods[a].foodtime);
	}
	for (a = 0; a < foodsize; a++)
	{
		fscanf(fptr, "%d", &foods[a].foodmax);
		if (foods[a].foodmax > foodmax)//判定最大数
			foodmax = foods[a].foodmax;
		foods[a].orderfinnum = foods[a].ordernum = foods[a].remain = foods[a].timer = 0;
	}
	fscanf(fptr, "%d %d", &w1, &w2);
	sets = (set*)malloc(setsize * sizeof(set));
	fscanf(fptr, "%s", sets[0].setname);
	a = c = 0;
	while (!feof(fptr))
	{
		fscanf(fptr, "%s", b);
		d = findfood(b);
		if (d == -1 && a < setsize && !feof(fptr))
		{
			sets[a].size = c;
			a++;
			strcpy(sets[a].setname, b);
			c = 0;
		}
		else if (a < setsize)
		{
			sets[a].foodid[c] = d;
			c++;
		}
	}
	sets[a].size = c - 1;
	fclose(fptr);
}
void makefood()
{
	int a;
	for (a = 0; a < foodsize; a++)
	{
		if (foods[a].remain < foods[a].foodmax)
			foods[a].timer++;
		if (foods[a].timer == foods[a].foodtime)
		{
			if (foods[a].ordernum == 0)
				foods[a].remain++;
			else
			{
				foods[a].ordernum--;
				foods[a].orderfinnum++;
			}
			foods[a].timer = 0;
		}
	}
}
void allocateorder(int a)
{
	int x;
	for (x = 0; x < orders[a].size; x++)
	{
		if (foods[orders[a].foodid[x]].remain == 0)
			foods[orders[a].foodid[x]].ordernum++;
		orders[a].foodcon[x] = 0;
	}
	orders[a].con = 1;
	ordernum++;
}
void processorder(int a, int c)
{
	int b;
	for (b = 0; b < orders[a].size; b++)
	{
		if (foods[orders[a].foodid[b]].remain > 0 && orders[a].foodcon[b] == 0)
		{
			foods[orders[a].foodid[b]].remain--;
			orders[a].foodcon[b] = 1;
		}
		else if (c == 1 && foods[orders[a].foodid[b]].orderfinnum > 0 && orders[a].foodcon[b] == 0)
		{
			foods[orders[a].foodid[b]].orderfinnum--;
			orders[a].foodcon[b] = 1;
		}

	}
	if (judgeorder(a) == 2)
	{
		if (porder == a)
			porder = -1;
		orders[a].finishtime = time;
		orders[a].con = 2;
		ordernum--;
	}
	else
	{
		if (porder == -1)
			porder = a;
	}
}

void printorder()
{
	int a;
	int hour, minute, second;
	FILE* fptr;
	fptr = fopen("output.txt", "w");
	for (a = 0; a < ordersize; a++)
	{
		if (orders[a].con == 2)
		{
			hour = orders[a].finishtime / 3600;
			minute = (orders[a].finishtime % 3600) / 60;
			second = orders[a].finishtime % 60;
			fprintf(fptr, "%02d:%02d:%02d\n", hour, minute, second);
		}
		else
			fprintf(fptr, "Fail\n");
	}
	fclose(fptr);
}
void doorder()
{
	int a, b;
	if (ordernum != 0)
	{
		for (a = porder; orders[a].ordertime < time && a < ordersize; a++)
			if (orders[a].con == 1)
			{
				processorder(a, 1);
			}
	}
	if (time <= 79200)
	{
		a = findorder(porder);
		if (a != -1)
			b = checkorder(1);
		else
			b = checkorder(0);
		if (a != -1 && b == 1)
		{
			allocateorder(a);
			if (porder == -1)
				porder = a;
			processorder(a, 0);
			if (ordernum == w1)
				ordercon = 2;
		}
	}
	makefood();
}

void printText(int x, int y, const char* text, int fontSize, const char* fontType, COLORREF fillColor, COLORREF textColor)
{
	// 保存当前字体设置
	LOGFONT oldFont;
	gettextstyle(&oldFont);

	// 保存当前背景模式和颜色
	int oldBkMode = getbkmode();
	COLORREF oldBkColor = getbkcolor();
	COLORREF oldTextColor = gettextcolor();

	// 设置新的字体样式
	LOGFONT newFont = oldFont;
	newFont.lfHeight = fontSize;
	_tcscpy(newFont.lfFaceName, fontType);
	settextstyle(&newFont);

	// 设置新的填充颜色和字体颜色
	setbkmode(OPAQUE);
	setbkcolor(fillColor);
	settextcolor(textColor);

	// 在指定位置输出文本
	outtextxy(x, y, text);

	// 恢复原先的字体设置
	settextstyle(&oldFont);
	setbkmode(oldBkMode);
	setbkcolor(oldBkColor);
	settextcolor(oldTextColor);
}
void drawRoundRect(int left, int top, int right, int bottom, int aa, int bb, COLORREF fillColor)
{
	// 保存当前填充颜色
	COLORREF oldFillColor = getfillcolor();
	// 设置新的填充颜色
	setfillcolor(fillColor);
	setlinecolor(fillColor);
	solidroundrect(left, top, right, bottom, aa, bb);
	// 恢复原先的填充颜色
	setfillcolor(oldFillColor);
}
void drawRoundRectWithBorder(int left, int top, int right, int bottom, int aa, int bb, COLORREF fillColor, COLORREF borderColor)
{
	// 保存当前填充颜色和边框颜色
	COLORREF oldFillColor = getfillcolor();
	COLORREF oldBorderColor = getlinecolor();

	// 设置新的填充颜色和边框颜色
	setfillcolor(fillColor);
	setlinecolor(borderColor);

	fillroundrect(left, top, right, bottom, aa, bb);

	// 恢复原先的填充颜色和边框颜色
	setfillcolor(oldFillColor);
	setlinecolor(oldBorderColor);
}

void clickorderfood(int x)
{
	char k[6];
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	setfont(34, 0, "仿宋");
	drawRoundRectWithBorder(940, 190, 1290, 450, 20, 20, WHITE, BLUE);
	outtextxy(950, 200, "你点的套餐:");
	outtextxy(950, 240, foods[x].foodname);
	outtextxy(950, 280, "点餐时间:");
	outtextxy(950, 320, timept);
	outtextxy(950, 360, "订单号:");
	sprintf(k, "%d", ordersize);
	outtextxy(950, 400, k);
	setfillcolor(LIGHTBLUE);
	solidroundrect(800, 290, 880, 370, 30, 30);
	setfont(40, 0, "楷体");
	setbkcolor(LIGHTBLUE);
	outtextxy(800, 310, "确定");
	while (1)
	{
		peekmessage(&a);
		if (a.message == WM_LBUTTONDOWN)
		{
			if (a.x > 800 && a.x < 880 && a.y>290 && a.y < 370)
			{
				orders[ordersize].ordertime = time;
				orders[ordersize].foodid[0] = x;
				orders[ordersize].size = 1;
				orders[ordersize].con = 0;
				orders[ordersize].showtime = shine;
				ordersize++;
				check = 1;
				break;
			}
			else if (a.x > 600 && a.x < 700 && a.y > 600 && a.y < 700)
			{
				printorder();
				exit(0);
			}
		}
		else if (a.message == WM_RBUTTONDOWN)
			if (a.x > 800 && a.x < 900 && a.y > 800 && a.y < 900)
				break;
	}
}
void clickorderset(int x)
{
	char k[6];
	int b;
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	setfont(34, 0, "仿宋");
	drawRoundRectWithBorder(940, 190, 1290, 450, 20, 20, WHITE, BLUE);
	outtextxy(950, 200, "你点的套餐:");
	outtextxy(950, 240, sets[x].setname);
	outtextxy(950, 280, "点餐时间:");
	outtextxy(950, 320, timept);
	outtextxy(950, 360, "订单号:");
	sprintf(k, "%d", ordersize);
	outtextxy(950, 400, k);
	setfillcolor(LIGHTBLUE);
	solidroundrect(800, 290, 880, 370, 30, 30);
	setfont(40, 0, "楷体");
	setbkcolor(LIGHTBLUE);
	outtextxy(800, 310, "确定");
	while (1)
	{
		peekmessage(&a);
		if (a.message == WM_LBUTTONDOWN)
		{
			if (a.x > 800 && a.x < 880 && a.y>290 && a.y < 370)
			{
				orders[ordersize].ordertime = time;
				for (b = 0; b < sets[x].size; b++)
					orders[ordersize].foodid[b] = sets[x].foodid[b];
				orders[ordersize].size = sets[x].size;
				orders[ordersize].con = 0;
				orders[ordersize].showtime = shine;
				ordersize++;
				check = 1;
				break;
			}
			else if (a.x > 600 && a.x < 700 && a.y > 600 && a.y < 700)
			{
				printorder();
				exit(0);
			}
		}
		else if (a.message == WM_RBUTTONDOWN)
			if (a.x > 800 && a.x < 900 && a.y > 800 && a.y < 900)
				break;
	}
}
void drawfoodset()
{
	int x, y, z;
	char w[4];
	drawRoundRectWithBorder(940, 820, 1250, 870, 20, 20, RGB(162, 159, 237), BLUE);
	printText(950, 830, "当前状态：处理状态", 30, "楷体", RGB(162, 159, 237), WHITE);
	setfillcolor(RED);
	settextcolor(BLACK);
	setbkcolor(YELLOW);
	setfont(25, 0, "楷体");
	setfillcolor(YELLOW);
	solidroundrect(45, 18, 210, 50, 20, 20);
	outtextxy(50, 20, "食物单品名称");
	solidroundrect(225, 18, 285, 50, 20, 20);
	outtextxy(230, 20, "余量");
	solidroundrect(288, 18, 400, 50, 20, 20);
	outtextxy(290, 20, "制作进程");
	solidroundrect(410, 18, 540, 50, 20, 20);
	outtextxy(420, 20, "套餐名称");
	solidroundrect(570, 18, 645, 50, 20, 20);
	outtextxy(580, 20, "余量");
	setfont(30, 0, "Times New Roman");
	for (x = 0; x < foodsize; x++)
	{
		setfillcolor(YELLOW);
		solidroundrect(25, x * 50 + 65, 350, x * 50 + 105, 20, 20);
		sprintf(w, "%d", foods[x].remain);
		outtextxy(50, x * 50 + 70, foods[x].foodname);
		outtextxy(275, x * 50 + 70, w);
		sprintf(w, "%d", foods[x].timer);
		outtextxy(320, x * 50 + 70, w);
	}
	for (x = 0; x < setsize; x++)
	{
		setfillcolor(YELLOW);
		solidroundrect(360, x * 50 + 65, 650, x * 50 + 105, 20, 20);//  x右边改为了650，鼠标识别
		outtextxy(370, x * 50 + 70, sets[x].setname);
		y = 0;
		for (z = 0; z <= foodmax && y != -1; z++)
		{
			for (y = 0; y < sets[x].size; y++)//判定多套餐
				if (foods[sets[x].foodid[y]].remain == z)
				{
					sprintf(w, "%d", z);
					outtextxy(625, x * 50 + 70, w);
					y = -1;
					break;
				}
		}
	}
}
void drawinitiate()
{

	drawRoundRect(800, 800, 900, 900, 20, 20, LIGHTBLUE);
	printText(810, 830, "点餐", 40, "楷体", LIGHTBLUE, WHITE);
}
void failprint()
{
	char a[4];
	if (ordersize - 1 > w1 && orders[ordersize - 1].con == 0 && orders[ordersize - 1].showtime > -1)
	{
		sprintf(a, "%d", ordersize - 1);
		printText(720, 700, "抱歉，系统关闭中", 30, "宋体", RED, BLACK);
		printText(1000, 700, a, 30, "宋体", RED, BLACK);
		printText(1040, 700, "号订单下单失败", 30, "宋体", RED, BLACK);
	}
}
void printunfinish() {
	int x = 1430, y = 610;
	for (int i = 0; i < ordersize; i++) {
		if (orders[i].con == 1) {
			char temp[10], str[10];
			int hour, minute, second;
			hour = orders[i].finishtime / 3600;
			minute = (orders[i].finishtime % 3600) / 60;
			second = orders[i].finishtime % 60;
			sprintf(temp, "%02d:%02d:%02d", hour, minute, second);
			sprintf(str, "%d", i);
			drawRoundRect(x - 5, y - 5, x + 25, y + 25, 10, 10, RGB(123, 199, 230));
			printText(x, y, str, 25, "Times New Roman", RGB(123, 199, 230), YELLOW);
			y += 30;
		}
	}

}
void printfinish() {
	int x = 1380;
	int y = 110;
	for (int i = 0; i < ordersize; i++) {
		if (orders[i].con == 2 && orders[i].showtime > -1) //多显示一秒
		{
			char temp[10], str[10];
			int hour, minute, second;
			hour = orders[i].finishtime / 3600;
			minute = (orders[i].finishtime % 3600) / 60;
			second = orders[i].finishtime % 60;
			sprintf(temp, "%02d:%02d:%02d", hour, minute, second);
			settextstyle(25, 0, _T("Times New Roman"));
			sprintf(str, "%d", i);
			drawRoundRect(x - 5, y - 7, x + 25, y + 23, 20, 20, RGB(123, 199, 230));
			printText(x, y, str, 25, "隶书", RGB(123, 199, 230), YELLOW);
			drawRoundRect(x + 95, y - 7, x + 210, y + 25, 20, 20, RGB(123, 199, 230));
			printText(x + 100, y, temp, 25, "隶书", RGB(123, 199, 230), YELLOW);
			y += 30;
		}
	}
}
void drawrightside() {
	setfillcolor(RGB(237, 237, 237));
	setlinecolor(RGB(237, 237, 237));
	fillroundrect(1300, 0, 1599, 500, 20, 20);
	fillroundrect(1300, 501, 1599, 999, 20, 20);
	setfont(50, 0, "行楷");
	drawRoundRect(1300, 8, 1480, 50, 20, 20, RGB(86, 97, 130));  //完成订单
	drawRoundRect(1300, 508, 1480, 550, 20, 20, RGB(86, 97, 130));  //正在制作
	drawRoundRect(1310, 68, 1420, 100, 20, 20, RGB(86, 97, 130));  //订单号
	drawRoundRect(1440, 68, 1580, 100, 20, 20, RGB(86, 97, 130));  //完成时间
	drawRoundRect(1390, 562, 1500, 600, 20, 20, RGB(86, 97, 130));  //订单号
	printText(1310, 10, "完成订单", 40, "隶书", RGB(86, 97, 130), BLACK);
	printText(1310, 510, "正在制作", 40, "隶书", RGB(86, 97, 130), BLACK);
	printText(1320, 70, "订单号", 30, "行楷", RGB(86, 97, 130), BLACK);
	printText(1450, 70, "完成时间", 30, "行楷", RGB(86, 97, 130), BLACK);
	printText(1400, 570, "订单号", 30, "行楷", RGB(86, 97, 130), BLACK);
}
void print_the_time()
{
	timept[0] = TIME / 36000 + '0';
	timept[1] = TIME / 3600 % 10 + '0' + 7;
	timept[2] = ':';
	timept[3] = TIME / 600 % 6 + '0';
	timept[4] = TIME / 60 % 10 + '0';
	timept[5] = ':';
	timept[6] = TIME / 10 % 6 + '0';
	timept[7] = TIME % 10 + '0';
	timept[8] = '\0';
	if (timept[1] > '9')
	{
		timept[0]++;
		timept[1] -= 10;
	}
	printText(660, 25, timept, 60, "Times New Roman", RGB(102, 127, 204), WHITE);
}
void drawtheclock()
{

	drawRoundRectWithBorder(650, 0, 860, 100, 20, 20, RGB(102, 127, 204), WHITE);
	drawRoundRectWithBorder(860, 30, 950, 80, 20, 20, LIGHTBLUE, WHITE);
	if (TIME != -1)
	{
		print_the_time();
	}
	printText(864, 40, "speed up", 27, "Times New Roman", LIGHTBLUE, WHITE);    //speed up location
}
void change_TIME()
{
	if (ac == 0)
		ac = 1;
	if (ac != 100)
		Sleep(1000 / ac);
	else
	Sleep(5);
		TIME++;
}
void drawexit()
{
	drawRoundRect(600, 600, 700, 700, 20, 20, RGB(189, 134, 201));
	printText(620, 630, "Exit", 40, "Times New Roman", RGB(189, 134, 201), WHITE);

}
void checkclick()
{
	drawRoundRect(800, 800, 900, 900, 20, 20, LIGHTBLUE);
	printText(810, 830, "取消", 40, "楷体", LIGHTBLUE, WHITE);
	drawRoundRectWithBorder(940, 820, 1250, 870, 20, 20, RGB(162, 159, 237), BLUE);
	printText(950, 830, "当前状态：点餐状态", 30, "楷体", RGB(162, 159, 237), WHITE);
	printfinish();
	printunfinish();
	int x;
	int t = 0;
	while (t == 0)
	{
		peekmessage(&a);
		if (a.message == WM_LBUTTONDOWN)
		{
			if (a.x >= 25 && a.x <= 350)
			{
				for (x = 0; x < foodsize; x++)
					if (a.y >= 65 + 50 * x && a.y <= 105 + 50 * x)
					{
						clickorderfood(x);
						t = 1;
						break;
					}
			}
			else if (a.x >= 360 && a.x <= 625)
			{
				for (x = 0; x < setsize; x++)
					if (a.y >= 65 + 50 * x && a.y <= 105 + 50 * x)
					{
						clickorderset(x);
						t = 1;
						break;
					}
			}
			else if (a.x > 600 && a.x < 700 && a.y > 600 && a.y < 700)
			{
				printorder();
				exit(0);
			}
		}
		else if (a.message == WM_RBUTTONDOWN)
		{
			if (a.x > 800 && a.x < 900 && a.y>800 && a.y < 900)
			{
				t = 1;
				break;
			}
		}
	}
	setbkcolor(BLACK);
	cleardevice();
	drawRoundRect(800, 800, 900, 900, 20, 20, LIGHTBLUE);
	printText(810, 830, "点餐", 40, "楷体", LIGHTBLUE, WHITE);
}
void drawProgressBar(ProgressBar* bar) {
	setfillcolor(LIGHTGRAY);
	solidrectangle(bar->x, bar->y, bar->x + BAR_WIDTH, bar->y + BAR_HEIGHT);
	setfillcolor(GREEN);
	solidrectangle(bar->x, bar->y, bar->x + bar->value * (BAR_WIDTH - SLIDER_WIDTH) / 100, bar->y + BAR_HEIGHT);
	setfillcolor(RED);
	solidrectangle(bar->x + bar->value * (BAR_WIDTH - SLIDER_WIDTH) / 100, bar->y, bar->x + bar->value * (BAR_WIDTH - SLIDER_WIDTH) / 100 + SLIDER_WIDTH, bar->y + BAR_HEIGHT);
}
void  dragbar()
{
	int x;
	ProgressBar bar = { 965, 20, 50 };
	char str[4];
	while (1)
	{
		drawProgressBar(&bar);
		drawRoundRectWithBorder(1200, 90, 1280, 140, 20, 20, YELLOW, WHITE);
		x = bar.value;
		if (x == 100)
			x = 200;
		sprintf_s(str, "%d",x);
		printText(1220, 100, str, 40, "黑体", YELLOW, BLACK);
		printText(1000, 100, "加速时间倍数:", 30, "楷体", RED, BLACK);
		if (peekmessage(&a))
		{
			if (a.message == WM_MOUSEMOVE) {
				if (a.x >= bar.x && a.x <= bar.x + BAR_WIDTH && a.y >= bar.y && a.y <= bar.y + BAR_HEIGHT) {
					bar.value = (a.x - bar.x) * 100 / (BAR_WIDTH - SLIDER_WIDTH);
					if (bar.value < 0) bar.value = 0;
					if (bar.value > 100) bar.value = 100;
				}
			}
			else if (a.message == WM_LBUTTONDOWN)
				if (a.x > 860 && a.x < 950 && a.y > 30 && a.y < 80)
				{
					drawRoundRectWithBorder(955, 8, 1290, 150, 20, 20, LIGHTBLUE, WHITE);
					break;
				}
				else if (a.x > 600 && a.x < 700 && a.y > 600 && a.y < 700)
				{
					printorder();
					exit(0);
				}
		}
	}
	ac = x;
}
void changefin(int a)
{
	int hour, minute, second;
	hour = orders[a].finishtime / 3600;
	minute = (orders[a].finishtime % 3600) / 60;
	second = orders[a].finishtime % 60;
	printf("%02d:%02d:%02d ",hour, minute, second);
}
void changestart(int a)
{
	int hour, minute, second;
	hour = orders[a].ordertime / 3600;
	minute = (orders[a].ordertime % 3600) / 60;
	second = orders[a].ordertime % 60;
	printf("%02d:%02d:%02d ", hour, minute, second);
}
void print()
{
	int a,b;
	for (a = 0; a < ordersize; a++)
	{
		printf("订单编号%d ", a);
		printf("订单所含食物");
		for (b = 0; b < orders[a].size; b++)
		{
			printf("%s\t", foods[orders[a].foodid[b]].foodname);
			printf(" ");
		}
		printf("订单下单时间");
		changestart(a);
		if (orders[a].con == 2)
		{
			printf("订单完成时间");
			changefin(a);
		}
		else if(orders[a].con==1)
		{
			printf("订单已分配未完成");
		}
		else
		{
			printf("因系统关闭订单下单失败");
		}
		printf("\n");
	}
}

int main()
{
	printf("请输入您希望完成的订单以及失败订单的显示秒数：");
	scanf("%d", &shine);
	initgraph(1600, 1000);
	loadimage(&img, "2.jpg", 1600, 1000);
	putimage(0, 0, &img);	// 在另一个位置再次显示背景
	drawinitiate();
	initializefoodset();
	drawexit();
	printText(40, 40, "欢迎来到麦当劳点餐系统", 40, "楷体", YELLOW, BLACK);
	printText(40, 100, "点击点餐按钮开始点餐", 40, "楷体", YELLOW, BLACK);
	printText(40, 160, "点击Exit按钮退出", 40, "楷体", YELLOW, BLACK);
	while (1)
	{
		peekmessage(&a);
		if (a.message == WM_LBUTTONDOWN)
		{
			if (a.x > 800 && a.x < 900 && a.y > 800 && a.y < 900)
				break;
			else if (a.x > 600 && a.x < 700 && a.y > 600 && a.y < 700)
				exit(0);
		}
	}
	setbkcolor(BLACK);
	cleardevice();
	loadimage(&img, "3.jpg", 1600, 1000);
	putimage(0, 0, &img);
	Sleep(100);
	while (TIME < 61200)
	{
		drawRoundRectWithBorder(955, 8, 1290, 150, 20, 20, LIGHTBLUE, WHITE);
		drawRoundRect(710, 690, 1260, 740, 20, 20, RED);
		drawinitiate();
		drawtheclock();
		check = 0;
		time = TIME + 25200;
		drawrightside();
		drawfoodset();
		drawexit();	
		failprint();
		while (peekmessage(&a))
		{
			if (a.message == WM_LBUTTONDOWN)
			{
				if (a.x > 860 && a.x < 950 && a.y > 30 && a.y < 80)
				{
					printfinish();
					printunfinish();
					dragbar();
				}
				else if (a.x > 800 && a.x < 900 && a.y > 800 && a.y < 900)
				{
					checkclick();
					putimage(0, 0, &img);
					drawRoundRectWithBorder(955, 8, 1290, 150, 20, 20, LIGHTBLUE, WHITE);
					drawRoundRect(710, 690, 1260, 740, 20, 20, RED);
					drawinitiate();
					drawrightside();
					drawfoodset();
					drawtheclock();
					drawexit();
				}
				else if (a.x > 600 && a.x < 700 && a.y > 600 && a.y < 700)
				{
					printorder();
					print();
					exit(0);
				}
			}
		}
		doorder();
		failprint();
		printfinish();
		printunfinish();
		for (int i = 0; i < ordersize; i++)
		{
			if (orders[i].showtime > -1&&orders[i].con!=1) //多显示一秒
				orders[i].showtime--;
		}
		change_TIME();
		drawtheclock();
	}
	printorder();
	print();
	closegraph();
	return 0;
}
