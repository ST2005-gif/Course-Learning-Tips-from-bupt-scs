#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const int n; // 全局变量，表示图的顶点数
typedef struct Stack
{
    int *base;
    int *top;
} stack;
// 初始化栈
void init_stack(stack *s, int n)
{
    s->base = (int *)malloc(sizeof(int) * n);
    s->top = s->base;
}
// 入栈
void push(stack *s, int x)
{
    *(s->top) = x;
    s->top++;
}
// 打印路径
int printPath(int prev[n], int begin, int end)
{
    stack s;
    init_stack(&s, n);
    if (prev[end] == -1)
    {
        printf("No path from %d to %d\n", begin + 1, end + 1);
        return 0;
    }
    printf("Path from %d to %d: ", begin + 1, end + 1);
    int i, j;
    for (i = end, j = 0; i != begin; i = prev[i], j++)
        push(&s, i + 1);
    push(&s, begin + 1);
    for (i = s.top - s.base - 1; i > 0; i--)
        printf("%d->", s.base[i]);
    printf("%d length is : %d\n", s.base[0], j);
    return j;
}
void bfs(int arr[n][n], int begin, int end, int prev[n]) // 广度优先搜索算法
{
    int visited[n];                      // 定义一个数组，用于记录节点是否被访问过
    int queue[n], front = 0, back = 0;   // 定义一个队列，用于存储待访问的节点
    memset(visited, 0, sizeof(visited)); // 初始化visited数组为0，表示所有节点都未被访问过
    visited[begin] = 1;                  // 将起始节点标记为已访问
    queue[back++] = prev[begin] = begin; // 将起始节点加入队列，并记录其前驱节点
    while (front < back)                 // 当队列不为空时，继续搜索
    {
        int current = queue[front++]; // 取出队列中的第一个节点
        for (int i = 0; i < n; i++)   // 遍历当前节点的所有邻接节点
        {
            if (arr[current][i] && visited[i] != 1) // 如果邻接节点未被访问过，则将其标记为已访问，并记录其前驱节点
            {
                visited[i] = 1;    // 将当前节点标记为已访问
                prev[i] = current; // 将当前节点的前驱节点设为当前节点
                queue[back++] = i; // 将当前节点加入队列
                if (i == end)      // 如果找到了目标节点，则返回
                    return;
            }
        }
    }
    return; // 如果搜索完所有节点仍未找到目标节点，则返回
}
int main()
{
    FILE *file = fopen("maze.txt", "r");
    int begin, endn,nearest,nearestend,i,length;
    fscanf(file, "%d %d %d", &n, &begin, &endn);
    int end[endn], prev[n];
    for (i = 0; i < endn; i++)
    {
        fscanf(file, "%d", &end[i]);
        end[i]--; // 转换为0基索引
    }
    begin--; // 转换为0基索引
    int arr[n][n], arrx[n][n];
    memset(arr, 0, sizeof(arr));
    while (1)
    {
        int x, y;
        fscanf(file, "%d %d", &x, &y);
        if (x == 0 && y == 0)
            break;
        arr[x - 1][y - 1] = arr[y - 1][x - 1] = 1; // 因为是无向图，所以对称
    }
    for (i = 0; i < endn; i++)
    {
        memcpy(arrx, arr, sizeof(arr));
        memset(prev, -1, sizeof(prev)); // 初始化父节点数组为-1
        bfs(arrx, begin, end[i], prev);
        length=printPath(prev, begin, end[i]);
        if(i==0)
        {
            nearest=length;
            nearestend=end[i];
        }
        else if(length<nearest)
        {
            nearest=length;
            nearestend=end[i];
        }
    }
    printf("The nearest exit is : %d , its length is %d\n",nearestend+1,nearest);
    fclose(file);
    return 0;
}