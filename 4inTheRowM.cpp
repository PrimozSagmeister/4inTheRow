#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

void RandomW(float w[8905])
{
	for(int i = 0; i < 8905; i++)
	{
		w[i] = (rand() % 30000) / 7500.0 - 2;
	}
}

void printout(int board[7][7])
{
	for(int j = 5; j >= 0; j--)
	{
		for(int i = 0; i < 7; i++)
		{
			cout << board[i][j] << "|";
		}
		cout << endl;
	}
	for(int i = 0; i < 7; i++)
	{
		cout << "__";
	}
	cout << endl;
	for(int i = 0; i < 7; i++)
	{
		cout << i+1 << " ";
	}
	cout << endl;
}

int checkWin(int board[7][7], int n)
{
	for(int i = 0; i < 7; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			if(board[i][j] == n && board[i][j+1] == n && board[i][j+2] == n && board[i][j+3] == n)
			{
				return n;
			}
		}
	}
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			if(board[i][j] == n && board[i+1][j] == n && board[i+2][j] == n && board[i+3][j] == n)
			{
				return n;
			}
		}
	}
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			if(board[i][j] == n && board[i+1][j+1] == n && board[i+2][j+2] == n && board[i+3][j+3] == n)
			{
				return n;
			}
		}
	}
	for(int i = 0; i < 4; i++)
	{
		for(int j = 5; j >= 3; j--)
		{
			if(board[i][j] == n && board[i+1][j-1] == n && board[i+2][j-2] == n && board[i+3][j-3] == n)
			{
				return n;
			}
		}
	}
	return 0;
}



