//
// Created by 007 on 2019/11/29.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#if 0
#else   // 注意：通过gcc编译时，要添加 -lm 选项。
#define LOG2(x) ((log((double)(x))) / (log(2.0)))
#endif
#define MAX_SIZE 998

int Data[1000] = {0};   //存储数据
int n = 0;              //length
int k = 10;
int thfile = 0;
clock_t startTime, endTime, ToltalTime;

typedef int Type;
typedef struct _FibonacciNode
{
    Type   key;                        // 值
    int degree;
    struct _FibonacciNode *left;       // 左兄弟
    struct _FibonacciNode *right;
    struct _FibonacciNode *child;      // 第一个孩子节点
    struct _FibonacciNode *parent;
    int marked;
}FibNode;

typedef struct _FibonacciHeap{
    int   keyNum;                    // 堆中节点的总数
    int   maxDegree;
    struct _FibonacciNode *min;
    struct _FibonacciNode **cons;    // 最大度的内存区域
}FibHeap;

static FibNode *fib_heap_search(FibHeap *heap, Type key);

static void fib_node_remove(FibNode *node)
{
    node->left->right = node->right;
    node->right->left = node->left;
}

void fib_node_add(FibNode *node, FibNode *root)
{
    node->left        = root->left;
    root->left->right = node;
    node->right       = root;
    root->left        = node;
}

void fib_node_cat(FibNode *a, FibNode *b)
{
    FibNode *tmp;

    tmp            = a->right;
    a->right       = b->right;
    b->right->left = a;
    b->right       = tmp;
    tmp->left      = b;
}

FibHeap* fib_heap_make()
{
    FibHeap* heap;

    heap = (FibHeap *) malloc(sizeof(FibHeap));
    if (heap == NULL)
    {
        printf("Error: make FibHeap failed\n");
        return NULL;
    }

    heap->keyNum = 0;
    heap->maxDegree = 0;
    heap->min = NULL;
    heap->cons = NULL;

    return heap;
}

FibNode* fib_node_make(Type key)
{
    FibNode * node;

    node = (FibNode *) malloc(sizeof(FibNode));
    if (node == NULL)
    {
        printf("Error: make Node failed\n");
        return NULL;
    }
    node->key    = key;
    node->degree = 0;
    node->left   = node;
    node->right  = node;
    node->parent = NULL;
    node->child  = NULL;

    return node;
}

void fib_heap_insert_node(FibHeap *heap, FibNode *node)
{
    if (heap->keyNum == 0)
        heap->min = node;
    else
    {
        fib_node_add(node, heap->min);
        if (node->key < heap->min->key)
            heap->min = node;
    }
    heap->keyNum++;
}

void fib_heap_insert_key(FibHeap *heap, Type key)
{
    FibNode *node;

    if (heap==NULL)
        return ;

    node = fib_node_make(key);
    if (node == NULL)
        return ;

    fib_heap_insert_node(heap, node);
}

FibNode *fib_heap_remove_min(FibHeap *heap)
{
    FibNode *min = heap->min;

    if (heap->min == min->right)
        heap->min = NULL;
    else
    {
        fib_node_remove(min);
        heap->min = min->right;
    }
    min->left = min->right = min;

    return min;
}

void fib_heap_link(FibHeap * heap, FibNode * node, FibNode *root)
{
    // 将node从双链表中移除
    fib_node_remove(node);
    // 将node设为root的孩子
    if (root->child == NULL)
        root->child = node;
    else
        fib_node_add(node, root->child);

    node->parent = root;
    root->degree++;
    node->marked = 0;
}

void fib_heap_cons_make(FibHeap * heap)
{
    int old = heap->maxDegree;

    // 计算log2(x)，"+1"意味着向上取整！
    // ex. log2(13) = 3，向上取整为3+1=4。
    heap->maxDegree = LOG2(heap->keyNum) + 1;

    // 如果原本空间不够，则再次分配内存
    if (old >= heap->maxDegree)
        return ;

    // 因为度为heap->maxDegree可能被合并，所以要maxDegree+1
    heap->cons = (FibNode **)realloc(heap->cons,
                                     sizeof(FibHeap *) * (heap->maxDegree + 1));
}

