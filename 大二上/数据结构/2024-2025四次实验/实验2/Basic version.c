#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Stack// 定义一个栈结构体
{
    int *base; // 栈底指针
    int *top;  // 栈顶指针
} stack;
void init_stack(stack *s, int n);// 初始化栈
void push(stack *s, int x);// 入栈
void pop(stack *s, int *x);// 出栈
void init_stack(stack *s, int n)
{
    s->base = (int *)malloc(sizeof(int) * n); // 分配栈空间
    s->top = s->base;                         // 栈顶指针指向栈底
}
// 入栈
void push(stack *s, int x)
{
    *(s->top) = x; // 将元素x放入栈顶
    s->top++;      // 栈顶指针后移
}
// 出栈
void pop(stack *s, int *x)
{
    s->top--; // 栈顶指针前移
    *x = *(s->top); // 将栈顶元素赋值给x
}
int main()
{
    FILE *file = fopen("maze.txt", "r"); // 打开文件
    int begin, endn, n, k, obegin;
    fscanf(file, "%d %d %d", &n, &begin, &endn); // 读取迷宫的节点、起点和终点数
    int end[endn];
    for (int i = 0; i < endn; i++)
    {
        fscanf(file, "%d", &end[i]); // 读取终点
        end[i]--;                    // 将终点减1，方便后续操作
    }
    begin--; // 将起点减1，方便后续操作
    obegin = begin; // 保存起点
    int arr[n][n], arrx[n][n]; // 定义迷宫数组
    int x[n]; // 定义标记数组
    memset(arrx, 0, sizeof(arrx)); // 将标记数组初始化为0
    while (1)
    {
        int x, y;
        fscanf(file, "%d %d", &x, &y); // 读取节点的通道
        if (x == 0 && y == 0) // 如果读到0 0，表示节点信息结束
            break;
        arrx[x - 1][y - 1] = arrx[y - 1][x - 1] = 1; // 将节点关系标记为1
    }
    for (int i = 0; i < endn; i++)
    {
        begin = obegin; // 将起点重置为初始起点
        memset(x, 0, sizeof(x)); // 将标记数组重置为0
        memcpy(arr, arrx, sizeof(arrx)); // 将标记数组复制到迷宫数组中
        stack s; // 定义栈
        init_stack(&s, n); // 初始化栈
        push(&s, begin); // 将起点入栈
        do
        {
            for (k = 0; k < n; k++) // 遍历起点所在的行
                if (x[k] == 0) // 如果该位置未被访问过
                    if (arr[begin][k] == 1) // 如果该位置有通道
                        break; // 找到下一个可以走的点
            if (k != n) // 如果找到了可以走的点
            {
                arr[begin][k] = arr[k][begin] = 0; // 将该点标记为已访问
                push(&s, k); // 将该点入栈
                x[begin] = 1; // 将起点标记为已访问
                if (k == end[i]) // 如果该点是终点
                    break; // 结束循环
                begin = k; // 将起点更新为该点
            }
            else // 如果没有找到可以走的点
            {
                x[begin] = 0; // 将起点标记为未访问
                pop(&s, &begin); // 将栈顶元素出栈，作为新的起点
                if (s.top == s.base) // 如果栈为空
                    break; // 结束循环
                begin = *(s.top - 1); // 将栈顶元素作为新的起点
            }
        } while (1);
        if (s.top == s.base) // 如果栈为空
            printf("No path from %d to %d\n", obegin + 1, end[i] + 1); // 输出没有路径
        else
        {
            printf("Path from %d to %d is : ", obegin + 1, end[i] + 1); // 输出路径
            for (k = 0; k < s.top - s.base - 1; k++) // 遍历栈中的元素
                printf("%d->", *(s.base + k) + 1); // 输出路径
            printf("%d length is : %d\n", *(s.base + k) + 1, s.top - s.base-1); // 输出路径长度
        }
        free(s.base);
    }
    fclose(file); // 关闭文件
    return 0;
}