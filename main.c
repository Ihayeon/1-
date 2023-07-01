#define _CRT_SECURE_NO_WARNINGS    // fopen 보안 경고로 인한 컴파일 에러 방지
#include <stdio.h>      // fopen, fread, fclose 함수
#include <stdlib.h>  //abs 함수
#include <string.h> //strcat, strcmp 함수
#include <unistd.h> //sleep 함수
#include <time.h> //시간측정
#define MAX 100

int attribute_txt(char *txt_name); //txt파일 검사 몇자, 몇 문장인지
int game_start(char *txt_name); //타자연습 시작
    int compare(char *txt, char *input, int len); //입력한 것과 맞는지 확인
int result(int how_much_time); //결과 그리고 결과 파일출력
    int report_card(double progress, double accuracy, 
    double speed, int how_much_time);//결과 파일 출력

int total_line=0; //전체 문장수
int total_char=0; //전체 글자수 
int total_fault=0; //전체 틀린 단어 개수
int how_many_char=0; //얼마나 입력했는지 (진행상황)
int how_many_input=0; //얼마나 입력했는지 (실질적 input)

int main(){
    time_t start, end; //시간 측정을 위한 변수
    int total_time; //총 걸린시간
    printf("타자연습 프로그램에 오신걸 환영합니다.\n");
    printf("타자연습을 위해 원하시는 txt파일 이름을 입력해주세요(.txt 제외).\n");
    char name[20]; //입력받을 파일 이름
    char dot_txt[5]=".txt"; //.txt 그자체
    scanf("%[^\n]",name); //txt 파일 입력
    strcat(name,dot_txt); //붙혀주기
    sleep(1);
    printf("\n선택하신 파일 검사를 시작하겠습니다.\n");
    sleep(2);
    attribute_txt(name); //txt 파일 검사
    printf("타자연습 도중 중간에 그만두시고 싶으시다면 GG 혹은 gg를 입력해주세요.");
    printf("\n타자연습을 시작합니다.\n");
    sleep(1);
    
    for(int i=3;i>0;i--){
        printf("\n%d\n",i);
        sleep(1);
    }
    printf("시작\n");
    
    start=time(NULL);
    game_start(name);//프로그램 시작 
    end=time(NULL);
    total_time=(double)(end - start);//총시간
    result(total_time);
    return 0;
}

int attribute_txt(char *txt_name){
    FILE* fp = fopen(txt_name, "r");  //test파일을 r(읽기) 모드로 열기
    char buffer[MAX] = { 0, };
    if (fp==NULL){ printf("\n%s 파일을 찾을 수 없습니다.\n",txt_name);}
    
    char cha;
    while(!feof(fp))  // 파일의 끝이 아니라면
    {
        fgets(buffer, 100, fp);  // 최대 100칸짜리 한줄 읽기
        total_line+=1;
        total_char+=strlen(buffer);
    }

    printf("\n해당 %s파일은 ",txt_name);
    printf("총 %d문장, ",total_line);
    printf("%d글자입니다(공백포함).\n",total_char);
    //sleep(2);
    fclose(fp); //파일 포인터 닫기
    return 0;
}

int game_start(char *txt_name){
    FILE* fp = fopen(txt_name, "r");  //test파일을 r(읽기) 모드로 열기
    char input[100];
    char buffer[MAX] = { 0, };
    while(!feof(fp))  // 파일의 끝이 아니라면
    {
        fgets(buffer, 100, fp);  // 최대 100칸짜리 한줄 읽기
        buffer[strlen(buffer)-1]='\0'; //마지막 값 없애서 txt 마지막 쓰레기값 없애기
        printf("\n%s\n",buffer);
        scanf(" %[^\n]",input);  //앞쪽에 공백을 통해 \n 가 읽혀서 코드가 넘어가는걸 방지
        if(strcmp(input,"GG")==0){break;}
        if(strcmp(input,"gg")==0){break;}//만약 GG 혹은 gg를 입력했을경우 탈출
        total_fault+=compare(buffer,input,strlen(buffer));
        how_many_char+=(strlen(buffer)+1);
        how_many_input+=strlen(input);
    }

    fclose(fp);
    return 0;
}

int compare(char *txt, char *input, int len){
    int fault=0;
    for(int i=0;i<len;i++){
        if(txt[i]!=input[i]){fault++;}  
    }
    
    if(strlen(txt)<strlen(input)){fault+=strlen(input)-strlen(txt);} //제시된 문장보다 더 오버해서 입력했을경우

    return fault;
}

int result(int how_much_time){
    double progress, accuracy, speed;
    progress= ((double)how_many_char/((double)total_char))*100; 
    accuracy = (1-((double)total_fault /(double) how_many_char)) *100;
    speed = ((double)how_many_input/(double)how_much_time)*60; //분당 몇타수?
    printf("%d     %d",how_many_char,total_char);



    printf("\n진행도: %.2lf%%\n",progress);
    printf("걸린시간: %d초 \n\n",how_much_time);

    printf("실수: %d글자 \n",total_fault);
    printf("정확도: %.1lf%%\n", accuracy);
    printf("타수: %.d\n",(int)speed); //타자수/시간 

    report_card(progress,accuracy,speed,how_much_time);

}

int report_card(double progress, double accuracy, double speed,int how_much_time){
    FILE *fp = fopen("report card.txt", "w");
    fputs("--------타자연습 프로그램 타수 진단--------\n", fp);
    fprintf(fp,"\n진행도: %.2lf%%\n",progress);
    fprintf(fp,"걸린시간: %d초 \n\n",how_much_time);
    fprintf(fp,"실수: %d글자 \n",total_fault);
    fprintf(fp,"정확도: %.1lf%%\n", accuracy);
    fprintf(fp,"타수: %.d\n",(int)speed); 
    fputs("\n--------피드백--------\n", fp);
    if(progress<100){fputs("\n다음에는 끝까지 타자연습을 해봅시다!\n", fp);}
    if(speed<100 && accuracy<90){fputs("전반적인 타자연습을 꾸준히 하며 실력을 키워나갑시다!!\n", fp);}
    else if(accuracy<90){fputs("타자를 빠르게 잘 치고 있지만, 그만큼 정확도도 신경을 써주고 연습해봐요!\n", fp);}
    else if(speed<100){fputs("타자를 더 빠르게 치는 연습을 하면 좋을 것 같아요!\n", fp);}
    else{fputs("아주 훌륭합니다!! 하지만 멈추지 말고, 타수를 꾸준히 높여나가는 연습을 해봅시다!!\n", fp);}
    fclose(fp);
}