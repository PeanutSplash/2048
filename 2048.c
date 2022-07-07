/**********************************
2048小游戏v1.2版本 
增加了2048AI 游戏胜利界面 
bug:第三次运行程序时最佳分数会归0 
作者：PeanutSplash
日期：2022/7/7
**********************************/
#include "acllib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define random(x) (rand()%x) //随机产生[0-x)的整数 
#define LENGTH 100
#define false 0
#define SIZE 4
/*全局变量*/
struct images{//图片结构体 
	ACL_Image block_0;//数字0即空方块 
	ACL_Image block_2;//数字2 
	ACL_Image block_4;//数字4 
	ACL_Image block_8;//数字8 
	ACL_Image block_16;//数字16 
	ACL_Image block_32;//数字32 
	ACL_Image block_64;//数字64 
	ACL_Image block_128;//数字128 
	ACL_Image block_256;//数字256 
	ACL_Image block_512;//数字512 
	ACL_Image block_1024;//数字1024 
	ACL_Image block_2048;//数字2048 
	ACL_Image background;//背景色 
	ACL_Image over;//gameover
	ACL_Image backgound_color;//大背景色 
	ACL_Image restart;//重试 
	ACL_Image restart_hover;//重试(after)
	ACL_Image score;//分数
	ACL_Image score_background;//用于填充分数的背景 
	ACL_Image bestscore;//最高得分 
	ACL_Image win;//胜利 
	ACL_Image newgame;//新游戏
	ACL_Image newgame_hover;//新游戏（after） 
}IMAGES;
int Array[4][4]={0};//代表2048的二维数组 
int score=0;//得分 
int maxScore=-1;//最高得分  
int ArrayStatus=0;//0表示初始化界面 1为gameover界面 2为胜利界面 
int depthMAX; //最深长度 
int node;//节点 
int bestChoose;//最佳路径 
/*函数实现*/
/*功能方面函数*/
void LoadAllImages(){//加载所有图片 
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
void paintNumber(int x,int y,int TextSize,ACL_Color TextColor,ACL_Color TextBkColor, char num[]){//画数字 	参数：x坐标 y坐标 字体大小 字体颜色 要输出的文字 
    beginPaint();
    setTextSize(TextSize);//设置字体大小 
    setTextColor(TextColor);//设置字体颜色 
    setTextBkColor(TextBkColor);//设置背景 
    paintText(x,y,num);//输出字体
    endPaint();
}
void paintFrame(){	//初始化界面  
	int i,j;
	/*初始化数据*/
	initConsole ();//启用终端
	for(i=0;i<SIZE;i++){
		for(j=0;j<SIZE;j++){
			Array[i][j]=0;
		}
	}
	score=0;
	ArrayStatus=0;
	getMaxScore();//读取最大得分
	/*画背景*/ 
	beginPaint();
		putImage(&IMAGES.backgound_color,0,0);
    	putImage(&IMAGES.background,25,130);//上半部分150px为计分板，左右25px为两边间隙 
    	putImage(&IMAGES.score,315,18);//分数方块 
    	putImage(&IMAGES.bestscore,400,18);//最高分数方块 
    endPaint();
    paintNumber(35,5,86,RGB(119,110,101),RGB(251,248,241),"2048");
    paintNumber(30,95,24,RGB(119,110,101),RGB(251,248,241),"合并数字到达2048！");
    paintNumber(347,52,24,RGB(255,255,254),RGB(188,172,159),"0");
    paintMaxScoreNum();
	paintBlock();
    paintFirstBlock();
}
void paintFirstBlock(){//开局随机生成两个数字方块 
	int i,j;
	int randomx[4]; 
	while(1){
		for(i=0;i<4;i++){
			randomx[i]=random(4);//生成二维数组中的随机坐标 
		}
		if(randomx[0]==randomx[2]&&randomx[1]==randomx[3]) continue;//两个生成的数字方块重合了 
		else break;	
	} 
	Array[randomx[0]][randomx[1]]=2;//有一个方块为2 
	Array[randomx[2]][randomx[3]]=blockGeneration();
    paintBlock();
}
void keyboardEvent(int key,int event){//绑定键盘事件  参数：按键 键盘事件 
	int iseffect; //是否为有效操作
    if(event!=KEY_DOWN)
        return;
    if(isOver(Array)){  //判断是否游戏结束，若结束则输出结束信息并退出循环
    	printf("已满!\n");
		return;
	}else if(isWin(Array)){
		printf("胜利!\n");
		return;
	}
    switch(key)
    {
    case 0x57://W
    case VK_UP:
			iseffect = upMove(Array);//向上移动 
        break;
    case 0x53://S
    case VK_DOWN:
			iseffect = downMove(Array);//向下移动 
        break;
    case 0x41://A
    case VK_LEFT:
    		iseffect = leftMove(Array);//向左移动 
        break;
    case 0x44://D
    case VK_RIGHT:
   	 		iseffect = rightMove(Array);//向右移动 
        break;
    case 0x42 ://AI模式 
		startTimer(0,10);//开始计时器运行 
    }
    if(iseffect) nextStep(); //若为有效操作，执行下一步函数
}
void timerEvent(int tid){//计时器事件 
	if(StratAI(Array)==1||printArrayNumInConsole()==1){
		cancelTimer(0);//当数组满时或出现2048时停止运行 
	}
	nextStep();//下一步操作 
}
int printArrayNumInConsole(){//输出二维数组的数据在终端 
	int i,j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			//printf("%d\t",Array[i][j]);
			if(Array[i][j]==2048) {//当出现2048返回1 
				return 1;
			}
		}
		//printf("\n");
	}
		//printf("\n");
}
void mouseEvent(int x,int y,int bt,int event){//鼠标事件 参数：x坐标 y坐标 按键 事件 
	if(ArrayStatus==1||ArrayStatus==2){
		if(event==BUTTON_DOWN){
			if(bt==LEFT_BUTTON){
       	 		paintOutNum(x,y);//鼠标点击事件 
      	 	}
 	   }
    	paintDownBlock(x,y);//鼠标移动到某区域事件 
	}
}
int Setup(){
    initWindow("2048",DEFAULT,DEFAULT,550,650);
    LoadAllImages();
    paintFrame();
    registerKeyboardEvent(keyboardEvent);//键盘事件  
    registerTimerEvent(timerEvent);//计时器 
    return 0;
}
void paintOutNum(int x,int y){// 点击事件	x坐标y坐标 
	if(ArrayStatus==1){
		if(x> 223&&x<314&&y>430&&y<480){//点击重试按钮的区域 
			printf("重试!\n");
			paintFrame();//初始化 
		}
	}
	else if(ArrayStatus==2){
		if(x> 223&&x<314&&y>480&&y<530){//点击重试按钮的区域 
			printf("新游戏!\n");
			paintFrame();//初始化 
		}
	}
}
void paintDownBlock(int x,int y){//鼠标移动到某区域的事件	参数：x坐标y坐标 
		beginPaint();
		if(ArrayStatus==1){
			if(x>223&&x<314&&y>430&&y<480){//鼠标移动到按钮的区域 
				putImage(&IMAGES.restart_hover,225,430);//重试按钮(After)
			}
			else{
				putImage(&IMAGES.restart,225,430);
			}
		} 
		else if(ArrayStatus==2){
			if(x>223&&x<314&&y>480&&y<530){//鼠标移动到按钮的区域 
				putImage(&IMAGES.newgame_hover,225,480);//新游戏按钮(After)
				printf("已绘画新游戏按钮！(After)\n");
				printf("ArrayStatus=%d\n",ArrayStatus);
			}
			else{
				putImage(&IMAGES.newgame,225,480);//新游戏按钮
				printf("已绘画新游戏按钮！\n");
				printf("ArrayStatus=%d\n",ArrayStatus);
			}
		}
			endPaint();
}
void outPutNumBlock(int i,int j){//用于根据数字输出对应的数字方块	参数：行和列 
	if(Array[i][j]==0){
    	putImage(&IMAGES.block_0,45+j*(LENGTH+20),150+i*(LENGTH+20));//每个数字方块之间间隔20，数字方块大小为100*100 
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
void getMaxScore(){//从文件读取最佳分数 
	char StrLine[20];  //每行最大读取的字符数
    FILE * fp = fopen("score.txt", "r");
    fgets(StrLine,sizeof(StrLine),fp);  //读取一行
    maxScore=atoi(StrLine);
    return ;
}
void saveMaxScore(){//保存最大分数到文件 
	char scoreStr[20];
	char maxScoreStr[20];
	itoa(score, scoreStr, 10);//maxScore转化成字符型存入str 10进制 
	FILE *fp1;
	fp1 = fopen("score.txt","w+"); 
	if(fp1!=NULL){    //文件已经创建  则删除一行
		int currentScore=atoi(scoreStr);//当前得分 
		if(maxScore<currentScore){//当文件中的数字比当前得分小时才写入 
			maxScore=currentScore;
			itoa(maxScore, maxScoreStr, 10);//maxScore转化成字符型存入str 10进制 
			fputs(maxScoreStr,fp1);
			paintMaxScoreNum();
		}	
	}
	fclose(fp1);
}
void paintBlock(){//打印数字方块 
	int i,j;
	beginPaint();
	for(i=0;i<SIZE;i++){
    	for(j=0;j<SIZE;j++){
    		outPutNumBlock(i,j);
    	}
    }
	endPaint();
}
void deleteScreen(){//清除屏幕 
	int i,j;
	beginPaint();
	for(i=0;i<SIZE;i++){
    	for(j=0;j<SIZE;j++){
    		putImage(&IMAGES.block_0,45+i*(LENGTH+20),150+j*(LENGTH+20));
    	}
    }
	endPaint();
}
/*算法方面函数*/
int blockGeneration(){//控制生成2和4的概率 
	int x=random(10);//随机产生[0-10)的整数
	if(x<2) return 4;
	else if(x>=2&&x<10) return 2;
}
int getNumDigits(int num){//判断一个整数有多少位 参数：要判断的整形数字 
	int digits=0;
	while(num!=0){
        num/=10;
        digits++;
    }
    return digits;
}
void paintScoreNum(){//输出得分数字 
	char str[20];//用于存放转化成字符型的score 
	itoa(score, str, 10);//score转化成字符型存入str 10进制 
   	switch (getNumDigits(score)){//不同位数画的起始位置不一样 
   		case 1:paintNumber(347,52,24,RGB(255,255,254),RGB(188,172,159),str);break;
		case 2:paintNumber(342,52,24,RGB(255,255,254),RGB(188,172,159),str);break;
		case 3:paintNumber(335,52,24,RGB(255,255,254),RGB(188,172,159),str);break;
		case 4:paintNumber(329,52,24,RGB(255,255,254),RGB(188,172,159),str);break;
		case 5:paintNumber(322,52,24,RGB(255,255,254),RGB(188,172,159),str);break;
   	}
}
void paintMaxScoreNum(){//输出最大得分数字 
	char str1[20];//用于存放转化成字符型的maxScore 
	itoa(maxScore, str1, 10);//score转化成字符型存入str1 10进制 
	switch (getNumDigits(maxScore)){//画最高得分的 
   		case 1:paintNumber(460,52,24,RGB(255,255,254),RGB(188,172,159),str1);break;
		case 2:paintNumber(455,52,24,RGB(255,255,254),RGB(188,172,159),str1);break;
		case 3:paintNumber(448,52,24,RGB(255,255,254),RGB(188,172,159),str1);break;
		case 4:paintNumber(442,52,24,RGB(255,255,254),RGB(188,172,159),str1);break;
		case 5:paintNumber(435,52,24,RGB(255,255,254),RGB(188,172,159),str1);break;
   	}
} 
void controlInEachUnit(int *unit){//对数组进行加和推格	
	int i,j;
	//加和
	int point=0; //指定格
	for( i=1;i<SIZE;i++){
		if(unit[i]==0) continue; //如果有空格子，比较下一格与指定格
		else if(unit[i]==unit[point]){ //否则，只与前一格比较
			unit[point]*=2;
			unit[i]=0;
			//出现正确移动，加分
			score+=unit[point];
			saveMaxScore();
			paintScoreNum();
		}
		point = i;
	}
	//推格
	for( i=SIZE-1;i>=0;i--){
		if(unit[i]==0){
			for( j=i+1;j<SIZE;j++){
				unit[j-1]=unit[j];
			}
			unit[SIZE-1]=0;
		}
	}
}
void nextStep(){//移动后的下一步操作 
	int i,j;
	int add_able[SIZE][2];  //可以加的格子
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
		deleteScreen(); //清空屏幕 
    	paintBlock();//绘画屏幕 
	}
}
int upMove(int Array_up[4][4]){//向上移动	参数：要操作的数组  
	int i,j;
	int unit[SIZE];
	int copy_Array[SIZE][SIZE]; //复制一份操作前的2048网格作为备用以便后续判断是否为有效操作
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
		//将操作后的单元重新写回2048网格
		for( j=0;j<SIZE;j++){
			Array_up[j][i]=unit[j];
		}
	}
	for( i=0;i<SIZE;i++){
		for( j=0;j<SIZE;j++){
			if(copy_Array[i][j]!=Array_up[i][j]) return 1;//操作有效时返回1 
		}
	}
	return 0;//无效返回0 
}
int downMove(int Array_down[4][4]){//向下移动	参数：要操作的数组  
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
			if(ori_table[i][j]!=Array_down[i][j]) return 1;//操作有效时返回1 
		}
	}
	return 0;//无效返回0 
}

