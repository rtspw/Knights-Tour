#include <iostream>
#include "mystackll.h"
#include "tile.h"

using namespace std;

void generateChessboard(tile myChessboard[8][8]);
void printChessboard(tile myChessboard[8][8]);
bool isLegal(int x, int y, tile myChessboard[8][8]);
bool findSolution(int xLoc, int yLoc, myStackLL<tile> &mainStack, tile myChessboard[8][8]);

int main() {

    myStackLL<tile> mainStack(64);
    tile chessboard[8][8];
    int xLoc, yLoc;

    generateChessboard(chessboard);
    printChessboard(chessboard);

    xLoc = yLoc = 0;

    try {

        if(findSolution(xLoc, yLoc, mainStack, chessboard)) {
            cout << "Solution exists." << endl;
        }
        else {
            cout << "No solution exists." << endl;
        }


//        // Initial tile in stack (prevents initial empty stack error)
//        mainStack.push(chessboard[xLoc][yLoc]);

//        // Ends when all 64 tiles are taken
//        while(!mainStack.full()) {


//            //cout << endl;
//            cout << " - - - Performing next move - - - - - " << mainStack.size() << endl;

//            //printChessboard(chessboard);

//            // If the top value isn't stack (for after backtracks), push in
//            if(chessboard[xLoc][yLoc] != mainStack.peek()) {
//                mainStack.push(chessboard[xLoc][yLoc]);
//                //cout << "Now top of stack : " << mainStack.peek() << mainStack.peek().isTaken() << " || " << mainStack.peek().getAttempts() << endl;
//            }

//            // Go through all 8 possible moves, saved in solution array in tile class
//            while(chessboard[xLoc][yLoc].getAttempts() < 8) {
//                nextX = chessboard[xLoc][yLoc].getAttemptX();
//                nextY = chessboard[xLoc][yLoc].getAttemptY();

//                chessboard[xLoc][yLoc]++;

//                // If move legal, set current as taken and prepare new values
//                if(isLegal(nextX, nextY, chessboard)) {

//                    //cout << "Before: " <<  chessboard[xLoc][yLoc] << " with value : " << chessboard[xLoc][yLoc].isTaken() << endl;

//                    chessboard[xLoc][yLoc].setTaken(true);

//                    //cout << "Before: " <<  chessboard[xLoc][yLoc] << " with value : " << chessboard[xLoc][yLoc].isTaken() << endl;

//                    xLoc = nextX;
//                    yLoc = nextY;

//                    //cout << "After: " <<  chessboard[xLoc][yLoc] << " with value : " << chessboard[xLoc][yLoc].isTaken() <<  endl;

//                    break;
//                }
//            }

//            // If all attempts done, pop from stack until tile with solutions left
//            if(chessboard[xLoc][yLoc].getAttempts() >= 8) {

//                //cout << " | | | | |  | | | |   BACKTRACKING " << endl;

//                mainStack.pop();
//                chessboard[xLoc][yLoc].setTaken(false);
//                chessboard[xLoc][yLoc].resetAttempts();

//                //cout << "Before backtrack: " <<  chessboard[xLoc][yLoc] << " with taken : " << chessboard[xLoc][yLoc].isTaken() << " and attemptval: " << chessboard[xLoc][yLoc].getAttempts() << endl;

//                xLoc = mainStack.peek().getX();
//                yLoc = mainStack.peek().getY();

//                chessboard[xLoc][yLoc].setTaken(false);

//                //cout << "After backtrack: " <<  chessboard[xLoc][yLoc] << " with value : " << chessboard[xLoc][yLoc].isTaken() << " and attemptval: " << chessboard[xLoc][yLoc].getAttempts() << endl;
//            }
//        }
//        cout << "FINISHED!" << endl;






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

bool findSolution(int xLoc, int yLoc, myStackLL<tile> &mainStack, tile myChessboard[8][8]) {
    int nextX, nextY; // make sure value is set each loop
    nextX = nextY = 0;
    bool moveToNext;

    mainStack.push(myChessboard[xLoc][yLoc]);
    while(!mainStack.full()) {

        moveToNext = false;

        cout << " | | | | | CURRENT MOVE : : " << myChessboard[xLoc][yLoc] << " | | | | | "  << " w/ attempts : " << myChessboard[xLoc][yLoc].getAttempts() << endl;
        cout << " Top of Stack : " << mainStack.peek() << endl;


        int oldX = xLoc;
        int oldY = yLoc;

        while(myChessboard[xLoc][yLoc].getAttempts() < 8 && !moveToNext) {
            nextX = myChessboard[xLoc][yLoc].getAttemptX();
            nextY = myChessboard[xLoc][yLoc].getAttemptY();
            ++myChessboard[xLoc][yLoc];
            if(isLegal(nextX, nextY, myChessboard)) {
                myChessboard[xLoc][yLoc].setTaken(true);
                if(mainStack.peek() != myChessboard[xLoc][yLoc]) {
                    cout << "Attempts before push : " << myChessboard[xLoc][yLoc].getAttempts() << endl;
                    mainStack.push(myChessboard[xLoc][yLoc]);
                }
                xLoc = nextX;
                yLoc = nextY;
                moveToNext = true;

                cout << " Setting next position : " << myChessboard[xLoc][yLoc] << endl;
            }
        }

        if(myChessboard[oldX][oldY].getAttempts() >= 8 && !moveToNext) {
            cout << "BACKTRACKING! :: " << myChessboard[oldX][oldY] << endl;
            myChessboard[oldX][oldY].setTaken(false);  // MAIN ISSUE!!!
            myChessboard[oldX][oldY].resetAttempts();
            xLoc = mainStack.peek().getX();
            yLoc = mainStack.peek().getY();
            mainStack.pop();
        }

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



