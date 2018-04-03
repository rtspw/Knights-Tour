#include <iostream>
#include <sstream>
#include <ctime>
#include "mystackll.h"
#include "tile.h"

using namespace std;

void generateChessboard(tile myChessboard[8][8]);
void printChessboard(tile myChessboard[8][8]);
bool isLegal(int x, int y, tile myChessboard[8][8]);
bool findSolution(int xLoc, int yLoc, myStackLL<tile> &mainStack, tile myChessboard[8][8]);

void askForValues(int &xLoc, int &yLoc);

size_t getDegree(int x, int y, tile myChessboard[8][8]);

int main() {

    myStackLL<tile> mainStack(64);
    tile chessboard[8][8];
    int xLoc, yLoc;

    generateChessboard(chessboard);
    printChessboard(chessboard);

    xLoc = yLoc = -1;

    while(xLoc < 0 || xLoc > 7 || yLoc < 0 || yLoc > 7) {
        askForValues(xLoc, yLoc);
        if(xLoc < 0 || xLoc > 7 || yLoc < 0 || yLoc > 7)
            cout << "Values must be between 0 and 7." << endl;
    }

    try {
        if(findSolution(xLoc, yLoc, mainStack, chessboard)) {
            cout << "Solution exists." << endl;
        }
        else {
            cout << "No solution exists." << endl;
        }
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

    return 0;
}


/* askForValues
 * Initially asks user for inputs and sanitizes inputs */
void askForValues(int &xLoc, int &yLoc) {

    int y = -1;
    int x = -1;
    string input;
    string xRowNames[8] = {"KR","KH","KB","K","Q","QB","QH","QR"};

    cout << "Enter intial X value (KR, KH...): " << flush;
    getline(cin, input);

    // Matches text value and matches it with number
    for(size_t i = 0; i < 8; ++i) {
        for(size_t j = 0; j < input.length(); ++j)
            input[j] = toupper(input[j]);
        if(input == xRowNames[i])
            x = i;
    }

    cout << "Enter initial Y value (0 - 7): " << flush;
    getline(cin, input);
    stringstream ss;
    ss << input;
    ss >> y;
    xLoc = x;
    yLoc = y;
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

bool findSolution(int xLoc, int yLoc, myStackLL<tile> &mainStack, tile myChessboard[8][8]) {

    int nextX = 0;
    int nextY = 0;
    int smallX;
    int smallY;
    int minDegree;
    bool hasPossibleNextMove;

    while(!mainStack.full()) {


        cout << " | | | | | CURRENT MOVE : : " << myChessboard[xLoc][yLoc] << " | | | | | "  << " w/ attempts : " << myChessboard[xLoc][yLoc].getAttempts() << endl;
        //cout << " Top of Stack : " << mainStack.peek() << " of size : " << mainStack.size() << endl;

        mainStack.push(myChessboard[xLoc][yLoc]);
        myChessboard[xLoc][yLoc].setTaken(true);

        minDegree = 9;
        smallX = 0;
        smallY = 0;
        hasPossibleNextMove = false;

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

        if(!hasPossibleNextMove && mainStack.size() < 64) {
            myChessboard[xLoc][yLoc].setTaken(false);
            xLoc = mainStack.peek().getX();
            yLoc = mainStack.peek().getY();
            mainStack.pop();
        }
//        else {
            xLoc = smallX;
            yLoc = smallY;
//        }





//        while(myChessboard[xLoc][yLoc].getAttempts() < 8 && !moveToNext) {
//            nextX = myChessboard[xLoc][yLoc].getAttemptX(-1);
//            nextY = myChessboard[xLoc][yLoc].getAttemptY(-1);
//            //cout << "Trying value (" << nextX << "," << nextY << ") on attempt : " << myChessboard[xLoc][yLoc].getAttempts() << endl;
//            ++myChessboard[xLoc][yLoc];
//            if(isLegal(nextX, nextY, myChessboard)) {
//                if(mainStack.peek() != myChessboard[xLoc][yLoc]) {
//                    //cout << "Attempts before push : " << myChessboard[xLoc][yLoc].getAttempts() << endl;
//                    mainStack.push(myChessboard[xLoc][yLoc]);
//                }
//                myChessboard[xLoc][yLoc].setTaken(true);
//                xLoc = nextX;
//                yLoc = nextY;
//                moveToNext = true;

//                //cout << " Setting next position : " << myChessboard[xLoc][yLoc] << endl;
//            }
//        }

//        if(myChessboard[oldX][oldY].getAttempts() >= 8 && !moveToNext) {
//            //cout << "BACKTRACKING! :: " << myChessboard[oldX][oldY] << " with taken : " << myChessboard[oldX][oldY].isTaken() << endl;
//            myChessboard[oldX][oldY].setTaken(false);  // MAIN ISSUE!!!
//            myChessboard[oldX][oldY].resetAttempts();

//            xLoc = mainStack.peek().getX();
//            yLoc = mainStack.peek().getY();
//            mainStack.pop();
//        }

        printChessboard(myChessboard);
    }

    if(mainStack.empty())
        return false;

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