int leftMove(int Array_left[4][4]){//向左移动 	参数：要操作的数组 
	int i,j;
	int unit[SIZE];
	int ori_table[SIZE][SIZE]; 
	for( i=0;i<SIZE;i++){
		for( j=0;j<SIZE;j++){
			ori_table[i][j]=Array_left[i][j];                                                                                                                            //name:LondonWu  （防伪用）
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
			if(ori_table[i][j]!=Array_left[i][j]) return 1;//操作有效时返回1 
		}
	}
	return 0;//无效返回0 
}
int rightMove(int Array_right[4][4]){//向右移动	参数：要操作的数组 
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
			if(ori_table[i][j]!=Array_right[i][j]) return 1;//操作有效时返回1 
		}
	}
	return 0;//无效返回0 
}
//void paintWin(){//胜利界面 
//	beginPaint();
//	putImage(&IMAGES.win,25,130);
//	putImage(&IMAGES.restart,225,430);//重试按钮 
//	endPaint();
//	ArrayStatus=2;//第二种情况 
//}
int isOver(int Array_judgment[4][4]){//判满  参数：要传入的数组 
	int i,j;
	for( i=0;i<SIZE;i++){ //判断有无空格子
		for( j=0;j<SIZE;j++){
			if(Array_judgment[i][j]==0) return 0;
		}
	}
	for( i=0;i<SIZE;i++){ //判断横向有无连续相同数字格子
		for( j=1;j<SIZE;j++){
			if(Array_judgment[i][j]==Array_judgment[i][j-1]) return 0;
		}
	}
	for( i=0;i<SIZE;i++){ //判断纵向有无连续相同格子
		for( j=1;j<SIZE;j++){
			if(Array_judgment[j][i]==Array_judgment[j-1][i]) return 0;
		}
	}
	ArrayStatus=1;//表示当前数组已满 
	Sleep(150);
	beginPaint();
	putImage(&IMAGES.over,25,130);
	putImage(&IMAGES.restart,225,430);//重试按钮 
	endPaint();
	registerMouseEvent(mouseEvent);//鼠标事件
	return 1; //以上条件都不满足，游戏结束
}
int isWin(int Array_judgment[4][4]){//判满  参数：要传入的数组 
	int i,j;
	for( i=0;i<SIZE;i++){ //判断有无空格子
		for( j=0;j<SIZE;j++){
			if(Array_judgment[i][j]==2048){
				ArrayStatus=2;//表示当前已胜利 
				system("pause");
				Sleep(150);
				beginPaint();
				putImage(&IMAGES.win,25,130);
				putImage(&IMAGES.newgame,225,480);
				endPaint();
				registerMouseEvent(mouseEvent);//鼠标事件
				return 1; //游戏胜利 
			}
		}
	}
	return 0;
}
int operateGame(int Array_direction[4][4],int i){//移动函数，通过i来决定往哪个方向移动0123分别对应上下左右   参数：要操作的数组 、方向 
	int flag=-1;
	switch(i){
		case 0:flag=upMove(Array_direction);break;
		case 1:flag=downMove(Array_direction);break;
		case 2:flag=leftMove(Array_direction);break;
		case 3:flag=rightMove(Array_direction);break;
	}
	return flag;
}
int AI(int checker[4][4],int deep){//AI函数  传入要操作的数组以及步深 
    ++node;  //经历的节点数
    int eChecker[4][4];
    if (deep >= depthMAX)
        return 1;//统计当前局面得分
    int i,j;
    int free;
    int tempScore;
    int nowScroe=0;
    int bestScroe = -1;
    for (i = 0; i < 4; ++i){ 	//尝试四个方向
        memcpy(eChecker,checker, 16*4);     //拷贝一份原始数据进行评估
        if (operateGame(Array,i)!=0){     //该方向可以移动
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
                if (deep == 0)        //修改最优方向
                    bestChoose=i;
            }
        }
    }
    return bestScroe;//最优得分 
}
int StratAI(int checker[4][4]){//开始AI操作 传入参数为需要操作的数组 
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
    if(isOver(checker)==1)  //检测是否结束
        return 1;//如果满了返回1 
    return 0;//没满返回0 
}
