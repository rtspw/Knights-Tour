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
    size_t getX();
    size_t getY();
    size_t getAttempts();
    void resetAttempts();
    int getAttemptX();
    int getAttemptY();
    bool isTaken();

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

    int movesX[8] = {1, 2, 2, 1, -1, -2, -2, -1};
    int movesY[8] = {2, 1, -1, -2, -2, -1, 1, 2};



};

#endif // TILE_H