void fib_heap_consolidate(FibHeap *heap)
{
    int i, d, D;
    FibNode *x, *y, *tmp;

    fib_heap_cons_make(heap);//开辟哈希所用空间
    D = heap->maxDegree + 1;

    for (i = 0; i < D; i++)
        heap->cons[i] = NULL;

    // 合并相同度的根节点，使每个度数的树唯一
    while (heap->min != NULL)
    {
        x = fib_heap_remove_min(heap);    // 取出堆中的最小树(最小节点所在的树)
        d = x->degree;                    // 获取最小树的度数
        // heap->cons[d] != NULL，意味着有两棵树(x和y)的"度数"相同。
        while (heap->cons[d] != NULL)
        {
            y = heap->cons[d];            // y是"与x的度数相同的树"
            if (x->key > y->key)        // 保证x的键值比y小
            {
                tmp = x;
                x = y;
                y = tmp;

            }
            fib_heap_link(heap, y, x);    // 将y链接到x中
            heap->cons[d] = NULL;
            d++;
        }
        heap->cons[d] = x;
    }
    heap->min = NULL;

    // 将heap->cons中的结点重新加到根表中
    for (i=0; i<D; i++)
    {
        if (heap->cons[i] != NULL)
        {
            if (heap->min == NULL)
                heap->min = heap->cons[i];
            else
            {
                fib_node_add(heap->cons[i], heap->min);
                if ((heap->cons[i])->key < heap->min->key)
                    heap->min = heap->cons[i];
            }
        }
    }
}

FibNode* _fib_heap_extract_min(FibHeap *heap){
    if (heap==NULL || heap->min==NULL)
        return NULL;

    FibNode *child = NULL;
    FibNode *min = heap->min;
    while (min->child != NULL){
        child = min->child;
        fib_node_remove(child);
        if (child->right == child)
            min->child = NULL;
        else
            min->child = child->right;

        fib_node_add(child, heap->min);
        child->parent = NULL;
    }

    fib_node_remove(min);
    if (min->right == min)
        heap->min = NULL;
    else{
        heap->min = min->right;
        fib_heap_consolidate(heap);
    }
    heap->keyNum--;

    return min;
}

void renew_degree(FibNode *parent, int degree)
{
    parent->degree -= 1; //？？？？？
}

void fib_heap_cut(FibHeap *heap, FibNode *node, FibNode *parent){
    fib_node_remove(node);
    renew_degree(parent, node->degree);
    // node没有兄弟
    if (node == node->right)
        parent->child = NULL;
    else
        parent->child = node->right;

    node->parent = NULL;
    node->left = node->right = node;
    node->marked = 0;
    // 将"node所在树"添加到"根链表"中
    fib_node_add(node, heap->min);
}

void fib_heap_cascading_cut(FibHeap *heap, FibNode *node){
    FibNode *parent = node->parent;
    if (parent == NULL)                 //??????????????
        return ;

    if (node->marked == 0)
        node->marked = 1;
    else
    {
        fib_heap_cut(heap, node, parent);
        fib_heap_cascading_cut(heap, parent);
    }
}

void fib_heap_decrease(FibHeap *heap, FibNode *node, Type key){
    FibNode *parent;

    if (heap==NULL || heap->min==NULL ||node==NULL)
        return ;

    node->key = key;
    parent = node->parent;
    if (parent!=NULL && node->key < parent->key){
        // 将node从父节点parent中剥离出来，并将node添加到根链表中
        fib_heap_cut(heap, node, parent);
        fib_heap_cascading_cut(heap, parent);
    }

    // 更新最小节点
    if (node->key < heap->min->key)
        heap->min = node;
}

FibNode* fib_node_search(FibNode *root, Type key){
    FibNode *t = root;    // 临时节点
    FibNode *p = NULL;    // 要查找的节点

    if (root==NULL)
        return root;

    do{
        if (t->key == key){
            p = t;
            break;}
        else{
            if ((p = fib_node_search(t->child, key)) != NULL)
                break;
        }
        t = t->right;
    } while (t != root);

    return p;
}

