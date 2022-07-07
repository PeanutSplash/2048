/**********************************
2048С��Ϸv1.2�汾 
������2048AI ��Ϸʤ������ 
bug:���������г���ʱ��ѷ������0 
���ߣ�������
���ڣ�2022/7/7
**********************************/
#include "acllib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define random(x) (rand()%x) //�������[0-x)������ 
#define LENGTH 100
#define false 0
#define SIZE 4
/*ȫ�ֱ���*/
struct images{//ͼƬ�ṹ�� 
	ACL_Image block_0;//����0���շ��� 
	ACL_Image block_2;//����2 
	ACL_Image block_4;//����4 
	ACL_Image block_8;//����8 
	ACL_Image block_16;//����16 
	ACL_Image block_32;//����32 
	ACL_Image block_64;//����64 
	ACL_Image block_128;//����128 
	ACL_Image block_256;//����256 
	ACL_Image block_512;//����512 
	ACL_Image block_1024;//����1024 
	ACL_Image block_2048;//����2048 
	ACL_Image background;//����ɫ 
	ACL_Image over;//gameover
	ACL_Image backgound_color;//�󱳾�ɫ 
	ACL_Image restart;//���� 
	ACL_Image restart_hover;//����(after)
	ACL_Image score;//����
	ACL_Image score_background;//�����������ı��� 
	ACL_Image bestscore;//��ߵ÷� 
	ACL_Image win;//ʤ�� 
	ACL_Image newgame;//����Ϸ
	ACL_Image newgame_hover;//����Ϸ��after�� 
}IMAGES;
int Array[4][4]={0};//����2048�Ķ�ά���� 
int score=0;//�÷� 
int maxScore=-1;//��ߵ÷�  
int ArrayStatus=0;//0��ʾ��ʼ������ 1Ϊgameover���� 2Ϊʤ������ 
int depthMAX; //����� 
int node;//�ڵ� 
int bestChoose;//���·�� 
/*����ʵ��*/
/*���ܷ��溯��*/
void LoadAllImages(){//��������ͼƬ 
	loadImage("image/block_0.jpg" , &IMAGES.block_0 );
	loadImage("image/block_2.jpg" , &IMAGES.block_2 );
	loadImage("image/block_4.jpg" , &IMAGES.block_4 );
	loadImage("image/block_8.jpg" , &IMAGES.block_8 );
	loadImage("image/block_16.jpg" , &IMAGES.block_16 );
	loadImage("image/block_32.jpg" , &IMAGES.block_32);
	loadImage("image/block_64.jpg" , &IMAGES.block_64);
	loadImage("image/block_128.jpg" , &IMAGES.block_128 );
	loadImage("image/block_256.jpg" , &IMAGES.block_256 );
	loadImage("image/block_512.jpg" , &IMAGES.block_512 );
	loadImage("image/block_1024.jpg" , &IMAGES.block_1024 );
	loadImage("image/block_2048.jpg" , &IMAGES.block_2048 );
	loadImage("image/background.jpg" , &IMAGES.background );
	loadImage("image/backgound_color.jpg" , &IMAGES.backgound_color );
	loadImage("image/over.jpg" , &IMAGES.over );
	loadImage("image/score.jpg" , &IMAGES.score );
	loadImage("image/restart.jpg" , &IMAGES.restart );
	loadImage("image/restart_hover.jpg" , &IMAGES.restart_hover );
	loadImage("image/score_background.jpg" , &IMAGES.score_background );
	loadImage("image/bestscore.jpg" , &IMAGES.bestscore );
	loadImage("image/win.jpg" , &IMAGES.win );
	loadImage("image/newgame.jpg" , &IMAGES.newgame );
	loadImage("image/newgame_hover.jpg" , &IMAGES.newgame_hover );
}
void paintNumber(int x,int y,int TextSize,ACL_Color TextColor,ACL_Color TextBkColor, char num[]){//������ 	������x���� y���� �����С ������ɫ Ҫ��������� 
    beginPaint();
    setTextSize(TextSize);//���������С 
    setTextColor(TextColor);//����������ɫ 
    setTextBkColor(TextBkColor);//���ñ��� 
    paintText(x,y,num);//�������
    endPaint();
}
void paintFrame(){	//��ʼ������  
	int i,j;
	/*��ʼ������*/
	initConsole ();//�����ն�
	for(i=0;i<SIZE;i++){
		for(j=0;j<SIZE;j++){
			Array[i][j]=0;
		}
	}
	score=0;
	ArrayStatus=0;
	getMaxScore();//��ȡ���÷�
	/*������*/ 
	beginPaint();
		putImage(&IMAGES.backgound_color,0,0);
    	putImage(&IMAGES.background,25,130);//�ϰ벿��150pxΪ�Ʒְ壬����25pxΪ���߼�϶ 
    	putImage(&IMAGES.score,315,18);//�������� 
    	putImage(&IMAGES.bestscore,400,18);//��߷������� 
    endPaint();
    paintNumber(35,5,86,RGB(119,110,101),RGB(251,248,241),"2048");
    paintNumber(30,95,24,RGB(119,110,101),RGB(251,248,241),"�ϲ����ֵ���2048��");
    paintNumber(347,52,24,RGB(255,255,254),RGB(188,172,159),"0");
    paintMaxScoreNum();
	paintBlock();
    paintFirstBlock();
}
void paintFirstBlock(){//������������������ַ��� 
	int i,j;
	int randomx[4]; 
	while(1){
		for(i=0;i<4;i++){
			randomx[i]=random(4);//���ɶ�ά�����е�������� 
		}
		if(randomx[0]==randomx[2]&&randomx[1]==randomx[3]) continue;//�������ɵ����ַ����غ��� 
		else break;	
	} 
	Array[randomx[0]][randomx[1]]=2;//��һ������Ϊ2 
	Array[randomx[2]][randomx[3]]=blockGeneration();
    paintBlock();
}
void keyboardEvent(int key,int event){//�󶨼����¼�  ���������� �����¼� 
	int iseffect; //�Ƿ�Ϊ��Ч����
    if(event!=KEY_DOWN)
        return;
    if(isOver(Array)){  //�ж��Ƿ���Ϸ�����������������������Ϣ���˳�ѭ��
    	printf("����!\n");
		return;
	}else if(isWin(Array)){
		printf("ʤ��!\n");
		return;
	}
    switch(key)
    {
    case 0x57://W
    case VK_UP:
			iseffect = upMove(Array);//�����ƶ� 
        break;
    case 0x53://S
    case VK_DOWN:
			iseffect = downMove(Array);//�����ƶ� 
        break;
    case 0x41://A
    case VK_LEFT:
    		iseffect = leftMove(Array);//�����ƶ� 
        break;
    case 0x44://D
    case VK_RIGHT:
   	 		iseffect = rightMove(Array);//�����ƶ� 
        break;
    case 0x42 ://AIģʽ 
		startTimer(0,10);//��ʼ��ʱ������ 
    }
    if(iseffect) nextStep(); //��Ϊ��Ч������ִ����һ������
}
void timerEvent(int tid){//��ʱ���¼� 
	if(StratAI(Array)==1||printArrayNumInConsole()==1){
		cancelTimer(0);//��������ʱ�����2048ʱֹͣ���� 
	}
	nextStep();//��һ������ 
}
int printArrayNumInConsole(){//�����ά������������ն� 
	int i,j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			//printf("%d\t",Array[i][j]);
			if(Array[i][j]==2048) {//������2048����1 
				return 1;
			}
		}
		//printf("\n");
	}
		//printf("\n");
}
void mouseEvent(int x,int y,int bt,int event){//����¼� ������x���� y���� ���� �¼� 
	if(ArrayStatus==1||ArrayStatus==2){
		if(event==BUTTON_DOWN){
			if(bt==LEFT_BUTTON){
       	 		paintOutNum(x,y);//������¼� 
      	 	}
 	   }
    	paintDownBlock(x,y);//����ƶ���ĳ�����¼� 
	}
}
int Setup(){
    initWindow("2048",DEFAULT,DEFAULT,550,650);
    LoadAllImages();
    paintFrame();
    registerKeyboardEvent(keyboardEvent);//�����¼�  
    registerTimerEvent(timerEvent);//��ʱ�� 
    return 0;
}
void paintOutNum(int x,int y){// ����¼�	x����y���� 
	if(ArrayStatus==1){
		if(x> 223&&x<314&&y>430&&y<480){//������԰�ť������ 
			printf("����!\n");
			paintFrame();//��ʼ�� 
		}
	}
	else if(ArrayStatus==2){
		if(x> 223&&x<314&&y>480&&y<530){//������԰�ť������ 
			printf("����Ϸ!\n");
			paintFrame();//��ʼ�� 
		}
	}
}
void paintDownBlock(int x,int y){//����ƶ���ĳ������¼�	������x����y���� 
		beginPaint();
		if(ArrayStatus==1){
			if(x>223&&x<314&&y>430&&y<480){//����ƶ�����ť������ 
				putImage(&IMAGES.restart_hover,225,430);//���԰�ť(After)
			}
			else{
				putImage(&IMAGES.restart,225,430);
			}
		} 
		else if(ArrayStatus==2){
			if(x>223&&x<314&&y>480&&y<530){//����ƶ�����ť������ 
				putImage(&IMAGES.newgame_hover,225,480);//����Ϸ��ť(After)
				printf("�ѻ滭����Ϸ��ť��(After)\n");
				printf("ArrayStatus=%d\n",ArrayStatus);
			}
			else{
				putImage(&IMAGES.newgame,225,480);//����Ϸ��ť
				printf("�ѻ滭����Ϸ��ť��\n");
				printf("ArrayStatus=%d\n",ArrayStatus);
			}
		}
			endPaint();
}
void outPutNumBlock(int i,int j){//���ڸ������������Ӧ�����ַ���	�������к��� 
	if(Array[i][j]==0){
    	putImage(&IMAGES.block_0,45+j*(LENGTH+20),150+i*(LENGTH+20));//ÿ�����ַ���֮����20�����ַ����СΪ100*100 
    }
	else if(Array[i][j]==2){
		putImage(&IMAGES.block_2,45+j*(LENGTH+20),150+i*(LENGTH+20));
	}
	else if(Array[i][j]==4){
		putImage(&IMAGES.block_4,45+j*(LENGTH+20),150+i*(LENGTH+20));
	}
	else if(Array[i][j]==8){
		putImage(&IMAGES.block_8,45+j*(LENGTH+20),150+i*(LENGTH+20));
	}
	else if(Array[i][j]==16){
		putImage(&IMAGES.block_16,45+j*(LENGTH+20),150+i*(LENGTH+20));
	}
	else if(Array[i][j]==32){
		putImage(&IMAGES.block_32,45+j*(LENGTH+20),150+i*(LENGTH+20));
	}
	else if(Array[i][j]==64){
		putImage(&IMAGES.block_64,45+j*(LENGTH+20),150+i*(LENGTH+20));
	}
	else if(Array[i][j]==128){
		putImage(&IMAGES.block_128,45+j*(LENGTH+20),150+i*(LENGTH+20));
	}
	else if(Array[i][j]==256){
		putImage(&IMAGES.block_256,45+j*(LENGTH+20),150+i*(LENGTH+20));
	}
	else if(Array[i][j]==512){
		putImage(&IMAGES.block_512,45+j*(LENGTH+20),150+i*(LENGTH+20));
	}
	else if(Array[i][j]==1024){
		putImage(&IMAGES.block_1024,45+j*(LENGTH+20),150+i*(LENGTH+20));
	}
	else if(Array[i][j]==2048){
		putImage(&IMAGES.block_2048,45+j*(LENGTH+20),150+i*(LENGTH+20));
	}
}
void getMaxScore(){//���ļ���ȡ��ѷ��� 
	char StrLine[20];  //ÿ������ȡ���ַ���
    FILE * fp = fopen("score.txt", "r");
    fgets(StrLine,sizeof(StrLine),fp);  //��ȡһ��
    maxScore=atoi(StrLine);
    return ;
}
void saveMaxScore(){//�������������ļ� 
	char scoreStr[20];
	char maxScoreStr[20];
	itoa(score, scoreStr, 10);//maxScoreת�����ַ��ʹ���str 10���� 
	FILE *fp1;
	fp1 = fopen("score.txt","w+"); 
	if(fp1!=NULL){    //�ļ��Ѿ�����  ��ɾ��һ��
		int currentScore=atoi(scoreStr);//��ǰ�÷� 
		if(maxScore<currentScore){//���ļ��е����ֱȵ�ǰ�÷�Сʱ��д�� 
			maxScore=currentScore;
			itoa(maxScore, maxScoreStr, 10);//maxScoreת�����ַ��ʹ���str 10���� 
			fputs(maxScoreStr,fp1);
			paintMaxScoreNum();
		}	
	}
	fclose(fp1);
}
void paintBlock(){//��ӡ���ַ��� 
	int i,j;
	beginPaint();
	for(i=0;i<SIZE;i++){
    	for(j=0;j<SIZE;j++){
    		outPutNumBlock(i,j);
    	}
    }
	endPaint();
}
void deleteScreen(){//�����Ļ 
	int i,j;
	beginPaint();
	for(i=0;i<SIZE;i++){
    	for(j=0;j<SIZE;j++){
    		putImage(&IMAGES.block_0,45+i*(LENGTH+20),150+j*(LENGTH+20));
    	}
    }
	endPaint();
}
/*�㷨���溯��*/
int blockGeneration(){//��������2��4�ĸ��� 
	int x=random(10);//�������[0-10)������
	if(x<2) return 4;
	else if(x>=2&&x<10) return 2;
}
int getNumDigits(int num){//�ж�һ�������ж���λ ������Ҫ�жϵ��������� 
	int digits=0;
	while(num!=0){
        num/=10;
        digits++;
    }
    return digits;
}
void paintScoreNum(){//����÷����� 
	char str[20];//���ڴ��ת�����ַ��͵�score 
	itoa(score, str, 10);//scoreת�����ַ��ʹ���str 10���� 
   	switch (getNumDigits(score)){//��ͬλ��������ʼλ�ò�һ�� 
   		case 1:paintNumber(347,52,24,RGB(255,255,254),RGB(188,172,159),str);break;
		case 2:paintNumber(342,52,24,RGB(255,255,254),RGB(188,172,159),str);break;
		case 3:paintNumber(335,52,24,RGB(255,255,254),RGB(188,172,159),str);break;
		case 4:paintNumber(329,52,24,RGB(255,255,254),RGB(188,172,159),str);break;
		case 5:paintNumber(322,52,24,RGB(255,255,254),RGB(188,172,159),str);break;
   	}
}
void paintMaxScoreNum(){//������÷����� 
	char str1[20];//���ڴ��ת�����ַ��͵�maxScore 
	itoa(maxScore, str1, 10);//scoreת�����ַ��ʹ���str1 10���� 
	switch (getNumDigits(maxScore)){//����ߵ÷ֵ� 
   		case 1:paintNumber(460,52,24,RGB(255,255,254),RGB(188,172,159),str1);break;
		case 2:paintNumber(455,52,24,RGB(255,255,254),RGB(188,172,159),str1);break;
		case 3:paintNumber(448,52,24,RGB(255,255,254),RGB(188,172,159),str1);break;
		case 4:paintNumber(442,52,24,RGB(255,255,254),RGB(188,172,159),str1);break;
		case 5:paintNumber(435,52,24,RGB(255,255,254),RGB(188,172,159),str1);break;
   	}
} 
void controlInEachUnit(int *unit){//��������мӺ��Ƹ�	
	int i,j;
	//�Ӻ�
	int point=0; //ָ����
	for( i=1;i<SIZE;i++){
		if(unit[i]==0) continue; //����пո��ӣ��Ƚ���һ����ָ����
		else if(unit[i]==unit[point]){ //����ֻ��ǰһ��Ƚ�
			unit[point]*=2;
			unit[i]=0;
			//������ȷ�ƶ����ӷ�
			score+=unit[point];
			saveMaxScore();
			paintScoreNum();
		}
		point = i;
	}
	//�Ƹ�
	for( i=SIZE-1;i>=0;i--){
		if(unit[i]==0){
			for( j=i+1;j<SIZE;j++){
				unit[j-1]=unit[j];
			}
			unit[SIZE-1]=0;
		}
	}
}
void nextStep(){//�ƶ������һ������ 
	int i,j;
	int add_able[SIZE][2];  //���Լӵĸ���
	int add_able_num=0;
	for( i=0;i<SIZE;i++){
		for( j=0;j<SIZE;j++){
			if(Array[i][j]==0){
				add_able[add_able_num][0]=i;
				add_able[add_able_num][1]=j;
				add_able_num++;
			}
		}
	}
	if(add_able_num==0) return;
	else{
		int add_num = blockGeneration();
		int add_point=random(add_able_num);
		Array[add_able[add_point][0]][add_able[add_point][1]]=add_num;
		deleteScreen(); //�����Ļ 
    	paintBlock();//�滭��Ļ 
	}
}
int upMove(int Array_up[4][4]){//�����ƶ�	������Ҫ����������  
	int i,j;
	int unit[SIZE];
	int copy_Array[SIZE][SIZE]; //����һ�ݲ���ǰ��2048������Ϊ�����Ա�����ж��Ƿ�Ϊ��Ч����
	for( i=0;i<SIZE;i++){
		for( j=0;j<SIZE;j++){
			copy_Array[i][j]=Array_up[i][j];
		}
	}
	for( i=0;i<SIZE;i++){
		for( j=0;j<SIZE;j++){
			unit[j]=Array_up[j][i];
		}
		controlInEachUnit(unit);
		//��������ĵ�Ԫ����д��2048����
		for( j=0;j<SIZE;j++){
			Array_up[j][i]=unit[j];
		}
	}
	for( i=0;i<SIZE;i++){
		for( j=0;j<SIZE;j++){
			if(copy_Array[i][j]!=Array_up[i][j]) return 1;//������Чʱ����1 
		}
	}
	return 0;//��Ч����0 
}
int downMove(int Array_down[4][4]){//�����ƶ�	������Ҫ����������  
	int i,j;
	int unit[SIZE];
	int ori_table[SIZE][SIZE]; 
	for( i=0;i<SIZE;i++){
		for( j=0;j<SIZE;j++){
			ori_table[i][j]=Array_down[i][j];
		}
	}
	for( i=0;i<SIZE;i++){
		for( j=SIZE-1;j>=0;j--){
			unit[SIZE-j-1]=Array_down[j][i];
		}
		controlInEachUnit(unit);
		for( j=SIZE-1;j>=0;j--){
			Array_down[j][i]=unit[SIZE-j-1];
		}
	}
	for( i=0;i<SIZE;i++){
		for( j=0;j<SIZE;j++){
			if(ori_table[i][j]!=Array_down[i][j]) return 1;//������Чʱ����1 
		}
	}
	return 0;//��Ч����0 
}

