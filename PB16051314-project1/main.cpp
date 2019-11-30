//
// Created by 世界之窗 on 2019-10-20.
//
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>
#include <string.h>
#include <cstring>
#include <sstream>

using namespace std;

int quick_sort(){
    return 0;
}

void W_Random(void){
    int a = 1 ;
    int b = 65535;
    srand((unsigned)time(0));
    int i;
    int count = 32999;
    FILE *fp = NULL;

    fp = fopen("./234", "a");
    while (count > 0) {
        i = (rand() % (b - a + 1)) + a;
        fprintf(fp, "%d\n", i);
        count -- ;
    }
}

int main(){
//    quick_sort();

    char output[100] = "../ex1/output/quick_sort/result_";


    int i = 11;
    char itc[10];
    sprintf(itc,"%d",i);
    printf("%s\n",itc);
    strcat(output, itc);
    strcat(output, ".txt");
    printf("%s",output);

}

