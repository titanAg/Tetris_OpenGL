// Kyle Orcutt

#include "block.h"
using namespace std;
Block::Block()
{
	this->x = 0;
	this->y = 0;
	this->value = 0;
	this->state = 0;
}
Block::Block(int x, int y, int value, int state) {
	this->x = x;
	this->y = y;
	this->value = value;
	this->state = state;
}
int Block::getX() {
	return x;
}
int Block::getY() {
	return y;
}
int Block::getValue() {
	return value;
}
int Block::getState() {
	return state;
}
void Block::setValue(int value) {
	this->value = value;
}
void Block::setState(int state) {
	this->state = state;
}