int leftMove(int Array_left[4][4]){//�����ƶ� 	������Ҫ���������� 
	int i,j;
	int unit[SIZE];
	int ori_table[SIZE][SIZE]; 
	for( i=0;i<SIZE;i++){
		for( j=0;j<SIZE;j++){
			ori_table[i][j]=Array_left[i][j];                                                                                                                            //name:LondonWu  ����α�ã�
		}
	}
	for( i=0;i<SIZE;i++){
		for( j=0;j<SIZE;j++){
			unit[j]=Array_left[i][j];
		}
		controlInEachUnit(unit);
		for( j=0;j<SIZE;j++){
			Array_left[i][j]=unit[j];
		}
	}
	for( i=0;i<SIZE;i++){
		for( j=0;j<SIZE;j++){
			if(ori_table[i][j]!=Array_left[i][j]) return 1;//������Чʱ����1 
		}
	}
	return 0;//��Ч����0 
}
int rightMove(int Array_right[4][4]){//�����ƶ�	������Ҫ���������� 
	int i,j;
	int unit[SIZE];
	int ori_table[SIZE][SIZE];
	for( i=0;i<SIZE;i++){
		for( j=0;j<SIZE;j++){
			ori_table[i][j]=Array_right[i][j];
		}
	}
	for( i=0;i<SIZE;i++){
		for( j=SIZE-1;j>=0;j--){
			unit[SIZE-j-1]=Array_right[i][j];
		}
		controlInEachUnit(unit);
		for( j=SIZE-1;j>=0;j--){
			Array_right[i][j]=unit[SIZE-j-1];
		}
	}
	for( i=0;i<SIZE;i++){
		for( j=0;j<SIZE;j++){
			if(ori_table[i][j]!=Array_right[i][j]) return 1;//������Чʱ����1 
		}
	}
	return 0;//��Ч����0 
}
//void paintWin(){//ʤ������ 
//	beginPaint();
//	putImage(&IMAGES.win,25,130);
//	putImage(&IMAGES.restart,225,430);//���԰�ť 
//	endPaint();
//	ArrayStatus=2;//�ڶ������ 
//}
int isOver(int Array_judgment[4][4]){//����  ������Ҫ��������� 
	int i,j;
	for( i=0;i<SIZE;i++){ //�ж����޿ո���
		for( j=0;j<SIZE;j++){
			if(Array_judgment[i][j]==0) return 0;
		}
	}
	for( i=0;i<SIZE;i++){ //�жϺ�������������ͬ���ָ���
		for( j=1;j<SIZE;j++){
			if(Array_judgment[i][j]==Array_judgment[i][j-1]) return 0;
		}
	}
	for( i=0;i<SIZE;i++){ //�ж���������������ͬ����
		for( j=1;j<SIZE;j++){
			if(Array_judgment[j][i]==Array_judgment[j-1][i]) return 0;
		}
	}
	ArrayStatus=1;//��ʾ��ǰ�������� 
	Sleep(150);
	beginPaint();
	putImage(&IMAGES.over,25,130);
	putImage(&IMAGES.restart,225,430);//���԰�ť 
	endPaint();
	registerMouseEvent(mouseEvent);//����¼�
	return 1; //���������������㣬��Ϸ����
}
int isWin(int Array_judgment[4][4]){//����  ������Ҫ��������� 
	int i,j;
	for( i=0;i<SIZE;i++){ //�ж����޿ո���
		for( j=0;j<SIZE;j++){
			if(Array_judgment[i][j]==2048){
				ArrayStatus=2;//��ʾ��ǰ��ʤ�� 
				system("pause");
				Sleep(150);
				beginPaint();
				putImage(&IMAGES.win,25,130);
				putImage(&IMAGES.newgame,225,480);
				endPaint();
				registerMouseEvent(mouseEvent);//����¼�
				return 1; //��Ϸʤ�� 
			}
		}
	}
	return 0;
}
int operateGame(int Array_direction[4][4],int i){//�ƶ�������ͨ��i���������ĸ������ƶ�0123�ֱ��Ӧ��������   ������Ҫ���������� ������ 
	int flag=-1;
	switch(i){
		case 0:flag=upMove(Array_direction);break;
		case 1:flag=downMove(Array_direction);break;
		case 2:flag=leftMove(Array_direction);break;
		case 3:flag=rightMove(Array_direction);break;
	}
	return flag;
}
int AI(int checker[4][4],int deep){//AI����  ����Ҫ�����������Լ����� 
    ++node;  //�����Ľڵ���
    int eChecker[4][4];
    if (deep >= depthMAX)
        return 1;//ͳ�Ƶ�ǰ����÷�
    int i,j;
    int free;
    int tempScore;
    int nowScroe=0;
    int bestScroe = -1;
    for (i = 0; i < 4; ++i){ 	//�����ĸ�����
        memcpy(eChecker,checker, 16*4);     //����һ��ԭʼ���ݽ�������
        if (operateGame(Array,i)!=0){     //�÷�������ƶ�
            tempScore = 0;
            free = 0;
            for (j = 0; j < 16; ++j){
                if (eChecker[0][j] == 0){           
                    ++free;
                    eChecker[0][j] = 2;
                    tempScore += AI(eChecker, deep+1) * 0.9;
                    eChecker[0][j] = 4;
                    tempScore += AI(eChecker, deep+1) * 0.1;
                    eChecker[0][j] = 0;
                }
            }
            if (free!=0) tempScore /= free;
            else tempScore = -99999999;
            if (nowScroe + tempScore > bestScroe){ 
                bestScroe = nowScroe + tempScore;
                if (deep == 0)        //�޸����ŷ���
                    bestChoose=i;
            }
        }
    }
    return bestScroe;//���ŵ÷� 
}
int StratAI(int checker[4][4]){//��ʼAI���� �������Ϊ��Ҫ���������� 
    depthMAX = 3;
    int i;
    while (1){
        node = 0;
        if(AI(checker,0)==-1) return 1;
        if (node >= 10000 || depthMAX >= 8)
            break;
        depthMAX += 1;
    }
    operateGame(checker,bestChoose) ;
    if(isOver(checker)==1)  //����Ƿ����
        return 1;//������˷���1 
    return 0;//û������0 
}
