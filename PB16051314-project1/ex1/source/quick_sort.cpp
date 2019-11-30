//
// Created by 世界之窗 on 2019-10-21.
//
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>
#include <string.h>
#include <cstring>

#include<time.h>

using namespace std;

int PARTITION(int A[], int p, int r){
    int x = A[r];
    int i = p - 1;
    int temp;
    for (int j = p; j <= r-1; ++j) {
        if(A[j] <= x){
            i ++;
            temp = A[i];
            A[i] = A[j];
            A[j] = temp;
        }

    }
    temp = A[i+1];
    A[i+1] = A[r];
    A[r] = temp;
    return i + 1;
}


void quick_sort(int A[], int p, int r){
    int q;
    if (p < r){
        q = PARTITION(A, p, r);
        quick_sort(A, p, q-1);
        quick_sort(A, q+1, r);
    }
}

int main(){
    int Data[32799] = {0}; //32768
    int N = 15;
    char output[] = "../ex1/output/quick_sort/result_";
    char itc[10];


    FILE *fpRead = fopen("../ex1/input/input_strings.txt","r");
    if(fpRead == NULL)
    {
        printf("No File!\n");
        return 0;
    }
    while (N >= 3) {
        int count = int(pow(2, N));
//        printf("%d\n", count);
        for (int i = 0; i < count; i++) {
            fscanf(fpRead, "%d\n", &Data[i]);
        }
        rewind(fpRead);

        clock_t startTime,endTime;
        startTime = clock();
        quick_sort(Data, 0, count-1);
        endTime = clock();

//        printf(,);

        strcpy(output, "../ex1/output/quick_sort/result_");
        sprintf(itc,"%d",N);
        strcat(output, itc);
        strcat(output, ".txt");

        FILE *fp = NULL;
        fp = fopen(output, "wt");
        for (int i = 0; i < count; i++) {
            fprintf(fp, "%d\n", Data[i]);
        }

        char ttime[]="../ex1/output/quick_sort/time.txt";
        FILE *tt = NULL;
        tt = fopen(ttime, "a+");
        fprintf(tt, "file result_%d running time:%lf s\n", N, (double)(endTime - startTime) / CLOCKS_PER_SEC);

        N = N - 2;
    }
    return 0;
}