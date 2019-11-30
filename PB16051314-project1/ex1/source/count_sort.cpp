//
// Created by 世界之窗 on 2019-10-27.
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


void count_sort(int A[], int length, int k){
    int C[k+1] ;
    int BB[length+1];
    for (int i = 1; i <= k ; ++i)
        C[i] = 0;
    for (int j = 1; j <= length; ++j)
        C[A[j]] = C[A[j]] + 1;
    for (int l = 1; l <= k ; ++l)
        C[l] = C[l] + C[l-1];
    for (int m = length; m >= 1; m--) {
        BB[C[A[m]]] = A[m];
        C[A[m]] = C[A[m]] - 1;
    }
    for (int n = 1; n <=length ; ++n)
        A[n] = BB[n];
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
        for (int i = 1; i <= count; i++) {
            fscanf(fpRead, "%d\n", &Data[i]);
        }
        rewind(fpRead);

        clock_t startTime,endTime;
        startTime = clock();
        count_sort(Data, count, 65535);
        endTime = clock();

        strcpy(output, "../ex1/output/count_sort/result_");
        sprintf(itc,"%d",N);
        strcat(output, itc);
        strcat(output, ".txt");

        FILE *fp = NULL;
        fp = fopen(output, "wt");

        for (int i = 1; i <= count; i++) {
            fprintf(fp, "%d\n", Data[i]);
        }

        char ttime[]="../ex1/output/count_sort/time.txt";
        FILE *tt = NULL;
        tt = fopen(ttime, "a+");
        fprintf(tt, "file result_%d running time:%lf s\n", N, (double)(endTime - startTime) / CLOCKS_PER_SEC);

        
        N = N - 2;
    }
    return 0;
}
