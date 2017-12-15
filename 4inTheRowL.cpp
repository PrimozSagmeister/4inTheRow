#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

void writeW(float w[8905], int n)
{
	fstream dat;
	stringstream ss;
	ss << n;
	string str = ss.str();
	string weight = "/Users/primoz/Desktop/Deep Learning/Convolution Network/4vVrstoCon/Weights/weights" + str + ".txt";  

	dat.open(weight, ios::out | ios::trunc);

	for(int i = 0; i < 8905; i++)
	{
		dat << w[i] << "  ";
	}

	dat.close();
}

void readW(float w[8905], int n)
{
	fstream dat;
	stringstream ss;
	ss << n;
	string str = ss.str();
  	string ime = "/Users/primoz/Desktop/Deep Learning/Convolution Network/4vVrstoCon/Weights/weights" + str + ".txt";
	float vrednost;
 	string vrs;
  	dat.open(ime.c_str(), ios::in);
  	if (!dat.is_open()) 
	{
  		cout << "Napaka pri odpiranju datoteke!" << endl;
 	}
	else
	{
		for(int i = 0; i < 8905; i++)
		{
			dat >> vrednost;
			w[i] = vrednost;
		}
  	} 

  	dat.close();
}

void BtoM(int board[7][7], int map[72], int p)
{
	p = p % 2;
	for(int i = -1; i < 8; i++)
	{
		for(int j = -1; j < 7; j++)
		{
			if(p == 0)
			{
				if(i >= 0 && i <= 6 && j >= 0 && j <= 5)
				{
					if(board[i][j] == 2)
					{
						map[j+(i*7)] = -1;
					}
					else
					{
						map[j+(i*7)] = board[i][j];
					}
				}
				else
				{
					map[j+(i*7)] = 0;
				}
			}
			else
			{
				if(i >= 0 && i <= 6 && j >= 0 && j <= 5)
				{
					if(board[i][j] == 1)
					{
						map[j+(i*7)] = -1;
					}
					else if(board[i][j] == 2)
					{
						map[j+(i*7)] = 1;
					}
					else
					{
						map[j+(i*7)] = board[i][j];
					}
				}
				else
				{
					map[j+(i*7)] = 0;
				}
			}
		}
	}
}

void learn(int map[], float w[], float wish, float rez, float cL[1460][2], float oL[10][2], float c[], float o[])
{
	float El;
	int wn = 8904;
	int cn = 1459;

	int con[] = {1, 5, 25, 125, 0, 0, 2};
	int v[] = {750, 10, 0, 0, 0, 0, 1};
	int size[] = {9, 7, 5, 3, 0};
	if(rez < 0.05)
	{
		rez = 0.05;
	}
	if(rez > 0.95)
	{
		rez = 0.95;
	}
	for(int i = v[6]; i >= 0; i--)
	{
		for(int j = v[i]-1; j >= 0; j--)
		{
			if(i == v[6])
			{
				El = (-1) * (wish - rez) * rez * (1 - rez) * o[j];
				w[wn] -= El * 0.005;
				oL[j][1] = (-1) * (wish - rez) * rez * (1 - rez) * w[wn];
				wn--;
			}
			else
			{
				for(int k = v[i+1] - 1; k >= 0; k--)
				{
					if(i == 0)
					{
						El = oL[k][1] * oL[k][0] * c[cn];
						w[wn] -= El * 0.005;
						cL[cn][1] = oL[k][1] * oL[k][0] * w[wn];
						wn--;
					}
				}
				cn--;
			}
		}
	}

	int nn = 1459;
	int scn;
	for(int i = con[6]; i >= 0; i--)
	{
		for(int j = con[i+1]-1; j >= 0; j--)
		{
			scn = cn;
			for(int l = size[i+1]-2; l >= 0; l--)
			{
				for(int d = size[i+1]-1; d >= 0; d--)
				{
					if(i == 0)
					{
						if(scn < 0)
						{
							scn += 72;
						}
						for(int ww = 0; ww < 9; ww++)
						{
							El = cL[nn][1] * cL[nn][0] * map[scn];
							w[wn-ww] -= El * 0.005;
							if((ww+1) % 3 == 0)
							{
								scn -= size[i] - 2;
							}
						}
					}
					else
					{
						for(int ww = 0; ww < 9; ww++)
						{
							El = cL[nn][1] * cL[nn][0] * c[scn];
							w[wn-ww] -= El * 0.005;	
							cL[scn][1] = cL[nn][1] * cL[nn][0] * w[wn-ww];
							if((ww+1) % 3 == 0)
							{
								scn -= size[i] - 2;
							}
						}
					}
					scn = cn-1;
					nn--;
				}
			}
			if(j % 5 == 0)
			{
				cn -= ((size[i]-1) * (size[i]));
			}
			wn -= 9;
		}
	}
}

