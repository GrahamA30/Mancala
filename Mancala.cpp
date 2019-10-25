#include <iostream>
#include <iomanip>
#include <limits.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MIN 1
#define MAX 0
using namespace std;

void printBoard(int board[]);
int getUserMove(int board[]);
bool checkBoard(int board[]); //check if either side of the board is empty
bool takeMove(int board[], int spotinarray, int human_or_comp); //return true if the move is able to capture opponents marbles otherwise false
int findAcross(int ending);
int findOnlyMove(int board[]);
typedef struct Move { 
	int binNum;
	int score;
} Move;
Move minmax(int board[], int d, int maxD, int minOrMax, int alpha, int beta);
int computerMove(int board[], int maxD);
int evalFunction(int board[], int minOrMax); //Evaluate the board based on who is currently playing


int main() {
	srand((unsigned)time(NULL)); 
	int m;
	int playerTurn;
	int coinFlip = rand() % 2; //heads = 1, tails = 0
	int levelChoice;

	cout << "Heads or tails?" << endl;
	cout << "For heads, enter 1" << endl;
	cout << "For tails, enter 0" << endl;
	int input;
	cin >> input
		;
	while (input != 1 && input != 0){
		cout << "Invalid Input" << endl;
		cout << "Try Again" << endl;
		cin >> input;
	}

	if (coinFlip = input) { //Player goes first
		playerTurn = 0;
	}
	else { //computer goes first.
		playerTurn = 1;
	}

	int maxDepth = 2;
	cout << "Do you want to play level 1 or level 2? (Enter 1 or 2)." << endl;
	cin >> levelChoice;
	while (levelChoice != 1 && levelChoice != 2){
		cout << "Invalid Level" << endl;
		cout << "Try Again" << endl;
		cin >> levelChoice;
	}

	if (levelChoice == 2) {
		cout << "What would you like the depth to be?" << endl;
		cout << "Please select a value greater than 0 and less than 11.(suggestion 4 or 5)" << endl;
		cin >> maxDepth;
		while (maxDepth <= 0) {
			cout << "Please select a value greater than 0 and less than 11." << endl;
			cin >> maxDepth;
		}
	}

	int userMove = -1;
	int board[14] = { 4,4,4,4,4,4,0,4,4,4,4,4,4,0 };
	bool gameOver = false;
	int onlyMove =-1;
	while (!gameOver) {
		if (playerTurn == MIN) { //computer move

			if ((onlyMove = findOnlyMove(board)) != -1) {
				cout << "Forced to make the only move possible." << endl;
				m = onlyMove;
			} 
			else {
				m = computerMove(board, maxDepth);
			}
			cout << "Computer moved at " << m << endl;
			takeMove(board, m, MAX);
			printBoard(board); 
			gameOver = checkBoard(board);
			if (gameOver) {
				break;
			}
			else {
				playerTurn = 0; //swicth turn.
			}
		}
		else {
			cout << "Your Move" << endl;
			printBoard(board);
			userMove = getUserMove(board);
			takeMove(board, userMove, MIN);
			printBoard(board);
			gameOver = checkBoard(board);
			if (gameOver) {
				break;
			}
			else {
				playerTurn = 1;
			}
		}
	}

	cout << "GAME OVER!" << endl;
	if (board[6] > board[13]) {
		cout << "YOU WIN!" << endl;
	}
	else if (board[13] > board[6]) {
		cout << "COMPUTER WIN!" << endl;
	}
	else {
		cout << "DRAW!" << endl;
	}
	printBoard(board);

	system("pause");
	return 0;
}

bool takeMove(int board[], int choice, int minOrMax) {
	int avoid; //location of the other player's Mancala
	if (MAX == minOrMax) { //computer turn
		avoid = 6;
	}
	else { //user turn
		avoid = 13;
	}

	int marblesHolding = board[choice];
	board[choice] = 0; 
	int next = choice + 1;

	while(marblesHolding > 0) {
		if (next == avoid) { //skip over other player's Mancala
			if (avoid == 13) {
				next = 0; 
			}
			else {
				next = 7;
			}
		}

		int nextBinMarbles = board[next];
		if (marblesHolding == 1 && nextBinMarbles == 0) {
			if (next != 13 || next != 6) { //stop if the last marble lands in someone's Mancala

				if (MIN == minOrMax && next > -1 && next < 6) { //capture marbles for the user
					int across = findAcross(next); //
					if (board[across] > 0) {
						board[6] = board[6] + 1 + board[across]; //add marbles to mancala 
						board[across] = 0; 
						return true;
					}
				}

				if (MAX == minOrMax && next > 6 && next < 13) { //capture marbles for the computer
					int across = findAcross(next); 
					if (board[across] > 0) {
						board[13] = board[13] + 1 + board[across]; 
						board[across] = 0; 
						return true;
					}
				}
			}
		}

		board[next] = nextBinMarbles + 1;
		next = (next + 1) % 14; 
		marblesHolding--;
	}
	return false;

}

