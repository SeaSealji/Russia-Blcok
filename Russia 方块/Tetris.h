#pragma once
#include<vector>
#include<graphics.h>
#include"Block.h"
using namespace std;
class Tetris
{
public:
	Tetris(int rows,int cols,int left,int top,int bolckSize);
	void init();
	void play();
private:
	void KeyEven();
	void updateWindow();
	int getDelay();//返回距离上一次调用,间隔时间 ,第一次调用返回0
	void drop();
	void clearLine();
	void moveLeftRight(int offset);//左右移动控制
	void rotate();//旋转
	void drawScore();
	int delay;
	bool update;//是否更新
	vector<vector<int>> map;
	int rows;
	int cols;
	int leftMargin;
	int topMargin;
	int bloackSize;
	int score;
	void checkOver();
	void saveScore();
	void displayOver();
	IMAGE imgBg;
	IMAGE imgOver;
	IMAGE imgWin;

	Block* curBlock;
	Block* nextBlock;
	
	//备用方块
	Block bakBlock;//当前方块降落时保存上一个合法位置
	int highestScore;//最高分
	int level;
	int lineCount;//当前已经消除了多少行 
	bool gameOver;
};

