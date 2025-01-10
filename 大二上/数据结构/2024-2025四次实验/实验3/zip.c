#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
typedef struct node
{
    int weight;                 // 权值
    int parent, lchild, rchild; // 左右孩子和父节点的下标
    int flag;                   // 0表示该结点未访问过，1表示访问过
} HTNnode;
typedef struct id
{
    char content;  // 字符
    int frequency; // 频率
} info;
int Huffman_num = 0;  // Huffman编码表中的元素个数
int count = 0;        // 统计压缩前文本中字符的个数
int zipcount = 0;     // 统计压缩后文本中字符的个数
int max = INT_MIN;    // Huffman编码后最长的编码长度
int i;                // 用于循环
char c;               // 用于读取文本文件中的字符
int ascii[128] = {0}; // ascii数组，用于统计文件中字符出现的频率
FILE *inputfile;      // 输入文件指针
FILE *outputfile;     // 输出文件指针

void findnode(HTNnode HuffmanTree[], int n, int *s1, int *s2);               // 找到权值最小的两个结点
char *pad_string(const char *str, int target_length);                        // 将01串补齐到指定长度
void bad_char_heuristic(char *str, int size, int badchar[2]);                // BM算法中的坏字符规则
int BM_search(char *text, char *pattern, int check[]);                       // BM算法
void read_file();                                                            // 读取文件
void count_frequency(info Huffman_code[]);                                   // 统计文件中字符出现的频率
void build_Huffman_tree(info Huffman_code[], HTNnode HuffmanTree[]);         // 构建Huffman树
void build_Huffman_code(HTNnode HuffmanTree[], char Huffman_charcode[][60]); // 构建Huffman编码表
void print_Huffman_code(info Huffman_code[], char Huffman_charcode[][60]);   // 打印Huffman编码表
void code_file(info Huffman_code[], char Huffman_charcode[][60]);            // 编码文件
void decode_file(info Huffman_code[], char Huffman_charcode[][60]);          // 解码文件
int find_word(info Huffman_code[], char Huffman_charcode[][60]);             // 查找输入的单词

