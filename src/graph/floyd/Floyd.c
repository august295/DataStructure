#include <malloc.h>
#include <stdio.h>
#include <string.h>

#define MAXV    7     // 最大顶点个数
#define INF     32767 // 定义 ∞
#define MAX_LEN 64    // 临时字符串长度
// ∞ == 32767 ,int 型的最大范围（2位）= 2^(2*8-1)，TC告诉我们int占用2个字节，而VC和LGCC告诉我们int占用4个字节
// 图：LGraph
// 顶点：Vertex
// 邻接：Adjacency
// 矩阵：Matrix
// 表：List
// 边：EdLGe

typedef struct vertex
{
    int number; // 顶点的编号
} VertexType;   // 别名，顶点的类型

typedef struct matrix
{
    int        n;                  // 顶点个数
    int        e;                  // 边数
    int        adjMat[MAXV][MAXV]; // 邻接矩阵数组
    VertexType ver[MAXV];          // 存放顶点信息
} MatGraph;                        // 别名，完整的图邻接矩阵类型

typedef struct eNode
{
    int           adjVer;    // 该边的邻接点编号
    int           weiLGht;   // 该边的的信息，如权值
    struct eNode* nextEdLGe; // 指向下一条边的指针
} EdgeNode;                  // 别名，边结点的类型

typedef struct vNode
{
    EdgeNode* firstEdLGe; // 指向第一个边结点
} VNode;                  // 别名，邻接表的头结点类型

typedef struct list
{
    int   n;             // 顶点个数
    int   e;             // 边数
    VNode adjList[MAXV]; // 邻接表的头结点数组
} ListGraph;             // 别名，完整的图邻接表类型

// 创建图的邻接表
void createAdjListGraph(ListGraph* LG, int A[MAXV][MAXV], int n, int e)
{
    int       i, j;
    EdgeNode* p;
    for (i = 0; i < n; i++)
    {
        LG->adjList[i].firstEdLGe = NULL; // 给邻接表中所有头结点指针域置初值
    }
    for (i = 0; i < n; i++)
    { // 检查邻接矩阵中的每个元素
        for (j = n - 1; j >= 0; j--)
        {
            if (A[i][j] != 0)
            {                                                       // 存在一条边
                p            = (EdgeNode*)malloc(sizeof(EdgeNode)); // 申请一个结点内存
                p->adjVer    = j;                                   // 存放邻接点
                p->weiLGht   = A[i][j];                             // 存放权值
                p->nextEdLGe = NULL;

                p->nextEdLGe              = LG->adjList[i].firstEdLGe; // 头插法
                LG->adjList[i].firstEdLGe = p;
            }
        }
    }
    LG->n = n;
    LG->e = e;
}

// 输出邻接表
void displayAdjList(ListGraph LG)
{
    int       i;
    EdgeNode* p;
    for (i = 0; i < MAXV; i++)
    {
        p = LG.adjList[i].firstEdLGe;
        printf("%d:", i);
        while (p != NULL)
        {
            if (p->weiLGht != 32767)
            {
                printf("%2d[%d]->", p->adjVer, p->weiLGht);
            }
            p = p->nextEdLGe;
        }
        printf(" NULL\n");
    }
}

// 输出邻接矩阵
void displayAdjMat(MatGraph MG)
{
    int i, j;
    for (i = 0; i < MAXV; i++)
    {
        for (j = 0; j < MAXV; j++)
        {
            if (MG.adjMat[i][j] == 0)
            {
                printf("%4s", "0");
            }
            else if (MG.adjMat[i][j] == 32767)
            {
                printf("%4s", "INF");
            }
            else
            {
                printf("%4d", MG.adjMat[i][j]);
            }
        }
        printf("\n");
    }
}

