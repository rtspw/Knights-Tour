#include <iostream>
#include <sstream>
#include <time.h>
#include "mystackll.h"
#include "tile.h"

using namespace std;

void generateChessboard(tile myChessboard[8][8]);
void printChessboard(tile myChessboard[8][8]);

bool performTour();
bool isLegal(int x, int y, tile myChessboard[8][8]);
bool findSolution(int xLoc, int yLoc, bool &closed, myStackLL<tile> &mainStack, tile myChessboard[8][8]);
bool isClosed(int x, int y, int ogX, int ogY, tile myChessboard[8][8]);
bool askForRedo();

void askForValues(int &xLoc, int &yLoc);
void printSolution(bool closed, clock_t &time, myStackLL<tile> &mainStack);

size_t getDegree(int x, int y, tile myChessboard[8][8]);

int main() {
    while(performTour()) {;}
    return 0;
}

bool performTour() {

    myStackLL<tile> mainStack(64);
    clock_t t = clock();
    tile chessboard[8][8];
    int xLoc, yLoc;
    bool closed = false;

    generateChessboard(chessboard);
    printChessboard(chessboard);

    xLoc = yLoc = -1;

    // Gets initial values from user
    while(xLoc < 0 || xLoc > 7 || yLoc < 0 || yLoc > 7) {
        askForValues(xLoc, yLoc);
        if(xLoc < 0 || xLoc > 7 || yLoc < 0 || yLoc > 7)
            cout << "Values must be between 0 and 7." << endl << endl;
    }

    // Finds solution and prints if exists
    try {
        if(findSolution(xLoc, yLoc, closed, mainStack, chessboard))
            printSolution(closed, t, mainStack);
        else
            cout << "No solution exists." << endl;
    }
    catch(MYSTACKLL_ERRORS e) {
        if(e == LL_STACK_EMPTY)
            cout << "ERROR: STACK EMPTY!" << endl;
        else if(e == LL_STACK_FULL)
            cout << "ERROR: STACK FULL!" << endl;
        else if(e == LL_BAD_SIZE)
            cout << "ERROR: BAD STACK SIZE!" << endl;
    }
    catch(...) {
        cout << "UNKNOWN ERROR OCCURED!" << endl;
    }
    return askForRedo();
}

/* isClosed
 * Checks if final move is adjacent to original */
bool isClosed(int x, int y, int ogX, int ogY, tile myChessboard[8][8]) {
    for(size_t i = 0; i < 8; ++i) {
        int attemptX = myChessboard[x][y].getAttemptX(i);
        int attemptY = myChessboard[x][y].getAttemptY(i);
        if(attemptX == ogX && attemptY == ogY)
            return true;
    }
    return false;
}

/* askForValues
 * Initially asks user for inputs and sanitizes inputs */
void askForValues(int &xLoc, int &yLoc) {

    int y = -1;
    int x = -1;
    string input;
    string xRowNames[8] = {"KR","KK","KB","K","Q","QB","QK","QR"};

    cout << "Enter intial X value (KR, KH...): " << flush;
    getline(cin, input);

    // Matches text value and matches it with number
    for(size_t i = 0; i < 8; ++i) {
        for(size_t j = 0; j < input.length(); ++j)
            input[j] = toupper(input[j]);
        if(input == xRowNames[i])
            x = i;
    }

    // Gets Y value as integer
    cout << "Enter initial Y value (0 - 7): " << flush;
    getline(cin, input);
    stringstream ss;
    ss << input;
    ss >> y;
    xLoc = x;
    yLoc = y;

}

/* askForRedo
 * Checks if user wants to run program again with different values */
bool askForRedo() {
    string input;
    cout << "Try with new initial values (y/n)? ";
    getline(cin, input);
    return toupper(input[0]) == 'Y';
}

/* printSolution
 * Prints in a list, with new line every 8 outputs */
