#if !defined GUARD_server_HPP
#define GUARD_server_HPP

//server code

/** @file TicTacToeServer.hpp.cpp
@author Mehrnaz Ahmadi Joobaneh & Suraj Mandaniya 
@date 2015-12-05
@version 1.0.0
@brief Game server code.
*/

#include <WinSock.h>
#include <iostream>
#include <Windows.h>
#include <sstream>
#include "GameSocket.hpp"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#pragma comment (lib, "ws2_32.lib") 

int Counter; //# of clients connected
GameSocket gsock = GameSocket();

char player = 'X';

//one new thread per client which receive/sends messages and/or displays them
int ServerThread(int ID)
{
	//char *sendbuf = new char[256];
	char *recvbuf = new char[256];
	int size = 0;
	while (true)
	{
		if ((size = recv(gsock.Connections[ID], recvbuf, 256, NULL)) > 0)//is the message empty?
		{
			//loop through all clients
			for (int a = 0; a < Counter; a++)
			{
				if (gsock.Connections[a] == gsock.Connections[ID])//don't send the message to the sender
				{
					cout << "Client[" << ID + 1 << "]: " << recvbuf << endl;
				}
				else//send it to everyone else
				{
					send(gsock.Connections[a], recvbuf, 256, NULL);
				}
			}
		}
	}
}

void executeServer()
{	
	gsock.Connections = (SOCKET*)calloc(64, sizeof(SOCKET));
	gsock.bindTheServer();

	listen(gsock.sListen, 64);//listen for incoming connections

	while (true)//search for connections
	{
		if (gsock.acceptConnection())// new client
		{
			gsock.Connections[Counter] = gsock.hSocket;
			char *Name = new char[64];//the name of the client

			sprintf_s(Name, 64, "%i", Counter);
			if (*Name == '0')
				player = 'X';
			else if (*Name == '1')
				player = 'O';
			send(gsock.Connections[Counter], Name, 64, NULL);//send ID to client

			send(gsock.Connections[Counter], "Welcome to the Game!", 64, NULL);//

			cout << "NewConnection!" << endl;

			Counter++;//prepare for next client
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ServerThread, (LPVOID)(Counter - 1), NULL, NULL);//our client thread

		}

		Sleep(50);//wait 50 milisecnds
	}

	gsock.cleanUp();
}//end executeServer



#endif