Move minmax(int board[], int d, int maxD, int minOrMax, int alpha, int beta) {
	//minOrMax is MAX if its the computer turn and MIN if its the user turn

	Move mTemp, m;
	int boardCopy[14];
	bool isEmpty;
	isEmpty = checkBoard(board);

	if (isEmpty) {
				 //return INT_MAX as the score if MAX is playing and INT_MIN if MIN is playing; bin number not important
		if (minOrMax == MIN) {
			m.score = INT_MIN;
			m.binNum = -1; 
			return m;
		}
		else {
			m.score = INT_MAX;
			m.binNum = -1;
			return m;
		}
	}
	else if (d == maxD) {
		m.score = evalFunction(board, minOrMax);
		m.binNum = -1; 
		return m;
	}
	else {
		if (minOrMax == MIN) { //user turn
			m.score = INT_MAX;
			m.binNum = -1;
			for (int i = 0; i <= 5; i++) { //check every move on the user side.
				if (board[i] != 0) {
					for (int j = 0; j < 14; j++) {
						boardCopy[j] = board[j];
					}

					takeMove(board, i, minOrMax);
					mTemp = minmax(board, d + 1, maxD, MAX, alpha, m.score);

					if (m.score <= alpha) { 
						break; 
					}

					if (mTemp.score < m.score) {
						m.score = mTemp.score;
						m.binNum = i;						 
					}

					for (int k = 0; k < 14; k++) {
						board[k] = boardCopy[k];
					}
				}
			}
			return m;
		}
		else //computer turn
		{
			m.score = INT_MIN;
			m.binNum = -1;
			for (int i = 7; i <= 12; i++) { //check every move on the computer side
				if (board[i] != 0) {
					for (int j = 0; j < 14; j++) {
						boardCopy[j] = board[j];
					}

					takeMove(board, i, minOrMax); 

					mTemp = minmax(board, d + 1, maxD, MIN, m.score, beta); 
					if (m.score > beta) {
						break; 
					}

					if (mTemp.score >= m.score) {
						m.score = mTemp.score;
						m.binNum = i;
					}

					for (int k = 0; k < 14; k++) {
						board[k] = boardCopy[k];
					}
				}
			}
			return m;
		}
	}
}

int computerMove(int board[], int maxD)
{
	Move m;
	int alpha = INT_MIN;
	int beta = INT_MAX;

	m = minmax(board, 0, maxD, MAX, alpha, beta);
	return m.binNum;
}

int evalFunction(int board[], int minOrMax) { 
	int boardCopy[14];
	bool captureAcross; //returns true if you can capture opponent marbles

	for (int j = 0; j < 14; j++) {
		boardCopy[j] = board[j];
	}

	double userSide = (board[0] + board[1] + board[2] + board[3] + board[4] + board[5]) * 0.38; //multiplying by 0.38 for variation										
	double userMancala = (board[6]) * 3.45; //multiple by a higher constant because these are guaranteed pieces
	double userValue = userSide +  userMancala;
						
	double cpuSide = (board[7] + board[8] + board[9] + board[10] + board[11] + board[12])*0.38; //multiply by 0.38 for variation														
	double cpuMancala = (board[13]) * 3.45; //multiple by a higher constant because these are guaranteed pieces
	double cpuValue= cpuSide + cpuMancala;

	double valueDifference = cpuValue - userValue;
	int boardValue = static_cast<int>(valueDifference + 0.5);

	for (int i = 0; i <= 5; i++) { //check if user can capture computer's marbles
		captureAcross = takeMove(boardCopy, i, minOrMax);
		if (captureAcross) {
			int acrosslocation = findAcross(i);
			int acrosspieces = boardCopy[acrosslocation];
			boardValue = static_cast<int>(boardValue + ((acrosspieces*0.5)*-1)); //multiple by -1 because this move is not desireable
		}
	}

	for (int i = 7; i <= 12; i++) { //check if computer can capture user's marbles
		captureAcross = takeMove(boardCopy, i, minOrMax);
		if (captureAcross) {
			int acrosslocation = findAcross(i);
			int acrosspieces = boardCopy[acrosslocation];
			boardValue = static_cast<int>(boardValue + ((acrosspieces*0.5)));
		}
	}

	return boardValue;
}


