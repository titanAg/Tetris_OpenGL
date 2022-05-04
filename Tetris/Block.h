#pragma once
class Block
{
private:
	int x;
	int y;
	int value;
	int state;
public:
	Block();
	Block(int x, int y, int value, int state);
	int getX();
	int getY();
	int getValue();
	int getState();
	void setValue(int value);
	void setState(int state);
};