void findnode(HTNnode HuffmanTree[], int n, int *s1, int *s2)
{
    *s1 = *s2 = 1;
    for (int i = 1; i <= n; i++)
    {
        if (HuffmanTree[i].weight >= HuffmanTree[*s1].weight)
        {
            *s2 = *s1 = i;
        }
    }
    for (int i = 1; i <= n; i++)
    {
        if (HuffmanTree[i].parent == 0 && HuffmanTree[i].flag == 0)
        {
            if (HuffmanTree[i].weight < HuffmanTree[*s1].weight)
            {
                *s2 = *s1;
                *s1 = i;
            }
            else if (HuffmanTree[i].weight < HuffmanTree[*s2].weight && HuffmanTree[i].weight >= HuffmanTree[*s1].weight)
            {
                *s2 = i;
            }
        }
    }
    HuffmanTree[*s1].flag = HuffmanTree[*s2].flag = 1;
    if (*s1 > *s2)
    {
        int temp;
        temp = *s1;
        *s1 = *s2;
        *s2 = temp;
    }
}
char *pad_string(const char *str, int target_length)
{
    int length = strlen(str);
    if (length >= target_length)
    {
        char *result = strdup(str); // 使用strdup复制字符串
        return result;
    }
    char *result = (char *)malloc(target_length + 1);
    strcpy(result, str);
    for (int i = length; i < target_length; i++)
    {
        result[i] = '0';
    }
    result[target_length] = '\0'; // 确保字符串以null结尾
    return result;
}
void bad_char_heuristic(char *str, int size, int badchar[2])
{
    int i;
    for (i = 0; i < 2; i++)
    {
        badchar[i] = -1;
    }
    for (i = 0; i < size; i++)
    {
        badchar[(int)str[i] - '0'] = i;
    }
}
int BM_search(char *text, char *pattern, int check[])
{
    int m = strlen(pattern);
    int n = strlen(text);
    int badchar[2];
    bad_char_heuristic(pattern, m, badchar);
    int s = 0;
    while (s <= (n - m))
    {
        int j = m - 1;
        while (j >= 0 && pattern[j] == text[s + j])
        {
            j--;
        }
        if (j < 0 && check[s] == 0)
        {
            check[s] = 1;
            return s;
        }
        else
        {
            s += fmax(1, j - badchar[text[s + j] - '0']);
        }
    }
    return -1;
}
void read_file()
{
    char input[20];
    char output[20];
    printf("请输入要压缩的文件名：");
    scanf("%s", input);
    printf("请输入要保存的文件名：");
    scanf("%s", output);
    inputfile = fopen(input, "r");
    outputfile = fopen(output, "w+");
    while ((c = fgetc(inputfile)) != EOF)
    {
        ascii[c]++;
        count++;
    }
    for (i = 0; i < 128; i++)
    {
        if (ascii[i] != 0)
        {
            Huffman_num++;
        }
    }
}
void count_frequency(info Huffman_code[])
{
    int k;
    for (k = i = 0; i < 128; i++)
    {
        if (ascii[i] != 0)
        {
            Huffman_code[k].frequency = ascii[i];
            Huffman_code[k].content = i;
            k++;
        }
    }
}
void build_Huffman_tree(info Huffman_code[], HTNnode HuffmanTree[])
{
    for (i = 1; i <= 2 * Huffman_num - 1; i++)
    {
        if (i <= Huffman_num)
            HuffmanTree[i].weight = Huffman_code[i - 1].frequency;
        else
            HuffmanTree[i].weight = 0;
        HuffmanTree[i].parent = 0;
        HuffmanTree[i].lchild = 0;
        HuffmanTree[i].rchild = 0;
        HuffmanTree[i].flag = 0;
    }
    for (i = Huffman_num + 1; i <= 2 * Huffman_num - 1; i++)
    {
        int s1, s2;
        findnode(HuffmanTree, i - 1, &s1, &s2);
        HuffmanTree[s1].parent = HuffmanTree[s2].parent = i;
        HuffmanTree[i].lchild = s1;
        HuffmanTree[i].rchild = s2;
        HuffmanTree[i].weight = HuffmanTree[s1].weight + HuffmanTree[s2].weight;
    }
}
void build_Huffman_code(HTNnode HuffmanTree[], char Huffman_charcode[][60])
{
    char cd[Huffman_num];
    cd[Huffman_num - 1] = '\0';
    for (i = 1; i <= Huffman_num; i++)
    {
        int start = Huffman_num - 1;
        int j, temp;
        for (int j = i, temp = HuffmanTree[i].parent; temp != 0; j = temp, temp = HuffmanTree[temp].parent)
        {
            if (HuffmanTree[temp].lchild == j)
                cd[--start] = '0';
            else
                cd[--start] = '1';
        }
        strcpy(Huffman_charcode[i], cd + start);
        if (Huffman_num - start > max)
            max = Huffman_num - start;
    }
    max--;
    for (i = 1; i <= Huffman_num; i++)
    {
        strcpy(Huffman_charcode[i], pad_string(Huffman_charcode[i], max));
    }
}
void print_Huffman_code(info Huffman_code[], char Huffman_charcode[][60])
{
    for (i = 1; i <= Huffman_num; i++)
    {
        printf("%c ", Huffman_code[i - 1].content);
        printf("%s\n", Huffman_charcode[i]);
    }
}
void code_file(info Huffman_code[], char Huffman_charcode[][60])
{
    rewind(inputfile);
    while ((c = fgetc(inputfile)) != EOF)
    {
        for (i = 1; i <= Huffman_num; i++)
        {
            if (Huffman_code[i - 1].content == c)
            {
                fprintf(outputfile, "%s", Huffman_charcode[i]);
                break;
            }
        }
    }
}
void decode_file(info Huffman_code[], char Huffman_charcode[][60])
{
    rewind(outputfile);
    char target[max + 1];
    while (fread(target, sizeof(char), max, outputfile) == max)
    {
        target[max] = '\0';
        for (i = 1; i <= Huffman_num; i++)
        {
            if (strcmp(Huffman_charcode[i], target) == 0)
            {
                printf("%c", Huffman_code[i - 1].content);
                break;
            }
        }
    }
    rewind(outputfile);
    while ((c = fgetc(outputfile)) != EOF)
        zipcount++;
}
int find_word(info Huffman_code[], char Huffman_charcode[][60])
{
    int check;
    char word[count];
    char transword[max * count];
    memset(word, '\0', sizeof(word));
    memset(transword, '\0', sizeof(transword));
    printf("please input a word,input q! to exit:");
    scanf("%s", word);
    if (word[0] == 'q' && word[1] == '!')
        return 0;
    for (i = 0; word[i] != '\0'; i++)
    {
        check = 0;
        for (int k = 0; k < Huffman_num; k++)
        {
            if (word[i] == Huffman_code[k].content)
            {
                strcat(transword, Huffman_charcode[k + 1]);
                check = 1;
            }
        }
        if (check == 0)
        {
            printf("The word %s does not appear in the file.", word);
            return 1;
        }
    }
    int frequency = 0;
    char alltrans[zipcount]; // 存储编码后的所有字符
    rewind(outputfile);
    int alltrans_con[zipcount];
    memset(alltrans_con, 0, sizeof(alltrans_con));
    int index = 0;
    while ((c = fgetc(outputfile)) != EOF)
    {
        alltrans[index++] = c;
    }
    int location = BM_search(alltrans, transword, alltrans_con);
    while (location != -1)
    {
        if (location % max == 0)
        {
            printf("location:%d\n", location / max + 1);
            frequency++;
        }
        location = BM_search(alltrans, transword, alltrans_con);
    }
    if (frequency != 0)
        printf("The word %s appears %d times in the file.", word, frequency);
    else
        printf("The word %s does not appear in the file.", word);
}
int main()
{
    read_file();                                                  // 读取文件
    info Huffman_code[Huffman_num];                               // 储存文件中出现的字符及其频率
    count_frequency(Huffman_code);                                // 统计文件中字符出现的频率
    HTNnode Huffman_Tree[Huffman_num * 2];                        // Huffman树的结点数组
    build_Huffman_tree(Huffman_code, Huffman_Tree);               // 构建Huffman树
    char Huffman_charcode[Huffman_num + 1][60];                   // Huffman编码表
    build_Huffman_code(Huffman_Tree, Huffman_charcode);           // 构建Huffman编码表
    print_Huffman_code(Huffman_code, Huffman_charcode);           // 打印Huffman编码表
    code_file(Huffman_code, Huffman_charcode);                    // 编码文件
    decode_file(Huffman_code, Huffman_charcode);                  // 解码文件
    printf("\nzip rate:%lf%%\n", zipcount * 100 / (8.0 * count)); // 打印压缩率
    while (find_word(Huffman_code, Huffman_charcode) != 0)        // 查找单词
    {
        printf("\n");
    }
    fclose(inputfile);
    fclose(outputfile);
    return 0;
}