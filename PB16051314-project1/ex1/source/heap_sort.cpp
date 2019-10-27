//
// Created by 世界之窗 on 2019-10-24.
//

#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>
#include <string.h>
#include <cstring>

using namespace std;

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

void MAX_HEAPIFTY(int A[], int i, int length){
    int l = LEFT(i);
    int r = RIGHT(i);
    int largest;
    int temp;
    int heap_size = length - 1;
    if (l <= heap_size && A[l] > A[i])
        largest = l;
    else
        largest = i;
    if (r <= heap_size && A[r] > A[largest])
        largest = r;
    if (largest != i){
        temp = A[i];
        A[i] = A[largest];
        A[largest] = temp;
        MAX_HEAPIFTY(A, largest, length);
    }
}

void BUILD_MAX_HEAP(int A[], int length){
    int heap_size = length;
    for (int i = ((length)/2)-1; i >= 0; i--) {
        MAX_HEAPIFTY(A, i, heap_size);
    }
}

void HEAPSORT(int A[], int length){
    BUILD_MAX_HEAP(A, length);
    int heap_size = length;
    int temp;
    for (int i = length-1; i >= 1; i-- ) {
        temp = A[0];
        A[0] = A[i];
        A[i] = temp;
        heap_size --;
        MAX_HEAPIFTY(A, 0, heap_size);
    }
}


int main(){
    int Data[32799] = {0}; //32768
    int N = 15;
    char output[] = "../ex1/output/1heap_sort/result_";
    char itc[10];


    FILE *fpRead = fopen("../ex1/input/input_strings.txt","r");
    if(fpRead == NULL)
    {
        printf("No File!\n");
        return 0;
    }
    rewind(fpRead);
    while (N >= 3) {
        int count = int(pow(2, N));
//        printf("%d\n", count);
        for (int i = 0; i < count; i++) {
            fscanf(fpRead, "%d\n", &Data[i]);
        }
        rewind(fpRead);

        HEAPSORT(Data, count);

//有一个小问题：为什么这里路经变了会影响数据的值...
        strcpy(output, "../ex1/output/1heap_sort/result_");
        sprintf(itc,"%d",N);
        strcat(output, itc);
        strcat(output, ".txt");

        FILE *fp = NULL;
        fp = fopen(output, "wt");

        for (int i = 0; i < count; i++) {
            fprintf(fp, "%d\n", Data[i]);
        }

        N = N - 2;
    }
    return 0;
}