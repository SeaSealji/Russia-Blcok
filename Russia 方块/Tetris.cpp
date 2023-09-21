#include "Tetris.h"
#include<time.h>
#include<conio.h>
#include<stdlib.h>
#include"Block.h"
#include<fstream>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")//导入声音文件
//const int SPEED_NORMAL = 500;
#define MAX_LEVEL 5
#define RECORDER_FILE "recorder.txt"
#include<iostream>
using namespace std;
const int SPEED_NORMAL[MAX_LEVEL] = { 500,300,150,100,80 };
const int SPEED_QUICK = 50;
Tetris::Tetris(int rows, int cols, int left, int top, int blockSize)
	:rows(rows),cols(cols),leftMargin(left),topMargin(top),bloackSize(blockSize)
{
	for (int i = 0; i < rows; i++) {
		vector<int>maprow;
		for (int j = 0; j < cols; j++) {
			maprow.push_back(0);
		}
		map.push_back(maprow);
	}
}

void Tetris::init()
{
	mciSendString("play res/bg.mp3 repeat", 0, 0, 0);
	delay = SPEED_NORMAL[0];
	//配置随机种子
	srand(time(NULL));
	//创建游戏窗口
	initgraph(938,896);
	//加载背景图片
	loadimage(&imgBg, "res/bg2.png");
	loadimage(&imgWin,"res/win.png");
	loadimage(&imgOver,"res/over.png");

	//初始化游戏数据
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
			map[i][j] = 0;
	}
	score = 0;
	lineCount = 0;
	level = 1;

	//初始化最高分
	ifstream file(RECORDER_FILE);
	if (!file.is_open()) {
		cout << RECORDER_FILE << "打开失败" << endl;
		highestScore = 0;
	}
	else {
		file >> highestScore;
	}
	file.close();
	gameOver = false;
	
}

void Tetris::play()
{
	init();
	nextBlock = new Block;
	curBlock = nextBlock;
	nextBlock = new Block;
	int timer = 0;
	while (1) {
		//接受用户输入
		KeyEven();
		timer += getDelay();
		if (timer > delay) {
			timer = 0;
			drop();
			//渲染游戏画面
			update = true;
		}
		if (update) {
			update = false;
			//更新游戏画面 
			updateWindow();
			//更新游戏数据 
			clearLine();
		}
		if (gameOver) {
			//保存分数
			saveScore();
			//更新游戏结束界面
			displayOver();
			system("pause");
			init();
		}
	}
}

void Tetris::KeyEven()
{
	unsigned char ch;
	bool rotateFlag = false;
	int dx = 0;
	if (_kbhit())//如果有按键输入
	{
		ch=_getch();
		//按下方向键会先返回两个字符 第一个224 第二个 下:80,上:72,左:75,右:77
		if (ch == 224)
		{
			ch = _getch();
			switch (ch) {
			case 72:
				rotateFlag = true;
				break;
			case 80:
				delay = SPEED_QUICK;
				break;
			case 75:
				dx = -1;
				break;
			case 77:
				dx = 1;
				break;
			default:
				break;
			}
		}
	}
	if (rotateFlag) {
		// 做旋转
		rotate();
		update = true;
	}
	if (dx != 0) {
		//实现左右移动
		moveLeftRight(dx);
		update = true;
	}
}

void Tetris::updateWindow()
{
	putimage(0, 0, &imgBg); //绘制背景图片
	IMAGE** imgs = Block::getImages();
	BeginBatchDraw();
	for(int i=0;i<rows;i++)
		for (int j = 0; j < cols; j++) {
			if (map[i][j] == 0)continue;
			int x = j * bloackSize + leftMargin;
			int y = i * bloackSize + topMargin;
			putimage(x, y,imgs[map[i][j]-1]);
		}
	curBlock->draw(leftMargin, topMargin);
	nextBlock->draw(689, 150);

	drawScore();//绘制分数

	EndBatchDraw();
}

