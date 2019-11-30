//
// Created by 世界之窗 on 2019-11-14.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100000

int n = -1;
float e[30][30];
int root[30][30];
float p[30], q[30];



void OPTIMAL_BST(float *p, float *q, int n){
    float w[30][30], t;
    int j = 0;
    for (int i = 1; i <= n+1; ++i) {
        e[i][i-1] = q[i-1];
        w[i][i-1] = q[i-1];
    }
    for (int l = 1; l <= n; ++l) {
        for (int i = 1; i <= n-l+1; ++i) {
            j = i + l - 1;
            e[i][j] = MAX;
            w[i][j] = w[i][j-1] + p[j] + q[j];
            for (int r = i; r <= j; ++r) {
                t = e[i][r-1] + e[r+1][j] +w[i][j];
                if (t < e[i][j]){
                    e[i][j] = t;
                    root[i][j] = r;
                }
            }
        }
    }
}
void PRINT_TREE(int x, int y, FILE *fp){
    if (x <= y){
        int r = root[x][y];
        fprintf(fp,"k%d ", r);
        PRINT_TREE(x, r-1, fp);
        PRINT_TREE(r+1, y, fp);
    } else if (x == (y + 1)){
        fprintf(fp,"d%d ", y);
    }

}


int main(){
    FILE *fp = NULL;
    fp = fopen("../ex1/input/input.txt", "r");
    if (fp == NULL){
        printf("There is no such file");
        return 0;
    }
//    printf("###\n");

    FILE *out = NULL;
    FILE * time = NULL;
    out = fopen("../ex1/output/result.txt", "a+");
    time = fopen("../ex1/output/time.txt", "a+");
    for (int w = 1; w <= 5 ; ++w) {
        fscanf(fp, "%d", &n);
//        printf("start doing %d\n", n);
        for (int i = 1; i <= n; ++i) {
            fscanf(fp, "%f", &p[i]);
//            printf("%f  ",p[i]);
        }
//        printf("\n");
        for (int j = 0; j < n+1; ++j) {
            fscanf(fp, "%f", &q[j]);
//            printf("%f  ",q[j]);
        }
//        printf("\n");
        clock_t startTime,endTime;
        startTime = clock();

        OPTIMAL_BST(p, q, n);

        endTime = clock();


        fprintf(out,"%f  ", e[1][n]);
        PRINT_TREE(1, n, out);
        fprintf(out,"\n");
        fprintf(time, "N = %d running time:%lf s\n", n, (double)(endTime - startTime) / CLOCKS_PER_SEC);


//        printf("%f\n", e[1][n]);
    }
    return 0;
}



