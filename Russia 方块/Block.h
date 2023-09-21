#pragma once
#include<graphics.h>
#include<vector>
using namespace std;
struct Point {
	int row;
	int col;
};
class Block
{
public:
	Block();
	Block& operator=(const Block& other);
	void drop();
	void moveLeftRight(int offset);
	void retate();//Ðý×ª 
	void draw(int leftMargin,int topMargin);
	static IMAGE** getImages();

	bool blockInMap(const vector<vector<int>>& map);
	//Point* getSmallBlocks();
	void rotate();
	void solidify(vector<vector<int>>& map);
	int getBlockType();
private:
	Point smallBlocks[4];
	IMAGE *img;
	static IMAGE* imgs[7];
	static int size;
	int blockType;
};

