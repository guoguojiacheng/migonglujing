#include "pch.h"
#include <iostream>
#include <stdlib.h>

using namespace std;
/*
迷宫路径：找到从左上角到右下角的路径 如果有则打印出来
*/
const int WAY_CNT = 4;

const int WAY_UP = 0;
const int WAY_DOWN = 1;
const int WAY_LEFT = 2;
const int WAY_RIGHT = 3;

const int WAY_OK = 5;
const int WAY_ERR = 6;

class Mazenode//迷宫节点类型
{
public:
	Mazenode()
	{
		for (int i = 0; i < WAY_CNT; ++i)
		{
			_state[i] = WAY_OK;
		}
	}
	void set_x(int x) { _x = x; }
	void set_y(int y) { _y = y; }
	void set_val(int data) { _val = data; }

	int get_val() { return _val; }
	int get_row() { return _x; }
	int get_col() { return _y; }
	void ajdust(int way) { _state[way] = WAY_ERR; }
	int get_way(int way) { return _state[way]; }
private:
	int _val;
	int _x;
	int _y;
	int _state[WAY_CNT];
};

class Stack//栈类型
{
public:
	Stack(int top = -1, int size = 10)
		:_top(top), _size(size)
	{
		_stack = (Mazenode*)malloc(sizeof(Mazenode) * size);
	}
	void push(const Mazenode& node)
	{
		if (_top == _size)
		{
			resize(2 * _size);
		}
		this->_top++;
		_stack[_top] = node;
	}
	void pop() {
		if (empty())
		{
			return;
		}
		_top--;
	}
	Mazenode& top() { return _stack[_top]; }
	bool empty() { return _top == -1; }
	~Stack() { free(_stack); }
private:
	Mazenode* _stack;
	int _top;
	int _size;
	void resize(int size)
	{
		_stack = (Mazenode*)realloc(_stack, sizeof(Mazenode) * size);
		_size *= 2;
	}
};

class Maze//迷宫类型
{
public:
	Maze(int r = 10, int c = 10)
		:_row(r), _col(c), _stack(Stack())
	{
		_pmazes = new Mazenode * [r];
		for (int i = 0; i < r; ++i)
		{
			_pmazes[i] = new Mazenode[c];
		}
	}
	void initMazenode(int r, int c, int data)
	{
		_pmazes[r][c].set_x(r);
		_pmazes[r][c].set_y(c);
		_pmazes[r][c].set_val(data);
	}
	void adjustMazeNode()
	{
		for (int i = 0; i < _row; ++i)
		{
			for (int j = 0; j < _col; ++j)
			{
				if (i == 0 || _pmazes[i - 1][j].get_val() == 0)
				{//上方不通
					_pmazes[i][j].ajdust(WAY_UP);
				}
				if ((i + 1) == _row || _pmazes[i + 1][j].get_val() == 0)
				{//下方不通
					_pmazes[i][j].ajdust(WAY_DOWN);
				}
				if (j == 0 || _pmazes[i][j - 1].get_val() == 0)
				{//左方不通
					_pmazes[i][j].ajdust(WAY_LEFT);
				}
				if ((j + 1) == _col || _pmazes[i][j + 1].get_val() == 0)
				{
					_pmazes[i][j].ajdust(WAY_RIGHT);
				}
			}
		}
	}
	//找迷宫路径
	void adjustMazeNodefindMazePath()
	{
		/*
		向右走 则右设为err 下一个的左设为err
		如果没有路可走，则出栈
		*/
		if (_pmazes[0][0].get_val() == 0)
			return;
		_stack.push(_pmazes[0][0]);
		int row = 0;
		int col = 0;
		while (!_stack.empty())//退出条件：1栈空，2走到终点
		{
			//找出可走的方向，否则退栈
			int way = WAY_CNT;
			Mazenode& node = _stack.top();
			row = node.get_row();
			col = node.get_col();
			for (int i = 0; i < WAY_CNT; ++i)
			{
				if (_pmazes[row][col].get_way(i) == WAY_OK)
				{
					way = i;
					break;
				}
			}

			if (way == WAY_CNT)
			{
				_stack.pop();
			}
			else
			{
				/*const int WAY_UP = 0;
				const int WAY_DOWN = 1;
				const int WAY_LEFT = 2;
				const int WAY_RIGHT = 3;*/
				_pmazes[row][col].ajdust(way);
				//找到要入栈的节点
				if (way == WAY_UP || way == WAY_LEFT)
				{
					if (way == WAY_UP)
					{
						row -= 1;
					}
					else
					{
						col -= 1;
					}
					_pmazes[row][col].ajdust(way + 1);
				}
				else
				{
					if (way == WAY_DOWN)
					{
						row += 1;
					}
					else
					{
						col += 1;
					}
					_pmazes[row][col].ajdust(way - 1);
				}
				_stack.push(_pmazes[row][col]);
			}
			if (row == _row - 1 && col == _col - 1)
			{
				break;
			}
		}
	}
	void showMazePath()
	{
		int row = -1;
		int col = -1;
		while (!_stack.empty())
		{
			row = _stack.top().get_row();
			col = _stack.top().get_col();
			_pmazes[row][col].set_val(-1);
			_stack.pop();
		}

		for (int i = 0; i < _row; ++i)
		{
			for (int j = 0; j < _col; ++j)
			{
				if (_pmazes[i][j].get_val() == -1)
				{
					cout << "*" << " ";
					continue;
				}
				cout << _pmazes[i][j].get_val() << " ";
			}
			cout << endl;
		}
	}

private:
	int _row;
	int _col;
	Mazenode** _pmazes;
	Stack _stack;
};

int main()
{
	cout << "请输入迷宫行列:";
	int row, col;
	cin >> row >> col;
	Maze maze(row, col);
	cout << "请输入迷宫节点信息(1表示能走，0表示不能走):" << endl;
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			int data;
			cin >> data;
			maze.initMazenode(i, j, data);
		}
	}
	// 调整迷宫节点方向的行走状态
	maze.adjustMazeNode();
	cout << "adjust over" << endl;
	// 开始寻找迷宫路径
	maze.adjustMazeNodefindMazePath();
	cout << "======================================================" << endl;
	// 打印迷宫寻找的结果
	maze.showMazePath();

	return 0;
}
