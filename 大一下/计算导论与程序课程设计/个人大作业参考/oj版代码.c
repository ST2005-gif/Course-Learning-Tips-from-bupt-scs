#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
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
} order;
int w1;
int w2;
int time = 0;
int ordernum = 0;
int ordercon = 2;
food* foods = NULL;
int foodsize = 0;
set* sets = NULL;
int setsize = 0;
order* orders = NULL;
int ordersize = 0;
int porder = -1;
int timechange1(int a, int b, int c)
{
	return (a * 3600 + b * 60 + c);
}
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
	if (ordernum == w1 && ordercon == 2&&a==1)
	{
		ordercon = 1;
		return 1;
	}
	else if (ordernum < w2 && ordercon == 1)
	{
		ordercon = 2;
		return 0;
	}
	else if (ordernum <w1&& ordercon==2)
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
	int a,c,d;
	char b[60];
	FILE* fptr;
	fptr = fopen("dict.dic", "r");
	fscanf(fptr, "%d %d", &foodsize,&setsize);
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
		foods[a].orderfinnum = foods[a].ordernum = foods[a].remain = foods[a].timer = 0;
	}
	fscanf(fptr, "%d %d", &w1, &w2);
	sets = (set*)malloc(setsize * sizeof(set));
	fscanf(fptr, "%s", sets[0].setname);
	a = c=0;
	while(!feof(fptr))
	{
		fscanf(fptr, "%s", b);
		d = findfood(b);
		if ( d== -1&&a<setsize&&!feof(fptr))
		{
			sets[a].size = c ;
			a++;
			strcpy(sets[a].setname, b);
			c = 0;
		}
		else if(a<setsize)
		{
			sets[a].foodid[c] = d;
			c++;
		}
	}
	sets[a].size = c-1;
	fclose(fptr);
}
void initialzeorder()
{
	int x;
	int a, b, c;
	char d[51];
	scanf("%d", &ordersize);
	orders = (order*)malloc(ordersize * sizeof(order));
	for (x = 0; x < ordersize; x++)
	{
		scanf("%d:%d:%d", &a, &b, &c);
		orders[x].ordertime = timechange1(a, b, c);
		scanf("%s", d);
		a = findfood(d);
		if (a != -1)
		{
			orders[x].foodid[0] = a;
			orders[x].size = 1;
		}
		else
		{
			a = findset(d);
			for (b = 0; b < sets[a].size; b++)
				orders[x].foodid[b] = sets[a].foodid[b];
			orders[x].size = sets[a].size;
		}
	}
	for (x = 0; x < ordersize; x++)
		orders[x].con = 0;
}
void makefood()
{
	int a;
	for (a = 0; a < foodsize; a++)
	{
		if(foods[a].remain<foods[a].foodmax)
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
		if(foods[orders[a].foodid[x]].remain==0)
		foods[orders[a].foodid[x]].ordernum++;
		orders[a].foodcon[x] = 0;
	}
	orders[a].con = 1;
	ordernum++;
}
void processorder(int a,int c)
{
	int b;
	for (b = 0; b < orders[a].size; b++)
	{
				if (foods[orders[a].foodid[b]].remain > 0 && orders[a].foodcon[b] == 0)
		{
					foods[orders[a].foodid[b]].remain--;
					orders[a].foodcon[b] = 1;
		}
				else if (c==1&&foods[orders[a].foodid[b]].orderfinnum > 0 && orders[a].foodcon[b] == 0)
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
	for (a = 0; a < ordersize-1; a++)
	{
		if (orders[a].con == 2)
		{
			hour = orders[a].finishtime / 3600;
			minute = (orders[a].finishtime % 3600) / 60;
			second = orders[a].finishtime % 60;
			printf("%02d:%02d:%02d\n", hour, minute, second);
		}
		else
			printf("Fail\n");
	}
	if (orders[a].con == 2)
	{
		hour = orders[a].finishtime / 3600;
		minute = (orders[a].finishtime % 3600) / 60;
		second = orders[a].finishtime % 60;
		printf("%02d:%02d:%02d", hour, minute, second);
	}
	else
		printf("Fail");
}
int main()
{
	int a,b;
	initializefoodset();
	initialzeorder();
	for (time = 25200; time < 86400; time++)
	{
		if (ordernum != 0)
		{
			for (a =porder; orders[a].ordertime < time&&a<ordersize; a++)
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
			if (a != -1&&b==1)
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
		if ((time > orders[ordersize-1].ordertime ||time>79200)&& ordernum == 0)
			break;
	}
		printorder();
	return 0;
}