#include <iostream>
#include <sstream>
#include <time.h>
#include <chrono>
#include <thread>
#include "mystackll.h"
#include "pqueuearray.h"
#include "tile.h"

using namespace std;

void generateChessboard(tile myChessboard[8][8]);
void printChessboard(tile myChessboard[8][8]);

bool performTour();
bool isLegal(int x, int y, tile myChessboard[8][8]);
bool findSolution(int xLoc, int yLoc, bool &closed, myStackLL<tile> &mainStack, tile myChessboard[8][8]);
bool findSolutionW(int xLoc, int yLoc, bool &closed, myStackLL<tile> &mainStack, tile myChessboard[8][8]);
bool findSolutionQueue(int &xLoc, int &yLoc, bool &closed, pQueueArray<tile, int> &mainQueue, tile myChessboard[8][8]);
bool findSolutionQueueW(int &xLoc, int &yLoc, bool &closed, pQueueArray<tile, int> &mainQueue, tile myChessboard[8][8]);
bool isClosed(int x, int y, int ogX, int ogY, tile myChessboard[8][8]);
bool askForRedo();
bool askForUseStack();
bool askForReplay();
bool askAlgorithmType();

void askForValues(int &xLoc, int &yLoc);
void printReplay(myStackLL<tile> &mainStack);
void printReplayQueue(pQueueArray<tile, int> &mainQueue);
void printSolution(bool closed, clock_t &time, myStackLL<tile>& mainStack);
void printSolutionQueue(bool closed, clock_t &time, pQueueArray<tile, int> &mainQueue);
void findMoveOfLowestDegree(const int &xLoc, const int &yLoc, int &smallX, int &smallY, bool &nextMove, tile myChessboard[8][8]);

size_t getDegree(int x, int y, tile myChessboard[8][8]);

int main() {
    while(performTour()) {;}
    return 0;
}

