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
	int getDelay();//���ؾ�����һ�ε���,���ʱ�� ,��һ�ε��÷���0
	void drop();
	void clearLine();
	void moveLeftRight(int offset);//�����ƶ�����
	void rotate();//��ת
	void drawScore();
	int delay;
	bool update;//�Ƿ����
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
	
	//���÷���
	Block bakBlock;//��ǰ���齵��ʱ������һ���Ϸ�λ��
	int highestScore;//��߷�
	int level;
	int lineCount;//��ǰ�Ѿ������˶����� 
	bool gameOver;
};

