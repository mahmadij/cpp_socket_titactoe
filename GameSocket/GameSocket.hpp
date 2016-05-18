#if !defined GUARD_GameSocket_HPP
#define GUARD_GameSocket_HPP

/** @file GameSocket.hpp
@author Mehrnaz Ahmadi Joobaneh & Suraj Mandaniya 
@date 2015-12-05
@version 1.0.0
@brief Game socket class.
*/

#include <WinSock.h>
#include <iostream>
#include <Windows.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#pragma comment (lib, "ws2_32.lib") 


class GameSocket {
public:
	SOCKADDR_IN serverAddress;//saves the address and the port of the server
	int addrLen = sizeof(serverAddress);//length of the address
	int Counter; //# of clients connected

	SOCKET hSocket;
	SOCKET sListen;
	SOCKET *Connections;

	WSAData wsaData;

	GameSocket();

	int connectToServer();	

	void bindTheServer();	

	bool acceptConnection();	

	void cleanUp();
	
};//end class

#endif