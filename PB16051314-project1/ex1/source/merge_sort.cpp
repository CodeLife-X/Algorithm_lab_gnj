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


using namespace std;

int MERGE(int A[], int p, int q, int r){
    int n1 = q - p + 1;
    int n2 = r - q;
    int L[n1 + 2], R[n2 + 1];
    for (int i = 1; i <= n1 ; ++i)
        L[i] = A[p + i -1];
    for (int j = 1; j <= n2; ++j)
        R[j] = A[q + j];
    L[n1+1] = 9999999;
    R[n2+1] = 9999999;
    int i = 1, j = 1;
    for (int k = p; k <= r; ++k) {
        if (L[i] <= R[j]){
            A[k] = L[i];
            i ++;
        } else{
            A[k] = R[j];
            j ++;
        }
    }
}


void merge_sort(int A[], int p, int r){
    int q;
    if (p < r){
        q = (p + r) / 2;
        merge_sort(A, p, q);
        merge_sort(A, q+1, r);
        MERGE(A, p, q, r);
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
        for (int i = 1; i <= count; i++) {
            fscanf(fpRead, "%d\n", &Data[i]);
        }
        rewind(fpRead);

        merge_sort(Data, 1, count);

        strcpy(output, "../ex1/output/merge_sort/result_");
        sprintf(itc,"%d",N);
        strcat(output, itc);
        strcat(output, ".txt");

        FILE *fp = NULL;
        fp = fopen(output, "wt");

        for (int i = 1; i <= count; i++) {
            fprintf(fp, "%d\n", Data[i]);
        }

        N = N - 2;
    }
    return 0;
}
