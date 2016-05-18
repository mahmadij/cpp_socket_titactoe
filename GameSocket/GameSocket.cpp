#pragma once

/** @file GameSocket.hpp
@author Mehrnaz Ahmadi Joobaneh & Suraj Mandaniya 
@date 2015-12-05
@version 1.0.0
@brief implementation for Game socket class functions.
*/

#include <WinSock.h>
#include <iostream>
#include <Windows.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "GameSocket.hpp"
using namespace std;

#pragma comment (lib, "ws2_32.lib") 

GameSocket::GameSocket() {
	// initialize WSA		
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		cerr << "WSAStartup failed: " << iResult << endl;
	}
	hSocket = socket(AF_INET, SOCK_STREAM, NULL);
	sListen = socket(AF_INET, SOCK_STREAM, NULL);
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");//127.0.0.1 is the local host change it to connect to other servers
	serverAddress.sin_port = htons(2222);
	serverAddress.sin_family = AF_INET;
}

int GameSocket::connectToServer()
{
	return connect(hSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress));//connect to the server
}

void GameSocket::bindTheServer()
{
	bind(sListen, (SOCKADDR*)&serverAddress, sizeof(serverAddress));//we bind the server to our address and our port
}

bool GameSocket::acceptConnection()
{
	if (hSocket = accept(sListen, (SOCKADDR*)&serverAddress, &addrLen))
		return true;
	else
		return false;
}

void GameSocket::cleanUp()
{
	closesocket(hSocket);
	closesocket(sListen);
	WSACleanup();
}

