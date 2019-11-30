//
// Created by 世界之窗 on 2019-11-26.
//
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 998

int Data[1000] = {0};   //存储数据
int n = 0;              //length
int k = 10;
int thfile = 0;
clock_t startTime, endTime, ToltalTime;

//注意：数组位置从0开始！！
int PARENT(int i){
    int x = (i + 1) / 2 - 1;
    return x ;
}

int LEFT(int i){
    return 2 * (i + 1) - 1;
}

int RIGHT(int i){
    return 2 * (i + 1);
}

void MIN_HEAPIFTY(int A[], int i, int length){
    int l = LEFT(i);
    int r = RIGHT(i);
    int smallest;
    int temp;
    int heap_size = length - 1;
    if (l <= heap_size && A[l] < A[i])
        smallest = l;
    else
        smallest = i;
    if (r <= heap_size && A[r] < A[smallest])
        smallest = r;
    if (smallest != i){
        temp = A[i];
        A[i] = A[smallest];
        A[smallest] = temp;
        MIN_HEAPIFTY(A, smallest, length);
    }
}

void BUILD_MIN_HEAP(int A[], int length){
    int heap_size = length;
    for (int i = ((length)/2)-1; i >= 0; i--) {
        MIN_HEAPIFTY(A, i, heap_size);
    }
}

void HEAPSORT(int A[], int length){
    BUILD_MIN_HEAP(A, length);
    int heap_size = length;
    int temp;
    for (int i = length-1; i >= 1; i-- ) {
        temp = A[0];
        A[0] = A[i];
        A[i] = temp;
        heap_size --;
        MIN_HEAPIFTY(A, 0, heap_size);
    }
}

void inset_fun(int start){
    int value = Data[start];    //存储插入的节点值
    int current = start;
//    int parr = 0;
    while (current > 0){
//        parr = PARENT(current);
        if (value > Data[PARENT(current)])  //如果节点大于父节点，满足最小堆的性质
            break;
        else{   //交换节点大小
            Data[current] =  Data[PARENT(current)];
            current = PARENT(current);
        }
    }
    Data[current] = value;
}
//Done!
void Heap_Insert(int num){
    if(n >= MAX_SIZE)
        printf("\n\ntoo large!!\n\n");

    Data[n] = num;
    inset_fun(n);
    n ++;
}

int Search(int num){
    int pos = -1;
    for (int i = 0; i < n; ++i)
        if (Data[i] == num)
            pos = i;

    return pos;
}

//Done
void DECREASE_KEY(int num){
    int pos = Search(num);

    startTime = clock();
    if(pos == -1)
        printf("There is a Search Error!\n");
    //此处可以设定 k 的值
    Data[pos] -= k;
    inset_fun(pos);     //维护最小堆的性质
    endTime = clock();
    ToltalTime = ToltalTime + endTime - startTime;
}

void delete_fun(int pos, int end){
    int current = pos;
    int value = Data[current];
    int change = LEFT(current);

    while (change <= end){
        //有左右孩子而且右孩子更小
        if (change < end && (Data[change+1] < Data[change]))
            change ++;
        if (value <= Data[change])
            break;
        else{    //当前节点和较小的子节点交换，而且将current变为change节点
            Data[current] = Data[change];
            current = change;
            change = LEFT(current);
        }
    }
    Data[current] = value;
}

void DELETE(int num){
    int index = Search(num);

    startTime = clock();
    if(index == -1)
        printf("delete --- no such value");

    //!!!!!!!!! 这个地方容易出错！
    Data[index] = Data[n-1]; //???
    n --;
    delete_fun(index, n-1);
    endTime = clock();
    ToltalTime = ToltalTime + endTime - startTime;
}

int EXTRACT_MIN(void){
    int min = Data[0];
    Data[0] = Data[n-1];
    n --;
    delete_fun(0, n-1);
    return min;
}

int main() {
    int min;
    int insert_num, decrease_num, delete_num, extract_num;
    int insert_value, decrease_value, delete_value, extract_value;

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
    timefile = fopen("../ex1/output/binary_heap/time.txt", "a+");
    if (timefile == NULL) {
        printf("No such File: %s !\n", "../ex1/output/binary_heap/result.txt");
        return 0;
    }
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
    BUILD_MIN_HEAP(Data, n);
//    printf("%d\n",Data[0]);
//    Heap_Insert(2);
//    printf("%d\n",Data[0]);
//    printf("%d\n\n", Search(2));

    fclose(fpRead);

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
        Heap_Insert(insert_value);
        endTime = clock();
        ToltalTime = ToltalTime + endTime - startTime;
    }
//    ToltalTime = ToltalTime / insert_num;
    fprintf(timefile, "file in data%d insert %d num total time is %lf\n",
            thfile, insert_num, (double)ToltalTime/CLOCKS_PER_SEC);

//    Heap_Insert(19);
//    printf("%d\n", Search(19));
    fclose(fpRead);

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
//        if(decrease_value == 19)
//            printf("there is 19!\n");
        DECREASE_KEY(decrease_value);
    }
    fprintf(timefile, "file in data%d DECREASE-KEY %d num total time is %lf\n",
            thfile, decrease_num, (double)ToltalTime/CLOCKS_PER_SEC);

//    DECREASE_KEY(19);
//    printf("%d\n", Search(9));
    fclose(fpRead);

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
    //时间放入内部，减去搜索的时间
    ToltalTime = 0;
    fscanf(fpRead, "%d\n", &delete_num);
    for (int j = 0; j < delete_num; ++j) {
        fscanf(fpRead, "%d\n", &delete_value);
        DELETE(delete_value);
    }
    fprintf(timefile, "file in data%d delete %d num total time is %lf\n",
            thfile, delete_num, (double)ToltalTime/CLOCKS_PER_SEC);

    fclose(fpRead);

//    HEAPSORT(Data, n);
//    for (int j = n -1; j >= n-10; --j) {
//        printf("%d\n", Data[j]);
//    }
//-----------------------------------------------------
//-----------------以下为EXTRACT-MIN过程----------------
//-----------------------------------------------------
    strcpy (extract_file, module[0]);
    strcat(extract_file, module[thfile]);
    strcat (extract_file, module[10]);

    strcpy (result_file, "../ex1/output/binary_heap/result.txt");
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
//    printf("\n%d\n\n", extract_num);
    fprintf(outputfile, "data%d file: ", thfile);
    for (int j = 0; j < extract_num; ++j) {
        startTime = clock();
        min = EXTRACT_MIN();
        endTime = clock();
        ToltalTime = ToltalTime + endTime - startTime;
//        if (min == 9)
//            printf("There is 9999!\n");
        fprintf(outputfile, "%d ", min);
    }
    fprintf(timefile, "file in data%d EXTRACT-MIN %d num total time is %lf\n",
            thfile, extract_num, (double)ToltalTime/CLOCKS_PER_SEC);

    fprintf(timefile, "\n");
    fprintf(outputfile, "\n");


    fclose(fpRead);

//    fclose(outputfile);

//    int in_9 = Search(9);
//    HEAPSORT(Data, n);
//
//    printf("%d",in_9);
//    printf("%d\n",Data[n-1]);
//    printf("%d\n",Data[1]);
//    printf("%d",Data[2]);
//    printf("%d",n);
//    for (int j = n -1; j >= n-10; --j) {
//        printf("%d\n", Data[j]);
//    }
    }
}





