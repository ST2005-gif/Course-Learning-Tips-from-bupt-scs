#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define LH 1
#define EH 0
#define RH -1
typedef struct products // 商品信息结构体
{
    int no;            // 商品编号
    char name[20];     // 商品名称
    char type[20];     // 商品类型
    int price;         // 商品价格
    char describe[60]; // 商品描述
} product;
product item[120];      // 商品数组
product nosort[120];    // 编号排序数组
product pricesort[120]; // 价格排序数组
product typesort[120];  // 类型排序数组
product namesort[120];  // 名称排序数组
int size_of_item;       // 商品数量
typedef struct BSTNode
{
    product data;           // 商品信息
    int bf;                 // 平衡因子
    struct BSTNode *left;   // 指向比较值小的商品
    struct BSTNode *middle; // 指向比较值相同的商品
    struct BSTNode *right;  // 指向比较值大的商品
} bstnode, *bstree;
bstree noavl;    // 按编号排序的AVL树
bstree nameavl;  // 按名称排序的AVL树
bstree typeavl;  // 按类型排序的AVL树
bstree priceavl; // 按价格排序的AVL树

void readfile(FILE *fp, int start, int num);               // 读取文件
int compare(product a, product b, int id);                 // 比较函数
void RotateLeft(bstree *p);                                // 左旋
void RotateRight(bstree *p);                               // 右旋
void RightBalance(bstree *T);                              // 右平衡调整
void LeftBalance(bstree *T);                               // 左平衡调整
void InsertAVL(bstree *T, product e, int *taller, int id); // 插入AVL树
void BinarySort(bstree T, product temp[], int *index);     // 二分插入排序
void free_avl_tree(bstree *T);                             // 释放AVL树
void build_avl_tree();                                     // 建立AVL树
void create_binary_sort();                                 // 建立二分插入排序
void remove_random_items(int n);                           // 随机删除n个商品
int find_item();                                           // 查找商品