float calc(float w[], int board[7][7], int p, float wish)
{
	float c[1460];
	float rez = 0;
	int map[72];
	int wn = 0;
	int cn = 0;
	int nn = 0;
	float cL[1460][2];
	
	BtoM(board, map, p);

	int con[] = {1, 5, 25, 125, 0, 0, 2};
	int v[] = {750, 10, 0, 0, 0, 0, 1};
	int size[] = {9, 7, 5, 3, 0};

	for(int i = 0; i < con[6] + 1; i++)
	{
		for(int j = 0; j < con[i+1]; j++)
		{
			for(int k = 0; k < size[i+1]; k++)
			{
				for(int l = 0; l < size[i+1] - 1; l++)
				{
					if(i == 0)
					{
						c[cn] = map[k*size[i]+l]*w[wn]+map[k*size[i]+l+1]*w[wn+1]+map[k*size[i]+l+2]*w[wn+2];
						c[cn] += map[k*size[i]+l+size[i]-1]*w[wn+3]+map[k*size[i]+l+size[i]]*w[wn+4]+map[k*size[i]+l+size[i]+1]*w[wn+5];
						c[cn] += map[k*size[i]+l+((size[i]-1)*2)]*w[wn+6]+map[k*size[i]+l+((size[i]-1)*2)+1]*w[wn+7]+map[k*size[i]+l+((size[i]-1)*2)+2]*w[wn+8];
						if(c[cn] < 0)
						{
							c[cn] *= 0.01;
							cL[cn][0] = 0.01;
							cL[cn][1] = 0;
						}
						else
						{
							cL[cn][0] = 1;
							cL[cn][1] = 0;
						}
						c[cn] += 0.5;
					}
					else
					{
						c[cn] = c[nn+k*size[i]+l]*w[wn]+c[nn]*w[wn+k*size[i]+l+1]+c[nn+k*size[i]+l+2]*w[wn+2];
						c[cn] += c[nn+k*size[i]+l+size[i]-1]*w[wn+3]+c[nn+k*size[i]+l+size[i]]*w[wn+4]+c[nn+k*size[i]+l+size[i]+1]*w[wn+5];
						c[cn] += c[nn+k*size[i]+l+((size[i]-1)*2)]*w[wn+6]+c[nn+k*size[i]+l+((size[i]-1)*2)+1]*w[wn+7]+c[nn+k*size[i]+l+((size[i]-1)*2)+2]*w[wn+8];

						if(c[cn] < 0 && i != con[6])
						{
							c[cn] *= 0.01;
							cL[cn][0] = 0.01;
							cL[cn][1] = 0;
						}
						else if(c[cn] >= 0 && i != con[6])
						{
							cL[cn][0] = 1;
							cL[cn][1] = 0;
						}
						else if(i == con[6])
						{
							c[cn] = 1/(1+exp(-c[cn]));
							cL[cn][0] = c[cn] * (1 - c[cn]);
							cL[cn][1] = 0;
						}
					}
					cn++;
				}
			}
			wn += 9;
			if(j % 5 == 4 && i != 0)
			{
				nn += size[i] * (size[i]-1);
			}
		}
	}

	float o[10];
	float oL[10][2];
	for(int i = 0; i < v[6] + 1; i++)
	{
		for(int j = 0; j < v[i]; j++)
		{
			if(i == v[6])
			{
				int k = 0;
				if(j == 0)
				{
					rez = o[j] * w[wn];
				}
				else
				{
					rez += o[j] * w[wn];
				}
				wn++;
			}
			else
			{
				for(int k = 0; k < v[i+1]; k++)
				{
					if(i == 0)
					{
						if(j == 0)
						{
							o[k] = c[j+710] * w[wn];
						}
						else
						{
							o[k] += c[j+710] * w[wn];
						}
					}
					wn++;
					if(o[k] < 0 && j == v[i]-1)
					{
						o[k] *= 0.01;
						oL[k][0] = 0.01;
						oL[k][1] = 0;
					}
					else
					{
						oL[k][0] = 1;
						oL[k][1] = 0;
					}
				}
			}
		}
	}
	float rez1 = rez;
	rez = 1/(1+exp(-rez));
	if(wish > -0.5)
	{
		learn(map, w, wish, rez, cL, oL, c, o);
	}
	return rez1;
}

void cross(float w1[8905], float w2[8905], float w3[8905], float w4[8905])
{
	for(int i = 0; i < 8905; i++)
	{
		if(rand() % 2 == 0)
		{
			w3[i] = w1[i];
			w4[i] = w2[i];
		}
		else
		{
			w4[i] = w1[i];
			w3[i] = w2[i];
		}
	}
}  

void mutation(float w[8905])
{
	int r;
	for(int i = 0; i < 8905; i++)
	{
		r = rand() % 100;
		if(r < 4)
		{
			w[i] += (rand() % 1500) / 750.0 - 2;
		}
		else if(r < 7)
		{
			w[i] *= (rand() % 1500) / 750.0 - 2;
		}
		else if(r < 9)
		{
			w[i] *= -1;
		}
	}
}