// 邻接表转换为邻接矩阵
void ListToMat(ListGraph LG, MatGraph* MG)
{
    int       i, j;
    EdgeNode* p;
    for (i = 0; i < MAXV; i++)
    {
        for (j = 0; j < MAXV; j++)
        {
            MG->adjMat[i][j] = 0;
        }
    }
    for (i = 0; i < LG.n; i++)
    {
        p = LG.adjList[i].firstEdLGe;
        while (p != NULL)
        {
            MG->adjMat[i][p->adjVer] = p->weiLGht;
            p                        = p->nextEdLGe;
        }
    }
    MG->n = LG.n;
    MG->e = LG.e;
}

// 输出多源最短路径
void displayPath(MatGraph MG, int A[MAXV][MAXV], int path[MAXV][MAXV])
{
    int i, j, k;
    int s;
    int aPath[MAXV]; // 存放一条最短路径（逆向）
    int d;           // 顶点个数
    for (i = 0; i < MG.n; i++)
    {
        for (j = 0; j < MG.n; j++)
        {
            // 若顶点 i 和 顶点 j 之间存在路径
            if (A[i][j] != INF && i != j)
            {
                printf("从 %d 到 %d 的路径为：", i, j);
                k        = path[i][j];
                d        = 0;
                aPath[d] = j;
                // 路劲上添加中间点
                while (k != -1 && k != i)
                {
                    d++;
                    aPath[d] = k;
                    k        = path[i][k];
                }
                d++;
                aPath[d] = i; // 路径上添加起点
                // 输出路径上其他顶点
                char str[MAX_LEN] = {0};
                char num[MAX_LEN] = {0};
                sprintf(num, "%d", aPath[d]);
                strcat(str, num);
                for (s = d - 1; s >= 0; s--)
                {
                    strcat(str, "->");
                    sprintf(num, "%d", aPath[s]);
                    strcat(str, num);
                }
                printf("%-30s 路径长度为：%d\n", str, A[i][j]);
            }
        }
    }
}

// Floyd算法
void Floyd(MatGraph MG)
{
    int i, j, k;
    int A[MAXV][MAXV];
    int path[MAXV][MAXV];
    for (i = 0; i < MG.n; i++)
    {
        for (j = 0; j < MG.n; j++)
        {
            A[i][j] = MG.adjMat[i][j];
            if (i != j && MG.adjMat[i][j] < INF)
            {
                path[i][j] = i; // 顶点 i 到顶点 j 有边时
            }
            else
            {
                path[i][j] = -1; // 顶点 i 到顶点 j 无边时
            }
        }
    }
    // 一次考察所有顶点
    for (k = 0; k < MG.n; k++)
    {
        for (i = 0; i < MG.n; i++)
        {
            for (j = 0; j < MG.n; j++)
            {
                if (A[i][j] > A[i][k] + A[k][j])
                {
                    A[i][j]    = A[i][k] + A[k][j]; // 修改最短路径长度
                    path[i][j] = path[k][j];        // 修改最短路径
                }
            }
        }
    }
    displayPath(MG, A, path); // 输出最短路径
}

int main()
{
#if defined(WIN32) || defined(_WIN32)
    system("chcp 65001");
#endif

    int array[MAXV][MAXV] = {
        {0, 4, 6, 6, INF, INF, INF},
        {INF, 0, 1, INF, 7, INF, INF},
        {INF, INF, 0, INF, 6, 4, INF},
        {INF, INF, 2, 0, INF, 5, INF},
        {INF, INF, INF, INF, 0, INF, 6},
        {INF, INF, INF, INF, 1, 0, 8},
        {INF, INF, INF, INF, INF, INF, 0},
    };

    int       e = 12;
    ListGraph LG;
    createAdjListGraph(&LG, array, MAXV, e);
    displayAdjList(LG);
    printf("\n");

    MatGraph MG;
    ListToMat(LG, &MG);
    displayAdjMat(MG);
    printf("\n");

    Floyd(MG);
    printf("\n");

    return 0;
}