bool performTour() {

    tile chessboard[8][8];
    int xLoc, yLoc;
    bool closed = false;
    bool useStack = true;
    //bool useBruteForce = true;

    generateChessboard(chessboard);
    printChessboard(chessboard);

    xLoc = yLoc = -1;

    // Gets initial values from user
    while(xLoc < 0 || xLoc > 7 || yLoc < 0 || yLoc > 7) {
        askForValues(xLoc, yLoc);
        if(xLoc < 0 || xLoc > 7 || yLoc < 0 || yLoc > 7)
            cout << "Values must be between 1 and 8." << endl << endl;
    }

    useStack = askForUseStack();

    clock_t t = clock();

    if(useStack) {
        try {
            myStackLL<tile> mainStack(64);
            if(findSolution(xLoc, yLoc, closed, mainStack, chessboard)) {
                myStackLL<tile> temp(mainStack);
                printSolution(closed, t, mainStack);
                if(askForReplay())
                    printReplay(temp);
            }
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
    }
    else {
        try {
            pQueueArray<tile, int> mainQueue(64);
            if(findSolutionQueue(xLoc, yLoc, closed, mainQueue, chessboard)) {
                pQueueArray<tile, int> temp(mainQueue);
                printSolutionQueue(closed, t, mainQueue);
                if(askForReplay())
                    printReplayQueue(temp);
            }
            else
                cout << "No solution exists." << endl;
        }
        catch(PQ_ERRORS e) {
            if(e == Q_EMPTY)
                cout << "ERROR: QUEUE EMPTY!" << endl;
            else if(e == Q_FULL)
                cout << "ERROR: QUEUE FULL!" << endl;
            else if(e == Q_BAD_SIZE)
                cout << "ERROR: BAD QUEUE SIZE!" << endl;
        }
        catch(...) {
            cout << "UNKNOWN ERROR OCCURED!" << endl;
        }
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
    input = input.substr(0, 2);

    // Matches text value and matches it with number
    for(size_t i = 0; i < 8; ++i) {
        for(size_t j = 0; j < input.length(); ++j)
            input[j] = toupper(input[j]);
        if(input == xRowNames[i])
            x = i;
    }

    // Gets Y value as integer
    cout << "Enter initial Y value (1 - 8): " << flush;
    getline(cin, input);
    stringstream ss;
    ss << input;
    ss >> y;
    xLoc = x;
    yLoc = y - 1;

}

/* Asks user if they want to replay moves */
bool askForReplay() {
    string input;
    cout << "Play replay (y/n)? ";
    getline(cin, input);
    return toupper(input[0]) == 'Y';
}

/* askAlgorithmType
 * Asks if user wants to use brute-force or Warnsdorff */
bool askAlgorithmType() {
    string input;
    while(toupper(input[0]) != 'B' && toupper(input[0]) != 'W') {
        cout << "Brute-force or Warnsdorff (b/w)? ";
        getline(cin, input);
    }
    return toupper(input[0]) == 'B';
}

/* askForRedo
 * Checks if user wants to run program again with different values */
bool askForRedo() {
    string input;
    cout << "Try with new initial values (y/n)? ";
    getline(cin, input);
    return toupper(input[0]) == 'Y';
}

/* askForUseStack
 * Asks if user wants to use stack or queue to solve problem */
bool askForUseStack() {
    string input = " ";
    while(toupper(input[0]) != 'S' && toupper(input[0]) != 'Q') {
        cout << "Use stack or queue (S/Q)? ";
        getline(cin, input);
    }
    return toupper(input[0]) == 'S';
}

/* printReplay
 * Clears the console and replays movements one step at a time */
void printReplay(myStackLL<tile> &mainStack) {
    tile display[8][8];
    int currentX, currentY;
    generateChessboard(display);
    while(!mainStack.empty()) {
        currentX = mainStack.peek().getX();
        currentY = mainStack.pop().getY();
        display[currentX][currentY].setCurrent(true);
        display[currentX][currentY].setTaken(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        system("cls");
        printChessboard(display);
        display[currentX][currentY].setCurrent(false);
    }
}

void printReplayQueue(pQueueArray<tile, int> &mainQueue) {
    tile display[8][8];
    int currentX, currentY;
    generateChessboard(display);
    while(!mainQueue.empty()) {
        currentX = mainQueue.peek().getX();
        currentY = mainQueue.dequeue().getY();
        display[currentX][currentY].setCurrent(true);
        display[currentX][currentY].setTaken(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        system("cls");
        printChessboard(display);
        display[currentX][currentY].setCurrent(false);
    }
}

/* printSolution
 * Prints in a list, with new line every 8 outputs */
void printSolution(bool closed, clock_t &time, myStackLL<tile> &mainStack) {

    int iter = 0;
    time = clock() - time;

    cout << endl << "Solution type : ";
    (closed) ? cout <<  "CLOSED" : cout << "OPEN";
    cout << endl << "Method used : Linked List Stack" << endl;

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
        cout << reverseStack.pop();
        if(reverseStack.size() != 0)
            cout << ", ";
        ++iter;
        if(iter == 8) {
            cout << endl;
            iter = 0;
        }
    }
    cout << "- - - - - - - - - - - - - - - - - - - " << endl;
}

void printSolutionQueue(bool closed, clock_t &time, pQueueArray<tile, int> &mainQueue) {

    int iter = 0;
    time = clock() - time;
    cout << endl << "Solution type : ";
    (closed) ? cout <<  "CLOSED" : cout << "OPEN";
    cout << endl << "Method used : Priority Queue" << endl;

    cout << "Elapsed time : ";
    if((double)time == 0)
        cout << "<0ms" << endl;
    else
        cout << time << "ms" << endl;
    cout << "- - - - - - - - - - - - - - - - - - - " << endl;
    while(!mainQueue.empty()) {
        cout << mainQueue.dequeue();
        if(mainQueue.size() != 0)
            cout << ", ";
        if(++iter == 8) {
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

/* findMoveOfLowestDegree
 * Returns x and y value of next move with lowest degree (if exists) */
void findMoveOfLowestDegree(const int &xLoc, const int &yLoc, int &smallX, int &smallY, bool &nextMove, tile myChessboard[8][8]) {
    int nextX = 0;
    int nextY = 0;
    int minDegree = 9;
    for(size_t i = 0; i < 8; ++i) {
        nextX = myChessboard[xLoc][yLoc].getAttemptX(i);
        nextY = myChessboard[xLoc][yLoc].getAttemptY(i);
        if(isLegal(nextX, nextY, myChessboard)) {
            nextMove = true;
            int degreeOfMove = getDegree(nextX, nextY, myChessboard);
            if(degreeOfMove < minDegree) {
                smallX = nextX;
                smallY = nextY;
                minDegree = degreeOfMove;
            }
        }
    }
}

/* findSolutionW
 * Generates a solution stack based of that one German guy's algorithm */
bool findSolutionW(int xLoc, int yLoc, bool &closed, myStackLL<tile> &mainStack, tile myChessboard[8][8]) {

    int smallX;
    int smallY;
    int originalX = xLoc;
    int originalY = yLoc;

    bool hasPossibleNextMove;

    while(!mainStack.full()) {

        mainStack.push(myChessboard[xLoc][yLoc]);
        myChessboard[xLoc][yLoc].setTaken(true);
        hasPossibleNextMove = false;

        // Main loop : finds tile with lowest degree and sets next
        findMoveOfLowestDegree(xLoc, yLoc, smallX, smallY, hasPossibleNextMove, myChessboard);

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
    }
    closed = isClosed(xLoc, yLoc, originalX, originalY, myChessboard);
    return true;
}

bool findSolution(int xLoc, int yLoc, bool &closed, myStackLL<tile> &mainStack, tile myChessboard[8][8]) {

    int nextX = 0;
    int nextY = 0;
    int originalX = xLoc;
    int originalY = yLoc;
    int numberOfAttempts;

    bool hasPossibleNextMove;

    mainStack.push(myChessboard[xLoc][yLoc]);

    while(!mainStack.full()) {

        myChessboard[xLoc][yLoc].setTaken(true);
        hasPossibleNextMove = false;

        if(mainStack.peek() != myChessboard[xLoc][yLoc])
            mainStack.push(myChessboard[xLoc][yLoc]);

        // Finding next possible move
        numberOfAttempts = myChessboard[xLoc][yLoc].getAttempts();
        for(size_t i = numberOfAttempts; i < 8 && !hasPossibleNextMove; ++i) {
            nextX = myChessboard[xLoc][yLoc].getAttemptX(i);
            nextY = myChessboard[xLoc][yLoc].getAttemptY(i);
            ++myChessboard[xLoc][yLoc];

            if(isLegal(nextX, nextY, myChessboard))
                hasPossibleNextMove = true;
        }

        // Backtracking
        if(!hasPossibleNextMove && mainStack.size() < 64) {
            mainStack.pop();
            myChessboard[xLoc][yLoc].setTaken(false);
            myChessboard[xLoc][yLoc].resetAttempts();
            xLoc = mainStack.peek().getX();
            yLoc = mainStack.peek().getY();
        }
        else {
            xLoc = nextX;
            yLoc = nextY;
        }

        if(mainStack.empty())
            return false;

    }
    closed = isClosed(xLoc, yLoc, originalX, originalY, myChessboard);
    return true;
}

bool findSolutionQueueW(int &xLoc, int &yLoc, bool &closed, pQueueArray<tile, int> &mainQueue, tile myChessboard[8][8]) {

    int smallX,
        smallY,
        originalX = xLoc,
        originalY = yLoc,
        iter = 128;
    bool hasPossibleNextMove;

    mainQueue.enqueue(myChessboard[xLoc][yLoc], 128);
    while(!mainQueue.full()) {

        mainQueue.enqueue(myChessboard[xLoc][yLoc], iter--);
        myChessboard[xLoc][yLoc].setTaken(true);

        hasPossibleNextMove = false;

        findMoveOfLowestDegree(xLoc, yLoc, smallX, smallY, hasPossibleNextMove, myChessboard);
        xLoc = smallX;
        yLoc = smallY;
        if(mainQueue.empty())
            return false;
    }
    mainQueue.dequeue();
    mainQueue.enqueue(myChessboard[xLoc][yLoc], 0);
    closed = isClosed(xLoc, yLoc, originalX, originalY, myChessboard);
    return true;
}

bool findSolutionQueue(int &xLoc, int &yLoc, bool &closed, pQueueArray<tile, int> &mainQueue, tile myChessboard[8][8]) {
    int nextX = 0;
    int nextY = 0;
    int iter = 0;
    int originalX = xLoc;
    int originalY = yLoc;
    int numberOfAttempts;
    bool hasPossibleNextMove;
    mainQueue.enqueue(myChessboard[xLoc][yLoc], 0);

    while(!mainQueue.full()) {

        myChessboard[xLoc][yLoc].setTaken(true);
        hasPossibleNextMove = false;

        if(mainQueue.peek() != myChessboard[xLoc][yLoc])
            mainQueue.enqueue(myChessboard[xLoc][yLoc], ++iter);

        // Finding next possible move
        numberOfAttempts = myChessboard[xLoc][yLoc].getAttempts();
        for(size_t i = numberOfAttempts; i < 8 && !hasPossibleNextMove; ++i) {
            nextX = myChessboard[xLoc][yLoc].getAttemptX(i);
            nextY = myChessboard[xLoc][yLoc].getAttemptY(i);
            ++myChessboard[xLoc][yLoc];

            if(isLegal(nextX, nextY, myChessboard))
                hasPossibleNextMove = true;
        }

        // Backtracking
        if(!hasPossibleNextMove && mainQueue.size() < 64) {
            mainQueue.dequeue();
            --iter;
            myChessboard[xLoc][yLoc].setTaken(false);
            myChessboard[xLoc][yLoc].resetAttempts();
            xLoc = mainQueue.peek().getX();
            yLoc = mainQueue.peek().getY();
        }
        else {
            xLoc = nextX;
            yLoc = nextY;
        }

        if(mainQueue.empty())
            return false;
    }
    size_t rev = 0;
    pQueueArray<tile, int> reverse(64);
    for(size_t i = 0; i < 64; ++i) {
        reverse.enqueue(mainQueue.dequeue(), ++rev);
    }
    mainQueue = reverse;
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
    cout << endl;
    for(int j = 7; j >= 0; --j) {
        for(size_t i = 0; i < 8; ++i) {
            if(myChessboard[i][j].isCurrent())
                cout << "KNIGHT" << "\t";
            else if(myChessboard[i][j].isTaken())
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