bool checkBoard(int board[]) {
	bool isEmpty = true;
	for (int i = 0; i < 6; i++) {
		if (board[i] > 0) {
			isEmpty = false;
			break;
		}
	}

	if (isEmpty) { //if bottom is empty, then store all the marbles on computer side in computer's mancala
		board[13] = board[13] + board[12] + board[11] + board[10] + board[9] + board[8] + board[7];
		for (int i = 7; i < 13; i++) {
			board[i] = 0;
		}
		return true;
	}

	isEmpty = true;
	for (int i = 7; i < 13; i++) {
		if (board[i] > 0) {
			isEmpty = false;
			break;
		}
	}

	if (isEmpty) { //if top is empty, then store all the marbles on user side in user's mancala
		board[6] = board[6] + board[5] + board[4] + board[3] + board[2] + board[1] + board[0];
		for (int i = 0; i < 6; i++) {
			board[i] = 0; //empty all on human side
		}
		return true;
	}
	return false;
}

int findOnlyMove(int board[]) {
	int numMoves = 0, pos;

	for (int i = 7; i <= 12; i++) {
		if (board[i] > 0) {
			numMoves++;
			pos = i;
		}
	}

	if (numMoves == 1) {
		return pos;
	}
	else {
		return -1;
	}
}

int findAcross(int ending) { 
	int across = -1;
	if (ending >= 0 && ending <= 12 && ending != 6)
		across = 12 - ending;
	return across;
}

int getUserMove(int board[]) {
	int userMove;
	cout << "What move would you like to make?" << endl;
	cout << "Please enter either 0,1,2,3,4 or 5." << endl;
	cin >> userMove;
	
	while (userMove > 5 || userMove < 0 || board[userMove] == 0) { //validate selection
		cout << "Wrong selection. Bin is either empty or out of range" << endl;
		cout << "What move would you like to make?" << endl;
		cout << "Please enter either 0,1,2,3,4 or 5." << endl;
		cin >> userMove;
	}
	return userMove;
}

void printBoard(int board[]) {
	cout << "       |  12  |  11  |  10  |   9  |   8  |   7  |" << endl;
	cout << "________________________________________________________" << endl;
	cout << "|" << "      | (" << setfill('0') << setw(2) << board[12] << ") | ";
	cout << "(" << setfill('0') << setw(2) << board[11];
	cout << ") | " << "(" << setfill('0') << setw(2) << board[10] << ") | ";
	cout << "(" << setfill('0') << setw(2) << board[9] << ") | ";
	cout << "(" << setfill('0') << setw(2) << board[8] << ") | ";
	cout << "(" << setfill('0') << setw(2) << board[7] << ") |      |" << endl;
	cout << "|      |_________________________________________|      |" << endl;
	cout << "|  " << setfill('0') << setw(2) << board[13] << "  |";
	cout << "                                         ";
	cout << "|  " << setfill('0') << setw(2) << board[6] << "  |" << endl;
	cout << "|      |_________________________________________|      |" << endl;
	cout << "|" << "      | (" << setfill('0') << setw(2) << board[0] << ") | ";
	cout << "(" << setfill('0') << setw(2) << board[1];
	cout << ") | " << "(" << setfill('0') << setw(2) << board[2] << ") | ";
	cout << "(" << setfill('0') << setw(2) << board[3] << ") | ";
	cout << "(" << setfill('0') << setw(2) << board[4] << ") | ";
	cout << "(" << setfill('0') << setw(2) << board[5] << ") |	|" << endl;
	cout << "________________________________________________________" << endl;
	cout << "       |   0  |   1  |   2  |   3  |   4  |   5  |" << endl;
	cout << "\n";
}