#define _CRT_SECURE_NO_WARNINGS

#include <malloc.h>
#include <stdio.h>

typedef struct BucketNode
{
    int                data;
    struct BucketNode* next;
} BN;

// 输出线性表
void displayL(BN* L)
{
    BN* p = L; // p 指向首结点
    while (p != NULL)
    {                           // 不为空，依次遍历
        printf("%d ", p->data); // 打印
        p = p->next;            // p 移向下一个节点
    }
    printf("\n");
}

// 输出数组
void displayA(int* nums, int numsSize)
{
    for (int i = 0; i < numsSize; i++)
    {
        printf("%d ", nums[i]);
    }
    printf("\n");
}

/***************************************************************************
 * @date    2020/12/03
 * @brief   合并链表
 * @param   head    头指针
 * @param   list    顺序数据链表
 ***************************************************************************/
BN* Merge(BN* head, BN* list)
{
    BN* last   = head;
    last->next = list->next;
    while (last->next)
    {
        last = last->next;
    }
    return last;
}

/***************************************************************************
 * @date    2020/12/03
 * @brief   顺序插入节点
 * @param   list    代表第几个桶的链表
 * @param   value   数据
 ***************************************************************************/
void insert(BN* list, int value)
{
    BN* prev = list;
    BN* curr = list->next;
    BN* node = (BN*)malloc(sizeof(BN));

    node->data = value;
    node->next = NULL;
    if (curr == NULL)
    {
        prev->next = node;
    }
    else
    {
        while (curr != NULL && curr->data < value)
        {
            prev = curr;
            curr = curr->next;
        }
        prev->next = node;
        node->next = curr;
    }
}

/***************************************************************************
 * @date    2020/12/03
 * @brief   桶排序主程序
 * @param   array   数组
 * @param   size    数组大小
 * @param   num     几个桶
 ***************************************************************************/
void BucketSort(int array[], int size, int num)
{
    // 申请内存，二级指针，初始化，可以理解头指针没数据，从下一个开始存数数据
    BN** buckets = (BN**)malloc(sizeof(BN*) * num);
    for (int i = 0; i < num; i++)
    {
        *(buckets + i)         = (BN*)malloc(sizeof(BN));
        (*(buckets + i))->next = NULL;
    }

    // 1. 找到最大值和最小值求间隔（桶的大小）
    int max = array[0];
    int min = array[0];
    for (int i = 0; i < size; i++)
    {
        if (array[i] > max)
        {
            max = array[i];
        }
        if (array[i] < min)
        {
            min = array[i];
        }
    }
    int space = ((max - min) / num) + 1;

    // 2. 一个一个分桶排序
    for (int i = 0; i < size; i++)
    {
        int n = (array[i] - min) / space;
        insert(*(buckets + n), array[i]);
    }
    for (int i = 0; i < num; i++)
    {
        printf("第 %d 个桶数据: ", i);
        displayL((*(buckets + i))->next);
    }

    // // 3. 合并链表
    // BN* head   = (BN*)malloc(sizeof(BN));
    // head->next = NULL;
    // BN* last   = Merge(head, *(buckets + 0));
    // for (int i = 1; i < num; i++) {
    //     if ((*(buckets + i))->next) {
    //         last = Merge(last, *(buckets + i));
    //     }
    // }
    // head = head->next;

    // // 4. 把链表值返回数组
    // for (int i = 0; i < size; i++) {
    //     array[i] = head->data;
    //     head     = head->next;
    // }

    // 3+4. 当然也可以不合并链表，直接把数据返回数组
    int index = 0;
    for (int i = 0; i < num; i++)
    {
        if ((*(buckets + i))->next != NULL)
        {
            BN* temp = (*(buckets + i))->next;
            while (temp != NULL)
            {
                array[index++] = temp->data;
                temp           = temp->next;
            }
        }
    }
}

int main()
{
#if defined(WIN32) || defined(_WIN32)
    system("chcp 65001");
#endif
    int array[] = {49, 38, 65, 97, 76, 13, 27, 49, 10};
    int size      = sizeof(array) / sizeof(int);
    int BUCKETNUM = 5;

    // 打印原始数据
    printf("%d \n", size);
    displayA(array, size);
    BucketSort(array, size, BUCKETNUM);
    displayA(array, size);

    return 0;
}