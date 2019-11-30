//
// Created by 世界之窗 on 2019-11-15.
//
#include<stdio.h>
#include<string.h>
#include <time.h>
#include <iostream>


int c[200][200];
int b[200][200];
char f[200];
int stt[2][12] =
        {
                {
                        16, 10, 16, 20, 16, 30,
                        16, 40, 16, 50, 16, 60
                },
                {
                        15, 26, 30, 26, 45, 26,
                        60, 26, 75, 26, 90, 26
                }
        };


int Max(int m,int n,int i,int j)
{
    if(m > n)
    {
        b[i][j] = -1;
        return m;
    }
    else
    {
        b[i][j] = 1;
        return n;
    }
}

void print(int i,int j,int s,char x[],char y[])
{
    if(b[i][j] == 0)
    {
        f[s-1] = x[i-1];
        i --;
        j --;
        s --;
        print(i,j,s,x,y);
    }
    else if(b[i][j] == -1)
    {
        i --;
        print(i,j,s,x,y);
    }
    else if(b[i][j] == 1)
    {
        j --;
        print(i,j,s,x,y);
    }
}
/*-----------------------分割线--------------------------------*/
int LCS(char x[],char y[])
{
    int i,j;
    int x_len,y_len;
    x_len = strlen(x);
    y_len = strlen(y);
    for(i = 1;i <= x_len;i++)
    {
        for(j = 1;j <= y_len;j++)
        {
            if(x[i-1] == y[j-1])
            {
                c[i][j] = c[i-1][j-1] +1;
                b[i][j] = 0;
            }
            else
            {
                c[i][j] = Max(c[i-1][j],c[i][j-1],i,j);
            }
        }
    }
    print(x_len,y_len,c[x_len][y_len],x,y);
    return c[x_len][y_len];
}

void WWrite(int n, char *fp){
    FILE *wwrite = NULL;
    wwrite = fopen(fp, "a+");
    int o = 0;
    if (wwrite == NULL){
        printf("There is no such file");
    }
    for (int i = 0; i < n; ++i) {
        o = rand()%26;
        o+='A';
        fprintf(wwrite, "%c", o);
    }
    fprintf(wwrite, "\n");

}

void Creat_random_list(){
    char iiput_A[50] = "../ex2/input/inputA.txt";
    FILE *fp;
    fp=fopen(iiput_A,"w");//假如打开的是1.txt文件
    fclose(fp);//这样就清空了内容，简单！
    srand(time(NULL));    //种下随机种子
    for (int i = 0; i < 12; i = i + 2) {
//        srand(time(NULL));    //种下随机种子
        WWrite(stt[0][i], iiput_A);
//        srand(time(NULL));    //种下随机种子
        WWrite(stt[0][i+1], iiput_A);
    }
    fclose(fp);

    FILE *fq;
    char iiput_B[50] = "../ex2/input/inputB.txt";

    fq=fopen(iiput_B,"w");//假如打开的是1.txt文件
    fclose(fq);//这样就清空了内容，简单！
    for (int i = 0; i < 12; i = i + 2) {
//        srand(time(NULL));    //种下随机种子
        WWrite(stt[1][i], iiput_B);
//        srand(time(NULL));    //种下随机种子
        WWrite(stt[1][i+1], iiput_B);
    }
    fclose(fq);
}

int main()
{
    char X[200] = "sadfasdfasdf";
    char Y[200] = "asdfasdfasdf";
    int s;

//    Creat_random_list();

    FILE *input = NULL;
    FILE *result = NULL;
    FILE *time = NULL;
    time = fopen("../ex2/output/time.txt","a+");
    result = fopen("../ex2/output/result.txt", "a+");

    input = fopen("../ex2/input/inputA.txt", "r");
    if (input == NULL){
        printf("There is no such file");
        return 0;
    }
    for (int k = 0; k < 6; k = k + 1) {
        fscanf(input, "%s", X);
        fscanf(input, "%s", Y);
        clock_t startTime,endTime;
        startTime = clock();

        s = LCS(X,Y);

        endTime = clock();
        fprintf(result, "%d ",s);
        fprintf(result, "%s",f);
        fprintf(result, "\n");

        fprintf(time, "running time:%lf s\n",  (double)(endTime - startTime) / CLOCKS_PER_SEC);
//        printf("%d ",s);
//        printf("%s",f);
//        printf("\n");
    }

    input = fopen("../ex2/input/inputB.txt", "r");

    for (int k = 0; k < 6; k = k + 1) {
        fscanf(input, "%s", X);
        fscanf(input, "%s", Y);

        clock_t startTime,endTime;
        startTime = clock();

        s = LCS(X,Y);

        endTime = clock();
        fprintf(result, "%d ",s);
        fprintf(result, "%s",f);
        fprintf(result, "\n");
        fprintf(time, "running time:%lf s\n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
    }
    return 0;
}



