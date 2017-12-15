#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include <stdlib.h>
using namespace std;

#include "/Users/primoz/Desktop/Deep Learning/Convolution Network/4vVrstoCon/4inTheRowL.cpp" 
#include "/Users/primoz/Desktop/Deep Learning/Convolution Network/4vVrstoCon/4inTheRowM.cpp"

class Player{
	public:
	float w[8905];
};

int main()
{
 	int board[7][7];
 	
 	float plays[7];
 	float choice[2];
 	Player player[180];

	srand(time(NULL));
	int game = 0;
	
	for(int i = 0; i < 7; i++)
	{
		for(int j = 0; j < 7; j++)
		{
			board[i][j] = 0;
		}
	}

	int a, b;
	cout << "Put 1 for new learning or 2 for a game against yourself (your friend)" << endl;
	cout << "Put 11 to countinue learning or 3 for a game against AI" << endl;
	cin >> a;
	if(a == 1 || a == 11)
	{
		clock_t zacetek,konec;
		float cas;

		int p = 0;
		if(a == 1)
		{
			for(int i = 0; i < 180; i++)
			{
				RandomW(player[i].w);
			}
		}
		else if(a == 11)
		{
			for(int i = 0; i < 180; i++)
			{
				readW(player[i].w, i);
			}
		}
		zacetek = clock();
		int doit = 0;
		for(int sim = 0; sim < 10000; sim++)
		{
			for(int spec = 0; spec < 90; spec++)
			{
				while(board[0][6]+board[1][6]+board[2][6]+board[3][6]+board[4][6]+board[5][6]+board[6][6] != 42 && game == 0)
				{
					//policy gradient
					for(int ss = 0; ss < 20; ss++)
					{
						doit = 0;
						for(int i = 0; i < 7; i++)
						{
							if(board[i][6] < 6)
							{
								if(p % 2 == 0)
								{
									board[i][board[i][6]] = 1;
									if(checkWin(board, p%2+1))
									{
										calc(player[2 * spec].w, board, p, 1);
										doit = 1;
									}
									else if(doit == 1)
									{
										calc(player[2 * spec].w, board, p, 0.1);
									}
									board[i][board[i][6]] = 0;
									board[i][board[i][6]] = 2;
									if(checkWin(board, (p-1)%2+1))
									{
										calc(player[2 * spec].w, board, p, 0.8);
										doit = 1;
									}
									else if(doit == 1)
									{
										calc(player[2 * spec].w, board, p, 0.1);
									}
									board[i][board[i][6]] = 0;
								}	
								else
								{
									board[i][board[i][6]] = 2;
									if(checkWin(board, p%2+1))
									{
										calc(player[(2 * spec) + 1].w, board, p, 1);
										doit = 1;
									}
									else if(doit == 1)
									{
										calc(player[(2 * spec) + 1].w, board, p, 0.1);
									}
									board[i][board[i][6]] = 0;
									board[i][board[i][6]] = 1;
									if(checkWin(board, (p-1)%2+1))
									{
										calc(player[(2 * spec) + 1].w, board, p, 0.8);
										doit = 1;
									}
									else if(doit == 1)
									{
										calc(player[(2 * spec) + 1].w, board, p, 0.1);
									}
									board[i][board[i][6]] = 0;
								}
							}
						}
						if(doit == 0)
						{
							ss = 20;
						}
					}

					for(int i = 0; i < 7; i++)
					{
						if(board[i][6] == 6)
						{
							plays[i] = -10000000;
						}
						else if(board[i][6] < 6)
						{
							if(p % 2 == 0)
							{
								board[i][board[i][6]] = 1;
								plays[i] = calc(player[2 * spec].w, board, p, -0.1);
							}	
							else
							{
								board[i][board[i][6]] = 2;
								plays[i] = calc(player[(2 * spec) + 1].w, board, p, -0.1);
							}
							board[i][board[i][6]] = 0;
						}
					}
					choice[0] = plays[0];
					choice[1] = 0;
					for(int i = 0; i < 7; i++)
					{
						if(plays[i] > choice[0])
						{
							choice[0] = plays[i];
							choice[1] = i;
						}
					}
					if(p % 2 == 0)
					{
						board[int(choice[1])][board[int(choice[1])][6]] = 1;
					}
					else
					{
						board[int(choice[1])][board[int(choice[1])][6]] = 2;
					}
					board[int(choice[1])][6]++;
					game = checkWin(board, p%2+1);
					p++;
				}
				if(game == 1)
				{
					player[spec] = player[spec*2];
				}
				else if(game == 2)
				{
					player[spec] = player[(spec*2) + 1];
				}
				else
				{
					player[spec] = player[(spec*2) + 1];
				}
				for(int i = 0; i < 7; i++)
				{
					for(int j = 0; j < 7; j++)
					{
						board[i][j] = 0;
					}
				}
				game = 0;
				p = 0;
			}
			for(int spec = 0; spec < 45; spec++)
			{
				cross(player[2 * spec].w, player[(2 * spec) + 1].w, player[90 + (2 * spec)].w, player[(2 * spec) + 91].w);
			}
			for(int spec = 90; spec < 180; spec++)
			{
				mutation(player[spec].w);	
			}
			if(sim % 50 == 0 && sim > 3000)
			{
				cout << sim << endl;
				konec = clock();
				cas = (konec - zacetek) / (1.0 * CLOCKS_PER_SEC);
				if(cas/3600 > 6.5)
				{
					break;
				}
			}
		}
		for(int i = 0; i < 180; i++)
		{
			writeW(player[i].w, i);
		}
		konec = clock();
		cas = (konec - zacetek) / (1.0 * CLOCKS_PER_SEC);
		cout << endl << "Program was learning: " << cas/3600 << "h" << endl;
	}
	if(a == 2)
	{
		printout(board);

		int n = 0;
		int win = 0;
		while(game == 0)
		{
			for(int i = 0; i < 7; i++)
			{
				if(board[i][6] < 6)
				{
					if(n % 2 == 0)
					{
						board[i][board[i][6]] = 1;
						if(checkWin(board, n%2+1))
						{
							cout << "Winnig move: " << i + 1 << endl;
						}
						board[i][board[i][6]] = 0;
						board[i][board[i][6]] = 2;
						if(checkWin(board, (n-1)%2+1))
						{
							cout << "Not to lose move: " << i + 1 << endl;
						}
						board[i][board[i][6]] = 0;
					}	
					else
					{
						board[i][board[i][6]] = 2;
						if(checkWin(board, n%2+1))
						{
							cout << "Winnig move: " << i + 1 << endl;
						}
						board[i][board[i][6]] = 0;
						board[i][board[i][6]] = 1;
						if(checkWin(board, (n-1)%2+1))
						{
							cout << "Not to lose move: " << i + 1 << endl;
						}
						board[i][board[i][6]] = 0;
					}
				}
			}
			
			cin >> b;
			b--;
			if(n % 2 == 0)
			{
				board[b][board[b][6]] = 1;
			}
			else
			{
				board[b][board[b][6]] = 2;
			}
			board[b][6]++;

			printout(board);
			game = checkWin(board, n%2+1);
			n++;
		}
		if(game == 1)
		{
			cout << "Player 1 won" << endl;
			int map[72];
			BtoM(board, map, 1);
		}
		if(game == 2)
		{
			cout << "Player 2 won" << endl;
		}
	}
	if(a == 3)
	{
		RandomW(player[20].w);
		printout(board);

		int win = 0;
		while(game == 0)
		{
			cin >> b;
			b--;

			board[b][board[b][6]] = 1;
			board[b][6]++;

			printout(board);
			game = checkWin(board, 1);
			if(game != 0)
			{
				break;
			}
			for(int i = 0; i < 7; i++)
			{
				if(board[i][6] == 6)
				{
					plays[i] = -10000000;
				}
				else if(board[i][6] < 6)
				{
					board[i][board[i][6]] = 2;
					plays[i] = calc(player[20].w, board, 1, -0.1);
					cout << plays[i] << " " << calc(player[20].w, board, 1, -0.1) << " ";
					board[i][board[i][6]] = 0;
				}
			}
			cout << endl;
			choice[0] = plays[0];
			choice[1] = 0;
			for(int i = 0; i < 7; i++)
			{
				if(plays[i] > choice[0])
				{
					choice[0] = plays[i];
					choice[1] = i;
				}
			}
			board[int(choice[1])][board[int(choice[1])][6]] = 2;
			board[int(choice[1])][6]++;
			printout(board);
			game = checkWin(board, 2);
		}
		if(game == 1)
		{
			cout << "Player 1 won" << endl;
		}
		if(game == 2)
		{
			cout << "Player 2 won" << endl;
		}
	}
	if(a == 20)
	{
		RandomW(player[10].w);
		for(int i = 0; i < 1000; i ++)
		{
			cout << calc(player[10].w, board, 1, 0.6) << endl;
		}
	}
}










