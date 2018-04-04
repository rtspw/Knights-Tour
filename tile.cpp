#include "tile.h"

/// Initialization

tile::tile(size_t x,  size_t y) {
    xVal = x;
    yVal = y;
    attemptNext = 0;
    taken = false;
    current = false;
}

tile::~tile() {
    xVal = yVal = attemptNext = 0;
    taken = false;
    current = false;
}

tile::tile(const tile &other) {
    xVal = other.xVal;
    yVal = other.yVal;
    attemptNext = other.attemptNext;
    taken = other.taken;
    current = other.current;
}

tile &tile::operator=(const tile &other) {
    if(this != &other) {
        xVal = other.xVal;
        yVal = other.yVal;
        attemptNext = other.attemptNext;
        taken = other.taken;
        current = other.current;
    }
    return *this;
}

/// Functions

void tile::setX(size_t x) {
    xVal = x;
}

void tile::setY(size_t y) {
    yVal = y;
}
void tile::setTaken(bool a) {
    taken = a;
}

void tile::setCurrent(bool a) {
    current = a;
}

size_t tile::getX() {
    return xVal;
}

size_t tile::getY() {
    return yVal;
}

size_t tile::getAttempts() {
    return attemptNext;
}


void tile::resetAttempts() {
    attemptNext = 0;
}

int tile::getAttemptX(int val) {
    if (val < 0)
        val = attemptNext;
    return xVal + movesX[val];
}

int tile::getAttemptY(int val) {
    if (val < 0)
        val = attemptNext;
    return yVal + movesY[val];
}

bool tile::isTaken() {
    return taken;
}

bool tile::isCurrent() {
    return current;
}

/// Overloaded Operators

bool tile::operator==(const tile &other) {
    return xVal == other.xVal && yVal == other.yVal;
}

bool tile::operator!=(const tile &other) {
    return xVal != other.xVal || yVal != other.yVal;
}

tile &tile::operator++() {
    ++attemptNext;
    return *this;
}

tile tile::operator++(int) {
    tile temp = *this;
    ++*this;
    return temp;
}

tile &tile::operator--() {
    --attemptNext;
    return *this;
}

tile tile::operator--(int) {
    tile temp = *this;
    --*this;
    return temp;
}

std::ostream &operator<<(std::ostream &os, const tile &data) {
    os << data.xRowNames[data.xVal] << data.yVal;
    return os;
}
