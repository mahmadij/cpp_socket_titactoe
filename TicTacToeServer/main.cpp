//server code

/** @file main.cpp
@author Mehrnaz Ahmadi Joobaneh & Suraj Mandaniya 
@date 2015-12-05
@version 1.0.0
@brief Game server main entry point.
*/

#include <iostream>
#include "TicTacToeServer.hpp"

using namespace std;

int main()
{
	system("color 0a"); //green console color -> cool color!
	cout << "Server started!" << endl;
	//hiding the server code
	executeServer();
	return 0;
}