int Tetris::getDelay()
{
	static unsigned long long lastTime = 0;
	unsigned long long currentTime=GetTickCount();
	if (lastTime == 0) {
		lastTime = currentTime;
		return 0;
	}
	int ret = currentTime - lastTime;
	lastTime = currentTime;
	return ret;
}

void Tetris::drop()
{
	bakBlock = *curBlock;
	curBlock->drop();
	if (!curBlock->blockInMap(map)) {
		//固化方块
		bakBlock.solidify(map);
		delete curBlock;
		curBlock = nextBlock;
		nextBlock = new Block;
		//检查游戏是否结束
		checkOver();
		delay = SPEED_NORMAL[level - 1];
	}
	
}

void Tetris::clearLine()
{
	int lines = 0;
	int k = rows - 1;//存储数据的行数
	for (int i = rows - 1; i >= 0; i--) {
		//检查第i行是否为满
		int count = 0;
		for (int j = 0; j < cols; j++) {
			if (map[i][j]) {
				count++;
			}
			map[k][j] = map[i][j];//一边扫描一边存储;
		}
		if (count < cols) {
			k--;
		}
		else {//count==cols
			lines++;

		}
		
	}
	if (lines > 0) {
		//计算得分
		//to do
		int addScore[4] = { 10,30,60,80 };
		score += addScore[lines - 1];
		mciSendString("play res/xiaochu1.mp3", 0, 0, 0);
		update = true;

		//每100分为一关
		level=(score+99) / 100;
		if (level > MAX_LEVEL) {
			gameOver = true;
		}
		lineCount += lines;

	}
}

void Tetris::moveLeftRight(int offset)
{
	bakBlock = *curBlock;
	curBlock->moveLeftRight(offset);
	if (!curBlock->blockInMap(map)) {
		*curBlock = bakBlock;
	}
}

void Tetris::rotate()
{
	if (curBlock->getBlockType() == 7)return;

	bakBlock = *curBlock;
	curBlock->rotate();

	if (!curBlock->blockInMap(map)) {
		*curBlock = bakBlock;
	}

}

void Tetris::drawScore()
{
	char scoreText[32];
	sprintf_s(scoreText, sizeof(scoreText), "%d", score);
	setcolor(RGB(180, 180, 180));
	LOGFONT f;
	gettextstyle(&f);//获取当前字体
	f.lfHeight = 60;
	f.lfWidth = 30;
	f.lfQuality = ANTIALIASED_QUALITY;//设置字体为"抗锯齿"
	strcpy_s(f.lfFaceName,sizeof(f.lfFaceName),_T("Segoe UI BLACK"));
	settextstyle(&f);
	setbkmode(TRANSPARENT);//字体效果设置为透明
	outtextxy(670,727,scoreText);

	sprintf_s(scoreText, sizeof(scoreText), "%d", lineCount);//当前消除行
	settextstyle(&f);
	int xPos = 224 - f.lfWidth * strlen(scoreText);
	outtextxy(xPos, 817, scoreText);

	sprintf_s(scoreText, sizeof(scoreText), "%d", level);//当前关
	outtextxy(194, 727, scoreText);
	sprintf_s(scoreText, sizeof(scoreText), "%d", highestScore);//当前关
	outtextxy(670, 817, scoreText);
}

void Tetris::checkOver()
{
	gameOver=(curBlock->blockInMap(map) == false);
}

void Tetris::saveScore()
{
	if (score > highestScore) {
		highestScore = score;
		ofstream file(RECORDER_FILE);
		file << highestScore;
		file.close();

	}
}

void Tetris::displayOver()
{
	mciSendString("stop res/bg.mp3", 0, 0, 0);
	if (level <= MAX_LEVEL)
	{
		putimage(262, 361, &imgOver);
		mciSendString("play res/over.mp3", 0, 0, 0);
	}
	else {
		putimage(262, 361, &imgWin);
		mciSendString("play res/win.mp3", 0, 0, 0);
	}
}