void printSolution(bool closed, clock_t &time, myStackLL<tile> &mainStack) {

    int iter = 0;

    time = clock() - time;

    cout << "Solution type : ";
    if(closed)
        cout << "CLOSED" << endl;
    else
        cout << "OPEN" << endl;

        cout << "Elapsed time : ";
    if((double)time == 0)
        cout << "<0ms" << endl;
    else
        cout << time << "ms" << endl;
    cout << "- - - - - - - - - - - - - - - - - - - " << endl;
    myStackLL<tile> reverseStack(64);
    while(!mainStack.empty())
        reverseStack.push(mainStack.pop());
    while(!reverseStack.empty()) {
        cout << reverseStack.pop() << ", ";
        ++iter;
        if(iter == 8) {
            cout << endl;
            iter = 0;
        }
    }
    cout << "- - - - - - - - - - - - - - - - - - - " << endl;
}

/* getDegree
 * Gets the degree of the next move (x, y) */
size_t getDegree(int x, int y, tile myChessboard[8][8]) {
    size_t degree = 0;
    int nextX = 0;
    int nextY = 0;
    for(size_t i = 0; i < 8; ++i) {
        nextX = myChessboard[x][y].getAttemptX(i);
        nextY = myChessboard[x][y].getAttemptY(i);
        if(isLegal(nextX, nextY, myChessboard))
            ++degree;
    }
    return degree;
}

bool findSolution(int xLoc, int yLoc, bool &closed, myStackLL<tile> &mainStack, tile myChessboard[8][8]) {

    int smallX;
    int smallY;
    int nextX = 0;
    int nextY = 0;
    int originalX = xLoc;
    int originalY = yLoc;
    int minDegree;
    bool hasPossibleNextMove;

    while(!mainStack.full()) {

        //cout << " | | | | | CURRENT MOVE : : " << myChessboard[xLoc][yLoc] << " | | | | | "  << " w/ attempts : " << myChessboard[xLoc][yLoc].getAttempts() << endl;
        //cout << " Top of Stack : " << mainStack.peek() << " of size : " << mainStack.size() << endl;

        mainStack.push(myChessboard[xLoc][yLoc]);
        myChessboard[xLoc][yLoc].setTaken(true);

        minDegree = 9;
        smallX = 0;
        smallY = 0;
        hasPossibleNextMove = false;

        // Main loop : finds tile with lowest degree and sets next
        for(size_t i = 0; i < 8; ++i) {
            nextX = myChessboard[xLoc][yLoc].getAttemptX(i);
            nextY = myChessboard[xLoc][yLoc].getAttemptY(i);
            if(isLegal(nextX, nextY, myChessboard)) {
                hasPossibleNextMove = true;
                int degreeOfMove = getDegree(nextX, nextY, myChessboard);
                if(degreeOfMove < minDegree) {
                    smallX = nextX;
                    smallY = nextY;
                    minDegree = degreeOfMove;
                }
            }
        }

        // If all attempts unavaliable, backtracks using stack
        if(!hasPossibleNextMove && mainStack.size() < 64) {
            myChessboard[xLoc][yLoc].setTaken(false);
            xLoc = mainStack.peek().getX();
            yLoc = mainStack.peek().getY();
            mainStack.pop();
        }
        else {
            xLoc = smallX;
            yLoc = smallY;
        }

        // When no solution exists
        if(mainStack.empty())
            return false;

        printChessboard(myChessboard);
    }

    closed = isClosed(xLoc, yLoc, originalX, originalY, myChessboard);
    return true;
}

/* generateChessboard
 * Fills the 2D array with the corresponding tile values */
void generateChessboard(tile myChessboard[8][8]) {
    for(size_t i = 0; i < 8; ++i) {
        for(int j = 7; j >= 0; --j) {
            myChessboard[i][j].setX(i);
            myChessboard[i][j].setY(j);
        }
    }
}

/* printChessboard
 * Prints a grid chessboard for debug purposes */
void printChessboard(tile myChessboard[8][8]) {
    for(int j = 7; j >= 0; --j) {
        for(size_t i = 0; i < 8; ++i) {
            if(myChessboard[i][j].isTaken())
                cout << "x" << "\t";
            else
                cout << myChessboard[i][j] << "\t";
        }

        cout << endl << endl;
    }
}

/* isLegal
 * Checks if next move is in bounds and not already taken */
bool isLegal(int x, int y, tile myChessboard[8][8]) {
    return x < 8 && x >= 0 && y >= 0 && y < 8 && !myChessboard[x][y].isTaken();
}



