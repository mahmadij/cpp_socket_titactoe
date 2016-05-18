#if !defined GUARD_client_HPP
#define GUARD_client_HPP

/** @file TicTacToeClient.hpp
@author Mehrnaz Ahmadi Joobaneh & Suraj Mandaniya 
@date 2015-12-05
@version 1.0.0
@brief Game client logic.
*/
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <sstream>
#include <string>
#include "GameSocket.hpp"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")

using namespace std;

char matrix[9] = { '1','2','3','4','5','6','7','8','9' };
int moveCount = 0;
int id = 0;
int turn = 0;
char symbol = ' ';
char opp = ' ';
int change = 0;
int win = -1;
int Item;
GameSocket gsock = GameSocket();

//this re-draws the board on te screen
void Draw()
{
	std::stringstream ss;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			ss << matrix[i * 3 + j] << " ";
		}
		ss << endl;
	}
	std::string s = ss.str();
	cout << s;
}//end draw

//checking to see if a win has occured yet
int checkForWin()
{
	if ((matrix[0] == matrix[1] && matrix[1] == matrix[2])
		|| (matrix[3] == matrix[4] && matrix[4] == matrix[5])
		|| (matrix[6] == matrix[7] && matrix[7] == matrix[8])
		|| (matrix[0] == matrix[3] && matrix[3] == matrix[6])
		|| (matrix[1] == matrix[4] && matrix[4] == matrix[7])
		|| (matrix[2] == matrix[5] && matrix[5] == matrix[8])
		|| (matrix[0] == matrix[4] && matrix[4] == matrix[8])
		|| (matrix[2] == matrix[4] && matrix[4] == matrix[6]))
	{
		return turn;
	}
	else
		return -1;
}//end checkForWin

//client thread, one for each client
int ClientThread()
{
	char *recvbuf = new char[256];
	int size = 0;
	while (true)
	{
		if ((size = recv(gsock.hSocket, recvbuf, 256, NULL)) > 0)
		{
			++moveCount;
			//clear screen
			system("cls");
			Item = atoi(recvbuf);//string to int
			if (id == 1 || id == 0)
				matrix[Item - 1] = opp;
			else
				turn == 0 ? matrix[Item - 1] = 'X' : matrix[Item - 1] = 'O';
			win = checkForWin();
			if (win == id)
			{
				cout << "You are the winner!" << endl;
				break;
			}
			else if (win == ((id + 1) % 2) && id < 2)
			{
				cout << "Your opponent won!" << endl;
				break;
			}
			else if (win == ((id + 1) % 2) && id > 1)
			{
				cout << "Player " << turn << " won! Thank you for watching!" << endl;
				break;
			}
			Draw();//re-draw the board
			turn = (turn + 1) % 2;//change the turn
			if (turn == id)
				cout << "It is your turn. Please enter a number on the board: " << endl;
		}
		Sleep(50);//
	}
	return 0;
}

void executeClient()
{
	int iResult = 0;
	
	iResult = gsock.connectToServer();
	if (iResult != 0)
	{
		cerr << "Can't reach server!" << endl;
	}
	else
	{
		system("cls");//clear screen
		int ID = 0;

		char *nID = new char[64];//our id
		char *hello = new char[64];//a message from server


		//hSocket receives message 
		recv(gsock.hSocket, nID, 64, NULL);//with this function we can receive messages from the server
		recv(gsock.hSocket, hello, 64, NULL);
		ID = atoi(nID);//cast a char to an int
		id = ID;
		cout << hello << endl;
		if (ID % 2 == 0)
		{
			symbol = 'X';
			opp = 'O';
		}
		else if (ID % 2 == 1)
		{
			symbol = 'O';
			opp = 'X';
		}
		cout << "Tic Tac Toe" << endl;
		if (id == 0 || id == 1)
		{
			cout << "Your Symbol: " << symbol << endl;
			cout << "Your Opponent: " << opp << endl;
		}
		else
		{
			cout << "Enough players have joined, but don't worry! You can watch the game." << endl;
		}
		cout << "RULES:" << endl;
		cout << "You wait for your turn to choose." << endl;
		cout << "whichever player claiming three cells next to eachother in a linear way, wins." << endl;
		cout << "If you ready press [ENTER] again." << endl;

		cin.get();

		//clear screen
		system("cls");

		Draw();
		if (id == turn)
		{
			cout << "It is your turn." << endl;
		}
		else if (id != turn && id < 2)
		{
			cout << "It is your opponent's turn." << endl;
		}
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL);//our client thread

		//wait to send messages
		while (true)
		{
			if (id == 1 || id == 0)
			{
				int choice;
				char *sendbuf = new char[256];
				bool validMove = false;
				while (!validMove)
				{
					cin.getline(sendbuf, 256);
					choice = atoi(sendbuf);
					//check for invalid move
					if (matrix[choice - 1] == 'X' || matrix[choice - 1] == 'O' || choice > 9 || choice < 1 || choice == NULL)
					{
						cout << "Invalid move! Please try again!" << endl;
					}
					else
						validMove = true;
				}
				matrix[choice - 1] = symbol;
				send(gsock.hSocket, sendbuf, 256, NULL);
				++moveCount;
				system("cls");
				//check for win
				win = checkForWin();

				if (win == id)
				{
					cout << "You are the winner!" << endl;
					break;
				}
				else if (win == ((id + 1) % 2))
				{
					cout << "Your opponent won!" << endl;
					break;
				}
				if (moveCount == 9)//ran out of moves
				{
					cout << "No more tiles left to claim." << endl;
					break;
				}
				Draw();
				if (id == 1 || id == 0)
					cout << "It is your opponent's turn." << endl;

				Sleep(50);
			}//end if

		}//end while
		if (id > 1)
		{
			system("cls");
			cout << "Player " << turn << " won! Thank you for watching!" << endl;
		}
	}//end else

	//cleanup and finish
	gsock.cleanUp();
	system("pause");

}//end executeClient

#endif