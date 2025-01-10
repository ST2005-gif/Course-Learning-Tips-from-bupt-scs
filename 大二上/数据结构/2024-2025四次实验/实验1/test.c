#include <stdio.h>
#include <stdlib.h>
const int N;          // 定义总人数N
const int S;          // 定义开始报数的人的编号
const int M;          // 定义间隔的人数
const int X;          // 定义剩余人数
typedef struct Person // 定义人结构体，包含编号、姓名、性别、年龄及指向下一个人的指针
{
    int num;             // 定义一个整型变量num代表编号
    char *name;          // 定义一个字符指针变量name代表姓名
    int sex;             // 定义一个整型变量sex代表性别，0表示男性，1表示女性
    int age;             // 定义一个整型变量age代表年龄
    struct Person *next; // 定义一个指向Person结构体的指针变量next
} person;
person *create() // 创建一个循环链表，用于存储所有人的信息 返回构造完成的头节点
{
    person *head = NULL;        // 创建一个person类型的指针head，初始值为NULL
    person *tail = NULL;        // 创建一个person类型的指针tail，初始值为NULL
    for (int i = 0; i < N; i++) // 循环N次，创建N个person类型的节点
    {
        person *p = (person *)malloc(sizeof(person));                                               // 动态分配一个person类型的节点，并赋值给指针p
        printf("Please input the id, name, sex(0 for male, 1 for female) and age of the person\n"); // 提示用户输入person的id、name、sex和age
        scanf("%d %s %d %d", &p->num, &p->name, &p->sex, &p->age);                                  // 从用户输入中读取person的id、name、sex和age，并赋值给p
        if (head == NULL)                                                                           // 如果head为NULL，说明这是第一个节点，将p赋值给head和tail
            head = tail = p;                                                                        // 将p赋值给head和tail，表示链表的起始节点和尾节点
        else                                                                                        // 否则，将p赋值给tail的next，并将p赋值给tail
        {
            tail->next = p; // 将p节点插入到tail节点的后面
            tail = p;       // 更新tail节点为p节点
        }
    }
    tail->next = head; // 将尾部连接到头部，形成循环链表
    return head;       // 返回头节点
}
void print(person *p) // 打印一个人的信息，传入打印的节点
{
    printf("The message of the person is:No=%-2d Name=%-3s ", p->num, &p->name); // 打印人的编号和姓名
    if (p->sex == 0)                                                             // 如果人的性别为0，则打印性别为男，年龄
        printf("Sex=Male   Age=%-2d\n", p->age);
    else if (p->sex == 1) // 如果人的性别为1，则打印性别为女，年龄
        printf("Sex=Female Age=%-2d\n", p->age);
}
int check(person *p) // 检查链表中是否只剩下X个人，传入链表的头节点，返回1表示超过X人，返回0表示仅X人
{
    person *q = p;              // 定义指针q，指向p
    for (int i = 0; i < X; i++) // 循环X次
    {
        p = p->next; // p指向下一个节点
        if (p == q)  // 如果p指向q，说明链表遍历完成，返回0
            return 0;
    }
    return 1; // 如果循环结束，说明节点超过X，返回1
}
person *process(person *p) // 处理链表，传入链表的头节点，返回处理后的头节点
{
    person *q;                      // 定义一个指向person类型的指针q
    for (int i = 0; i < M - 2; i++) // 遍历链表，找到倒数第三个节点
    {
        printf("%d->", p->num); // 打印当前节点的值
        p = p->next;            // 将p指向下一个节点
    }
    printf("%d->%d(to be eliminated)\n", p->num, p->next->num);        // 打印倒数第三个节点和下一个节点的值
    q = p->next;                                                       // 将倒数第三个节点的下一个节点赋值给q
    printf("(iteration)The persone No %-2d is eliminated!\n", q->num); // 打印被淘汰的节点编号
    p->next = q->next;                                                 // 将倒数第三个节点的next指针指向被淘汰节点的下一个节点
    free(q);                                                           // 释放被淘汰节点的内存
    return p->next;                                                    // 返回处理后的头节点
}
person *iteration(person *p) // 迭代方式实现删除功能，传入链表的头节点，返回删除后的头节点
{
    while (check(p))    // 当check(p)返回值为真时，执行循环
        p = process(p); // 调用process函数，传入p，返回删除后的头节点
    return p;           // 返回p
}
person *recursion(person *p) // 递归方式实现删除功能，传入链表的头节点，返回删除后的头节点
{
    if (check(p))                     // 检查函数check(p)的返回值
        return recursion(process(p)); // 如果check(p)返回值为真，则调用recursion函数，传入process(p)的返回值
    else                              // 如果check(p)返回值为假，则返回p
        return p;
}
int main()
{
    int i;                       // 定义一个整型变量i
    printf("please input N:\n"); // 提示用户输入N
    scanf("%d", &N);             // 读取用户输入的N
    person *p = create();        // 创建一个person类型的指针head，指向create函数返回的地址
    for (i = 0; i < N; i++)      // 遍历链表，打印每个节点的值
    {
        print(p);    // 打印当前节点的值
        p = p->next; // 将指针指向下一个节点
    }
    printf("please input S, M and X:\n");                               // 提示用户输入S、M和X的值
    scanf("%d %d %d", &S, &M, &X);                                      // 读取用户输入的值
    while (p->num != S)                                                 // 遍历链表，找到第S个节点
        p = p->next;                                                    // 将指针p指向下一个节点
    print(p);                                                           // 打印第S个节点
    printf("please choose to solve by iteration(0) or recursion(1)\n"); // 提示用户选择迭代或递归求解
    scanf("%d", &i);                                                    // 读取用户输入
    if (i == 0)                                                         // 判断i的值，如果为0，则调用iteration函数
        p = iteration(p);                                               // 调用函数iteration，传入参数p
    else if (i == 1)                                                    // 如果i的值为1，则调用recursion函数
        p = recursion(p);                                               // 调用函数iteration，传入参数p
    printf("Elimination process finished!\n");                          // 打印结束信息
    for (i = 0; i < X; i++)                                             // 遍历链表，从第一个节点开始，依次打印每个节点的值
    {
        print(p);      // 打印当前节点的值
        person *q = p; // 定义一个指向person类型的指针q，指向p
        p = p->next;   // 将指针指向下一个节点
        free(q);       // 释放q指向的内存
    }
    return 0; // 返回0
}