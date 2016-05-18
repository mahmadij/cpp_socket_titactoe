//client code
/** @file main.cpp
@author Mehrnaz Ahmadi Joobaneh & Suraj Mandaniya 
@date 2015-12-05
@version 1.0.0
@brief the client side entry point for Tic Tac Toe game.
*/

#include "TicTacToeClient.hpp"
#include <iostream>

using namespace std;

int main()
{
	system("color 0a"); //will type green on the console -> cool color!
	cout << "Please press [ENTER]" << endl;
	cin.get();
	//hiding the server code
	executeClient();
	return 0;
}