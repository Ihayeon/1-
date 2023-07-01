#define _CRT_SECURE_NO_WARNINGS    // fopen 보안 경고로 인한 컴파일 에러 방지
#include <stdio.h>      // fopen, fread, fclose 함수
#include <stdlib.h>  //abs 함수
#include <string.h> //strcat, strcmp 함수
#define MAX 100
int main(){
FILE *fp = fopen("text.dat", "w");
fputs("I love c \n", fp);
fclose(fp);
}