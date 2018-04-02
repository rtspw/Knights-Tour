#include "tile.h"

/// Initialization

tile::tile(size_t x,  size_t y) {
    xVal = x;
    yVal = y;
    attemptNext = 0;
    taken = false;
}

tile::~tile() {
    xVal = yVal = attemptNext = 0;
    taken = false;
}

tile::tile(const tile &other) {
    xVal = other.xVal;
    yVal = other.yVal;
    attemptNext = other.attemptNext;
    taken = other.taken;
}

tile &tile::operator=(const tile &other) {
    if(this != &other) {
        xVal = other.xVal;
        yVal = other.yVal;
        attemptNext = other.attemptNext;
        taken = other.taken;
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

int tile::getAttemptX() {
    return xVal + movesX[attemptNext];
}

int tile::getAttemptY() {
    return yVal + movesY[attemptNext];
}

bool tile::isTaken() {
    return taken;
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
    os << "(" << data.xVal << "," << data.yVal << ")";
    return os;
}