FibNode *fib_heap_search(FibHeap *heap, Type key){
    if (heap==NULL || heap->min==NULL)
        return NULL;

    return fib_node_search(heap->min, key);
}

static void _fib_heap_delete(FibHeap *heap, FibNode *node)
{
    Type min = heap->min->key;
    fib_heap_decrease(heap, node, min-1);
    _fib_heap_extract_min(heap);
    free(node);
}

void fib_heap_delete(FibHeap *heap, Type key)
{
    FibNode *node;

    if (heap == NULL || heap -> min == NULL){
        printf("ERROR: fib_heap_delete 1");
        return ;
    }

    node = fib_heap_search(heap, key);
    if (node == NULL){
        printf("ERROR: fib_heap_delete 2_______not key %d\n", key);
        return ;
    }

    startTime = clock();
    _fib_heap_delete(heap, node);
    endTime = clock();
    ToltalTime = ToltalTime + endTime - startTime;
}

int main() {
    FibHeap *heap;
    FibNode *decrease;
    FibNode *minnode;

    int min;
    int insert_num, decrease_num, delete_num, extract_num;
    int insert_value, decrease_value, delete_value;

    char module[50][100];
    char build_file[100];
    char insert_file[100];
    char decrease_file[100];
    char delete_file[100];
    char extract_file[100];
    char result_file[100];

    FILE *fpRead = NULL;
    FILE *outputfile = NULL;
    FILE *timefile = NULL;

    strcpy (module[0], "../ex1/input/data");
    strcpy (module[1], "1");
    strcpy (module[2], "2");
    strcpy (module[3], "3");
    strcpy (module[4], "4");
    strcpy (module[5], "5");
    strcpy (module[6], "/build.txt");
    strcpy (module[7], "/insert.txt");
    strcpy (module[8], "/decrease.txt");
    strcpy (module[9], "/delete.txt");
    strcpy (module[10], "/extract.txt");


    for (thfile = 1; thfile <= 5; ++thfile) {
        timefile = fopen("../ex1/output/Fibonacci_heap/time.txt", "a+");
        if (timefile == NULL) {
            printf("No such File: %s !\n", "../ex1/output/Fibonacci_heap/result.txt");
            return 0;
        }
    printf("开始建堆\n");
//-----------------------------------------------------
//-----------------以下为建堆过程------------------------
//-----------------------------------------------------
        strcpy (build_file, module[0]);
        strcat(build_file, module[thfile]);
        strcat (build_file, module[6]);

        fpRead = fopen(build_file, "r");
        if (fpRead == NULL) {
            printf("No such File: %s !\n", build_file);
            return 0;
        }
        rewind(fpRead);

        fscanf(fpRead, "%d\n", &n);
        for (int i = 0; i < n; i++) {
            fscanf(fpRead, "%d\n", &Data[i]);
        }
//建堆
        heap = fib_heap_make();

        for (int i = 0; i < n; i++) {
            fib_heap_insert_key(heap, Data[i]);
        }


        fclose(fpRead);

        printf("开始插入\n");
//-----------------------------------------------------
//-----------------以下为插入过程------------------------
//-----------------------------------------------------
        strcpy (insert_file, module[0]);
        strcat(insert_file, module[thfile]);
        strcat (insert_file, module[7]);
        fpRead = fopen(insert_file, "r");
        if (fpRead == NULL) {
            printf("No such File: %s !\n", insert_file);
            return 0;
        }
        rewind(fpRead);
//插入
        ToltalTime = 0;
        fscanf(fpRead, "%d\n", &insert_num);
        for (int j = 0; j < insert_num; ++j) {
            fscanf(fpRead, "%d\n", &insert_value);
            startTime = clock();
            fib_heap_insert_key(heap, insert_value);
            endTime = clock();
            ToltalTime = ToltalTime + endTime - startTime;
        }
//    ToltalTime = ToltalTime / insert_num;
        fprintf(timefile, "file in data%d insert %d num total time is %lf\n",
                thfile, insert_num, (double)ToltalTime/CLOCKS_PER_SEC);

//    Heap_Insert(19);
//    printf("%d\n", Search(19));
        fclose(fpRead);

        printf("开始DECREASE_KEY\n");
//-----------------------------------------------------
//-----------------以下为DECREASE-KEY过程---------------
//-----------------------------------------------------
        strcpy (decrease_file, module[0]);
        strcat(decrease_file, module[thfile]);
        strcat (decrease_file, module[8]);
        fpRead = fopen(decrease_file, "r");
        if (fpRead == NULL) {
            printf("No such File: %s !\n", decrease_file);
            return 0;
        }
        rewind(fpRead);
//将指定元素的关键字减少10
        ToltalTime = 0;
        fscanf(fpRead, "%d\n", &decrease_num);
        for (int j = 0; j < decrease_num; ++j) {
            fscanf(fpRead, "%d\n", &decrease_value);

            decrease = fib_heap_search(heap, decrease_value);

            startTime = clock();
            fib_heap_decrease(heap, decrease, decrease->key - 10);
            endTime = clock();
            ToltalTime = ToltalTime + endTime - startTime;
        }
        fprintf(timefile, "file in data%d DECREASE-KEY %d num total time is %lf\n",
                thfile, decrease_num, (double)ToltalTime/CLOCKS_PER_SEC);

//    DECREASE_KEY(19);
//    printf("%d\n", Search(9));
//        decrease = fib_heap_search(heap, 601);
//        fib_heap_decrease(heap, decrease, decrease->key - 10);
        fclose(fpRead);

        printf("开始delete\n");
//-----------------------------------------------------
//-----------------以下为delete过程---------------------
//-----------------------------------------------------
        strcpy (delete_file, module[0]);
        strcat(delete_file, module[thfile]);
        strcat (delete_file, module[9]);
        fpRead = fopen(delete_file, "r");
        if (fpRead == NULL) {
            printf("No such File: %s !\n", delete_file);
            return 0;
        }
        rewind(fpRead);
//删除
        ToltalTime = 0;
        fscanf(fpRead, "%d\n", &delete_num);
        for (int j = 0; j < delete_num; ++j) {
            fscanf(fpRead, "%d\n", &delete_value);

//            printf("————————begin heap_delete\n");
            fib_heap_delete(heap, delete_value);
        }

//        printf("————————done heap_delete\n");
        fprintf(timefile, "file in data%d delete %d num total time is %lf\n",
                thfile, delete_num, (double)ToltalTime/CLOCKS_PER_SEC);

        fclose(fpRead);

        printf("开始EXTRACT_MIN\n");
//-----------------------------------------------------
//-----------------以下为EXTRACT-MIN过程----------------
//-----------------------------------------------------
        strcpy (extract_file, module[0]);
        strcat(extract_file, module[thfile]);
        strcat (extract_file, module[10]);

        strcpy (result_file, "../ex1/output/Fibonacci_heap/result.txt");
        fpRead = fopen(extract_file, "r");
        outputfile = fopen(result_file, "a+");
        if (fpRead == NULL || outputfile == NULL) {
            printf("No such File: %s !\n", extract_file);
            return 0;
        }
        rewind(fpRead);
//执行EXTRACT-MIN操作，返回最小值
        ToltalTime = 0;
        fscanf(fpRead, "%d\n", &extract_num);
        fprintf(outputfile, "data%d file: ", thfile);
        for (int j = 0; j < extract_num; ++j) {
            startTime = clock();
            minnode = _fib_heap_extract_min(heap);
            endTime = clock();
            ToltalTime = ToltalTime + endTime - startTime;
            fprintf(outputfile, "%d ", minnode->key);
        }
        fprintf(timefile, "file in data%d EXTRACT-MIN %d num total time is %lf\n",
                thfile, extract_num, (double)ToltalTime/CLOCKS_PER_SEC);
        fprintf(timefile, "\n");
        fprintf(outputfile, "\n");

        fclose(fpRead);
//        printf("-------------------------------\n");
    }
}

















