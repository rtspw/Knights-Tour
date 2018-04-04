#ifndef TILE_H
#define TILE_H

#include <cstdlib>
#include <iostream>


class tile {

public:
    tile(size_t x = 0, size_t y = 0);
    ~tile();
    tile(const tile &other);
    tile &operator=(const tile &other);

    void setX(size_t x);
    void setY(size_t y);
    void setTaken(bool a);
    void setCurrent(bool a);
    size_t getX();
    size_t getY();
    size_t getAttempts();
    void resetAttempts();
    int getAttemptX(int val);
    int getAttemptY(int val);
    bool isTaken();
    bool isCurrent();

    bool operator==(const tile &other);
    bool operator!=(const tile &other);
    tile &operator++();
    tile operator++(int);
    tile &operator--();
    tile operator--(int);

    friend
    std::ostream &operator<<(std::ostream &os, const tile &data);

private:

    size_t xVal;
    size_t yVal;
    size_t attemptNext;
    bool taken;
    bool current;

    int movesX[8] = {1, 2, 2, 1, -1, -2, -2, -1};
    int movesY[8] = {2, 1, -1, -2, -2, -1, 1, 2};

    std::string xRowNames[8] = {"KR","KK","KB","K","Q","QB","QK","QR"};
    int yRowNames[8] = {1, 2, 3, 4, 5, 6, 7, 8};

};

#endif // TILE_H