void readfile(FILE *fp, int start, int num)
{
    char ch;
    for (int i = start; i < start + num; i++)
    {
        fscanf(fp, "%d", &item[i].no); // 读取商品编号
        fgetc(fp);                     // 读取空格
        if ((ch = fgetc(fp)) == '"')
            for (int j = 0; (ch = fgetc(fp)) != '"'; j++)
                item[i].name[j] = ch;
        fgetc(fp); // 读取空格
        if ((ch = fgetc(fp)) == '"')
            for (int j = 0; (ch = fgetc(fp)) != '"'; j++)
                item[i].type[j] = ch;
        fscanf(fp, "%d", &item[i].price); // 读取商品价格
        fgetc(fp);                        // 读取空格
        if ((ch = fgetc(fp)) == '"')
            for (int j = 0; (ch = fgetc(fp)) != '"'; j++)
                item[i].describe[j] = ch; // 读取商品描述
        fgetc(fp);                        // 读取换行符
    }
}
int compare(product a, product b, int id) // 比较函数，不同id对应不同的比较方式
{
    if (id == 1)
        return a.no - b.no;
    else if (id == 2)
        return strcmp(a.name, b.name);
    else if (id == 3)
        return strcmp(a.type, b.type);
    else if (id == 4)
        return a.price - b.price;
    else
        return strcmp(a.describe, b.describe);
}
void RotateLeft(bstree *p) // 左旋
{
    bstree rc = (*p)->right;
    (*p)->right = rc->left;
    rc->left = *p;
    *p = rc;
}
void RotateRight(bstree *p) // 右旋
{
    bstree lc = (*p)->left;
    (*p)->left = lc->right;
    lc->right = *p;
    *p = lc;
}
void RightBalance(bstree *T) // 右平衡调整
{
    bstree rc, ld;
    rc = (*T)->right;
    switch (rc->bf)
    {
    case RH:
        (*T)->bf = rc->bf = EH;
        RotateLeft(T);
        break;
    case LH:
        ld = rc->left;
        switch (ld->bf)
        {
        case RH:
            (*T)->bf = LH;
            rc->bf = EH;
            break;
        case EH:
            (*T)->bf = rc->bf = EH;
            break;
        case LH:
            (*T)->bf = EH;
            rc->bf = RH;
            break;
        }
        ld->bf = EH;
        RotateRight(&(*T)->right);
        RotateLeft(T);
    }
}
void LeftBalance(bstree *T) // 左平衡调整
{
    bstree lc, rd;
    lc = (*T)->left;
    switch (lc->bf)
    {
    case LH:
        (*T)->bf = lc->bf = EH;
        RotateRight(T);
        break;
    case RH:
        rd = lc->right;
        switch (rd->bf)
        {
        case LH:
            (*T)->bf = RH;
            lc->bf = EH;
            break;
        case EH:
            (*T)->bf = lc->bf = EH;
            break;
        case RH:
            (*T)->bf = EH;
            lc->bf = LH;
            break;
        }
        rd->bf = EH;
        RotateLeft(&(*T)->left);
        RotateRight(T);
    }
}
void InsertAVL(bstree *T, product e, int *taller, int id) // 插入结点，保持平衡
{
    if (*T == NULL)
    {
        *T = (bstree)malloc(sizeof(bstnode));
        (*T)->data = e;
        (*T)->left = (*T)->right = (*T)->middle = NULL;
        (*T)->bf = 0;
        *taller = 1; // 插入成功，树的高度增加
    }
    else
    {
        if (compare(e, (*T)->data, id) == 0) // 如果插入元素已存在，则不插入
        {
            InsertAVL(&(*T)->middle, e, taller, id); // 插入到中间结点的中间链表中
            *taller = 0;
            return;
        }
        else if (compare(e, (*T)->data, id) < 0)
        {
            InsertAVL(&(*T)->left, e, taller, id);
            if (*taller)
            {
                switch ((*T)->bf)
                {
                case LH:
                    LeftBalance(T);
                    *taller = 0;
                    break;
                case EH:
                    (*T)->bf = LH;
                    *taller = 1;
                    break;
                case RH:
                    (*T)->bf = EH;
                    *taller = 0;
                    break;
                }
            }
        }
        else
        {
            InsertAVL(&(*T)->right, e, taller, id);
            if (*taller)
            {
                switch ((*T)->bf)
                {
                case LH:
                    (*T)->bf = EH;
                    *taller = 0;
                    break;
                case EH:
                    (*T)->bf = RH;
                    *taller = 1;
                    break;
                case RH:
                    RightBalance(T);
                    *taller = 0;
                    break;
                }
            }
        }
    }
    return;
}
void BinarySort(bstree T, product temp[], int *index) // 二分插入排序
{
    if (T != NULL)
    {
        BinarySort(T->left, temp, index);
        bstree p = T;
        while (p != NULL)
        {
            temp[*index] = p->data;
            (*index)++;
            p = p->middle;
        }
        BinarySort(T->right, temp, index);
    }
}
void free_avl_tree(bstree *T) // 释放AVL树
{
    if (*T != NULL)
    {
        free_avl_tree(&(*T)->left);
        free_avl_tree(&(*T)->right);
        free(*T);
        *T = NULL;
    }
}
void build_avl_tree() // 建立AVL树
{
    free_avl_tree(&noavl);
    free_avl_tree(&priceavl);
    free_avl_tree(&nameavl);
    free_avl_tree(&typeavl);
    int no_taller, name_taller, type_taller, price_taller;
    no_taller = name_taller = type_taller = price_taller = 0;
    for (int i = 0; i < size_of_item; i++)
    {
        InsertAVL(&noavl, item[i], &no_taller, 1);
        InsertAVL(&nameavl, item[i], &name_taller, 2);
        InsertAVL(&typeavl, item[i], &type_taller, 3);
        InsertAVL(&priceavl, item[i], &price_taller, 4);
    }
}
void create_binary_sort()
{
    int index0, index1, index2, index3;
    index0 = index1 = index2 = index3 = 0;
    BinarySort(noavl, nosort, &index0);       // 按编号排序
    BinarySort(priceavl, pricesort, &index1); // 按价格排序
    BinarySort(nameavl, namesort, &index2);   // 按名称排序
    BinarySort(typeavl, typesort, &index3);   // 按类型排序
}
void remove_random_items(int n) // 随机删除n个商品
{
    int indices[size_of_item];
    for (int i = 0; i < size_of_item; i++)
    {
        indices[i] = i;
    }
    for (int i = size_of_item - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }
    for (int i = 0; i < n; i++)
    {
        int key = indices[i]; // 随机生成要删除的商品编号
        for (int j = key; j < size_of_item - 1; j++)
        {
            item[j] = item[j + 1];
        }
    }
    size_of_item -= n;
}
int find_item()
{
    int choice, mid;
    printf("请输入要查找的内容,0.退出 1.名称 2.编号 3.类型\n");
    scanf("%d", &choice);
    if (choice == 0)
        return 0;
    else if (choice == 1)
    {
        printf("请输入要查找的名称\n");
        char name[20];
        scanf("%s", name);
        int low = 0;
        int high = size_of_item - 1;
        while (low <= high)
        {
            mid = (low + high) / 2;
            if (strcmp(name, namesort[mid].name) == 0)
            {
                printf("商品编号:%d\n", namesort[mid].no);
                printf("商品名称:%s\n", namesort[mid].name);
                printf("商品类型:%s\n", namesort[mid].type);
                printf("商品价格:%d", namesort[mid].price);
                return 1;
            }
            else if (strcmp(name, namesort[mid].name) < 0)
                high = mid - 1;
            else
                low = mid + 1;
        }
    }
    else if (choice == 2)
    {
        printf("请输入要查找的编号\n");
        int no;
        scanf("%d", &no);
        int low = 0;
        int high = size_of_item - 1;
        while (low <= high)
        {
            mid = (low + high) / 2;
            if (no == nosort[mid].no)
            {
                printf("商品编号:%d\n", nosort[mid].no);
                printf("商品名称:%s\n", nosort[mid].name);
                printf("商品类型:%s\n", nosort[mid].type);
                printf("商品价格:%d", nosort[mid].price);
                return 1;
            }
            else if (no < nosort[mid].no)
                high = mid - 1;
            else
                low = mid + 1;
        }
    }
    else if (choice == 3)
    {
        bstree T = NULL;
        printf("请输入要查找的类型\n");
        char type[20];
        memset(type, 0, sizeof(type));
        scanf("%s", type);
        int count = 0;
        int token = 0;
        for (int i = 0; i < size_of_item; i++)
        {
            if (strcmp(type, typesort[i].type) == 0)
            {
                InsertAVL(&T, typesort[i], &token, 4); // 把类型相同的商品按价格排序
                count++;
            }
        }
        if (count > 0)
        {
            printf("共有%d个该类型商品\n", count);
            product temp[count];
            int index = 0;
            BinarySort(T, temp, &index);
            for (int i = 0; i < count && i < 10; i++)
            {
                printf("商品编号:%d 商品名称:%s 商品价格:%d 商品描述:%s\n", temp[i].no, temp[i].name, temp[i].price, temp[i].describe);
            }
            return 1;
        }
    }
    printf("没有找到该商品");
    return 1;
}
int main()
{
    srand(time(NULL));
    size_of_item = 100;
    FILE *fp = fopen("product infomation.txt", "r");
    readfile(fp, 0, size_of_item);
    build_avl_tree();
    create_binary_sort();
    readfile(fp, size_of_item, 20);
    size_of_item = size_of_item + 20;
    build_avl_tree();
    create_binary_sort();
    remove_random_items(20);
    build_avl_tree();
    create_binary_sort();
    while (find_item() == 1)
    {
        printf("\n");
    }
    fclose(fp);
    return 0;
}